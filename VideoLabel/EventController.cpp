#include "stdafx.h"
#include "EventController.h"
#include "LabelXMLFile.h"
//#include "CVideoLabelFile.h"
#include "CImportDlg.h"
#include "Resource.h"
#include <locale>//头文件
CEventController* CEventController::_instance = NULL;
CEventController::Garbor CEventController::garbor;
CEventController* CEventController::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new CEventController();
	}
	return _instance;
}
CString CEventController::OnImportVideoDirectory()
{
	CImportDlg dlg(TYPE_CIMPDIRDLG);
	if (dlg.DoModal() == IDOK)
	{
		m_videoPath = dlg.GetPathName();
		SaveEnv(m_videoPath, m_labelPath);
		return m_videoPath;
	}
	else
	{
		return NULL;
	}
}
CString CEventController::OnImportLabelFile()
{
	CString empty;
	CImportDlg dlg(TYPE_CIMPFILEDLG);
	if (dlg.DoModal() == IDOK)
	{
		m_labelPath = dlg.GetPathName();
		if (!PathFileExistsW(m_labelPath))
		{
			AfxMessageBox(_T("路径有误！"));
			return empty;
		}
		SaveEnv(m_videoPath, m_labelPath);
		return m_labelPath;
	}
	else
	{
		return empty;
	}
}
BOOL CEventController::ReadEnv(CString& folderPath, CString &labelXMLPath)
{
	CStdioFile f;
	BOOL res = f.Open(_T("env.dat"),CFile::modeRead);
	if (res == FALSE)
		return FALSE;
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定
	f.ReadString(folderPath);
	f.ReadString(labelXMLPath);
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
	return TRUE;
}
void CEventController::SaveEnv(CString folderPath, CString labelXMLPath)
{
	USES_CONVERSION;
	if (folderPath.IsEmpty() || labelXMLPath.IsEmpty())
		return;
	CStdioFile f;
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定
	DeleteFile(_T("env.dat"));
	f.Open(_T("env.dat"), CFile::modeCreate | CFile::modeWrite);
	folderPath.Replace(_T("\\"), _T("/"));
	labelXMLPath.Replace(_T("\\"), _T("/"));
	f.WriteString(folderPath);
	f.Write("\n", 1);
	f.WriteString(labelXMLPath);
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
	f.Close();
	
}
//BOOL CEventController::OnAddLabel(CString fileName, CString domain, CString type, CString labelName, CRect drawrect, CRect winRect, CRect picRect, COLORREF color)
//{
//	CImageLabelFileIOController::GetInstance()->AddImageLabel(fileName, );
//	CLabel label;
//	label.rect = GetFrect(drawrect, winRect, picRect);
//	label.domain = domain;
//	label.type = type;
//	label.color = color;
//	label.name = labelName;
//	CImageLabelFileIOController::GetInstance()->AddImageLabel(fileName, label);
//	CImageLabelFileIOController::GetInstance()->SaveFileToXML();
//	return TRUE;
//}