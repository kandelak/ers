// ers_wrapper.h
#pragma once
#include <string>

void run_segmentation(
    const std::string& input_dir,
    const std::string& output_dir = "output",
    double lambda = 0.5,
    double sigma = 5.0,
    int superpixels = 400,
    bool four_connected = false,
    bool save_contour = false,
    bool save_mean = false,
    bool save_csv = false,
    bool verbose = false,
    const std::string& time_dir = ""
);
