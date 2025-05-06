from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os
import subprocess
import glob

__version__ = "0.0.1"

# Get the absolute path to the source files
source_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "ers_python"))
lib_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "lib_ers"))

# Get all C++ source files from lib_ers
lib_sources = glob.glob(os.path.join(lib_dir, "*.cpp"))

# Get OpenCV paths using pkg-config
def get_opencv_paths():
    try:
        # Try opencv4 first
        cflags = subprocess.check_output(['pkg-config', '--cflags', 'opencv4']).decode('utf-8').strip()
        libs = subprocess.check_output(['pkg-config', '--libs', 'opencv4']).decode('utf-8').strip()
    except subprocess.CalledProcessError:
        # Fall back to opencv
        cflags = subprocess.check_output(['pkg-config', '--cflags', 'opencv']).decode('utf-8').strip()
        libs = subprocess.check_output(['pkg-config', '--libs', 'opencv']).decode('utf-8').strip()
    
    include_dirs = [d[2:] for d in cflags.split() if d.startswith('-I')]
    library_dirs = [d[2:] for d in libs.split() if d.startswith('-L')]
    libraries = [d[2:] for d in libs.split() if d.startswith('-l')]
    
    return include_dirs, library_dirs, libraries

# Get Boost paths
def get_boost_paths():
    boost_include = "/opt/homebrew/opt/boost/include"
    boost_lib = "/opt/homebrew/opt/boost/lib"
    return [boost_include], [boost_lib], ["boost_timer"]

opencv_include_dirs, opencv_library_dirs, opencv_libraries = get_opencv_paths()
boost_include_dirs, boost_library_dirs, boost_libraries = get_boost_paths()

ext_modules = [
    Pybind11Extension(
        "ers_py",  # Module name exposed to Python
        ["ers_python/ers_wrapper.cpp"] + lib_sources,  # Include all source files
        define_macros=[("VERSION_INFO", __version__)],
        cxx_std=17,
        include_dirs=[source_dir, lib_dir] + opencv_include_dirs + boost_include_dirs,
        library_dirs=opencv_library_dirs + boost_library_dirs,
        libraries=opencv_libraries + boost_libraries,
    ),
]

setup(
    name="ers_py",
    version=__version__,
    author="Aleksandre Kandelaki",
    author_email="kandelakialeksandre@gmail.com",
    description="Python bindings for ERS superpixel segmentation using pybind11",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": ["pytest"]},
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=[
        "numpy>=1.19.0",
        "pybind11>=2.6.0",
    ],
    packages=["ers_python"],
)
