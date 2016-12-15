#pragma once
#include "resource.h"

// CTxtDialog 对话框

class CTxtDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTxtDialog)

public:
	CTxtDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTxtDialog();

// 对话框数据
	enum { IDD = IDD_DLG_TXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CString m_str_text;
public:
	afx_msg void OnBnClickedOk();
	CString GetText();
};
