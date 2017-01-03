
// VideoLabelDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CVideoLabelDlg 对话框

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


// CVideoLabelDlg 消息处理程序

BOOL CVideoLabelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bInitDlg = TRUE;
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
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
	//list初始化
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
	m_lst_show.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);//整列拖拽
	UpdateData(FALSE);
	// TODO:  在此添加额外的初始化代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	
	
	m_Slider.SetMarginLeft(0);
	m_Slider.SetMarginRight(0);
	m_Slider.DrawFocusRect(FALSE);
	m_bg_pos = -1;
	m_ed_pos = -1;
	m_bUseKeyFrame = FALSE;
	//添加快捷键
	m_hAcc = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	CString folderPath, labelPath;
	if (CEventController::GetInstance()->ReadEnv(folderPath, labelPath) == TRUE)
	{
		OnImportLabelFile(labelPath);
		OnImportVideoDir(folderPath);
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CVideoLabelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bInitDlg == FALSE)
		return;
	// TODO: Add your message handler code here
	//创建对话框时，控件还没有创建，因此不能改变其大小(必须加上这两行代码)

	//计算窗口宽度和高度的改变量  
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
	SetTimer(4, 50, NULL);	//刷新专用
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CVideoLabelDlg::DrawTriangle(CDC* PDC, CPoint point, DWORD Color)
{
	/************************************************************************/
	/*             绘制小的倒等腰三角形，point为下方顶点位置                */
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
	CPaintDC dc(this); // 用于绘制的设备上下文
	if (IsIconic())
	{
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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
		//刷新
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
		m_stt_time.Format(_T("当前帧数：%d"), pos+1);
		UpdateData(FALSE);
		break;
	}
	case 2:
	{
		//opencv读取缓存
		if (m_player.ReadFrameToBuffer() == FALSE)
		{
			KillTimer(2);
			break;
		}
		break;
	}
	case 3:
	{
		//准备绘图的缓存
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

	// 获取图像宽和高,注意这里的宽和高不是图像的分辨率  
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
void CVideoLabelDlg::ShowAndReleaseImg(IplImage* img,CDC * pDC,int windowWidth, int windowHeight)    // ID 是Picture Control控件的ID号
{
	int iWndWidth = windowWidth;    
	int iWndHeight = windowHeight;
	int hmWidth = img->width;
	int hmHeight = img->height;

	// 获取图像宽和高,注意这里的宽和高不是图像的分辨率  
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
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO:  在此添加命令处理程序代码
	CString videoDir = CEventController::GetInstance()->OnImportVideoDirectory();
	OnImportVideoDir(videoDir);
}
void CVideoLabelDlg::OnImportVideoDir(CString videoPath)
{
	if (!PathFileExistsW(videoPath))
	{
		AfxMessageBox(_T("路径有误！"));
	}
	else
	{
		//prepare
		m_tre_file.DeleteAllItems();
		CVideoLabelFileIOController* fileController = CVideoLabelFileIOController::GetInstance();
		fileController->SetFolderPath(videoPath);
		fileController->SetStorePath(videoPath + _T("\\data.vlcf.xml"));
		//创立文件夹
		HTREEITEM hRoot = m_tre_file.InsertItem(videoPath);
		InitVideoFileTreeView(hRoot);
		m_tre_file.Expand(hRoot, TVE_EXPAND);
		if (fileController->ReadFileFromXML() == FALSE)
		{
			AfxMessageBox(_T("源目录没有标注记录文件哟！现在从零开始！"));
		}
		else
		{
			OnRefreshListShowCtrl();
		}
	}
}
void CVideoLabelDlg::OnBnClickImportLabelFile()
{
	// TODO:  在此添加命令处理程序代码
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
	//刷新界面

	//标签页刷新、标签树形结构刷新
	TCITEM item;
	item.mask = TCIF_TEXT;
	m_tab_label.DeleteAllItems();
	int index = 0;
	for (list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin(); it != m_labelXMLFileNode.childrenList.end(); it++)
	{
		//标签tab控件插入
		item.pszText = it->value.LockBuffer();
		m_tab_label.InsertItem(index++, &item);
	}
	//设置第一个页面被激活
	m_tab_label.SetCurSel(0);
	//主动调用函数刷新页面
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
	// TODO:  在此添加控件通知处理程序代码
	int index = m_tab_label.GetCurSel();
	//寻找标签页的数据
	int i = 0;
	list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin();
	while (i < index)
	{
		i++;
		it++;
	}
	//插入数据
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
	CString strText = GetVideoFileTreePath(hparent,TRUE);  //检索列表中项目文字  
	if (strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符  
		strText += _T("\\");
	strText += _T("*.*");
	//将当前目录下文件枚举并InsertItem树状显示  
	CFileFind file;                                       //定义本地文件查找  
	BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件  
	while (bContinue)
	{
		bContinue = file.FindNextFile();                  //查找下一个文件  
		if (!file.IsDirectory())          //找到文件为内容且不为点"."  
		{
			CString fileName = file.GetFileName();
			CString postFix = fileName.Right(3);
			if (postFix == "mp4" || postFix == "MP4" || postFix == "mkv" || postFix == "MKV" || postFix == "avi" || postFix == "AVI")
			{
				HTREEITEM hItem = m_tre_file.InsertItem(fileName, hparent); //添加盘符路径下树状文件夹
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
	//OnTreeView(hparent, m_tre_filePath.GetItemText(hparent));								//第二层往后递归调用
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
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM selectItem = m_tre_file.GetSelectedItem();
	if (m_tre_file.GetParentItem(selectItem) == NULL)
		return;
	CString videoPath = GetVideoFileTreePath(selectItem, TRUE);
	DWORD dwAttr = GetFileAttributes(videoPath);
	if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
	{//文件
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
		info.Format(_T("视频：%s 存在问题"), tmp);
		AfxMessageBox(info);
		return;
	}
		
	
	// 将解码得到图像信息从缓存中转换成IplImage格式放在frame中
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
	// TODO:  在此添加控件通知处理程序代码
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
	m_player.SetFramePos(m_Slider.GetPos());  //设置视频的起始帧
	m_bt_play_pause.SetWindowTextW(_T("PLAY"));
	m_player.PrepareBuffer();
	IplImage * img;
	m_player.ReadFrameFromBuffer(&img);
	PrepareBitmapAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	CBitmap* bmp = bitmapBuf.front();
	ShowBitMap(bmp, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	m_stt_time.Format(_T("当前帧数：%ld"), m_Slider.GetPos());
	UpdateData(FALSE);
	return 0;
}

void CVideoLabelDlg::OnBnClickedBtBg()
{
	OnBnClickedBtEd();
	// TODO:  在此添加控件通知处理程序代码
	m_bg_pos = m_player.GetDrawFramePos();
	if (m_bg_pos == -1)
		m_bg_pos = 0;
	ShowTriangleMark();
	//合法性检查：标记的开始位置应该在结束位置左侧
}

void CVideoLabelDlg::OnBnClickedBtEd()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ed_pos = m_player.GetDrawFramePos();
	ShowTriangleMark();
}
void CVideoLabelDlg::OnBnClickedBtClearpoint()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bg_pos = -1;
	m_ed_pos = -1;
	m_str_roi = _T("");
	ShowTriangleMark();
	UpdateData(FALSE);
}

void CVideoLabelDlg::OnBnClickedBtAddlabel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_ed_pos>=m_bg_pos)
	{
		UpdateData(TRUE);
		if (CEventController::GetInstance()->CheckROIStringValid(m_str_roi) == FALSE)
		{
			AfxMessageBox(_T("感兴趣区域不合法！"));
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
		AfxMessageBox(_T("开始时间需要设置在结束时间之前！"));
	}
}
BOOL CVideoLabelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	
	//暂时没起作用，拖动时候的刷新
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加命令处理程序代码
	////删除整行选中的===============
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
	// TODO:  在此添加控件通知处理程序代码
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
	m_player.SetFramePos(pos);  //设置视频的起始帧
	m_bt_play_pause.SetWindowTextW(_T("PLAY"));
	IplImage * img;
	m_player.ReadTmpFrameWithPos(&img, pos);
	m_Slider.SetPos(pos);
	ShowAndReleaseImg(img, m_picture.GetDC(), m_rect_pic.Width(), m_rect_pic.Height());
	m_stt_time.Format(_T("当前帧数：%ld"), pos);
	UpdateData(FALSE);
	return;
}
void CVideoLabelDlg::OnBnClickedBtLastKey()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
			//第三层不添加项，只能删除
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
	// TODO:  在此添加命令处理程序代码
	HTREEITEM hItem = m_tre_label.GetSelectedItem();
	CTxtDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetText();
		m_tre_label.InsertItem(str, hItem);
	}
	//labelXML文件修改
	UpdateLabelXMLFileNode();
	CLabelXMLFileIOController::SaveLabelXMLFileNode(m_labelXMLFileNode);
}
void CVideoLabelDlg::OnPressDelLabelItem()
{
	// TODO:  在此添加命令处理程序代码
	HTREEITEM hItem = m_tre_label.GetSelectedItem();
	m_tre_label.DeleteItem(hItem);
	//labelXML文件修改
	UpdateLabelXMLFileNode();
	CLabelXMLFileIOController::SaveLabelXMLFileNode(m_labelXMLFileNode);

}


void CVideoLabelDlg::OnPressModifyLabelItem()
{
	// TODO:  在此添加命令处理程序代码
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
		AfxMessageBox(_T("未选中标签"));
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
			AfxMessageBox(_T("感兴趣区域不合法，ROI无修改"));
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
	// TODO:  在此添加命令处理程序代码
	m_bUseKeyFrame = TRUE;
}



void CVideoLabelDlg::OnDisableKeyFrame()
{
	// TODO:  在此添加命令处理程序代码
	m_bUseKeyFrame = FALSE;
}


void CVideoLabelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	USES_CONVERSION;
	UpdateData(TRUE);
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
