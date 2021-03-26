
// MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <afxwin.h>
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "VideoInfo.h"
#include "opencv2/face.hpp"

using namespace cv::ml;
using namespace cv::face;
using namespace std;
using namespace cv;
////////////////////////////////////////////////////////////////////////////
/*
A static member is shared by all objects of the class. 
All static data is initialized to zero when the first object is created, if no other initialization is present. 
We can't put it in the class 
definition but it can be initialized outside the class 
using the scope resolution operator :: to identify which class it belongs to.
*/
CvMemStorage* CMFCDlg::storage = NULL;
CascadeClassifier CMFCDlg::haar_cascade;
int CMFCDlg::cameraFace = 0;
//////////////////////////////////////////////////////////////////////////////


Ptr<FaceRecognizer> modelPCA = createEigenFaceRecognizer();
Ptr<FaceRecognizer> modelFisher = createFisherFaceRecognizer();
Ptr<FaceRecognizer> modelLBP = createLBPHFaceRecognizer();
//static CvSVM svm;
static Ptr<SVM> svm;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCameracapture();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)

END_MESSAGE_MAP()


// CMFCDlg �Ի���



CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDlg::IDD, pParent)
	, m_pEnt(NULL)
	, m_pDir(NULL)
	, m_boolInitOK(false)
	, m_genderLabel(0)
	//, m_boolCloseVideo(0)
	, m_boolFolderOrImage(false)
	, m_boolModelFlag(false)
	, cameraCapture(false)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1_ImageFile, &CMFCDlg::OnBnClickedButton1Imagefile)
	//ON_BN_CLICKED(IDC_BUTTON1, &CMFCDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON1_NextImage, &CMFCDlg::OnBnClickedButton1Nextimage)
	ON_STN_CLICKED(IDC_PICTURE, &CMFCDlg::OnStnClickedPicture)
	ON_BN_CLICKED(IDC_BUTTON1_NextImage, &CMFCDlg::OnBnClickedButton1Nextimage)
	ON_BN_CLICKED(IDC_BUTTON_INITIAL, &CMFCDlg::OnBnClickedButtonInitial)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNCTION, &CMFCDlg::OnCbnSelchangeComboFunction)

	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_OpenVideo, &CMFCDlg::OnBnClickedOpenvideo)
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_FaceAlign, &CMFCDlg::OnBnClickedFacealign)
	ON_BN_CLICKED(IDC_CameraCapture, &CMFCDlg::OnBnClickedCameracapture)
END_MESSAGE_MAP()


