
// VideoLabelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CVideoLabelApp.h"
#include "CVideoLabelDlg.h"
#include "EventController.h"
#include "afxdialogex.h"
#include "CVideoLabelFile.h"
#include "CvvImage.h"
#include "CTxtDialog.h"
#include <vector>
#include "VideoCut/cut.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <math.h>

using namespace std;

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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoLabelDlg �Ի���

TCITEMW CVideoLabelDlg::GetTabSelectItem(CTabCtrl& tabCtrl, TCHAR *buffer)
{
	TCITEMW tabItem;
	tabItem.cchTextMax = 256;
	tabItem.pszText = buffer;
	tabItem.mask = TCIF_TEXT;
	tabCtrl.GetItem(tabCtrl.GetCurSel(), &tabItem);
	return tabItem;
}

CVideoLabelDlg::CVideoLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoLabelDlg::IDD, pParent)
	, m_stt_time(_T(""))
	, m_str_roi(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitDlg = FALSE;
}

void CVideoLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRE_FILE, m_tre_file);
	DDX_Control(pDX, IDC_TRE_LABEL, m_tre_label);
	DDX_Control(pDX, IDC_TAB_LABEL, m_tab_label);
	DDX_Control(pDX, IDC_STT_PIC, m_picture);
	DDX_Control(pDX, IDC_BT_PLAY_PAUSE, m_bt_play_pause);
	//  DDX_Control(pDX, IDC_SLD_PLAYER, m_sld_player);
	DDX_Control(pDX, IDC_BT_ADDLABEL, m_bt_addlabel);
	DDX_Control(pDX, IDC_BT_BG, m_bt_bg);
	DDX_Control(pDX, IDC_BT_ED, m_bt_ed);
	DDX_Control(pDX, IDC_LST_SHOW, m_lst_show);
	DDX_Control(pDX, IDC_SLIDER_MP, m_Slider);
	DDX_Text(pDX, IDC_STT_TIME, m_stt_time);
	DDX_Control(pDX, IDC_SLIDER_TIP, m_Slider_tip);
	DDX_Control(pDX, IDC_BT_LAST_KEY, m_bt_lastkey);
	DDX_Control(pDX, IDC_BT_NEXT_KEY, m_bt_nextkey);
	DDX_Control(pDX, IDC_PIC_MARK, m_pic_mark);
	DDX_Control(pDX, IDC_LAST1SEC, m_bt_lastSec);
	DDX_Control(pDX, IDC_BT_NEXT1SEC, m_bt_nextSec);
	DDX_Control(pDX, IDC_BT_CLEARPOINT, m_bt_clear);
	DDX_Text(pDX, IDC_EDT_ROI, m_str_roi);
}

BEGIN_MESSAGE_MAP(CVideoLabelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDR_ACC_EXITSUBVIDEO,&CVideoLabelDlg::OnExitSubVideoState)
	ON_COMMAND(IDR_ACC_PLAYPAUSE, &CVideoLabelDlg::OnBnClickedBtPlayPause)
	ON_COMMAND(IDR_ACC_LEFT, &CVideoLabelDlg::OnBnClickedBtLastKey)
	ON_COMMAND(IDR_ACC_RIGHT, &CVideoLabelDlg::OnBnClickedBtNextKey)
	ON_COMMAND(IDR_ACC_LASTFRAME,&CVideoLabelDlg::OnBnClickedLastFrame)
	ON_COMMAND(IDR_ACC_NEXTFRAME, &CVideoLabelDlg::OnBnClickedNextFrame)
	ON_COMMAND(ID_32771, &CVideoLabelDlg::OnBnClickImportVideoDir)
	ON_BN_CLICKED(IDC_BT_PLAY_PAUSE, &CVideoLabelDlg::OnBnClickedBtPlayPause)
	ON_COMMAND(ID_32772, &CVideoLabelDlg::OnBnClickImportLabelFile)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LABEL, &CVideoLabelDlg::OnTcnSelchangeTabLabel)
	ON_BN_CLICKED(IDC_BT_BG, &CVideoLabelDlg::OnBnClickedBtBg)
	ON_BN_CLICKED(IDC_BT_ED, &CVideoLabelDlg::OnBnClickedBtEd)
	ON_BN_CLICKED(IDC_BT_ADDLABEL, &CVideoLabelDlg::OnBnClickedBtAddlabel)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, &CVideoLabelDlg::OnSliderMessage)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, &CVideoLabelDlg::OnReleaseSliderMessage)
	ON_BN_CLICKED(IDC_BT_CLEARPOINT, &CVideoLabelDlg::OnBnClickedBtClearpoint)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_SHOW, &CVideoLabelDlg::OnNMDblclkLstShow)
	ON_COMMAND(ID__32774, &CVideoLabelDlg::OnDelLabel)
	ON_NOTIFY(NM_RCLICK, IDC_LST_SHOW, &CVideoLabelDlg::OnNMRClickLstShow)
	ON_NOTIFY(NM_DBLCLK, IDC_TRE_FILE, &CVideoLabelDlg::OnNMDblclkTreFile)
	ON_BN_CLICKED(IDC_BT_LAST_KEY, &CVideoLabelDlg::OnBnClickedBtLastKey)
	ON_BN_CLICKED(IDC_BT_NEXT_KEY, &CVideoLabelDlg::OnBnClickedBtNextKey)
	ON_BN_CLICKED(IDC_LAST1SEC, &CVideoLabelDlg::OnBnClickedLast1sec)
	ON_BN_CLICKED(IDC_BT_NEXT1SEC, &CVideoLabelDlg::OnBnClickedBtNext1sec)
	ON_NOTIFY(NM_RCLICK, IDC_TRE_LABEL, &CVideoLabelDlg::OnNMRClickTreLabel)
	ON_COMMAND(ID__32788, &CVideoLabelDlg::OnPressAddLabelItem)
	ON_COMMAND(ID__32789, &CVideoLabelDlg::OnPressDelLabelItem)
	ON_COMMAND(ID__32790, &CVideoLabelDlg::OnPressModifyLabelItem)
	ON_COMMAND(ID_32791, &CVideoLabelDlg::OnUseKeyFrame)
	ON_COMMAND(ID_32792, &CVideoLabelDlg::OnDisableKeyFrame)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CVideoLabelDlg ��Ϣ�������

