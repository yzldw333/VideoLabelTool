#pragma once
#include "resource.h"

// CTxtDialog �Ի���

class CTxtDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTxtDialog)

public:
	CTxtDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTxtDialog();

// �Ի�������
	enum { IDD = IDD_DLG_TXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CString m_str_text;
public:
	afx_msg void OnBnClickedOk();
	CString GetText();
};
