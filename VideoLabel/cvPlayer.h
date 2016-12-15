#ifndef CVPLAYER_H
#define CVPLAYER_H
#pragma once
#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <queue>
using namespace std;
enum STATE
{
	PLAYING = 1,
	PAUSE = 2,
	STOP = 3
};
class CVPlayer{
private:
	CString videoPath;
	CvCapture * capture;
	long drawPos;
	STATE m_state;
	queue<IplImage*> imgBuf;
	long subStart;
	long subEnd;
	long totalStart;
	long totalEnd;
	BOOL isSubVideo;
	
public:
	CVPlayer(){
		videoPath = _T(""); 
		m_state = STOP;
		capture = NULL;
		drawPos = 0;
	}
	~CVPlayer(){
		if (capture != NULL)
		{
			cvReleaseCapture(&capture);
		}
		ClearImgBuf();
	}
	void ClearImgBuf();
	STATE GetState();
	void SetState(STATE state);
	void Open(CString path);
	void Open();
	BOOL ReadFrameToBuffer();
	BOOL CVPlayer::ReadFrameFromBuffer(IplImage** pImg);
	int GetFramePos();
	int GetDrawFramePos();
	int GetTotalFrame();
	BOOL SetFramePos(int pos);
	void SetDrawFramePos(long pos);
	BOOL PrepareBuffer();
	BOOL CVPlayer::ReadTmpFrameWithPos(IplImage ** pImg, int pos);
	int GetFPS();
	void SetSubStart(long start);
	void SetSubEnd(long end);
	long GetSubStart();
	long GetSubEnd();
	void SetIsSubVideoState(BOOL state);
	BOOL GetIsSubVideoState();
};


#endif