BOOL CVideoLabelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bInitDlg = TRUE;
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
	int nScreenWidth, nScreenHeight;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (nScreenWidth <= 1400){
		SetWindowPos(NULL, 50, 50, 1280, 730, SWP_NOMOVE);
		m_Slider.SetBitmapChannel(IDB_BP_MINI_CHANNEL,IDB_BP_MINI_CHANNEL_ACTIVE, TRUE);
		m_Slider.SetBitmapThumb(IDB_BP_THUMB, IDB_BP_THUMB_ACTIVE, TRUE);
	}
	else
	{
		SetWindowPos(NULL, 50, 50, 1510, 1000, SWP_NOMOVE);
		m_Slider.SetBitmapChannel(IDB_BP_CHANNEL, IDB_BP_CHANNEL_ACTIVE, TRUE);
		m_Slider.SetBitmapThumb(IDB_BP_THUMB, IDB_BP_THUMB_ACTIVE, TRUE);
	}
	//list��ʼ��
	m_lst_show.DeleteAllItems();
	while (m_lst_show.DeleteColumn(0));
	m_lst_show.InsertColumn(1, _T("Index"), LVCFMT_CENTER, 100);
	m_lst_show.InsertColumn(2, _T("Start"), LVCFMT_CENTER, 120);
	m_lst_show.InsertColumn(3, _T("End"), LVCFMT_CENTER, 120);
	m_lst_show.InsertColumn(4, _T("Domain"), LVCFMT_CENTER, 100);
	m_lst_show.InsertColumn(5, _T("Type"), LVCFMT_CENTER, 100);
	m_lst_show.InsertColumn(6, _T("Label"), LVCFMT_CENTER, 200);
	m_lst_show.InsertColumn(7, _T("SubLabel"), LVCFMT_CENTER, 200);
	m_lst_show.InsertColumn(8, _T("Region of Interest"), LVCFMT_CENTER, 300);
	m_lst_show.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);//������ק
	UpdateData(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	
	
	m_Slider.SetMarginLeft(0);
	m_Slider.SetMarginRight(0);
	m_Slider.DrawFocusRect(FALSE);
	m_bg_pos = -1;
	m_ed_pos = -1;
	m_bUseKeyFrame = FALSE;
	//��ӿ�ݼ�
	m_hAcc = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	CString folderPath, labelPath;
	if (CEventController::GetInstance()->ReadEnv(folderPath, labelPath) == TRUE)
	{
		OnImportLabelFile(labelPath);
		OnImportVideoDir(folderPath);
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CVideoLabelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bInitDlg == FALSE)
		return;
	// TODO: Add your message handler code here
	//�����Ի���ʱ���ؼ���û�д�������˲��ܸı����С(������������д���)

	//���㴰�ڿ�Ⱥ͸߶ȵĸı���  
	int nIncrementX = cx - 200;
	int nIncrementY = cy - 150;
	m_tre_file.MoveWindow(0.01*cx, 0.01*cy, 0.12*cx, 0.99*cy);
	m_picture.MoveWindow(0.14*cx, 0.01*cy, 0.7*cx, 0.65*cy);
	int sliderwidth = 0.65*cx;
	m_rect_pic.SetRect(0, 0, 0.7*cx, 0.65*cy);
	m_pic_mark.MoveWindow(0.145*cx, 0.66*cy, sliderwidth, 0.015*cy);
	m_Slider.MoveWindow(0.14*cx, 0.69*cy, sliderwidth, 0.01*cy);
	m_Slider_tip.MoveWindow(0.145*cx, 0.71*cy, sliderwidth - 0.01*cx, 0.02*cy);
	m_lst_show.MoveWindow(0.14*cx, 0.78*cy, 0.85*cx, 0.21*cy);
	m_bt_play_pause.MoveWindow(0.14*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_lastSec.MoveWindow(0.21*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_nextSec.MoveWindow(0.28*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_lastkey.MoveWindow(0.35*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_nextkey.MoveWindow(0.42*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_bg.MoveWindow(0.49*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_ed.MoveWindow(0.56*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_bt_clear.MoveWindow(0.63*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	GetDlgItem(IDC_EDT_ROI)->MoveWindow(0.70*cx, 0.74*cy, 0.12*cx, 0.03*cy);
	m_bt_addlabel.MoveWindow(0.83*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	(GetDlgItem(IDC_STT_TIME))->MoveWindow(0.90*cx, 0.74*cy, 0.06*cx, 0.03*cy);
	m_tab_label.MoveWindow(0.85*cx, 0.01*cy, 0.15*cx, 0.03*cy);
	m_tre_label.MoveWindow(0.85*cx, 0.04*cy, 0.15*cx, 0.62*cy);
	SetTimer(4, 50, NULL);	//ˢ��ר��
}
void CVideoLabelDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	while (bitmapBuf.size())
	{
		CBitmap* bitmap = bitmapBuf.front();
		bitmap->DeleteObject();
		delete bitmap;
		bitmapBuf.pop();
	}
	
}
void CVideoLabelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CVideoLabelDlg::DrawTriangle(CDC* PDC, CPoint point, DWORD Color)
{
	/************************************************************************/
	/*             ����С�ĵ����������Σ�pointΪ�·�����λ��                */
	/************************************************************************/
	CPen Pen(PS_SOLID, 2, Color), *pOldPen;
	pOldPen = PDC->SelectObject(&Pen);
	PDC->MoveTo(point.x, point.y);
	PDC->LineTo(fmax(0, point.x - 5), fmax(0, point.y - 5));
	PDC->LineTo(point.x + 5, fmax(0, point.y - 5));
	PDC->LineTo(point.x, point.y);
	PDC->SelectObject(pOldPen);
	ReleaseDC(PDC);
}
void CVideoLabelDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	if (IsIconic())
	{
		
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

void CVideoLabelDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		//ˢ��
		if (bitmapBuf.size() == 0)
		{
			if (m_player.GetIsSubVideoState()==FALSE)
				m_ed_pos = m_Slider.GetRangeMax();
			m_player.SetState(STOP);
			m_Slider.SetPos(m_Slider.GetRangeMin());
			m_player.SetDrawFramePos(-1);
			m_player.SetFramePos(m_Slider.GetPos());
			m_bt_play_pause.SetWindowTextW(_T("PLAY"));
			ShowTriangleMark();
			KillTimer(1);
			break;
		}
		CBitmap * bitmap = bitmapBuf.front();
		ShowBitMap(bitmap, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
		bitmap->DeleteObject();
		delete bitmap;
		bitmapBuf.pop();
		int pos = m_player.GetDrawFramePos();
		m_player.SetDrawFramePos(pos + 1);
		m_Slider.SetPos(pos + 1);
		m_stt_time.Format(_T("��ǰ֡����%d"), pos+1);
		UpdateData(FALSE);
		break;
	}
	case 2:
	{
		//opencv��ȡ����
		if (m_player.ReadFrameToBuffer() == FALSE)
		{
			KillTimer(2);
			break;
		}
		break;
	}
	case 3:
	{
		//׼����ͼ�Ļ���
		IplImage *img;
		if (bitmapBuf.size() < 10){
			if (m_player.ReadFrameFromBuffer(&img) == FALSE)
			{
				KillTimer(3);
				break;
			}
			else
			{
				PrepareBitmapAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
			}
		}
		break;
	}
	case 4:
		ShowCutPoint(m_Slider_tip.GetDC(), m_rect_pic.Width() - 23, 20);
		ShowTriangleMark();
		KillTimer(4);
		break;
	default:
		break;
	}
}
void CVideoLabelDlg::ClearBitmap()
{
	while (!bitmapBuf.empty())
	{
		CBitmap* bmp = bitmapBuf.front();
		bmp->DeleteObject();
		delete bmp;
		bitmapBuf.pop();
	}
}
void CVideoLabelDlg::PrepareBitmapAndReleaseImg(IplImage* img, CDC *pDC, int windowWidth, int windowHeight)
{
	if (img == NULL)
		return;
	int iWndWidth = windowWidth;
	int iWndHeight = windowHeight;
	int hmWidth = img->width;
	int hmHeight = img->height;

	// ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
	double winRatio = iWndWidth*1.0 / iWndHeight;
	double picRatio = hmWidth*1.0 / hmHeight;
	int iShowWidth = iWndWidth;
	int iShowHeight = iWndHeight;
	int iShowX = 0;
	int iShowY = 0;
	if (picRatio >= winRatio)
	{
		iShowHeight = iShowWidth*1.0 / picRatio;
		iShowY = iWndHeight / 2 - iShowHeight / 2;
	}
	else
	{
		iShowWidth = iShowHeight*picRatio;
		iShowX = iWndWidth / 2 - iShowWidth / 2;
	}
	m_rect_pic_valid.SetRect(iShowX, iShowY, iShowWidth + iShowX, iShowHeight + iShowY);
	CDC MemDC;
	CBitmap *MemBitmap = new CBitmap();
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap->CreateCompatibleBitmap(pDC, iWndWidth, iWndHeight);
	CBitmap *pOldBit = MemDC.SelectObject(MemBitmap);
	MemDC.FillSolidRect(0, 0, iWndWidth, iWndHeight, RGB(0, 0, 0));
	CRect rect(iShowX, iShowY, iShowX + iShowWidth, iShowY + iShowHeight);
	CvvImage cimg;
	cimg.CopyOf(img);
	cvReleaseImage(&img);
	cimg.DrawToHDC(MemDC.GetSafeHdc(), &rect);
	cimg.Destroy();
	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(200,200,200));//PS_DOT
	CPen *pOldPen = MemDC.SelectObject(&pen);
	for (int i = 1; i <3; i++)
	{
		MemDC.MoveTo(iShowX, iShowY + iShowHeight*i / 3.0);
		MemDC.LineTo(iShowX + iShowWidth, iShowY + iShowHeight*i / 3.0);
	}
	for (int i = 1; i < 4; i++)
	{
		MemDC.MoveTo(iShowX+iShowWidth*i/4.0, iShowY);
		MemDC.LineTo(iShowX+iShowWidth*i/4.0, iShowY + iShowHeight);
	}
	MemDC.SelectObject(pOldPen);
	pen.DeleteObject();
	
	

	if (m_drawROI)
	{
		int startRow = m_iROI[0];
		int startCol = m_iROI[1];
		int endRow = m_iROI[2];
		int endCol = m_iROI[3];
		CPen pen2;
		pen2.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		pOldPen = MemDC.SelectObject(&pen2);
		MemDC.MoveTo(iShowX + (startCol - 1) / 4.0*iShowWidth , iShowY + (startRow - 1) / 3.0*iShowHeight);
		MemDC.LineTo(iShowX + (endCol) / 4.0 * iShowWidth , iShowY + (startRow - 1) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (endCol) / 4.0 * iShowWidth , iShowY + (endRow) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (startCol - 1) / 4.0 * iShowWidth , iShowY + (endRow) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (startCol - 1) / 4.0 * iShowWidth , iShowY + (startRow - 1) / 3.0 * iShowHeight);
		//pDC->SelectObject(pOldBrush);
		pen2.DeleteObject();
		MemDC.SelectObject(pOldPen);
	}
	UpdateData(TRUE);
	list<int> squareROIList = CEventController::GetInstance()->GetSquareListROI(m_str_roi);
	list<int>::iterator it;
	CPen pen3;
	pen3.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	pOldPen = MemDC.SelectObject(&pen3);
	for (it = squareROIList.begin(); it != squareROIList.end(); it++)
	{
		int num = *it;
		int row = num / 10;
		int col = num % 10;
		MemDC.MoveTo(iShowX + (col - 1) / 4.0*iShowWidth, iShowY + (row - 1) / 3.0*iShowHeight);
		MemDC.LineTo(iShowX + (col) / 4.0 * iShowWidth, iShowY + (row - 1) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col) / 4.0 * iShowWidth, iShowY + (row) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col - 1) / 4.0 * iShowWidth, iShowY + (row) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col - 1) / 4.0 * iShowWidth, iShowY + (row - 1) / 3.0 * iShowHeight);
		//pDC->SelectObject(pOldBrush);
	}
	pen3.DeleteObject();
	MemDC.SelectObject(pOldPen);
	MemDC.SelectObject(pOldBit);
	MemDC.DeleteDC();
	bitmapBuf.push(MemBitmap);
	ReleaseDC(pDC);
	//pDC->BitBlt(0, 0, iWndWidth, iWndHeight, &MemDC, 0, 0, SRCCOPY);
	
}
void CVideoLabelDlg::ShowBitMap(CBitmap *bitmap, CDC* pDC, int windowWidth, int windowHeight)
{
	if (bitmap == NULL)
		return;
	int iWndWidth = windowWidth;
	int iWndHeight = windowHeight;
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);
	
	CBitmap *pOldBit = MemDC.SelectObject(bitmap);
	
	pDC->BitBlt(0, 0, iWndWidth, iWndHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemDC.DeleteDC();
	ReleaseDC(pDC);
	pOldBit->DeleteObject();
	//delete bitmap;
}
void CVideoLabelDlg::ShowAndReleaseImg(IplImage* img,CDC * pDC,int windowWidth, int windowHeight)    // ID ��Picture Control�ؼ���ID��
{
	int iWndWidth = windowWidth;    
	int iWndHeight = windowHeight;
	int hmWidth = img->width;
	int hmHeight = img->height;

	// ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
	double winRatio = iWndWidth*1.0 / iWndHeight;
	double picRatio = hmWidth*1.0 / hmHeight;
	int iShowWidth = iWndWidth;
	int iShowHeight = iWndHeight;
	int iShowX = 0;
	int iShowY = 0;
	if (picRatio >= winRatio)
	{
		iShowHeight = iShowWidth*1.0 / picRatio;
		iShowY = iWndHeight / 2 - iShowHeight / 2;
	}
	else
	{
		iShowWidth = iShowHeight*picRatio;
		iShowX = iWndWidth / 2 - iShowWidth / 2;
	}
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, iWndWidth, iWndHeight);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, iWndWidth, iWndHeight, RGB(0, 0, 0)); 
	CRect rect(iShowX, iShowY, iShowX + iShowWidth, iShowY + iShowHeight);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(MemDC.GetSafeHdc(), &rect);
	cimg.Destroy();

	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(200, 200, 200));//PS_DOT
	CPen *pOldPen = MemDC.SelectObject(&pen);
	for (int i = 1; i < 3; i++)
	{
		MemDC.MoveTo(iShowX, iShowY + iShowHeight*i / 3.0);
		MemDC.LineTo(iShowX + iShowWidth, iShowY + iShowHeight*i / 3.0);
	}
	for (int i = 1; i < 4; i++)
	{
		MemDC.MoveTo(iShowX + iShowWidth*i / 4.0, iShowY);
		MemDC.LineTo(iShowX + iShowWidth*i / 4.0, iShowY + iShowHeight);
	}
	MemDC.SelectObject(pOldPen);
	pen.DeleteObject();
	if (m_drawROI)
	{
		int startRow = m_iROI[0];
		int startCol = m_iROI[1];
		int endRow = m_iROI[2];
		int endCol = m_iROI[3];
		CPen pen2;
		pen2.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		pOldPen = MemDC.SelectObject(&pen2);
		MemDC.MoveTo(iShowX + (startCol - 1) / 4.0*iShowWidth , iShowY + (startRow - 1) / 3.0*iShowHeight);
		MemDC.LineTo(iShowX + (endCol) / 4.0 * iShowWidth , iShowY + (startRow - 1) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (endCol) / 4.0 * iShowWidth , iShowY + (endRow) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (startCol - 1) / 4.0 * iShowWidth , iShowY + (endRow) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (startCol - 1) / 4.0 * iShowWidth , iShowY + (startRow - 1) / 3.0 * iShowHeight);
		//pDC->SelectObject(pOldBrush);
		pen2.DeleteObject();
		MemDC.SelectObject(pOldPen);
	}
	UpdateData(TRUE);
	list<int> squareROIList = CEventController::GetInstance()->GetSquareListROI(m_str_roi);
	list<int>::iterator it;
	CPen pen3;
	pen3.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = MemDC.SelectObject(&pen3);
	for (it = squareROIList.begin(); it != squareROIList.end(); it++)
	{
		int num = *it;
		int row = num / 10;
		int col = num % 10;
		MemDC.MoveTo(iShowX + (col-1) / 4.0*iShowWidth, iShowY + (row -1) / 3.0*iShowHeight);
		MemDC.LineTo(iShowX + (col) / 4.0 * iShowWidth, iShowY + (row - 1) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col) / 4.0 * iShowWidth, iShowY + (row) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col - 1) / 4.0 * iShowWidth, iShowY + (row) / 3.0 * iShowHeight);
		MemDC.LineTo(iShowX + (col - 1) / 4.0 * iShowWidth, iShowY + (row - 1) / 3.0 * iShowHeight);
		//pDC->SelectObject(pOldBrush);
	}
	pen3.DeleteObject();
	MemDC.SelectObject(pOldPen);
	pDC->BitBlt(0, 0, iWndWidth, iWndHeight, &MemDC, 0, 0, SRCCOPY);


	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);
	pOldBit->DeleteObject();
	cvReleaseImage(&img);
}

