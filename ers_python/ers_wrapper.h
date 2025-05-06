#pragma once

#include "MERCLazyGreedy.h"
#include "MERCInputImage.h"
#include "MERCOutputImage.h"
#include "Image.h"
#include "ImageIO.h"
#include "Tools.h"
#include <opencv2/opencv.hpp>

void run_ers_segmentation(const cv::Mat& input_image, double lambda, double sigma, int superpixels, bool four_connected);
