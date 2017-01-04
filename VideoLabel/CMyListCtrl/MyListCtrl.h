#pragma once

#include "../stdafx.h"
// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
public:
	//���������б�����ɫ
	void SetOddItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//����ż���б�����ɫ
	void SetEvenItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//�����ȵ��б�����ɫ
	void SetHoverItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//����ѡ���б�����ɫ
	void SetSelectItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//�����������ı���ɫ
	void SetOddItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//����ż�����ı���ɫ
	void SetEvenItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//�����ȵ����ı���ɫ
	void SetHoverItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//����ѡ�����ı���ɫ
	void SetSelectItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//����ѡ�����ı���ɫ
	void SetSSelectItemTextColor(COLORREF color, BOOL bDraw = TRUE);
	//����ѡ���б�����ɫ
	void SetSSelectItemBkColor(COLORREF color, BOOL bDraw = TRUE);
protected:
	BOOL m_bTracking;
	int m_nHoverIndex;//��ǰ�ȵ���Ŀ����
	COLORREF m_OddItemBkColor;//�����б�����ɫ
	COLORREF m_EvenItemBkColor;//ż���б�����ɫ
	COLORREF m_HoverItemBkColor;//�ȵ��б�����ɫ
	COLORREF m_SelectItemBkColor;//ѡ���б�����ɫ
	COLORREF m_OddItemTextColor;//�������ı���ɫ
	COLORREF m_EvenItemTextColor;//ż�����ı���ɫ
	COLORREF m_HoverItemTextColor;//�ȵ����ı���ɫ
	COLORREF m_SelectItemTextColor;//ѡ�����ı���ɫ
	COLORREF m_SSelectItemTextColor;
	COLORREF m_SSelectItemBkColor;
	int m_selIndex;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