void CVideoLabelDlg::OnPreviewImageWithPos()
{

	int pos = m_Slider.GetPos();
	IplImage * img;
	if (m_player.ReadTmpFrameWithPos(&img, pos))
	{
		ShowAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	}
}

void CVideoLabelDlg::ShowCutPoint(CDC * pDC, int windowWidth, int windowHeight)
{
	if (m_player.GetIsSubVideoState()||m_vec_cutPoint.size() <= 1){
		pDC->FillSolidRect(0, 0, windowWidth, windowHeight, RGB(0, 0, 0));
		ReleaseDC(pDC);
		return;
	}
		
	int iWndWidth = windowWidth;
	int iWndHeight = windowHeight;

	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, iWndWidth, iWndHeight);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	int len = m_vec_cutPoint.size();
	CPoint p0(0, 0);
	COLORREF col[] =
	{ 
		RGB(136, 104, 153),
		RGB(172, 84, 83),
		RGB(221, 206, 203),
		RGB(254, 255, 153),
		RGB(170, 155, 152),
		RGB(186, 189, 102),
		RGB(220, 0, 0),
		RGB(83, 86, 67),
		RGB(0, 67, 135),
		RGB(0, 206, 170),
		RGB(67, 171, 122),
		RGB(255, 250, 87),
		RGB(254, 137, 84),
		RGB(51, 0, 17)
	};
	for (int i = 0; i < len; i++)
	{
		int pos = m_vec_cutPoint[i];
		CPoint p1(pos*1.0 / m_player.GetTotalFrame()*iWndWidth, iWndHeight);
		MemDC.FillSolidRect(p0.x, p0.y, p1.x, p1.y, col[i%14]);
		p0.x = p1.x;
	}
	pDC->BitBlt(0, 0, iWndWidth, iWndHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	if (pOldBit!=NULL)
		pOldBit->DeleteObject();
	ReleaseDC(pDC);
}
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoLabelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CVideoLabelDlg::ShowTriangleMark()
{
	CRect rect_slider;
	m_Slider.GetWindowRect(&rect_slider);
	CDC* pDC = m_pic_mark.GetDC();
	pDC->FillSolidRect(0, 0, m_rect_pic.Width()*1.1, 20, RGB(240, 240, 240));
	ReleaseDC(pDC);
	//m_pic_mark.RedrawWindow(CRect(0,0,rect_slider.Width()-23,20),NULL,RDW_ERASE|RDW_UPDATENOW);
	CRect rect;
	m_Slider_tip.GetWindowRect(&rect);
	if (m_bg_pos >= 0)
	{
		int relativeX = (m_bg_pos - m_Slider.GetRangeMin())*1.0 / (m_Slider.GetRangeMax() - m_Slider.GetRangeMin()) * (m_rect_pic.Width()*0.98);
		CPoint point(relativeX, 10);
		DrawTriangle(m_pic_mark.GetDC(), point, RGB(0, 255, 0));
	}
	if (m_ed_pos >= 0)
	{
		int relativeX = (m_ed_pos - m_Slider.GetRangeMin())*1.0 / (m_Slider.GetRangeMax() - m_Slider.GetRangeMin()) * (m_rect_pic.Width()*0.98);
		CPoint point(relativeX, 20);
		DrawTriangle(m_pic_mark.GetDC(), point, RGB(255, 0, 0));
	}
}

