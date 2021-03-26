#pragma once

#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

class CVideoInfo
{
public:
	VideoCapture* stream1;
	Mat cameraImg;

	int m_FrameWidth;          //ͼƬ���
	int m_FrameHeight;         //ͼƬ�߶�

	void extractCameraFrame();
	void openCamera();
	CVideoInfo();
	~CVideoInfo();
};

