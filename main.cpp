#include <iostream>

#include <opencv2/opencv.hpp>
//#include <opencv/cxcore.hpp>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <zconf.h>

namespace fs = std::experimental::filesystem;


int main() {
  std::string
      workingDir = "/home/jglanz/Desktop/calibration-images";

  std::string
      cam0Dir = workingDir + "/cam0",
      cam1Dir = workingDir + "/cam1",
      cam2Dir = workingDir + "/cam2",
      cam3Dir = workingDir + "/cam3";

  for (auto & p : fs::directory_iterator(fs::u8path(workingDir))) {
    auto filename = p.path().string();
    if (filename.length() < 5)
      continue;

    auto ext = filename.substr(filename.length() - 4,4);
    std::cout << "Ext: " << ext << std::endl;


    if (ext != ".png")
      continue;


    auto lastSlash = filename.find_last_of('/');
    auto baseFilename = filename.substr(lastSlash + 1, filename.length() - lastSlash - 1);
    std::cout << "File: " << baseFilename << std::endl;

    auto srcMat = cv::imread(filename);
    auto totalSize = srcMat.total();

    cv::Mat
        img0(960,1280,CV_8UC3),
        img1(960,1280,CV_8UC3),
        img2(960,1280,CV_8UC3),
        img3(960,1280,CV_8UC3);

    register unsigned char *srcData, * destData, *destData1,*destData2,*destData3,*destData4;

    int width = 2560, height = 1920, bpp = 3;
    register int frameSize = width * height * bpp;


    srcData = srcMat.data;


    register unsigned char b1,b2,b3;
    register unsigned int offset = 0;
    //register int halfWidth = width / 2;
    register int column = -1, row = 0, realColumn,realOffset, singleWidth = width / 2;
    register int i;
    register int halfWay = frameSize / 2;

    // TODO: WHY IS THIS 39??
    for (i = 0; i < frameSize; i+=bpp) {
      column++;
      if (column >= width) {
        column = 0;
        row++;
      }

      if (row >= height / 2)
        row = 0;

      destData = (i < halfWay) ?
                 (column < singleWidth  ? img0.data : img1.data) :
                 (column < singleWidth ? img2.data : img3.data);

      realColumn = column % singleWidth;
      realOffset = row * 3 * singleWidth + (realColumn * 3);

      b1 = srcData[i];
      b2 = srcData[i + 1];
      b3 = srcData[i + 2];

      destData[realOffset] = b1;
      destData[realOffset + 1] = b2;
      destData[realOffset + 2] = b3;
    }


    fs::create_directories(fs::u8path(cam0Dir));
    fs::create_directories(fs::u8path(cam1Dir));
    fs::create_directories(fs::u8path(cam2Dir));
    fs::create_directories(fs::u8path(cam3Dir));

    auto cam0Filename = cam0Dir + "/" + baseFilename;

    cv::imwrite(cam0Filename,img0);
    cv::imwrite(cam1Dir + "/" + baseFilename,img1);
    cv::imwrite(cam2Dir + "/" + baseFilename,img2);
    cv::imwrite(cam3Dir + "/" + baseFilename,img3);

    std::cout << "Cam 0 filename: " << cam0Filename << std::endl;
    std::cout << "Total: " << totalSize << std::endl;


  }
  return 0;
}