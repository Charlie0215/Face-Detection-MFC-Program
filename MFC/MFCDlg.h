
// MFCDlg.h : 头文件
//

#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CvvImage.h"
#include "dirent.h"
#include "atltypes.h"
#include <opencv2/core/core.hpp>
#include "VideoInfo.h"

#define Device   0

using namespace cv;
using namespace std;

// CMFCDlg 对话框
class CMFCDlg : public CDialogEx
{
// 构造
public:
	CMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1Imagefile();
	CVideoInfo m_pVideoInfo;
	/*static VideoCapture stream2(int a);
	Mat cameraImg2;*/

	/*			Camera Opponent			*/			
	VideoCapture stream1;
	Mat cameraImg;

	int m_FrameWidth;          //图片宽度
	int m_FrameHeight;         //图片高度
	/*			Camera Opponent			*/


private:
	// 目录结构
	struct dirent *m_pEnt;
	// 目录结构
	DIR *m_pDir;
	//（图像文件路径
	CString m_Path;
	//文件结构
	char* m_ImageDir;
	//Haar Classifier Global variable
	static CvMemStorage* storage;
	////static CvHaarClassifierCascade* cascade; // Might not be used in C++11 env
	static CascadeClassifier haar_cascade;
	
	CRect m_PicCtlRect; //用来保存picture控件在客户区所处的位置
	HDC m_pPicCtlHdc; //用于保存控件的句柄

	// Detect Initialization
	bool m_boolInitOK;
	int m_genderLabel;
	bool m_boolFolderOrImage;
	//int m_boolCloseVideo;
	int m_ManNum;
	int m_WomanNum;
	int m_FrameNum;
	static int cameraFace;
	Mat ROI_Face;
	bool cameraCapture;

public:
	//afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton1Nextimage();
	void GetNextImage();
	//void detect_and_draw(Mat img);
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedButton1Nextimage();
	afx_msg void OnBnClickedButtonInitial();
	afx_msg void OnBnClickedFacealign();
	// Face Recognition Function
	void detect_and_draw_v2(const Mat& img, int num2Save = 0);
	afx_msg void OnCbnSelchangeComboFunction();
	void GederReognition(Mat img);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	afx_msg void OnBnClickedOpenvideo();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
private:
	// 指示当前的工作模式，true代表单张图片，False代表视频流
	bool m_boolModelFlag;
public:

	afx_msg void OnBnClickedCameracapture();
};

