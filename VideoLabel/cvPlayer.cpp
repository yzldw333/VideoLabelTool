#include "stdafx.h"
#include "cvPlayer.h"
#include "opencv2/opencv.hpp"

STATE CVPlayer::GetState()
{
	return m_state;
}
void CVPlayer::SetState(STATE state)
{
	this->m_state = state;
}
BOOL CVPlayer::Open()
{ 
	return Open(videoPath);
}
BOOL CVPlayer::Open(CString path)
{
	USES_CONVERSION;
	this->videoPath = path;
	ClearImgBuf();
	if (capture != NULL){
		cvReleaseCapture(&capture);
	}
	capture = NULL;
	capture = cvCaptureFromFile(T2A(path));
	if (capture == NULL)
		return FALSE;
	totalStart = 0;
	totalEnd = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	isSubVideo = FALSE;
	return TRUE;
}
BOOL CVPlayer::PrepareBuffer()
{
	USES_CONVERSION;
	if (capture == NULL)
		return FALSE;
	
	int num = 10;
	int i = 0;
	ClearImgBuf();
	while (i<num&&cvGrabFrame(capture))
	{
		IplImage * retrieveFrame = cvRetrieveFrame(capture);
		IplImage * newFrame = cvCloneImage(retrieveFrame);
		this->imgBuf.push(newFrame);
		i++;
	}
	if (i == 0)
		return FALSE;
	return TRUE;
}
 
BOOL CVPlayer::ReadFrameFromBuffer(IplImage ** pImg)
{
	if (imgBuf.size() > 0)
	{
		IplImage * front = imgBuf.front();
		*pImg = cvCloneImage(front);
		cvReleaseImage(&front);
		imgBuf.pop();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CVPlayer::ReadTmpFrameWithPos(IplImage ** pImg, int pos)
{
	if (SetFramePos(pos))
	{
		if (cvGrabFrame(capture))
		{
			IplImage * retrieveFrame = cvRetrieveFrame(capture);
			IplImage * newFrame = cvCloneImage(retrieveFrame);
			*pImg = newFrame;

			return TRUE;
		}
	}
	return FALSE;
}
BOOL CVPlayer::ReadFrameToBuffer()
{
	int maxnum = 10;
	int tmpEnd;
	if (isSubVideo)
		tmpEnd = subEnd;
	else
		tmpEnd = totalEnd;
	while (cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES)<tmpEnd&&imgBuf.size() < maxnum)
	{
 		if (cvGrabFrame(capture))
		{
			IplImage * retrieveFrame = cvRetrieveFrame(capture);
			IplImage * newFrame = cvCloneImage(retrieveFrame);
			imgBuf.push(newFrame);
		}
		else
		{
			break;
		}
	}
	if (imgBuf.size() < maxnum)
		return FALSE;
	else
		return TRUE;
}
int CVPlayer::GetFramePos()
{
	if (capture!=NULL)
		return cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES);
	else
		return -1;
}
int CVPlayer::GetDrawFramePos()
{
	return drawPos;
}
void CVPlayer::SetDrawFramePos(long pos)
{
	drawPos = pos;
}
int CVPlayer::GetTotalFrame()
{
	if (capture != NULL)
		return cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	else
		return -1;
}
void CVPlayer::SetIsSubVideoState(BOOL state)
{
	isSubVideo = state;
}
BOOL CVPlayer::GetIsSubVideoState()
{
	return isSubVideo; 
}
BOOL CVPlayer::SetFramePos(int pos)
{
	if (capture != NULL){
		BOOL res = cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
		if (res)
		{
			drawPos = pos;
		}
		return res;
	}
		
	else
		return -1;
}
void CVPlayer::SetSubStart(long start)
{
	subStart = start;
}
void CVPlayer::SetSubEnd(long end)
{
	subEnd = end;
}
long CVPlayer::GetSubStart()
{
	return subStart;
}
long CVPlayer::GetSubEnd()
{
	return subEnd;
}
int CVPlayer::GetFPS()
{
	if (capture != NULL)
		return cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	else
		return -1;
}

void CVPlayer::ClearImgBuf()
{
	while (imgBuf.size() > 0)
	{
		IplImage* img = imgBuf.front();
		cvReleaseImage(&img);
		imgBuf.pop();
	}
}