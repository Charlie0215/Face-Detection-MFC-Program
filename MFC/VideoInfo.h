#pragma once

#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

class CVideoInfo
{
public:
	VideoCapture* stream1;
	Mat cameraImg;

	int m_FrameWidth;          //Í¼Æ¬¿í¶È
	int m_FrameHeight;         //Í¼Æ¬¸ß¶È

	void extractCameraFrame();
	void openCamera();
	CVideoInfo();
	~CVideoInfo();
};

