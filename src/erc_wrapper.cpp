#include "ers_wrapper.h"

#include "MERCLazyGreedy.h"
#include "MERCInputImage.h"
#include "MERCOutputImage.h"
#include "Image.h"
#include "ImageIO.h"
#include "Tools.h"

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <algorithm>
#include <chrono>

#if defined(WIN32) || defined(_WIN32)
    #define DIRECTORY_SEPARATOR "\\"
#else
    #define DIRECTORY_SEPARATOR "/"
#endif

class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    double elapsed() {
        return std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - start
        ).count();
    }
};

void run_segmentation(
    const std::string& input_dir,
    const std::string& output_dir,
    double lambda,
    double sigma,
    int superpixels,
    bool four_connected,
    bool save_contour,
    bool save_mean,
    bool save_csv,
    bool verbose,
    const std::string& time_dir
) {
    using namespace boost::filesystem;

    path inputPath(input_dir);
    if (!is_directory(inputPath)) {
        throw std::runtime_error("Input directory not found: " + input_dir);
    }

    path outputPath(output_dir);
    if (!is_directory(outputPath)) {
        create_directories(outputPath);
    }

    std::vector<path> imagePaths;
    for (const auto& entry : directory_iterator(inputPath)) {
        if (is_regular_file(entry)) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png") {
                imagePaths.push_back(entry.path());
                if (verbose) {
                    std::cout << "Found: " << entry.path().string() << std::endl;
                }
            }
        }
    }

    std::sort(imagePaths.begin(), imagePaths.end());
    std::cout << imagePaths.size() << " images total..." << std::endl;

    int eightConnected = four_connected ? 0 : 1;
    MERCLazyGreedy merc;
    Timer timer;
    double totalTime = 0;
    cv::Mat timeMat(imagePaths.size(), 2, CV_64F);

    for (size_t idx = 0; idx < imagePaths.size(); ++idx) {
        const path& imagePath = imagePaths[idx];
        cv::Mat mat = cv::imread(imagePath.string());

        Image<RGBMap> inputImage;
        MERCInputImage<RGBMap> input;
        inputImage.Resize(mat.cols, mat.rows, false);

        for (int i = 0; i < mat.rows; ++i) {
            for (int j = 0; j < mat.cols; ++j) {
                RGBMap color((int)mat.at<cv::Vec3b>(i, j)[2],
                             (int)mat.at<cv::Vec3b>(i, j)[1],
                             (int)mat.at<cv::Vec3b>(i, j)[0]);
                inputImage.Access(j, i) = color;
            }
        }

        input.ReadImage(&inputImage, eightConnected);

        timer.restart();
        merc.ClusteringTreeIF(input.nNodes_, input, 0,
                              sigma * mat.channels(),
                              lambda * superpixels,
                              superpixels);
        double elapsed = timer.elapsed();
        totalTime += elapsed;
        timeMat.at<double>(idx, 0) = idx + 1;
        timeMat.at<double>(idx, 1) = elapsed;

        std::vector<int> labelsFlat = MERCOutputImage::DisjointSetToLabel(merc.disjointSet_);
        int** labels = new int*[mat.rows];
        for (int i = 0; i < mat.rows; ++i) {
            labels[i] = new int[mat.cols];
            for (int j = 0; j < mat.cols; ++j) {
                labels[i][j] = labelsFlat[j + i * mat.cols];
            }
        }

        Integrity::relabel(labels, mat.rows, mat.cols);

        std::string filenameBase = imagePath.filename().stem().string();
        std::string outputBase = outputPath.string() + DIRECTORY_SEPARATOR + filenameBase;

        if (save_contour) {
            cv::Mat contour = Draw::contourImage(labels, mat, std::vector<int>{0, 0, 204});
            cv::imwrite(outputBase + "_contours.png", contour);
            if (verbose) std::cout << "Saved contour to " << outputBase + "_contours.png" << std::endl;
        }

        if (save_mean) {
            cv::Mat meanImg = Draw::meanImage(labels, mat);
            cv::imwrite(outputBase + "_mean.png", meanImg);
            if (verbose) std::cout << "Saved mean image to " << outputBase + "_mean.png" << std::endl;
        }

        if (save_csv) {
            path csvFile(outputBase + ".csv");
            Export::CSV(labels, mat.rows, mat.cols, csvFile);
            if (verbose) std::cout << "Saved CSV to " << csvFile.string() << std::endl;
        }

        for (int i = 0; i < mat.rows; ++i) delete[] labels[i];
        delete[] labels;
    }

    if (!time_dir.empty()) {
        path timePath(time_dir);
        if (!is_directory(timePath)) create_directories(timePath);

        path perImgTimeFile = timePath / "eval_time_img.txt";
        path avgTimeFile = timePath / "eval_time.txt";

        Export::BSDEvaluationFile<double>(timeMat, 4, perImgTimeFile);

        cv::Mat avgTime(1, 1, CV_64F);
        avgTime.at<double>(0, 0) = totalTime / imagePaths.size();
        Export::BSDEvaluationFile<double>(avgTime, 6, avgTimeFile);
    }

    std::cout << "Average time per image: " << (totalTime / imagePaths.size()) << " seconds." << std::endl;
}
