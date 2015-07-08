//	CTextEditor.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#define CMD_DEFAULT_OBJ							CONSTLIT("cmdDefaultObj")
#define CMD_DEFAULT_POS							CONSTLIT("cmdDefaultPos")
#define CMD_UI_DISMISS_DIALOG					CONSTLIT("cmdUIDismissDialog")
#define CMD_UI_SHOW_DIALOG						CONSTLIT("cmdUIShowDialog")

#define ID_DISPLAY_COMMAND_PANE					CONSTLIT("idCommandPane")
#define ID_DISPLAY_DIALOG_PANE					CONSTLIT("idDialogPane")

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &Model) : IHISession(HI),
		m_Model(Model),
		m_Selection(Model),
		m_pCommandPane(NULL),
		m_pDialogPane(NULL)

//	CTransmuterSession constructor

	{
	}

bool CTransmuterSession::HitTestUnit (const CVector &vPos, CSpaceObject **retpObj)

//	HitTestUnit
//
//	Returns TRUE if vPos hits a unit (and returns the unit).

	{
	CSystem *pSystem = m_Model.GetUniverse().GetCurrentSystem();
	if (pSystem == NULL)
		return false;

	//	Loop over all objects that intersect with this position.

	SSpaceObjectGridEnumerator i;
	pSystem->EnumObjectsInBoxStart(i, vPos, 0.0);

	while (pSystem->EnumObjectsInBoxHasMore(i))
		{
		CSpaceObject *pObj = pSystem->EnumObjectsInBoxGetNext(i);

		//	If the object matches, then we're done.

		if (!pObj->IsDestroyed()
				&& (pObj->GetScale() == scaleShip || pObj->GetScale() == scaleStructure)
				&& pObj->PointInObject(pObj->GetPos(), vPos))
			{
			//	LATER: If there are multiple objects at this position,
			//	figure out some way to distinguish.

			if (retpObj)
				*retpObj = pObj;

			return true;
			}
		}

	//	Not found

	return false;
	}

void CTransmuterSession::OnChar (char chChar, DWORD dwKeyData)

//	OnChar
//
//	Handle character

	{
	switch (chChar)
		{
		case '0':
			SelectObjectGroup(GROUP_CARRIER);
			break;

		case '1':
			SelectObjectGroup(GROUP_WING_1);
			break;
		}
	}

void CTransmuterSession::OnCleanUp (void)

//	OnCleanUp
//
//	Clean up

	{
	m_Model.GetUniverse().UnregisterForNotifications(this);
	}

ALERROR CTransmuterSession::OnCommand (const CString &sCmd, void *pData)

//	OnCommand
//
//	Handle commands

	{
	if (strEquals(sCmd, CMD_UI_DISMISS_DIALOG))
		UIDismissDialog();

	else if (strEquals(sCmd, CMD_UI_SHOW_DIALOG))
		UIShowDialog((CDialogDesc *)pData);

	return NOERROR;
	}

ALERROR CTransmuterSession::OnInit (CString *retsError)

//	OnInit
//
//	Initialize

	{
	//	Initialize metrics

	m_xViewportCenter = m_HI.GetScreen().GetWidth() / 2;
	m_yViewportCenter = m_HI.GetScreen().GetHeight() / 2;

	//	Initialize the map scroll engine

	m_MapScroll.Init(m_Model.GetPOVPos());

	//	Register for events from the universe

	m_Model.GetUniverse().RegisterForNotifications(this);

	//	Create the command pane display

	m_pCommandPane = new CCommandPane(m_HI.GetVisuals(), COMMAND_PANE_DEFAULT, m_Model);
	m_Selection.RegisterForNotifications(m_pCommandPane);
	StartPerformance(m_pCommandPane, ID_DISPLAY_COMMAND_PANE, CReanimator::SPR_FLAG_DELETE_WHEN_DONE);

	//	Create the LRS pane display

	m_pLRSPane = new CLRSPane(m_HI.GetVisuals(), LRS_PANE_DEFAULT, m_Model);
	StartPerformance(m_pLRSPane, ID_DISPLAY_LRS_PANE, CReanimator::SPR_FLAG_DELETE_WHEN_DONE);

	return NOERROR;
	}

void CTransmuterSession::OnLButtonDblClick (int x, int y, DWORD dwFlags)

//	OnLButtonDblClick
//
//	Left-button double-click

	{
	}

void CTransmuterSession::OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture)