void CVideoLabelDlg::OnBnClickImportVideoDir()
{
	// TODO:  �ڴ���������������
	CString videoDir = CEventController::GetInstance()->OnImportVideoDirectory();
	OnImportVideoDir(videoDir);
}
void CVideoLabelDlg::OnImportVideoDir(CString videoPath)
{
	if (!PathFileExistsW(videoPath))
	{
		AfxMessageBox(_T("·������"));
	}
	else
	{
		//prepare
		m_tre_file.DeleteAllItems();
		CVideoLabelFileIOController* fileController = CVideoLabelFileIOController::GetInstance();
		fileController->SetFolderPath(videoPath);
		fileController->SetStorePath(videoPath + _T("\\data.vlcf.xml"));
		//�����ļ���
		HTREEITEM hRoot = m_tre_file.InsertItem(videoPath);
		InitVideoFileTreeView(hRoot);
		m_tre_file.Expand(hRoot, TVE_EXPAND);
		if (fileController->ReadFileFromXML() == FALSE)
		{
			AfxMessageBox(_T("ԴĿ¼û�б�ע��¼�ļ�Ӵ�����ڴ��㿪ʼ��"));
		}
		else
		{
			OnRefreshListShowCtrl();
		}
	}
}
void CVideoLabelDlg::OnBnClickImportLabelFile()
{
	// TODO:  �ڴ���������������
	CString labelPath = CEventController::GetInstance()->OnImportLabelFile();
	OnImportLabelFile(labelPath);
}
void CVideoLabelDlg::OnImportLabelFile(CString labelPath)
{
	if (labelPath.IsEmpty())
		return;
	m_labelXMLFileNode = CLabelXMLFileIOController::GetLabelXMLFileNode(labelPath);
	if (m_labelXMLFileNode.hasChildren == 0)
		return;
	//ˢ�½���

	//��ǩҳˢ�¡���ǩ���νṹˢ��
	TCITEM item;
	item.mask = TCIF_TEXT;
	m_tab_label.DeleteAllItems();
	int index = 0;
	for (list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin(); it != m_labelXMLFileNode.childrenList.end(); it++)
	{
		//��ǩtab�ؼ�����
		item.pszText = it->value.LockBuffer();
		m_tab_label.InsertItem(index++, &item);
	}
	//���õ�һ��ҳ�汻����
	m_tab_label.SetCurSel(0);
	//�������ú���ˢ��ҳ��
	OnTcnSelchangeTabLabel(NULL, NULL);
}
void CVideoLabelDlg::OnRefreshLabelTreeViewCtrl(HTREEITEM hTreeItem, LabelXMLFileNode node)
{
	HTREEITEM hNewItem;
	if (hTreeItem == NULL)
		hNewItem = m_tre_label.InsertItem(node.value);
	else{
		hNewItem = m_tre_label.InsertItem(node.value, hTreeItem);
		m_tre_label.Expand(hTreeItem, TVE_EXPAND);
	}
	if (node.hasChildren)
	{
		for (list<LabelXMLFileNode>::iterator it = node.childrenList.begin(); it != node.childrenList.end(); it++)
		{
			OnRefreshLabelTreeViewCtrl(hNewItem, *it);
		}
	}
}
void CVideoLabelDlg::OnRefreshListShowCtrl()
{
	m_lst_show.DeleteAllItems();
	HTREEITEM pItem = m_tre_file.GetSelectedItem();
	CString fileName = m_tre_file.GetItemText(pItem);

	CVideoLabelFileIOController* _instance = CVideoLabelFileIOController::GetInstance();
	list<CVideoFile>::iterator it = _instance->GetIteratorOfFile(fileName);
	if (it == _instance->videoFileList.end())
		return;
	list<CClip>::iterator itt;
	CString clipName;
	CString startTime, endTime;
	CString index;
	int i = 0;
	long long start, end;
	for (itt = (*it).clipList.begin(); itt != (*it).clipList.end(); itt++)
	{
		start = (*itt).start;
		end = (*itt).end;

		startTime.Format(_T("%I64d"), start);
		endTime.Format(_T("%I64d"), end);
		index.Format(_T("%d"), i);
		m_lst_show.InsertItem(i, index);
		m_lst_show.SetItemText(i, 1, startTime);
		m_lst_show.SetItemText(i, 2, endTime);
		m_lst_show.SetItemText(i, 3, itt->domain);
		m_lst_show.SetItemText(i, 4, itt->type);
		m_lst_show.SetItemText(i, 5, itt->label);
		m_lst_show.SetItemText(i, 6, itt->sublabel);
		m_lst_show.SetItemText(i, 7, itt->roi);
		i++;
	}
	m_lst_show.EnsureVisible(m_lst_show.GetItemCount() - 1, FALSE);
}
void CVideoLabelDlg::OnTcnSelchangeTabLabel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int index = m_tab_label.GetCurSel();
	//Ѱ�ұ�ǩҳ������
	int i = 0;
	list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin();
	while (i < index)
	{
		i++;
		it++;
	}
	//��������
	m_tre_label.DeleteAllItems();
	list<LabelXMLFileNode>::iterator itt = it->childrenList.begin();
	while (itt != it->childrenList.end())
	{
		OnRefreshLabelTreeViewCtrl(NULL, *itt);
		itt++;
	}
}

