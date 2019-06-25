//
// Created by danbogh on 2017/9/12.
//

#ifndef OPENCV_FACE_DETECTION_SCANNER_H
#define OPENCV_FACE_DETECTION_SCANNER_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace scanner {

    class Scanner {
    public:
        int resizeThreshold = 500;

        Scanner(Mat &bitmapMat);

        virtual ~Scanner();

        vector<Point> scanPoint();

    private:
        Mat srcBitmapMat;
        float resizeScale = 1.0f;

        Mat resizeImage();

        Mat preprocessImage(Mat &image);

        vector<Point> selectPoints(vector<Point> points, int selectTimes);

        vector<Point> sortPointClockwise(vector<Point> vector);

        long long pointSideLine(Point &lineP1, Point &lineP2, Point &point);
    };
}

#endif //OPENCV_FACE_DETECTION_SCANNER_H
