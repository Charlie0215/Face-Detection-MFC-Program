#include "stdafx.h"
#include "VideoInfo.h"


CVideoInfo::CVideoInfo()
{
	
}

void CVideoInfo::openCamera()
{
	//stream1 = VideoCapture(0);
	stream1 = new VideoCapture(0);

	if (!stream1->isOpened()){

		AfxMessageBox(_T("cannot open camera"));

		return;
	}

	*stream1 >> cameraImg;

	

	return;
}

void CVideoInfo::extractCameraFrame()
{
	*stream1 >> cameraImg;
	//flip(cameraImg, cameraImg, 1);
	//imshow("cool", cameraImg);

}

CVideoInfo::~CVideoInfo()
{

}
