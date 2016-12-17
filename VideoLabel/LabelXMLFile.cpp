#include "stdafx.h"
#include "LabelXMLFile.h"
#include "common.hpp"
#include "TinyXMLCommon.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include  <afxpriv.h>
CString CLabelXMLFileIOController::fileName;
LabelXMLFileNode CLabelXMLFileIOController::GetLabelXMLFileNode(CString fileName)
{
	USES_CONVERSION;
	char * xmlPath = T2A(fileName);
	TiXmlDocument doc(xmlPath);
	LabelXMLFileNode node;
	bool bSuccess = doc.LoadFile(TIXML_ENCODING_UTF8);
	if (!bSuccess) {
		TRACE("Could not load test file %s. Error='%s'. Exiting.\n", xmlPath, doc.ErrorDesc());
		exit(1);
	}
	CLabelXMLFileIOController::fileName = fileName;
	TiXmlElement* root = doc.RootElement();
	if (root != NULL)
	{
		node = OnGetXMLFileNode(root);
	}
	return node;
}
LabelXMLFileNode CLabelXMLFileIOController::OnGetXMLFileNode(TiXmlElement *item)
{
	LabelXMLFileNode node;
	node.hasChildren = 0;
	node.value = UTF8_To_UTF16(item->Attribute("name"));
	if (node.value == "")
		node.value = UTF8_To_UTF16(item->GetText());
	item = item->FirstChildElement();
	while (item != NULL)
	{
		node.hasChildren = 1;
		LabelXMLFileNode newNode = OnGetXMLFileNode(item);
		node.childrenList.push_back(newNode);
		item = item->NextSiblingElement();
	}
	return node;
}
void CLabelXMLFileIOController::SaveLabelXMLFileNode(LabelXMLFileNode &node)
{
	USES_CONVERSION;
	char * xmlPath = T2A(fileName);
	DeleteFile(fileName);
	TiXmlDocument* doc = new TiXmlDocument(xmlPath);
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	doc->LinkEndChild(decl);
	TiXmlElement* rootEle = new TiXmlElement("Root");
	doc->LinkEndChild(rootEle);
	for (list < LabelXMLFileNode >::iterator it = node.childrenList.begin(); it != node.childrenList.end(); it++)
	{
		TiXmlElement * groupEle = new TiXmlElement("group");
		TinyXMLElementSetCStringAttribute(groupEle, "name",it->value);
		for (list<LabelXMLFileNode>::iterator itt = it->childrenList.begin(); itt != it->childrenList.end(); itt++)
		{
			TiXmlElement * classEle = new TiXmlElement("class");
			TinyXMLElementSetCStringAttribute(classEle, "name", itt->value);
			for (list<LabelXMLFileNode>::iterator ittt = itt->childrenList.begin(); ittt != itt->childrenList.end(); ittt++)
			{
				TiXmlElement * labelEle = new TiXmlElement("label");
				TinyXMLElementSetCStringAttribute(labelEle, "name", ittt->value);
				for (list<LabelXMLFileNode>::iterator itttt = ittt->childrenList.begin(); itttt != ittt->childrenList.end(); itttt++)
				{
					TiXmlElement * sublabelEle = new TiXmlElement("sublabel");
					TinyXMLElementSetCStringAttribute(sublabelEle, "name", itttt->value);
					labelEle->LinkEndChild(sublabelEle);
				}

				classEle->LinkEndChild(labelEle);
			}
			groupEle->LinkEndChild(classEle);
		}
		rootEle->LinkEndChild(groupEle);
	}
	doc->SaveFile();
	delete doc;
}