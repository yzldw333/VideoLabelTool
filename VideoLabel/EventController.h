
// EventController.h
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "LabelXMLFile.h"
//#include "LabelXMLFile.h"

// CImageLabelApp: 
// 有关此类的实现，请参阅 ImageLabel.cpp
//

class CEventController
{

public:
	CString m_labelPath;
	CString m_videoPath;
	static CEventController* GetInstance();
	CString OnImportLabelFile();
	CString OnImportVideoDirectory();
	void OnDeleteImage();
	void OnModifyLabel();
	void SaveEnv(CString folderPath, CString labelXMLPath);
	BOOL ReadEnv(CString& folderPath, CString &labelXMLPath);
	BOOL CheckROIStringValid(CString& str);
	BOOL GetROI(CString str, int& startRow, int& startCol, int& endRow, int& endCol);
private:
	CEventController(){}
	static CEventController* _instance;
	class Garbor{
	public:
		Garbor(){
			printf("Garbor constructed!");
		}
		~Garbor(){
			printf("Garbor destruct!");
			delete CEventController::_instance;
		};
	};
public:
	static Garbor garbor;
	
};
