
// MFCDlg.h : ͷ�ļ�
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

// CMFCDlg �Ի���
class CMFCDlg : public CDialogEx
{
// ����
public:
	CMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	int m_FrameWidth;          //ͼƬ���
	int m_FrameHeight;         //ͼƬ�߶�
	/*			Camera Opponent			*/


private:
	// Ŀ¼�ṹ
	struct dirent *m_pEnt;
	// Ŀ¼�ṹ
	DIR *m_pDir;
	//��ͼ���ļ�·��
	CString m_Path;
	//�ļ��ṹ
	char* m_ImageDir;
	//Haar Classifier Global variable
	static CvMemStorage* storage;
	////static CvHaarClassifierCascade* cascade; // Might not be used in C++11 env
	static CascadeClassifier haar_cascade;
	
	CRect m_PicCtlRect; //��������picture�ؼ��ڿͻ���������λ��
	HDC m_pPicCtlHdc; //���ڱ���ؼ��ľ��

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
	// ָʾ��ǰ�Ĺ���ģʽ��true������ͼƬ��False������Ƶ��
	bool m_boolModelFlag;
public:

	afx_msg void OnBnClickedCameracapture();
};

