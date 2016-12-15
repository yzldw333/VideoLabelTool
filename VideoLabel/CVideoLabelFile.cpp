#include "stdafx.h"
#include "CVideoLabelApp.h"
#include "CVideoLabelFile.h"
#include <list>
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include  <afxpriv.h>
CVideoLabelFileIOController* CVideoLabelFileIOController::_instance = NULL;
CVideoLabelFileIOController::Garbor CVideoLabelFileIOController::garbor;
char* CreateUTF8TextInitWithString(CString strValue)
{
	char *buffer = NULL;
	int  length;

#ifdef _UNICODE  
	length = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, NULL, 0, NULL, NULL);
#else  
	return NULL;
#endif  
	if (length <= 0)
	{
		return NULL;
	}
	buffer = new char[length];
	if (buffer == NULL)
	{
		return NULL;
	}
	ZeroMemory(buffer, length);
#ifdef _UNICODE  
	WideCharToMultiByte(CP_UTF8, 0, strValue, -1, buffer, length, NULL, NULL);
#else  
	strcpy_s(buffer, length, strValue);
#endif  
	return buffer;
}


BOOL CreateString_InitWithUTF8Text(CString& str, const char* pUTF8Text)
{
	if (NULL == pUTF8Text)
	{
		return FALSE;
	}
	int  unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		pUTF8Text,
		-1,
		NULL,
		0);
	wchar_t*  pUnicode = new  wchar_t[unicodeLen + 1];
	if (NULL == pUnicode)
	{
		return FALSE;
	}
	MultiByteToWideChar(CP_UTF8, 0, pUTF8Text, -1, (LPWSTR)pUnicode, unicodeLen);

	str = pUnicode;

	delete[]pUnicode;
	return TRUE;
}

BOOL TinyXMLElementSetCStringAttribute(TiXmlElement* e, char* attr, CString value)
{
	char*utf_buffer;
	utf_buffer = CreateUTF8TextInitWithString(value);
	e->SetAttribute(attr, utf_buffer);
	free(utf_buffer);
	return TRUE;
}
void CVideoLabelFileIOController::SetFolderPath(CString folderPath)
{
	this->folderPath = folderPath;
}
void CVideoLabelFileIOController::SetStorePath(CString storePath)
{
	this->storePath = storePath;
}
list<CVideoFile>::iterator     CVideoLabelFileIOController::GetIteratorOfFile(CString fileName)
{
	list<CVideoFile>::iterator it;
	for (it = videoFileList.begin(); it != videoFileList.end(); it++)
	{
		if ((*it).fileName == fileName)
		{
			return it;
		}
	}
	return videoFileList.end();
}
list<CClip>::iterator CVideoLabelFileIOController::GetIteratorOfClip(CString fileName, int index)
{
	list<CVideoFile>::iterator it = GetIteratorOfFile(fileName);
	list<CClip>::iterator itt = it->clipList.begin();
	int i = 0;
	while (itt != it->clipList.end()&&i!=index)
	{
		i++;
		itt++;
	}
	return itt;
}

void CVideoLabelFileIOController::AddVideoFile(CString fileName)
{
	if (GetIfExistFile(fileName) == false)
	{
		CVideoFile file;
		file.fileName = fileName;
		videoFileList.push_back(file);
	}

}

