
// VideoLabelDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
#include "afxwin.h"
#include "cvPlayer.h"
#include "LabelXMLFile.h"
#include <queue>
#include <vector>
#include "CBitmapSlider\BitmapSlider.h"

#define WM_PREVIEW_IMAGE_WITH_POS WM_USER+930509
using namespace std;
// CVideoLabelDlg 对话框
class CVideoLabelDlg : public CDialogEx
{
// 构造
public:
	CVideoLabelDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VIDEOLABEL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
public:
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	void DrawTriangle(CDC* PDC, CPoint point, DWORD Color);
	DECLARE_MESSAGE_MAP()
	CString GetVideoFileTreePath(HTREEITEM htreeItem, BOOL rootNeeded);
	void InitVideoFileTreeView(HTREEITEM hparent);
	void ShowCutPoint(CDC * pDC, int windowWidth, int windowHeight);
	void ClearBitmap();
	void ShowAndReleaseImg(IplImage* img, CDC * pDC,int windowWidth,int windowHeight);
	void ShowBitMap(CBitmap *bitmap, CDC* pDC, int windowWidth, int windowHeight);
	void PrepareBitmapAndReleaseImg(IplImage* img, CDC *pDC, int windowWidth, int windowHeight);
	void OnRefreshLabelTreeViewCtrl(HTREEITEM hTreeItem, LabelXMLFileNode node);
	void OnRefreshListShowCtrl();
	void LocateImage(long pos);
	afx_msg void OnExitSubVideoState();
	void ShowTriangleMark();
	TCITEMW CVideoLabelDlg::GetTabSelectItem(CTabCtrl& tabCtrl, TCHAR *buffer);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTcnSelchangeTabLabel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtPlayPause();
	afx_msg void CVideoLabelDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickImportLabelFile();
	afx_msg void OnBnClickImportVideoDir();
	void OnImportVideoDir(CString videoPath);
	void OnImportLabelFile(CString labelPath);
	afx_msg void OnBnClickedBtBg();
	afx_msg void OnBnClickedBtEd();
	afx_msg void OnBnClickedBtAddlabel();
	afx_msg LRESULT OnSliderMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReleaseSliderMessage(WPARAM wParam, LPARAM lParam);
	void OnPreviewImageWithPos();
	void UpdateLabelXMLFileNode();
	void OnUpdateLabelXMLFileNode(HTREEITEM hItem, LabelXMLFileNode &parent);
protected:
	HICON m_hIcon;
	CTreeCtrl m_tre_file;
	CTreeCtrl m_tre_label;
	CTabCtrl m_tab_label;
	CButton m_bt_addlabel;
	CButton m_bt_bg;
	CButton m_bt_ed;
	long m_bg_pos;
	long m_ed_pos;
	HACCEL m_hAcc_Left;
	HACCEL m_hAcc_Right;
	HACCEL m_hAcc_SPACE;
	HACCEL m_hAcc;
	CStatic m_picture;
	CRect m_rect_pic;
	CRect m_rect_thb_bg;
	CRect m_rect_thb_ed;
	CVPlayer m_player;
	CButton m_bt_play_pause;
	queue<CBitmap *> bitmapBuf;
	CBitmap* tmpBitmap;
	LabelXMLFileNode m_labelXMLFileNode;
//	CSliderCtrl m_sld_player;
	vector<int> m_vec_cutPoint;
	CListCtrl m_lst_show;
	CBitmapSlider m_Slider;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtClearpoint();
	afx_msg void OnNMDblclkLstShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelLabel();
	afx_msg void OnNMRClickLstShow(NMHDR *pNMHDR, LRESULT *pResult);
private:

	CString m_stt_time;
	CStatic m_Slider_tip;
public:
	afx_msg void OnNMDblclkTreFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtLastKey();
	afx_msg void OnBnClickedBtNextKey();
	void OnBnClickedLastFrame();
	void OnBnClickedNextFrame();
	CButton m_bt_lastkey;
	CButton m_bt_nextkey;
	CStatic m_pic_mark;
	afx_msg void OnBnClickedLast1sec();
	afx_msg void OnBnClickedBtNext1sec();
	afx_msg void OnNMRClickTreLabel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPressAddLabelItem();
	afx_msg void OnPressDelLabelItem();
};