void CVideoLabelDlg::InitVideoFileTreeView(HTREEITEM hparent)
{
	CString strText = GetVideoFileTreePath(hparent,TRUE);  //�����б�����Ŀ����  
	if (strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�  
		strText += _T("\\");
	strText += _T("*.*");
	//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ  
	CFileFind file;                                       //���屾���ļ�����  
	BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�  
	while (bContinue)
	{
		bContinue = file.FindNextFile();                  //������һ���ļ�  
		if (!file.IsDirectory())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."  
		{
			CString fileName = file.GetFileName();
			CString postFix = fileName.Right(3);
			if (postFix == "mp4" || postFix == "MP4" || postFix == "mkv" || postFix == "MKV" || postFix == "avi" || postFix == "AVI")
			{
				HTREEITEM hItem = m_tre_file.InsertItem(fileName, hparent); //����̷�·������״�ļ���
				fileName = GetVideoFileTreePath(hItem, FALSE);
				CVideoLabelFileIOController::GetInstance()->AddVideoFile(fileName);
			}
		}
		else if (file.IsDirectory() && !file.IsDots())
		{
			CString fileName = file.GetFileName();
			HTREEITEM hItem = m_tre_file.InsertItem(fileName, hparent);
			InitVideoFileTreeView(hItem);
		}
	}
	//OnTreeView(hparent, m_tre_filePath.GetItemText(hparent));								//�ڶ�������ݹ����
}

CString CVideoLabelDlg::GetVideoFileTreePath(HTREEITEM htreeItem, BOOL rootNeeded)
{
	CString path = m_tre_file.GetItemText(htreeItem);
	HTREEITEM parent = m_tre_file.GetParentItem(htreeItem);
	if (rootNeeded)
	{
		while (parent != NULL)
		{
			path = m_tre_file.GetItemText(parent) + _T("\\") + path;
			parent = m_tre_file.GetParentItem(parent);
		}
	}
	else
	{
		while (m_tre_file.GetParentItem(parent) != NULL)
		{
			path = m_tre_file.GetItemText(parent) + _T("\\") + path;
			parent = m_tre_file.GetParentItem(parent);
		}
	}

	return path;
}


void CVideoLabelDlg::OnNMDblclkTreFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	HTREEITEM selectItem = m_tre_file.GetSelectedItem();
	if (m_tre_file.GetParentItem(selectItem) == NULL)
		return;
	CString videoPath = GetVideoFileTreePath(selectItem, TRUE);
	DWORD dwAttr = GetFileAttributes(videoPath);
	if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
	{//�ļ�
		if (PathFileExists(videoPath) == FALSE)
			return;
	}
	else
	{
		return;
	}

	if (m_player.Open(videoPath) == FALSE){
		CString tmp = videoPath;
		CString info;
		info.Format(_T("��Ƶ��%s ��������"), tmp);
		AfxMessageBox(info);
		return;
	}
		
	
	// ������õ�ͼ����Ϣ�ӻ�����ת����IplImage��ʽ����frame��
	m_player.PrepareBuffer();
	m_player.SetDrawFramePos(-1);
	IplImage * img;
	m_player.ReadFrameFromBuffer(&img);
	ClearBitmap();
	PrepareBitmapAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	if (!bitmapBuf.empty())
	{
		CBitmap* bmp = bitmapBuf.front();
		ShowBitMap(bmp, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	}
	OnRefreshListShowCtrl();
	m_bg_pos = -1;
	USES_CONVERSION;
	if (m_bUseKeyFrame)
		m_vec_cutPoint = cut(T2A(videoPath));
	ShowCutPoint(m_Slider_tip.GetDC(), m_rect_pic.Width()-23, 20);
	m_Slider.SetRange(0, m_player.GetTotalFrame(), TRUE);
	m_Slider.SetPos(0);

}



void CVideoLabelDlg::OnBnClickedBtPlayPause()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (GetDlgItem(IDC_EDT_ROI) == this->GetFocus())
		return;
	STATE state = m_player.GetState();
	if (state == STOP || state == PAUSE)
	{
		m_player.SetState(PLAYING);
		SetTimer(2, 2, NULL);
		Sleep(100);
		SetTimer(3, (int)(1000.0 / (m_player.GetFPS())), NULL);
		Sleep(100);
		SetTimer(1, (int)(1000.0 / (m_player.GetFPS())), NULL);
		
		m_bt_play_pause.SetWindowTextW(_T("PAUSE"));
	}
	else if (state == PLAYING)
	{
		m_player.SetState(PAUSE);
		KillTimer(1);
		KillTimer(3);
		KillTimer(2);
		m_bt_play_pause.SetWindowTextW(_T("PLAY"));
	}
	m_Slider.SetFocus();
}


void CVideoLabelDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
}

LRESULT CVideoLabelDlg::OnSliderMessage(WPARAM wParam, LPARAM lParam) {
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	while (bitmapBuf.size() > 0)
	{
		CBitmap * bitmap = bitmapBuf.front();
		bitmap->DeleteObject();
		delete bitmap;
		bitmapBuf.pop();
	}
	m_player.SetState(PAUSE);
	OnPreviewImageWithPos();
	return 0;
}

LRESULT CVideoLabelDlg::OnReleaseSliderMessage(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	m_player.SetFramePos(m_Slider.GetPos());  //������Ƶ����ʼ֡
	m_bt_play_pause.SetWindowTextW(_T("PLAY"));
	m_player.PrepareBuffer();
	IplImage * img;
	m_player.ReadFrameFromBuffer(&img);
	PrepareBitmapAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	CBitmap* bmp = bitmapBuf.front();
	ShowBitMap(bmp, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	m_stt_time.Format(_T("��ǰ֡����%ld"), m_Slider.GetPos());
	UpdateData(FALSE);
	return 0;
}

void CVideoLabelDlg::OnBnClickedBtBg()
{
	OnBnClickedBtEd();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bg_pos = m_player.GetDrawFramePos();
	if (m_bg_pos == -1)
		m_bg_pos = 0;
	ShowTriangleMark();
	//�Ϸ��Լ�飺��ǵĿ�ʼλ��Ӧ���ڽ���λ�����
}

void CVideoLabelDlg::OnBnClickedBtEd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_ed_pos = m_player.GetDrawFramePos();
	ShowTriangleMark();
}
void CVideoLabelDlg::OnBnClickedBtClearpoint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bg_pos = -1;
	m_ed_pos = -1;
	m_str_roi = _T("");
	ShowTriangleMark();
	UpdateData(FALSE);
}

void CVideoLabelDlg::OnBnClickedBtAddlabel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_ed_pos>=m_bg_pos)
	{
		UpdateData(TRUE);
		if (CEventController::GetInstance()->CheckROIStringValid(m_str_roi) == FALSE)
		{
			AfxMessageBox(_T("����Ȥ���򲻺Ϸ���"));
			return;
		}
		HTREEITEM selItem = m_tre_label.GetSelectedItem();
		if (selItem == NULL)
			return;
		CString str[3];
		int i = 0; 
		HTREEITEM pItem = selItem;
		while (pItem != NULL)
		{
			str[i++] = m_tre_label.GetItemText(pItem);
			pItem = m_tre_label.GetParentItem(pItem);
		}
		CString type = str[--i];
		CString label = str[--i];
		CString sublabel = _T("");
		if (i > 0){
			sublabel = str[--i];
		}
		
		TCHAR buffer[256] = { 0 };
		TCITEMW tabItem = GetTabSelectItem(m_tab_label, buffer);
		CString domain = tabItem.pszText;
		CString fileName = GetVideoFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
		CVideoLabelFileIOController::GetInstance()->AddClipLabel(fileName, m_bg_pos, m_ed_pos,m_str_roi,domain, type, label,sublabel);
		CVideoLabelFileIOController::GetInstance()->SaveFileToXML();
		OnRefreshListShowCtrl();
		m_str_roi = _T("");
		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox(_T("��ʼʱ����Ҫ�����ڽ���ʱ��֮ǰ��"));
	}
}
BOOL CVideoLabelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	//��ʱû�����ã��϶�ʱ���ˢ��
	if (pMsg->message == WM_PREVIEW_IMAGE_WITH_POS)
	{
		int pos = m_Slider.GetPos();
		IplImage * img;
		if (m_player.ReadTmpFrameWithPos(&img, pos))
		{
			ShowAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
		}
	}
	if (WM_KEYFIRST <= pMsg->message&&pMsg->message <= WM_KEYLAST)
	{
		HACCEL hAccel = m_hAcc;
		if (pMsg->wParam == ' '&&GetFocus()==GetDlgItem(IDC_EDT_ROI))
			return CDialogEx::PreTranslateMessage(pMsg);
		if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CVideoLabelDlg::OnExitSubVideoState()
{
	m_player.SetIsSubVideoState(FALSE);
	m_player.ClearImgBuf();
	ClearBitmap();
	m_player.SetFramePos(m_player.GetSubEnd());
	m_player.SetDrawFramePos(m_player.GetSubEnd()-1);
	//m_player.PrepareBuffer();
	m_Slider.SetRange(0, m_player.GetTotalFrame());
	m_Slider.SetPos(m_player.GetSubEnd());
	m_drawROI = FALSE;
	ShowCutPoint(m_Slider_tip.GetDC(), m_rect_pic.Width() - 23, 20);
	LocateImage(m_player.GetSubEnd());
}
void CVideoLabelDlg::OnNMDblclkLstShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	TRACE(_T("double click "));
	POSITION pos = m_lst_show.GetFirstSelectedItemPosition();
	int delId;
	if (pos != NULL)
	{
	    delId = m_lst_show.GetNextSelectedItem(pos);
		CString index = m_lst_show.GetItemText(delId, 0);
		CString fileName = m_tre_file.GetItemText(m_tre_file.GetSelectedItem());
		list<CClip>::iterator it = CVideoLabelFileIOController::GetInstance()->GetIteratorOfClip(fileName,_ttoi(index));
		m_player.SetSubStart(it->start);
		m_player.SetSubEnd(it->end);
		m_player.SetIsSubVideoState(TRUE);
		m_Slider.SetRange(it->start, it->end);
		m_Slider.SetPos(it->start);
		m_player.SetFramePos(it->start);
		m_player.ClearImgBuf();
		m_bg_pos = -1;
		m_ed_pos = -1;
		ClearBitmap();
		ShowCutPoint(m_Slider_tip.GetDC(), m_rect_pic.Width() - 23, 20);
		ShowTriangleMark();
		CString roi = m_lst_show.GetItemText(delId, 7);
		if (CEventController::GetInstance()->GetROI(roi, m_iROI[0], m_iROI[1], m_iROI[2], m_iROI[3]) == TRUE)
			m_drawROI = TRUE;
		m_str_roi = _T("");
		UpdateData(FALSE);
		LocateImage(it->start);
	}
	
}


