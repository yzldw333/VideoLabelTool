// DlgImportVideo.cpp : �����ļ��Ի���
//

#include "stdafx.h"
#include "CImportDlg.h"
#include "afxdialogex.h"
#include <Shlwapi.h>

// CDlgImportVideo �Ի���

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)


CImportDlg::CImportDlg(ImportDlgType type, CWnd* pParent)
	: CDialogEx(CImportDlg::IDD, pParent)
	, pathEdit(_T(""))
{
	this->type = type;
}
CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IVD_PATH_EDT, pathEdit);
	DDX_Control(pDX, IDC_IV_DLG_OPEN_BUT, openButton);
}

// CDlgImportVideo ��Ϣ�������
BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_IV_DLG_OPEN_BUT, &CImportDlg::OnBnClickedOpenBut)
	ON_BN_CLICKED(IDOK, &CImportDlg::OnBnClickedOk)
//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CImportDlg::OnInitDialog()
{
	switch (type)
	{
	case TYPE_CIMPDIRDLG:
		this->SetWindowTextW(_T("ѡ���ļ���"));
		break;
	case TYPE_CIMPFILEDLG:
		this->SetWindowTextW(_T("ѡ���ļ�"));
		break;
	}
	return true;
}

//�򿪵��봰�ڣ����ݳ�ʼ���õ���type���ɶ�Ӧ�ĶԻ���
void CImportDlg::OnBnClickedOpenBut()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	switch (type)
	{
	case TYPE_CIMPDIRDLG:
	{
		CFolderPickerDialog folderDlg(NULL, 0, this, 0);
		if (folderDlg.DoModal() == IDOK)
		{
			CString tmpPath = folderDlg.GetPathName();
			pathEdit = tmpPath;
			this->UpdateData(FALSE);
		}
		break;
	}
	case TYPE_CIMPFILEDLG:
	{
		CFileDialog fileDlg(TRUE, NULL, NULL, NULL);
		if (fileDlg.DoModal() == IDOK)
		{
			CString tmpPath = fileDlg.GetPathName();
			pathEdit = tmpPath;
			this->UpdateData(FALSE);
		}
		break;
	}
	}
	
}

void CImportDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	pathName = pathEdit;

	CDialogEx::OnOK();
}
CString CImportDlg::GetPathName()
{
	return pathName;
}

