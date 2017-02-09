// ImageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Poker.h"
#include "ImageDialog.h"


// CImageDialog dialog

IMPLEMENT_DYNAMIC(CImageDialog, CDialog)

CImageDialog::CImageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CImageDialog::IDD, pParent)
{

}

CImageDialog::~CImageDialog()
{
}

void CImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDialog, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CImageDialog message handlers

BOOL CImageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(100, 500, NULL);
	bStart = FALSE;
	nMode = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 100)
		Invalidate(TRUE);
	CDialog::OnTimer(nIDEvent);
}

void CImageDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	HDC hdc = ::GetDC(hwnd);
	//::InvalidateRect(hwnd, NULL, TRUE);
	//::SendMessage(hwnd, WM_PAINT, 0, 0);

	BITMAP structBitmapHeader;
	memset( &structBitmapHeader, 0, sizeof(BITMAP) );

	HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hBitmap, sizeof(BITMAP), &structBitmapHeader);


	::BitBlt(dc.m_hDC, 0, 0, structBitmapHeader.bmWidth - 8, structBitmapHeader.bmHeight - 30, hdc, 0, 0, SRCCOPY);

	if(bStart)
	{
		COLORREF cr = dc.GetPixel(ptOrg);
		int i = 0;
		for(i = 0; i < nIndex; i++)
			if(cr == crOrg[i])
				break;
		if(i == nIndex && nIndex < 3)
		{
			crOrg[nIndex++] = cr;
			if(nMode == 0)
				::SendMessage(hwndSend, WM_SENDCOLOR1, (WPARAM)&ptOrg, (LPARAM)&cr);
			else
				::SendMessage(hwndSend, WM_SENDCOLOR, (WPARAM)&ptOrg, (LPARAM)&cr);
		}
	}
	ReleaseDC(&dc);
	::ReleaseDC(hwnd, hdc);
}

void CImageDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(nMode == 0)
	{
		POINT pt;
		pt.x = point.x;
		pt.y = point.y;

		COLORREF cr = GetDC()->GetPixel(pt);

		::SendMessage(hwndSend, WM_SENDPOSITION, (WPARAM)&pt, (LPARAM)&cr);
	}
	else if(nMode == 1)
	{
		POINT pt;
		pt.x = point.x;
		pt.y = point.y;

		COLORREF cr = GetDC()->GetPixel(pt);

		ptOrg = pt;
		nIndex = 0;
		crOrg[nIndex++] = cr;
		bStart = TRUE;

		::SendMessage(hwndSend, WM_SEATPOSITION, (WPARAM)&pt, (LPARAM)&cr);
	}
	else if(nMode == 2)
	{
		POINT pt;
		pt.x = point.x;
		pt.y = point.y;

		::SendMessage(hwndSend, WM_JOINPOSITION, (WPARAM)&pt, 0);
	}
	else if(nMode == 3)
	{
		POINT pt;
		pt.x = point.x;
		pt.y = point.y;

		COLORREF cr = GetDC()->GetPixel(pt);

		::SendMessage(hwndSend, WM_EMPTYPOSITION, (WPARAM)&pt, (LPARAM)&cr);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

LRESULT CImageDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_SENDPOSITION)
	{
		//nMode = 0;
		POINT pt = *(POINT*)wParam;
		if(lParam == 0)
		{
			COLORREF cr = GetDC()->GetPixel(pt);

			nMode = 0;
			::SendMessage(hwndSend, WM_SENDPOSITION, (WPARAM)&pt, (LPARAM)&cr);
		}
		else if(lParam == 1)
		{
			nMode = 0;
			//::SendMessage(hwndSend, WM_SENDPOSITION, (WPARAM)&pt, (LPARAM)&cr);
		}
	}
	if(message == WM_SEATPOSITION)
	{
		POINT pt = *(POINT*)wParam;
		if(lParam == 0)
		{
			COLORREF cr = GetDC()->GetPixel(pt);

			ptOrg = pt;
			nIndex = 0;
			crOrg[nIndex++] = cr;
			bStart = TRUE;

			nMode = 1;
			::SendMessage(hwndSend, WM_SEATPOSITION, (WPARAM)&pt, (LPARAM)&cr);
		}
		else if(lParam == 1)
		{
			nMode = 1;
			//::SendMessage(hwndSend, WM_SENDPOSITION, (WPARAM)&pt, (LPARAM)&cr);
		}
	}
	if(message == WM_JOINPOSITION)
	{
		nMode = 2;
	}
	if(message == WM_EMPTYPOSITION)
	{
		nMode = 3;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}