//Right Menu Del Event
void CVideoLabelDlg::OnDelLabel()
{
	int delId = -1;
	// TODO:  �ڴ���������������
	////ɾ������ѡ�е�===============
	int count = m_lst_show.GetSelectedCount();
	if (count > 0){
		POSITION pos = m_lst_show.GetFirstSelectedItemPosition();
		while (pos != NULL)
		{
			delId = m_lst_show.GetNextSelectedItem(pos);
			CString fileName = GetVideoFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
			CVideoLabelFileIOController::GetInstance()->DeleteClipLabel(fileName, delId);
			CVideoLabelFileIOController::GetInstance()->SaveFileToXML();
		}
	}
	OnRefreshListShowCtrl();
	if (delId !=-1)
		m_lst_show.EnsureVisible(delId, TRUE);
}
void CVideoLabelDlg::OnNMRClickLstShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU3));
		CMenu* popup = menu.GetSubMenu(0);
		if (m_player.GetIsSubVideoState()==FALSE)
			popup->EnableMenuItem(ID__32790, 1);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CVideoLabelDlg::LocateImage(long pos)
{
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	while (bitmapBuf.size() > 0)
	{
		CBitmap * bitmap = bitmapBuf.front();
		bitmap->DeleteObject();
		delete bitmap;
		bitmapBuf.pop();
	}
	m_player.ClearImgBuf();
	m_player.SetFramePos(pos);  //������Ƶ����ʼ֡
	m_bt_play_pause.SetWindowTextW(_T("PLAY"));
	IplImage * img;
	m_player.ReadTmpFrameWithPos(&img, pos);
	m_Slider.SetPos(pos);
	ShowAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	m_stt_time.Format(_T("��ǰ֡����%ld"), pos);
	UpdateData(FALSE);
	return;
}
void CVideoLabelDlg::OnBnClickedBtLastKey()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	long pos = m_player.GetDrawFramePos();

	int len = m_vec_cutPoint.size();
	for (int i = 0; i < len; i++)
	{
		if (m_vec_cutPoint[i] < pos)
		{
			if (i + 1 == len)
			{
				LocateImage(m_vec_cutPoint[i]);
				return;
			}
			else if (i + 1 < len&&m_vec_cutPoint[i + 1] >= pos)
			{
				LocateImage(m_vec_cutPoint[i]);
				return;
			}
		}
	}
}
void CVideoLabelDlg::OnBnClickedBtNextKey()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	long pos = m_player.GetDrawFramePos();
	KillTimer(1);
	KillTimer(3);
	KillTimer(2);
	int len = m_vec_cutPoint.size();
	for (int i = 0; i < len; i++)
	{
		if (m_vec_cutPoint[i] > pos)
		{
			LocateImage(m_vec_cutPoint[i]);
			return;
		}
	}
}
void CVideoLabelDlg::OnBnClickedLastFrame()
{
	long pos = m_Slider.GetPos() - 1;
	if (pos >= m_Slider.GetRangeMin())
	{
		LocateImage(pos);
		m_Slider.SetPos(pos);
		m_Slider.UpdateData();
	}
}
void CVideoLabelDlg::OnBnClickedNextFrame()
{
	long pos = m_Slider.GetPos() + 1;
	if (pos <= m_Slider.GetRangeMax())
	{
		LocateImage(pos);
		m_Slider.SetPos(pos);
		m_Slider.UpdateData();
	}
}
void CVideoLabelDlg::OnBnClickedLast1sec()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int fps = m_player.GetFPS();
	long pos = m_Slider.GetPos() - fps;
	if (pos >= m_Slider.GetRangeMin())
	{
		LocateImage(pos);
		m_Slider.SetPos(pos);
		m_Slider.UpdateData();
	}
}
void CVideoLabelDlg::OnBnClickedBtNext1sec()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int fps = m_player.GetFPS();
	long pos = m_Slider.GetPos() + fps;
	if (pos <= m_Slider.GetRangeMax())
	{
		LocateImage(pos);
		m_Slider.SetPos(pos);
		m_Slider.UpdateData();
	}
}


