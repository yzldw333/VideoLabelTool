#include "stdafx.h"
#include "TinyXMLCommon.h"
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