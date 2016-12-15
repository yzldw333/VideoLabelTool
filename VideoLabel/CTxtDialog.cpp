// CTxtDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CTxtDialog.h"
#include "afxdialogex.h"
#include "resource.h"

// CTxtDialog 对话框

IMPLEMENT_DYNAMIC(CTxtDialog, CDialogEx)

CTxtDialog::CTxtDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxtDialog::IDD, pParent)
	, m_str_text(_T(""))
{

}

CTxtDialog::~CTxtDialog()
{
}

void CTxtDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TXT, m_str_text);
}


BEGIN_MESSAGE_MAP(CTxtDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTxtDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTxtDialog 消息处理程序


void CTxtDialog::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
CString CTxtDialog::GetText()
{
	return m_str_text;
}
