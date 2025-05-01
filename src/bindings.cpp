// bindings.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include "ers_wrapper.h"

namespace py = pybind11;

PYBIND11_MODULE(ers, m) {
    m.doc() = R"pbdoc(
        Entropy Rate Superpixel Segmentation (ERS) Python wrapper
        
        This module provides Python bindings for the ERS algorithm, which performs
        superpixel segmentation on images using entropy rate clustering.
        
        Example:
            >>> import ers
            >>> ers.run_segmentation("input_images", "output", lambda=0.5, sigma=5.0)
    )pbdoc";

    m.def("run_segmentation", 
          [](const std::string& input_dir,
             const std::string& output_dir,
             double lambda,
             double sigma,
             int superpixels,
             bool four_connected,
             bool save_contour,
             bool save_mean,
             bool save_csv,
             bool verbose,
             const std::string& time_dir) {
              try {
                  // Validate parameters
                  if (lambda <= 0) {
                      throw std::invalid_argument("lambda must be positive");
                  }
                  if (sigma <= 0) {
                      throw std::invalid_argument("sigma must be positive");
                  }
                  if (superpixels <= 0) {
                      throw std::invalid_argument("superpixels must be positive");
                  }
                  
                  return run_segmentation(input_dir, output_dir, lambda, sigma,
                                        superpixels, four_connected, save_contour,
                                        save_mean, save_csv, verbose, time_dir);
              } catch (const std::exception& e) {
                  throw py::error_already_set();
              }
          },
          py::arg("input_dir").doc("Directory containing input images"),
          py::arg("output_dir") = "output",
          py::arg("lambda") = 0.5,
          py::arg("sigma") = 5.0,
          py::arg("superpixels") = 400,
          py::arg("four_connected") = false,
          py::arg("save_contour") = false,
          py::arg("save_mean") = false,
          py::arg("save_csv") = false,
          py::arg("verbose") = false,
          py::arg("time_dir") = "",
          R"pbdoc(
          Run ERS segmentation on a folder of images.
          
          Args:
              input_dir (str): Directory containing input images
              output_dir (str, optional): Output directory. Defaults to "output"
              lambda (float, optional): Balance parameter. Defaults to 0.5
              sigma (float, optional): Gaussian kernel parameter. Defaults to 5.0
              superpixels (int, optional): Number of superpixels. Defaults to 400
              four_connected (bool, optional): Use 4-connected neighborhood. Defaults to False
              save_contour (bool, optional): Save contour images. Defaults to False
              save_mean (bool, optional): Save mean images. Defaults to False
              save_csv (bool, optional): Save CSV files. Defaults to False
              verbose (bool, optional): Enable verbose output. Defaults to False
              time_dir (str, optional): Directory to save timing information. Defaults to ""
              
          Returns:
              None
              
          Raises:
              ValueError: If input parameters are invalid
              RuntimeError: If segmentation fails
          )pbdoc");
}
