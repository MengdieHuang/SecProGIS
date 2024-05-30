// AdvHeaderCtrl.cpp : implementation file
//
// Subclassed CHeaderCtrl sample
//
// Written by Matt Weagle (matt_weagle@hotmail.com)
// Copyright (c) 2000
// 
// Modified: 
//		11.19.2000
//		Initial release
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AdvHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvHeaderCtrl

CAdvHeaderCtrl::CAdvHeaderCtrl():m_nSavedImage(-1)
{
}

CAdvHeaderCtrl::~CAdvHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CAdvHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CAdvHeaderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(HDN_ENDTRACKW, OnEndTrack)
	ON_NOTIFY_REFLECT(HDN_BEGINTRACKW, OnBeginTrack)
	ON_NOTIFY_REFLECT(HDN_ENDTRACKA, OnEndTrack)
	ON_NOTIFY_REFLECT(HDN_BEGINTRACKA, OnBeginTrack)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvHeaderCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
/*
	Init

	This initialization function must be called after the initial CHeaderCtrl
	has been created.  It subclasses the pHeader control passed in as a 
	parameter so that the CAdvHeaderCtrl can handle the reflected messages.
	
	Params:
		pHeader	pointer to created CHeaderCtrl

	Returns:
		BOOL	could the existing header be subclassed?
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CAdvHeaderCtrl::Init(CHeaderCtrl *pHeader,int resourceID)
{	
	ASSERT(pHeader && pHeader->GetSafeHwnd());
	if (!SubclassWindow(pHeader->GetSafeHwnd()))
	{
		OutputDebugString(_T("Unable to subclass existing header!\n"));
		return FALSE;
	}
	
	// Create the image list and attach it to the header control
	VERIFY(m_cImageList.Create(resourceID, 16, 4, ILC_COLOR24));//RGB(255, 0, 255)
	SetImageList(&m_cImageList);

	// Iterate through the items and attach the image list
	HDITEM hdi;
	for (int i=0; i < GetItemCount(); i++)
	{
		GetItem(i, &hdi);
		hdi.fmt |=  HDF_IMAGE;
		hdi.mask |= HDI_IMAGE | HDI_FORMAT /*| HDI_ORDER*/;
		hdi.iImage= i;
		SetItem( i, &hdi );
	}
	
	// Make the header control track 
	ModifyStyle(0, HDS_HOTTRACK);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnEndTrack

	When the user is done moving the Header control, we need to reset the
	image to the original image.  However, if they only double clicked on the 
	header divider, there will be no HDN_TRACK message so we'll be stuck with
	an invalid image index.  To avoid this, we only reset the image if the
	cached image index is valid.
	
	Params:
		pNMHDR	pointer to NMHDR structure
		pResult	pointer to LRESULT code

	Returns:
		None
*/
/////////////////////////////////////////////////////////////////////////////
void CAdvHeaderCtrl::OnEndTrack(NMHDR * pNMHDR, LRESULT* pResult)
{
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	NMHEADER *pHdr = (NMHEADER*)pNMHDR;
	
	if (-1 == m_nSavedImage)
		return;

	SetImage(pHdr->iItem, m_nSavedImage);
	m_nSavedImage = -1;
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/*
	OnBeginTrack

	Save the original image and set the new image when the user begins
	to drag a column.
	
	Params:
		pNMHDR	pointer to NMHDR structure
		pResult	pointer to LRESULT code

	Returns:
		None
*/
/////////////////////////////////////////////////////////////////////////////
void CAdvHeaderCtrl::OnBeginTrack(NMHDR * pNMHDR, LRESULT* pResult)
{
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	NMHEADER *pHdr = (NMHEADER*)pNMHDR;

	// Get the current image in the item and save the index
	HDITEM hdi;
	hdi.mask = HDI_IMAGE;
	GetItem(pHdr->iItem, &hdi);
	m_nSavedImage = hdi.iImage;

	// Set the new image
	SetImage(pHdr->iItem, 3);

	*pResult = 0;

}

/////////////////////////////////////////////////////////////////////////////
/*
	SetImage

	Helper function to set the image in a specified header item.  The image
	list must already have been created and attached prior to making this call.
	
	Params:
		nCol	Column whose image is to change
		nImage	0-based index into image list of new image for column

	Returns:
		BOOL	Could the image be updated?
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CAdvHeaderCtrl::SetImage(int nCol, int nImage)
{
	HDITEM hdi;
	hdi.mask = HDI_IMAGE | HDI_FORMAT;
	if (!GetItem(nCol, &hdi))
		return FALSE;

	hdi.fmt |=  HDF_IMAGE;
	hdi.iImage= nImage;
	return SetItem( nCol, &hdi );
}



