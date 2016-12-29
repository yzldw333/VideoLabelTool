#ifndef CVIDEOLABELCLIPFILE_H
#define CVIDEOLABELCLIPFILE_H
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                       视频标注-片段标注记录文件                      */
/************************************************************************/
#include "stdafx.h"
#include <list>
using namespace std;


//视频片段标注所需结构体
typedef struct CClip
{
	//HTREEITEM hTreeItem;				//便于在树形结构上对clip进行点选获取信息和修改
	long long start;
	long long end;
	CString type;
	CString label;
	CString sublabel;
	CString domain;
	CString roi;
	CString clipFileName;
}CClip;

typedef struct CVideoFile
{
	CString fileName;
	list<CClip> clipList;
}CVideoFile;


//标注文件基类
class CVideoLabelFileIOController
{
private:
	static CVideoLabelFileIOController* _instance;
	class Garbor{
	public:
		Garbor(){
			printf("Garbor constructed!");
		}
		~Garbor(){
			printf("Garbor destruct!");
			//if (CVideoLabelFileIOController::_instance)
				delete CVideoLabelFileIOController::_instance;
		};
	};
public:
	static Garbor garbor;
	static CVideoLabelFileIOController* GetInstance()
	{
		if (_instance == NULL)
		{
			_instance = new CVideoLabelFileIOController();
		}
		return _instance;
	}
	//////////////////////////////////////////////////////////////////////////

public:
	//本文件的地址信息
	CString storePath;


	//需要存储的信息
	CString folderPath;
	list<CVideoFile> videoFileList;
	bool isChanged;
	void SetFolderPath(CString folderPath);
	void SetStorePath(CString storePath);
	bool SaveFileToTMP();
	bool ReadFileFromTMP();
	bool SaveFileToXML();
	bool SaveFileToXML(CString path);
	bool ReadFileFromXML(CString path);
	bool ReadFileFromXML();
	void AddVideoFile(CString fileName);
	list<CVideoFile>::iterator GetIteratorOfFile(CString fileName);
	list<CClip>::iterator GetIteratorOfClip(CString fileName, int index);
	void AddClipLabel(CString fileName, long long start, long long end, CString roi, CString domain, CString type, CString label,CString sublabel);
	void ModifyClipLabel(CString fileName,int index,CClip newClip);
	void DeleteClipLabel(CString fileName, int index);
	bool GetIfExistFile(CString fileName);
};

#endif