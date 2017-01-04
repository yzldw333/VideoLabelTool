#include "stdafx.h"
#include "EventController.h"
#include "LabelXMLFile.h"
//#include "CVideoLabelFile.h"
#include "CImportDlg.h"
#include "Resource.h"
#include <locale>//头文件
#include <vector>
using namespace std;
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
		//SaveEnv(m_videoPath, m_labelPath);
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
		//SaveEnv(m_videoPath, m_labelPath);
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
	m_videoPath = folderPath;
	f.ReadString(labelXMLPath);
	m_labelPath = labelXMLPath;
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
	f.Close();
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
BOOL CEventController::CheckROIStringValid(CString& str)
{
	USES_CONVERSION;
	BOOL valid = TRUE;
	list<int>v;
	int maxRow = -1;
	int minRow = 4;
	int maxCol = -1;
	int minCol = 5;
	str = str.Trim();
	if (str == _T(""))
		str = _T("11 12 13 14 21 22 23 24 31 32 33 34");

	CString procStr = str;
	while (true)
	{
		CString n = procStr.SpanExcluding(_T(" "));

		int num = _ttoi(n);
		if (num <=0||num<10||num>99)
		{
			valid = FALSE;
		}
			
		int row = num / 10;
		int col = num % 10;
		if (row > maxRow)
		{
			maxRow = row;
		}
		if (row < minRow)
		{
			minRow = row;
		}
		if (col > maxCol)
		{
			maxCol = col;
		}
		if (col < minCol)
		{
			minCol = col;
		}
		v.push_back(num);
		procStr = procStr.Right(procStr.GetLength() - n.GetLength() - 1);
		if (procStr.IsEmpty())
		{
			break;
		}
	}
	if (valid == FALSE)
		return FALSE;
	for (int i = minRow; i <= maxRow&&valid; i++)
		for (int j = minCol; j <= maxCol&&valid; j++)
		{
			int tmp = i * 10 + j;
			int find = 0;
			for (list<int>::iterator it = v.begin(); it != v.end(); it++)
			{
				if (*it == tmp)
					find = 1;
			}
			if (find == 0)
			{
				valid = FALSE;
				break;
			}
				
		}
	return valid;

}

BOOL CEventController::GetROI(CString str, int& startRow, int& startCol, int& endRow, int& endCol)
{
	if (CheckROIStringValid(str) == FALSE)
		return FALSE;
	USES_CONVERSION;
	int maxRow = -1;
	int minRow = 4;
	int maxCol = -1;
	int minCol = 5;
	int row = 0;
	int col = 0;
	CString procStr = str;
	while (true)
	{
		CString n = procStr.SpanExcluding(_T(" "));
		row = _ttoi(n.Left(1));
		col = _ttoi(n.Right(1));
		if (row > maxRow)
		{
			maxRow = row;
		}
		if (row < minRow)
		{
			minRow = row;
		}
		if (col > maxCol)
		{
			maxCol = col;
		}
		if (col < minCol)
		{
			minCol = col;
		}
		procStr = procStr.Right(procStr.GetLength() - n.GetLength() - 1);
		if (procStr.IsEmpty())
		{
			break;
		}
	}
	startRow = minRow;
	startCol = minCol;
	endRow = maxRow;
	endCol = maxCol;
	return TRUE;
}
list<int> CEventController::GetSquareListROI(CString str)
{
	USES_CONVERSION;
	BOOL valid = TRUE;
	list<int>v;
	int maxRow = -1;
	int minRow = 4;
	int maxCol = -1;
	int minCol = 5;
	str = str.Trim();
	if (str == _T(""))
	{
		return v;
	}

	CString procStr = str;
	while (true)
	{
		CString n = procStr.SpanExcluding(_T(" "));

		int num = _ttoi(n);
		if (num <= 0 || num < 10 || num>99)
		{
			valid = FALSE;
		}

		int row = num / 10;
		int col = num % 10;
		if (row > maxRow)
		{
			maxRow = row;
		}
		if (row < minRow)
		{
			minRow = row;
		}
		if (col > maxCol)
		{
			maxCol = col;
		}
		if (col < minCol)
		{
			minCol = col;
		}
		v.push_back(num);
		procStr = procStr.Right(procStr.GetLength() - n.GetLength() - 1);
		if (procStr.IsEmpty())
		{
			break;
		}
	}
	return v;
}
