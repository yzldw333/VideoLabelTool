
// VideoLabel.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoLabelApp: 
// �йش����ʵ�֣������ VideoLabel.cpp
//

class CVideoLabelApp : public CWinApp
{
public:
	CVideoLabelApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoLabelApp theApp;