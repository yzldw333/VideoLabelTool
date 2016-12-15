// CTxtDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CTxtDialog.h"
#include "afxdialogex.h"
#include "resource.h"

// CTxtDialog �Ի���

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


// CTxtDialog ��Ϣ�������


void CTxtDialog::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
CString CTxtDialog::GetText()
{
	return m_str_text;
}