//	OnLButtonDown
//
//	Left-button down

	{
	//	Convert point to game-coordinates

	CVector vClickPos = ScreenToGamePos(x, y);

	//	Have we clicked on a unit?

	CSpaceObject *pUnit;
	if (HitTestUnit(vClickPos, &pUnit))
		{
		if (uiIsControlDown())
			m_Selection.SelectToggle(pUnit);
		else
			m_Selection.Select(pUnit);
		}

	//	Otherwise, we start a map drag

	else
		{
		m_MapScroll.StartDrag(x, y);
		*retbCapture = true;
		}
	}

void CTransmuterSession::OnLButtonUp (int x, int y, DWORD dwFlags)

//	OnLButtonUp
//
//	Left-button up

	{
	if (m_MapScroll.IsDragging())
		m_MapScroll.StopDrag();
	}

void CTransmuterSession::OnMouseMove (int x, int y, DWORD dwFlags)

//	OnMouseMove
//
//	Mouse move

	{
	//	If we're dragging the map, then do it

	if (m_MapScroll.IsDragging())
		m_MapScroll.MouseMove(x, y);
	}

void CTransmuterSession::OnObjDestroyed (SDestroyCtx &Ctx)

//	OnObjDestroyed
//
//	An object has been destroyed. Make sure we're not holding on to it.

	{
	m_Selection.OnObjDestroyed(Ctx);
	}

void CTransmuterSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)

//	OnPaint
//
//	Paint

	{
	//	Calc some metrics

	RECT rcView;
	rcView.left = 0;
	rcView.top = 0;
	rcView.right = Screen.GetWidth();
	rcView.bottom = Screen.GetHeight();

	DWORD dwFlags = 0;

	//	Paint the screen

	m_Model.GetUniverse().PaintPOV(Screen, rcView, dwFlags);
	}

void CTransmuterSession::OnRButtonDown (int x, int y, DWORD dwFlags)

//	OnRButtonDown
//
//	Right-button down

	{
	//	Convert point to game-coordinates

	CVector vClickPos = ScreenToGamePos(x, y);

	//	Have we clicked on a unit?

	CSpaceObject *pUnit;
	if (HitTestUnit(vClickPos, &pUnit))
		{
		if (m_pCommandPane)
			m_pCommandPane->ExecuteCommand(CMD_DEFAULT_OBJ, pUnit);
		}

	//	Otherwise, we clicked on empty space

	else
		{
		if (m_pCommandPane)
			m_pCommandPane->ExecuteCommand(CMD_DEFAULT_POS, vClickPos);
		}
	}

void CTransmuterSession::OnRButtonUp (int x, int y, DWORD dwFlags)

//	OnRButtonUp
//
//	Right-button up

	{
	}

void CTransmuterSession::OnUpdate (bool bTopMost)

//	OnUpdate
//
//	Handle updates

	{
	//	Prepare context

	SSystemUpdateCtx UpdateCtx;
	UpdateCtx.bForceEventFiring = true;

	//	Update

	m_Model.GetUniverse().Update(UpdateCtx);

	//	Scroll the map

	CVector vNewCenter;
	if (m_MapScroll.Update(&vNewCenter))
		{
		CSpaceObject *pPOV = m_Model.GetUniverse().GetPOV();
		pPOV->SetPos(vNewCenter);
		}

	//	We always invalidate

	HIInvalidate();
	}

CVector CTransmuterSession::ScreenToGamePos (int x, int y) const

//	ScreenToGamePos
//
//	Converts from screen coordinates to game coordinates.

	{
	return m_Model.GetPOVPos() + (g_KlicksPerPixel * CVector(x - m_xViewportCenter, m_yViewportCenter - y));
	}

void CTransmuterSession::UIDismissDialog (void)

//	UIDismissDialog
//
//	Close down the current dialog box.

	{
	if (m_pDialogPane)
		{
		StopPerformance(ID_DISPLAY_DIALOG_PANE);
		m_pDialogPane = NULL;
		}
	}

void CTransmuterSession::UIShowDialog (CDialogDesc *pDialogDesc)

//	UIShowDialog
//
//	Shows the given dialog box.

	{
	//	If we don't have a dialog box up, create a new one

	if (m_pDialogPane == NULL)
		{
		m_pDialogPane = new CDialogPane(m_HI, DIALOG_PANE_DEFAULT, m_Model);
		StartPerformance(m_pDialogPane, ID_DISPLAY_DIALOG_PANE, CReanimator::SPR_FLAG_DELETE_WHEN_DONE);
		}

	//	Set the descriptor

	m_pDialogPane->SetDescriptor(pDialogDesc);
	}
