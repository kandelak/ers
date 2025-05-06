#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include "ers_wrapper.h"

namespace py = pybind11;

// Original segmentation function
void run_ers_segmentation(const cv::Mat& mat, double lambda, double sigma, int superpixels, bool four_connected) {
    Image<RGBMap> inputImage;
    MERCInputImage<RGBMap> input;
    MERCLazyGreedy merc;

    inputImage.Resize(mat.cols, mat.rows, false);

    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            RGBMap color((int) mat.at<cv::Vec3b>(i, j)[2],
                         (int) mat.at<cv::Vec3b>(i, j)[1],
                         (int) mat.at<cv::Vec3b>(i, j)[0]);
            inputImage.Access(j, i) = color;
        }
    }

    input.ReadImage(&inputImage, four_connected ? 0 : 1);

    merc.ClusteringTreeIF(input.nNodes_, input, 0, sigma * mat.channels(), lambda * superpixels, superpixels);

    std::vector<int> label = MERCOutputImage::DisjointSetToLabel(merc.disjointSet_);

    int** labels = new int*[mat.rows];
    for (int i = 0; i < mat.rows; ++i) {
        labels[i] = new int[mat.cols];
        for (int j = 0; j < mat.cols; ++j) {
            labels[i][j] = label[j + i * mat.cols];
        }
    }

    Integrity::relabel(labels, mat.rows, mat.cols);

    // Clean up if needed, or add code to return labels if desired.
}

// Python wrapper
void run_ers_segmentation_py(py::array_t<uint8_t>& input_array,
                             double lambda, double sigma, int superpixels, bool four_connected) {
    py::buffer_info buf = input_array.request();
    if (buf.ndim != 3 || buf.shape[2] != 3) {
        throw std::runtime_error("Input image must be a 3D array with 3 channels (RGB)");
    }

    int height = buf.shape[0];
    int width = buf.shape[1];

    // Wrap numpy array into OpenCV Mat
    cv::Mat mat(height, width, CV_8UC3, (unsigned char*) buf.ptr);
    run_ers_segmentation(mat, lambda, sigma, superpixels, four_connected);
}

// Pybind11 module
PYBIND11_MODULE(ers_py, m) {
    m.def("run_ers_segmentation", &run_ers_segmentation_py,
          py::arg("image"),
          py::arg("lambda"),
          py::arg("sigma"),
          py::arg("superpixels"),
          py::arg("four_connected"),
          "Run ERS superpixel segmentation on an RGB image.");
}