void CVideoLabelFileIOController::AddClipLabel(CString fileName, long long start, long long end, CString domain, CString type, CString label)
{
	list<CVideoFile>::iterator it = GetIteratorOfFile(fileName);
	if (it == videoFileList.end())
	{
		AddVideoFile(fileName);
	}
	it = GetIteratorOfFile(fileName);
	if (it != videoFileList.end())
	{
		CClip clip;
		clip.start = start;
		clip.end = end;
		clip.type = type;
		clip.label = label;
		clip.domain = domain;
		(*it).clipList.push_back(clip);
	}

	isChanged = true;
}
void CVideoLabelFileIOController::ModifyClipLabel(CString fileName, long long start, long long end, CString domain, CString type, CString label)
{
	/*list<CVideoFile>::iterator it = GetIteratorOfFile(fileName);
	if (it != videoFileList.end())
	{
	list<CClip>::iterator itt = (*it).clipList.begin();
	while (itt != it->clipList.end())
	{
	itt++;
	}
	}
	itt->domain = domain;
	if (itt->start != start || itt->end != end)
	{
	if (end != 0)
	itt->end = itt->start + end;
	itt->start= itt->start + start;
	}
	itt->type = type;
	itt->label = label;
	itt->clipFileName = newClipFileName;
	isChanged = true; */
}
void CVideoLabelFileIOController::DeleteClipLabel(CString fileName, int index)
{
	list<CVideoFile>::iterator it = GetIteratorOfFile(fileName);
	if (it != videoFileList.end())
	{
		list<CClip>::iterator itt = (*it).clipList.begin();
		int i=0;
		while (itt != it->clipList.end())
		{
			if (i==index)
			{
				it->clipList.erase(itt);
				break;
			}
			i++;
			itt++;
		}
	}
	isChanged = true;
}
CString TimeConvert(long long ti)
{
	CString res;
	int hour = ti / 3600000;
	int minn = (ti-3600000*hour) / 60000;
	int secc = (ti-3600000*hour-minn*60000) / 1000;
	int ms = ti % 1000;
	res.Format(_T("%02d:%02d:%02d:%03d"), hour, minn, secc, ms);
	return res;
}
long long TimeConvert(CString str)
{
	int hour, minn, secc, ms;
	swscanf_s(str, _T("%02d:%02d:%02d:%03d"), &hour, &minn, &secc, &ms);
	long long ti = hour * 3600000 + minn * 60000 + secc * 1000 + ms;
	return ti;
}
bool CVideoLabelFileIOController::SaveFileToXML()
{
	return SaveFileToXML(storePath);
}
bool CVideoLabelFileIOController::SaveFileToXML(CString path)
{
	USES_CONVERSION;
	char * xmlPath = T2A(path);
	DeleteFile(path);
	TiXmlDocument* doc = new TiXmlDocument(xmlPath);
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	doc->LinkEndChild(decl);
	TiXmlElement* videoClipLabel = new TiXmlElement("VideoClipLabel");
	doc->LinkEndChild(videoClipLabel);
	TiXmlElement *folder = new TiXmlElement("Folder");
	char * utf_buffer;
	utf_buffer = CreateUTF8TextInitWithString(folderPath);
	folder->SetAttribute("path", utf_buffer);
	free(utf_buffer);
	videoClipLabel->LinkEndChild(folder);
	list<CVideoFile>::iterator it;
	for (it = videoFileList.begin(); it != videoFileList.end(); it++)
	{
		TiXmlElement *file = new TiXmlElement("VFile");
		utf_buffer = CreateUTF8TextInitWithString((*it).fileName);
		file->SetAttribute("name", utf_buffer);
		free(utf_buffer);
		list<CClip>::iterator itt;
		int i = 0;
		for (itt = (*it).clipList.begin(); itt != (*it).clipList.end(); itt++)
		{
			TiXmlElement* clip = new TiXmlElement("VClip");
			CString clipName;
			clipName = itt->clipFileName;
			utf_buffer = CreateUTF8TextInitWithString(clipName);
			clip->SetAttribute("name", utf_buffer);
			free(utf_buffer);
			clip->SetAttribute("start", itt->start);
			clip->SetAttribute("end", itt->end);
			utf_buffer = CreateUTF8TextInitWithString(itt->type);
			clip->SetAttribute("type", utf_buffer);
			free(utf_buffer);
			utf_buffer = CreateUTF8TextInitWithString(itt->label);
			clip->SetAttribute("label", utf_buffer);
			free(utf_buffer);
			utf_buffer = CreateUTF8TextInitWithString(itt->domain);
			clip->SetAttribute("domain", utf_buffer);
			free(utf_buffer);
			file->LinkEndChild(clip);
		}
		folder->LinkEndChild(file);
	}
	doc->SaveFile(); 
	delete doc;
	isChanged = false;
	return true;
}

bool CVideoLabelFileIOController::ReadFileFromXML()
{
	return ReadFileFromXML(storePath);
}
bool CVideoLabelFileIOController::ReadFileFromXML(CString path)
{
	//初始化环境
	videoFileList.clear();

	USES_CONVERSION;
	char * xmlPath = T2A(path);
	TiXmlDocument* doc = new TiXmlDocument(xmlPath);
	bool bSuccess = doc->LoadFile(TIXML_ENCODING_UTF8);
	if (!bSuccess) {
		TRACE("Could not load test file %s. Error='%s'. Exiting.\n", xmlPath, doc->ErrorDesc());
		delete doc;
		return false;
	}
	TiXmlElement* root = doc->RootElement();
	TiXmlElement* folder = root->FirstChildElement("Folder");
	CreateString_InitWithUTF8Text(folderPath, folder->Attribute("path"));
	TiXmlElement* file = folder->FirstChildElement("VFile");
	int fileNum = 0;
	
	/************************************************************************/
	/*                       获得File 数量                                  */
	/************************************************************************/
	while (file != NULL)
	{
		TiXmlElement* clip = file->FirstChildElement("VClip");
		CVideoFile cfile;
		CreateString_InitWithUTF8Text(cfile.fileName, file->Attribute("name"));
		while (clip != NULL)
		{
			CClip cclip;
			CString tiStr;
			long ti;
			CreateString_InitWithUTF8Text(tiStr, clip->Attribute("start"));
			
			swscanf_s(tiStr, _T("%ld"), &ti);
			cclip.start = ti;
			CreateString_InitWithUTF8Text(tiStr, clip->Attribute("end"));
			swscanf_s(tiStr, _T("%ld"), &ti);
			cclip.end = ti;
			CreateString_InitWithUTF8Text(cclip.type, clip->Attribute("type"));
			CreateString_InitWithUTF8Text(cclip.domain, clip->Attribute("domain"));
			CreateString_InitWithUTF8Text(cclip.label, clip->Attribute("label"));
			CreateString_InitWithUTF8Text(cclip.clipFileName, clip->Attribute("name"));
			cfile.clipList.push_back(cclip);
			clip = clip->NextSiblingElement("VClip");
		}
		fileNum++;
		videoFileList.push_back(cfile);
		file = file->NextSiblingElement("VFile");
	}
	
	delete doc;
	return true;
}
bool CVideoLabelFileIOController::GetIfExistFile(CString fileName)
{
	list<CVideoFile>::iterator it;
	for (it = videoFileList.begin(); it != videoFileList.end(); it++)
	{
		if ((*it).fileName == fileName)
			return true;
	}
	return false;
}