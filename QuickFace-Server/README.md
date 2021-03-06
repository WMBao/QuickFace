# QuickFace 

Software Engineering Course Project(Autumn 2016)

Weiming Bao (collaborating with Bo Pang and Zifan Liu)

v 1.0.0 - 2017.1.6


## Requirements:

|Name|Version|
|:-----:|:-----:|
|boost |>=1.54.0|
|OpenCV |>=2.4.8|
|Eigen |>=3.2.0|
|a recent compiler |>=gcc-4.9|
|ubuntu (optional) |>=14.04|optional

#### 'apt-get' Instructions for Requiring Packages:

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-5 g++-5 libboost-all-dev libeigen3-dev libopencv-dev opencv-data>
```


## Configuration

1. Clone with submodules: ``git clone --recursive https://github.com/WMBao/QuickFace.git``, or, if you've already cloned it, get the submodules with ``git submodule update --init --recursive`` inside the Server-side source code directory.

2. Build and install the source code with the following instructions (Run outside the source directory):
```Shell
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc-5 -DCMAKE_CXX_COMPILER=g++-5 -DOpenCV_haarcascades_DIR=/usr/share/opencv/haarcascades/ ../QuickFace/
make
make install
```

## Citation:

* _Fitting 3D Morphable Models using Local Features_, P. Huber, Z. Feng, W. Christmas, J. Kittler, M. Rätsch, IEEE International Conference on Image Processing (ICIP) 2015, Québec City, Canada [[PDF]](http://arxiv.org/abs/1503.02330).

* _A Multiresolution 3D Morphable Face Model and Fitting Framework_, P. Huber, G. Hu, R. Tena, P. Mortazavian, W. Koppen, W. Christmas, M. Rätsch, J. Kittler, International Conference on Computer Vision Theory and Applications (VISAPP) 2016, Rome, Italy [[PDF]](http://www.patrikhuber.ch/files/3DMM_Framework_VISAPP_2016.pdf).