// CMFCDlg ��Ϣ�������

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/******Initialize drop down list*****/

	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString(_T("Apply PCA"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString(_T("Apply Fisher"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString(_T("Apply LPB"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString(_T("Apply SVM"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->SetCurSel(1);


	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("7"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("9"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("11"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("13"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->AddString(_T("15"));
	((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->SetCurSel(4);


	/*********��ʼ��picture�ؼ�**********/
          //�����ؼ�λ��

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_pPicCtlHdc = GetDlgItem(IDC_PICTURE)->GetDC()->GetSafeHdc();   //���ؿؼ����
	GetDlgItem(IDC_PICTURE)->GetClientRect(m_PicCtlRect);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCDlg::OnStnClickedPicture()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������



}



void CMFCDlg::OnCbnSelchangeComboFunction()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������



}

/////////////////////////////////////////////////////////////////////////////////////////

void CMFCDlg::OnBnClickedButton1Imagefile()
{

	SetDlgItemText(IDC_BUTTON1_NextImage, _T("��һ��"));//ˢ�¿ؼ�
	/****detect initialization*****/
	if (m_boolInitOK == false)
	{
		AfxMessageBox(_T("Initialization!!!"));
		return;
	}

	/*******Model Selection TRUE - Single Picture ; False - Multiple Frame********/
	m_boolModelFlag = TRUE;

	if (!m_boolFolderOrImage){

		/**********��ʼ������**********/
		CString str;                                      //�洢ͼ��·�� Cstring object consist of a variable-length squence of characters. Base on the TCHAR data type. 16 bit character type
		BROWSEINFO bi;                                    //�����洢�û�ѡ�е�Ŀ¼��Ϣ
		TCHAR name[MAX_PATH];                             //�洢·�� In the Windows API (with some exceptions discussed in the following paragraphs), the maximum length for a path is MAX_PATH, which is defined as 260 characters.
		ZeroMemory(&bi, sizeof(BROWSEINFO));               //���Ŀ¼��Ӧ���ڴ�
		bi.hwndOwner = GetSafeHwnd();                //�õ����ھ�� Returns the window handle for a window. Useful to manipulate a window associate with a console application
		bi.pszDisplayName = name;

		/**********���öԻ��򲢶�ȡĿ¼��Ϣ**********/
		BIF_BROWSEINCLUDEFILES; // The browse dialog box can display URLs
		bi.lpszTitle = _T("Select folder");           //�Ի������ "Selector Folder" is Unicode wide charactor
		bi.ulFlags = 0x80;                          //���öԻ�����ʽ // The browse dialog box can display URLs
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);        //������ѡ���ļ��е�ID
		SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH)); //���ļ���Ϣ��ʽ���洢����Ӧ��������
		str.ReleaseBuffer();                              //��GerBuffer���ʹ�ã�����ڴ�
		m_Path = str;                                       //��·���洢��m_path��
		if (str.GetAt(str.GetLength() - 1) != '\\')
			m_Path += "\\";
		UpdateData(FALSE);

		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(idl);
			imalloc->Release();
		}

		/**********��ȡ��·���µĵ�һ���ļ�**********/
		//m_ImageDir = (LPSTR)(LPCTSTR)m_Path;

		int len = WideCharToMultiByte(CP_ACP, 0, m_Path, -1, NULL, 0, NULL, NULL);
		m_ImageDir = new char[len + 1];
		////char* m_ImageDir[len + 1];
		WideCharToMultiByte(CP_ACP, 0, m_Path, -1, m_ImageDir, len, NULL, NULL);


		m_pDir = opendir(m_ImageDir);
		//for (int i = 0; i < 1; i++)                     //����Ŀ¼ ..   ��  .
		//{
		//	m_pEnt = readdir(m_pDir); //readdir���������ܹ�ʵ�ֶԵ�ǰĿ¼�ṹ��m_pDir���е��ļ������ظ�˳���ȡ

		//}

		GetNextImage();
	}
	else
	{
		//CFileDialog  FDlg(TRUE);
		//if (FDlg.DoModal() == IDOK)
		//{
		//	m_Path = FDlg.GetPathName();
		//	UpdateData(false);
		//}

		////Identify img file


		//char* jpg = strstr((LPSTR)(LPCTSTR)m_Path, ".jpg");
		//char* bmp = strstr((LPSTR)(LPCTSTR)m_Path, ".bmp");
		//char* png = strstr((LPSTR)(LPCTSTR)m_Path, ".png");

		/**********ͨ�����ļ��Ի��������Ŀ���ļ���·��**********/
		CFileDialog  FDlg(TRUE);
		if (FDlg.DoModal() == IDOK)
		{
			m_Path = FDlg.GetPathName();
			UpdateData(false);
		}

		/**********�ж��Ƿ�Ϊͼ���ļ�**********/
		//char* jpg = strstr((LPSTR)(LPCTSTR)m_Path, ".jpg");
		//char* bmp = strstr((LPSTR)(LPCTSTR)m_Path, ".bmp");
		//char* png = strstr((LPSTR)(LPCTSTR)m_Path, ".png");



		//if (jpg == NULL && bmp == NULL && png == NULL){

		//	MessageBox(_T("Not An Image File"));

		//	return;
		//}
		

			int len = WideCharToMultiByte(CP_ACP, 0, m_Path, -1, NULL, 0, NULL, NULL);
			m_ImageDir = new char[len + 1];
			////char* m_ImageDir[len + 1];
			WideCharToMultiByte(CP_ACP, 0, m_Path, -1, m_ImageDir, len, NULL, NULL);

			char imageFullName[500];                  //����ͼ���ļ���ȫ·��
			Mat imageSrc;
			//sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��
			imageSrc = imread(m_ImageDir);		//Read image and store in mat
			detect_and_draw_v2(imageSrc);
		
	}


	

}





void CAboutDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CMFCDlg::GetNextImage()
{

		if (m_pDir && (m_pEnt = readdir(m_pDir)) != NULL){

			/**********�ж��Ƿ�Ϊͼ���ļ�**********/
			char* jpg = strstr(m_pEnt->d_name, ".jpg");
			char* bmp = strstr(m_pEnt->d_name, ".bmp");
			char* png = strstr(m_pEnt->d_name, ".png");
	
			if (jpg == NULL && bmp == NULL && png == NULL)
				GetNextImage();
			else {
				/**********��ʾ��ͼƬ**********/

				//////////////////////////////////////////////////////////1. USE IplImage Directly////////////////////////////////////
				//IplImage *imageSrc;
				//CvvImage imageSrcCvvImg;
				//char imageFullName[500];                  //����ͼ���ļ���ȫ·��

				//sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��
				//imageSrc = cvLoadImage(imageFullName);
				//imageSrcCvvImg.CopyOf(imageSrc);
				//imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
				//cvReleaseImage(&imageSrc);

				//////////////////////////////////////////////////////////////////2. USE Mat/////////////////////////////////////////////////////////////////////////////////////////
				//CvvImage imageSrcCvvImg;
				//char imageFullName[500];                  //����ͼ���ļ���ȫ·��
				//Mat imageSrc;
				////IplImage in;
				//IplImage* input;
				//sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��
				//imageSrc = imread(imageFullName);		//Read image and store in mat
				//input = cvCloneImage(&(IplImage)imageSrc); // IplImage* cvt Mat
				////in = imageSrc;
				////input = &in;
				////input = &imageSrc;*/
				//imageSrcCvvImg.CopyOf(input);
				//imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
				//cvReleaseImage(&input);
				////cvReleaseImage(in);
				
				/*Face Recongnition*/
				char imageFullName[500];                  //����ͼ���ļ���ȫ·��
				Mat imageSrc;
				sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��
				imageSrc = imread(imageFullName);		//Read image and store in mat
				detect_and_draw_v2(imageSrc);

				


				//////////////////////////////////////////////////////////////////3. Wrong convertion///////////////////////////////////////////////////////////
				//Mat input;
				//CvvImage imageSrcCvvImg;
				//char imageFullName[500];                  //����ͼ���ļ���ȫ·��

				//IplImage imgTmp = input;
				//IplImage*  imageSrc = cvCloneImage(&imgTmp);


				//sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��

				//imageSrc = cvLoadImage(imageFullName);
				//imageSrcCvvImg.CopyOf(imageSrc);
				//imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
				//cvReleaseImage(&imageSrc);

				
			}
			cv::waitKey(0);
		}

}


void CMFCDlg::detect_and_draw_v2(const Mat& img, int num2Save)
{

	//����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
	

	Mat grayImage;
	Mat Img = img;
	if (Img.channels() == 3)
		cv::cvtColor(Img, grayImage, CV_BGR2GRAY);
	else
		grayImage = Img;

	cv::normalize(grayImage, grayImage, 0, 255, NORM_MINMAX, CV_8UC1);
	
	/***********��ʼ�����������ر���***********/
	cvClearMemStorage(storage);
	/*CascadeClassifier haar_cascade;*/
	vector<Rect> object;
	/*haar_cascade.detectMultiScale(grayImage, object, 1.2, 2, 0 | CV_HAAR_DO_CANNY_PRUNING);*/

	haar_cascade.detectMultiScale(grayImage, object, 2, 2, CV_HAAR_DO_ROUGH_SEARCH | CV_HAAR_FIND_BIGGEST_OBJECT);
	//CvSeq* objects = cvHaarDetectObjects(grayImage, cascade, storage, 1.1, 2, 0, cvSize(30, 30));
	cout << object.size() << endl;
	//for (vector<cv::Rect>::const_iterator r = object.begin(); r != object.end(); r++)

	/******Largest Object********/

	int maxface = 0;
	int max_area = 0;

	if (object.size() > 0){

		for (int i = 0; i < object.size(); i++){

			if (object[i].x * object[i].y > max_area){
				max_area = object[i].x * object[i].y;
				maxface = i;

			}

		}

			//rectangle(cameraImg, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), Scalar(0, 255, 0), 1, 8, 0);
			rectangle(Img, cvPoint(object[maxface].x, object[maxface].y), cvPoint(object[maxface].x + object[maxface].width, object[maxface].y + object[maxface].height), Scalar(0, 0, 255), 1, 8, 0);
			Rect rect1(object[maxface].x, object[maxface].y, object[maxface].width, object[maxface].height);
			
			Img(rect1).copyTo(ROI_Face);
			//Mat ROI_Face = img(Rect(object[maxface].x, object[maxface].y, object[maxface].x + object[maxface].width, object[maxface].y + object[maxface].height));

			/**********����ֹ�����ڶ�����������˵����Ҫ��������ͼ��ı���**********/
			
			if (cameraCapture == true){
				KillTimer(1);
				stringstream ss;
				ss << cameraFace;
				string s1 = ss.str();
				string dir = "C:\\Users\\Owner\\Desktop\\���������ָ�\\" + s1 + ".png";
				imwrite(dir, ROI_Face);
				cameraFace++;
				cameraCapture = false;
				SetTimer(1, 100, NULL);
				
			}



			if (num2Save != 0){
				stringstream ss;
				ss << num2Save; // int to char
				string s1 = ss.str(); // char to string
				string dir = "C:\\Users\\Owner\\Desktop\\���������ָ�\\" + s1 + ".bmp";
				imwrite(dir,ROI_Face);
			}
			GederReognition(ROI_Face);
			ROI_Face.release();
	}

	
	

	CvvImage imageSrcCvvImg;
	IplImage* input;

	input = cvCloneImage(&(IplImage)img);

	imageSrcCvvImg.CopyOf(input);
	imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
	cvReleaseImage(&input);

}



void CMFCDlg::OnBnClickedButton1Nextimage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static int count = 0;
	if (m_boolModelFlag == false)
	{
		if (count % 2 == 0){
		SetDlgItemText(IDC_BUTTON1_NextImage, _T("Start"));
		KillTimer(1);
		count = 1;
		}
		else
		{
			SetDlgItemText(IDC_BUTTON1_NextImage, _T("Pause"));
			SetTimer(1, 100, NULL);
			count = 0;
		}
	}
	else
	{
		GetNextImage();
	}
}


void CMFCDlg::OnBnClickedButtonInitial()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*****Face Recognition Code*****/
	m_boolInitOK = true;
	storage = cvCreateMemStorage(0);
	haar_cascade.load("C:\\Users\\Owner\\Downloads\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
		



	/**********�����û���ѡ�������ط�����**********/
	int index = 0;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->GetCurSel();
	switch (index)
	{
	case 0:
		modelPCA->load("D:\\Desktop\\data\\data\\�Ա�ʶ�����ݿ⡪CAS-PEAL\\new\\PCA_Model.xml");
		break;
	case 1:
		modelFisher->load("D:\\Desktop\\data\\data\\�Ա�ʶ�����ݿ⡪CAS-PEAL\\new\\Fisher_Model.xml");
		break;
	case 2:
		modelLBP->load("D:\\Desktop\\data\\data\\�Ա�ʶ�����ݿ⡪CAS-PEAL\\new\\LBP_Model.xml");
		break;
	case 3:
		//svm.load("C:\\Users\\Owner\\Desktop\\�Ա�ʶ�����ݿ⡪CAS-PEAL\\SVM_SEX_Model.txt");
		svm = Algorithm::load<SVM>("C:\\Users\\Owner\\Desktop\\�Ա�ʶ�����ݿ⡪CAS-PEAL\\SVM_SEX_Model.txt");
		break;
	default:
		break;
	}



	MessageBox(_T("��ʼ�����"));
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}


void CMFCDlg::GederReognition(Mat img)
{
	int idFrameNum = 0;
	idFrameNum = ((CComboBox*)GetDlgItem(IDC_COMBO_NUM))->GetCurSel();
	switch (idFrameNum)
	{
	case 0:
	{
		idFrameNum = 1;
		break;
	}
	case 1:
	{
		idFrameNum = 3;
		break;
	}
	case 2:
	{
		idFrameNum = 5;
		break;
	}
	case 3:
	{
		idFrameNum = 7;
		break;
	}
	case 4:
	{
		idFrameNum = 9;
		break;
	}
	case 5:
	{
		idFrameNum = 11;
		break;
	}
	case 6:
	{
		idFrameNum = 13;
		break;
	}
	case 7:
	{
		idFrameNum = 15;
		break;
	}

	default:
	{
		break;
	}

	}


	int index = 0;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->GetCurSel();
	switch (index)
	{
	case 0:
	{
		resize(img, img, Size(92, 112));
		cv::cvtColor(img, img, CV_BGR2GRAY);
		m_genderLabel = modelPCA->predict(img);
		break;
	}
	case 1:
	{
		resize(img, img, Size(92, 112));
		cv::cvtColor(img, img, CV_BGR2GRAY);
		m_genderLabel = modelFisher->predict(img);
		break;
	}
	case 2:
	{
		resize(img, img, Size(92, 112));
		cv::cvtColor(img, img, CV_BGR2GRAY);
		m_genderLabel = modelLBP->predict(img);
		break;
	}
	case 3:
	{
		resize(img, img, Size(64, 64));
		HOGDescriptor *hog = new HOGDescriptor(cvSize(64, 64), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);
		vector<float>descriptors;
		hog->compute(img, descriptors, Size(1, 1), Size(0, 0));
		Mat SVMtrainMat = Mat::zeros(1, descriptors.size(), CV_32FC1);
		int n = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			SVMtrainMat.at<float>(0, n) = *iter;
			n++;
		}
		//m_genderLabel = svm.predict(SVMtrainMat);
		m_genderLabel = svm->predict(SVMtrainMat);


		break;
	}
	default:
	{
		break;
	}
	}
	/**********��ʾʶ����**********/
	if (m_boolModelFlag == TRUE)
	{
		if (m_genderLabel == 1)
		{
			GetDlgItem(IDC_EDIT_RecognitionResult)->SetWindowText(_T("Boy"));
		}
		else if (m_genderLabel == 2)
		{
			GetDlgItem(IDC_EDIT_RecognitionResult)->SetWindowText(_T("Girl"));
		}
	}
	else
	{
		// If Male
		if (m_genderLabel == 1){
			m_ManNum++;
		}
		else if (m_genderLabel == 2){
			m_WomanNum++;
		}
		m_FrameNum++;
		if (m_FrameNum == idFrameNum){
			if (m_ManNum > m_WomanNum){
				GetDlgItem(IDC_EDIT_RecognitionResult)->SetWindowText(_T("Boy"));
			}
			else
			{
				GetDlgItem(IDC_EDIT_RecognitionResult)->SetWindowText(_T("Girl"));
			}
			m_ManNum = 0;
			m_WomanNum = 0;
			m_FrameNum = 0;
		}
	}
	
}



void CMFCDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_boolFolderOrImage = !m_boolFolderOrImage;
	if (m_boolFolderOrImage == FALSE)
	{
	
		SetDlgItemText(IDC_BUTTON1_ImageFile, _T("Image Folder"));
	}
	else
	{
		SetDlgItemText(IDC_BUTTON1_ImageFile, _T("Image File"));
	}

	//m_boolCloseVideo = 1;

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CMFCDlg::OnBnClickedOpenvideo()
{
	SetDlgItemText(IDC_BUTTON1_NextImage, _T("Pause"));//ˢ�¿ؼ�//ˢ�¿ؼ�
	if (m_boolInitOK == FALSE)
	{
		MessageBox(_T("���Ƚ��г�ʼ��"));
		return;
	}


	/*******Model Selection TRUE - Single Picture ; False - Multiple Frame********/
	m_boolModelFlag = FALSE;

	m_pVideoInfo.openCamera();
	SetTimer(1, 100, NULL);//����һ��������������Ӧ���������ͼ����ʾ
	
}

//Timer
void CMFCDlg::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

			//cvt to GreyScale

			m_pVideoInfo.extractCameraFrame();
			flip(m_pVideoInfo.cameraImg, m_pVideoInfo.cameraImg, 1);

			detect_and_draw_v2(m_pVideoInfo.cameraImg);

			/*
			///////////////////Open Camera////////////////////////
			CvvImage imageSrcCvvImg;
			IplImage* input;

			input = cvCloneImage(&(IplImage)m_pVideoInfo.cameraImg);

			imageSrcCvvImg.CopyOf(input);
			imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
			cvReleaseImage(&input);
			*/
		
		

		CDialogEx::OnTimer(nIDEvent);
	}


//Align Face from img folder
void CMFCDlg::OnBnClickedFacealign()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_boolInitOK == false)
	{
		AfxMessageBox(_T("Initialization!!!"));
		return;
	}

	/*******Model Selection TRUE - Single Picture ; False - Multiple Frame********/

		/**********��ʼ������**********/
		CString str;                                      //�洢ͼ��·�� Cstring object consist of a variable-length squence of characters. Base on the TCHAR data type. 16 bit character type
		BROWSEINFO bi;                                    //�����洢�û�ѡ�е�Ŀ¼��Ϣ
		TCHAR name[MAX_PATH];                             //�洢·�� In the Windows API (with some exceptions discussed in the following paragraphs), the maximum length for a path is MAX_PATH, which is defined as 260 characters.
		ZeroMemory(&bi, sizeof(BROWSEINFO));               //���Ŀ¼��Ӧ���ڴ�
		bi.hwndOwner = GetSafeHwnd();                //�õ����ھ�� Returns the window handle for a window. Useful to manipulate a window associate with a console application
		bi.pszDisplayName = name;

		/**********���öԻ��򲢶�ȡĿ¼��Ϣ**********/
		BIF_BROWSEINCLUDEFILES; // The browse dialog box can display URLs
		bi.lpszTitle = _T("Select folder");           //�Ի������ "Selector Folder" is Unicode wide charactor
		bi.ulFlags = 0x80;                          //���öԻ�����ʽ // The browse dialog box can display URLs
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);        //������ѡ���ļ��е�ID
		SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH)); //���ļ���Ϣ��ʽ���洢����Ӧ��������
		str.ReleaseBuffer();                              //��GerBuffer���ʹ�ã�����ڴ�
		m_Path = str;                                       //��·���洢��m_path��
		if (str.GetAt(str.GetLength() - 1) != '\\')
			m_Path += "\\";
		UpdateData(FALSE);

		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(idl);
			imalloc->Release();
		}

		/**********��ȡ��·���µĵ�һ���ļ�**********/
		//m_ImageDir = (LPSTR)(LPCTSTR)m_Path;

		int len = WideCharToMultiByte(CP_ACP, 0, m_Path, -1, NULL, 0, NULL, NULL);
		m_ImageDir = new char[len + 1];
		////char* m_ImageDir[len + 1];
		WideCharToMultiByte(CP_ACP, 0, m_Path, -1, m_ImageDir, len, NULL, NULL);

		m_pDir = opendir(m_ImageDir);

		for (int i = 0; i < 2; i++){
			m_pEnt = readdir(m_pDir);
		}

		int SaveNum = 0;
		while(m_pDir && (m_pEnt = readdir(m_pDir)) != NULL)
		{
			char* pJpg = strstr(m_pEnt->d_name, ".jpg");
			char* pBmp = strstr(m_pEnt->d_name, ".bmp");
			char* pPng = strstr(m_pEnt->d_name, ".tif");
			char* pJPG = strstr(m_pEnt->d_name, ".JPG");

			if (pJpg == NULL && pBmp == NULL && pPng == NULL && pJPG == NULL)
			{
				continue;
			}
			SaveNum++;
			
			char imageFullName[500];                  //����ͼ���ļ���ȫ·��
			Mat imageSrc;
			sprintf_s(imageFullName, "%s%s", m_ImageDir, m_pEnt->d_name);  //ƴ���ļ�ȫ·��
			imageSrc = imread(imageFullName);		//Read image and store in mat

			detect_and_draw_v2(imageSrc, SaveNum);

			CvvImage imageSrcCvvImg;
			IplImage* input;
			input = cvCloneImage(&(IplImage)imageSrc);
			imageSrcCvvImg.CopyOf(input);
			imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
			cvReleaseImage(&input);
			/*CvvImage imageSrcCvvImg;
			IplImage* input;

			input = cvCloneImage(&(IplImage)img);

			imageSrcCvvImg.CopyOf(input);
			imageSrcCvvImg.DrawToHDC(m_pPicCtlHdc, m_PicCtlRect);
			cvReleaseImage(&input);
*/
		}
}


//Video Camera Capture Face
void CMFCDlg::OnBnClickedCameracapture()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	cameraCapture = true;

}