void CVideoLabelDlg::OnNMRClickTreLabel(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	HTREEITEM hItem = m_tre_label.GetSelectedItem();
	HTREEITEM hParent = m_tre_label.GetParentItem(hItem);
	int i = 0;
	while (hParent != NULL)
	{
		hParent = m_tre_label.GetParentItem(hParent);
		i++;
	}

	LPNMITEMACTIVATE pNMListView = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU4));
		if (i == 2)
			//�����㲻����ֻ��ɾ��
			menu.EnableMenuItem(ID__32788, 1);
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	
}
void CVideoLabelDlg::UpdateLabelXMLFileNode()
{
	TCHAR buffer[256] = { 0 };
	TCITEMW tabItem = GetTabSelectItem(m_tab_label, buffer);
	CString domain = tabItem.pszText;
	list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin();
	while (it != m_labelXMLFileNode.childrenList.end())
	{
		if (it->value == domain)
		{
			break;
		}
		it++;
	}
	if (it == m_labelXMLFileNode.childrenList.end())
		return;
	it->childrenList.clear();
	it->hasChildren = 0;
	HTREEITEM hItem = m_tre_label.GetRootItem();
	while (hItem != NULL)
	{
		OnUpdateLabelXMLFileNode(hItem, *it);
		hItem = m_tre_label.GetNextSiblingItem(hItem);
	}
}
void CVideoLabelDlg::OnUpdateLabelXMLFileNode(HTREEITEM hItem,LabelXMLFileNode& parent)
{
	parent.hasChildren = 1;
	LabelXMLFileNode node;
	node.hasChildren = 0;
	node.value = m_tre_label.GetItemText(hItem);
	HTREEITEM hChildItem = m_tre_label.GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		OnUpdateLabelXMLFileNode(hChildItem, node);
		hChildItem = m_tre_label.GetNextSiblingItem(hChildItem);
	}
	parent.childrenList.push_back(node);
}

void CVideoLabelDlg::OnPressAddLabelItem()
{
	// TODO:  �ڴ���������������
	HTREEITEM hItem = m_tre_label.GetSelectedItem();
	CTxtDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetText();
		m_tre_label.InsertItem(str, hItem);
	}
	//labelXML�ļ��޸�
	UpdateLabelXMLFileNode();
	CLabelXMLFileIOController::SaveLabelXMLFileNode(m_labelXMLFileNode);
}
void CVideoLabelDlg::OnPressDelLabelItem()
{
	// TODO:  �ڴ���������������
	HTREEITEM hItem = m_tre_label.GetSelectedItem();
	m_tre_label.DeleteItem(hItem);
	//labelXML�ļ��޸�
	UpdateLabelXMLFileNode();
	CLabelXMLFileIOController::SaveLabelXMLFileNode(m_labelXMLFileNode);

}


void CVideoLabelDlg::OnPressModifyLabelItem()
{
	// TODO:  �ڴ���������������
	POSITION pos = m_lst_show.GetFirstSelectedItemPosition();
	int index = m_lst_show.GetNextSelectedItem(pos);
	CString fileName = GetVideoFileTreePath(m_tre_file.GetSelectedItem(), FALSE);

	CClip clip,oldClip;
	oldClip = *CVideoLabelFileIOController::GetInstance()->GetIteratorOfClip(fileName, index);
	clip = oldClip;
	if (m_bg_pos >= 0)
		clip.start = m_bg_pos;
	if (m_ed_pos >= 0)
		clip.end = m_ed_pos;
	
	
	
	HTREEITEM selItem = m_tre_label.GetSelectedItem();
	if (selItem == NULL){
		AfxMessageBox(_T("δѡ�б�ǩ"));
		return;
	}
		
	CString str[3];
	int i = 0;
	HTREEITEM pItem = selItem;
	while (pItem != NULL)
	{
		str[i++] = m_tre_label.GetItemText(pItem);
		pItem = m_tre_label.GetParentItem(pItem);
	}
	CString type = str[--i];
	CString label = str[--i];
	CString sublabel = _T("");
	if (i > 0){
		sublabel = str[--i];
		label = label;
	}
	TCHAR buffer[256] = { 0 };
	TCITEMW tabItem = GetTabSelectItem(m_tab_label, buffer);
	CString domain = tabItem.pszText;
	clip.domain = domain;
	clip.label = label;
	clip.type = type;
	clip.sublabel = sublabel;
	if (m_str_roi.IsEmpty() == FALSE)
		if (CEventController::GetInstance()->CheckROIStringValid(m_str_roi))
		{
			clip.roi = m_str_roi;
			if (CEventController::GetInstance()->GetROI(m_str_roi, m_iROI[0], m_iROI[1], m_iROI[2], m_iROI[3]) == TRUE)
				m_drawROI = TRUE;
			m_str_roi = _T("");
			UpdateData(FALSE);
		}
		else
			AfxMessageBox(_T("����Ȥ���򲻺Ϸ���ROI���޸�"));
	CVideoLabelFileIOController::GetInstance()->ModifyClipLabel(fileName,index,clip);
	CVideoLabelFileIOController::GetInstance()->SaveFileToXML();

	CString startTime,endTime;
	startTime.Format(_T("%d"), clip.start);
	endTime.Format(_T("%d"), clip.end);
	m_lst_show.SetItemText(index, 1, startTime);
	m_lst_show.SetItemText(index, 2, endTime);
	m_lst_show.SetItemText(index, 3, clip.domain);
	m_lst_show.SetItemText(index, 4, clip.type);
	m_lst_show.SetItemText(index, 5, clip.label);
	m_lst_show.SetItemText(index, 6, clip.sublabel);
	m_lst_show.SetItemText(index, 7, clip.roi);
	LocateImage(m_player.GetDrawFramePos());
}


void CVideoLabelDlg::OnUseKeyFrame()
{
	// TODO:  �ڴ���������������
	m_bUseKeyFrame = TRUE;
}



void CVideoLabelDlg::OnDisableKeyFrame()
{
	// TODO:  �ڴ���������������
	m_bUseKeyFrame = FALSE;
}


void CVideoLabelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	USES_CONVERSION;
	UpdateData(TRUE);
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect picRect;
	m_picture.GetWindowRect(&picRect);
	ScreenToClient(&picRect);
	point.x -= picRect.left;
	point.y -= picRect.top;
	if (m_rect_pic_valid.PtInRect(point) == FALSE)
	{
		CDialogEx::OnLButtonUp(nFlags, point);
		return;
	}
	float gridWidth = m_rect_pic_valid.Width() / 4.0;
	float gridHeight = m_rect_pic_valid.Height() / 3.0;
	int col = (int)(point.x / gridWidth) + 1;
	int row = (int)(point.y / gridHeight) + 1;
	int num = row * 10 + col;
	if (m_str_roi.IsEmpty())
	{
		m_str_roi.Format(_T("%d"), num);
	}
	else
	{
		CString tmp;
		tmp.Format(_T("%d"), num);
		int index = m_str_roi.Find(tmp);
		if ( index== -1)
		{
			tmp = m_str_roi;
			m_str_roi.Format(_T("%s %d"), tmp, num);
		}
		else
		{
			if (index > 0)
			{
				m_str_roi.Replace(_T(" ")+tmp, _T(""));
			}
			else
			{
				m_str_roi.Replace(tmp, _T(""));
			}
		}
			
	}
	UpdateData(FALSE);
	LocateImage(m_player.GetDrawFramePos());
	CDialogEx::OnLButtonUp(nFlags, point);
}
