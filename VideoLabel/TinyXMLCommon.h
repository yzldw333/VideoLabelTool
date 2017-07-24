#pragma  once
#ifndef TINYXMLCOMMON_H
#define TINYXMLCOMMON_H

#include "stdafx.h"
#include "TinyXML/tinystr.h"
#include "TinyXML/tinyxml.h"

char* CreateUTF8TextInitWithString(CString strValue);
BOOL CreateString_InitWithUTF8Text(CString& str, const char* pUTF8Text);
BOOL TinyXMLElementSetCStringAttribute(TiXmlElement* e, char* attr, CString value);
#endif
