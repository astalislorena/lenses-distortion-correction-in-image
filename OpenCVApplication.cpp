#include "stdafx.h"
#include "common.h"
#include <vector>
#include <queue>
#include <random>
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <regex>

void getParams(std::vector<float>* paramaters) {
	char filename[MAX_PATH] = {};
	std::ifstream input("Resources/parameters.txt");
	if (!input.is_open()) {
		std::cout << "Could not open file Reources/parameters.txt\n";
		return;
	}
	double number = 0;
	while (input >> number) { 
		paramaters->push_back(number);
	
	}
}

bool isInside(Mat src, int i, int j) {
	if (i >= 0 && i < src.rows && j >= 0 && j < src.cols) {
		return true;
	}
	return false;
}

void distorsionElimination(std::vector<float> paramaters) {
	float fx = paramaters.at(0), fy = paramaters.at(1), u0 = paramaters.at(2), v0 = paramaters.at(3), k1 = paramaters.at(4), k2 = paramaters.at(5), p1 = paramaters.at(6), p2 = paramaters.at(7);
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname, IMREAD_GRAYSCALE);
		Mat dst(src.rows, src.cols, IMREAD_GRAYSCALE, Scalar(0, 0, 0));
		Mat dst2(src.rows, src.cols, IMREAD_GRAYSCALE, Scalar(0, 0, 0));
		for (int u = 0; u < dst.rows; u++) {
			for (int v = 0; v < dst.cols; v++) {
				// Coords in image plan
				float x = (u - v0) / fx;
				float y = (v - u0) / fy;

				// Coords in distors image
				float r2 = pow(x, 2) + pow(y, 2);
				float dx = x * (k1 * r2 + k2 * pow(r2, 2)) + 2 * p1 * x * y + p2 * (r2 + 2 * pow(x, 2));
				float dy = y * (k1 * r2 + k2 * pow(r2, 2)) + p1 * (r2 + 2 * pow(y, 2)) + 2 * p2 * x * y;

				// Coords in pixels in distors image
				float x_ = x + dx;
				float y_ = y + dy;
				float u_ = v0 + x_ * fx;
				float v_ = u0 + y_ * fy;

				// Interpolation
				int u00 = (int)u_;
				int v00 = (int)v_;
				int u01 = u00 + 1;
				int v01 = v00 + 1;

				if (isInside(src, u00, v00) && isInside(src, u00, v01) && isInside(src, u01, v00) && isInside(src, u01, v01)) {
					float i00 = src.at<uchar>(u00, v00) * (float) (u01 - u_) + src.at<uchar>(u00, v01) * (float) (u_ - u00);
					float i01 = src.at<uchar>(u00, v01) * (float) (u01 - u_) + src.at<uchar>(u01, v01) * (float) (u_ - u00);
					dst.at<uchar>(u, v) = i00 * (v01 - v_) + i01 * (v_ - v00);
				}
				dst2.at<uchar>(u, v) = src.at<uchar>(u_, v_);
			}
		}
		Mat A = (cv::Mat_<double>(3, 3) << fx, 0.0, u0, 0.0, fy, v0, 0.0, 0.0, 1.0);
		Mat distortionCoef = (cv::Mat_<double>(1, 4) << k1, k2, p1, p2);
		Mat openCVDst;
		undistort(src, openCVDst, A, distortionCoef);
		imshow("Source image", src);
		imshow("Undistorted image with interpolation", dst);
		imshow("Undistorted image without interpolation", dst2);
		imshow("OpenCV undistorted image", openCVDst);
		imwrite("open_cv_result.png", openCVDst);
		imwrite("result_with_interpolation.png", dst);
		imwrite("result_withthout_interpolation.png", dst2);
		waitKey();
	}
}

int main()
{
	system("cls");
	destroyAllWindows();
	std::vector<float> paramaters;
	getParams(&paramaters);
	distorsionElimination(paramaters);
	return 0;
}