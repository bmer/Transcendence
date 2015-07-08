//	CAmericaSession.cpp
//
//	CAmericaSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#define CMD_DEFAULT_OBJ							CONSTLIT("cmdDefaultObj")
#define CMD_DEFAULT_POS							CONSTLIT("cmdDefaultPos")
#define CMD_UI_DISMISS_DIALOG					CONSTLIT("cmdUIDismissDialog")
#define CMD_UI_SHOW_DIALOG						CONSTLIT("cmdUIShowDialog")

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &Model, TArray<CPanelSession *> Panels) : IHISession(HI),
		m_Model(Model),
		m_Panels(Panels)
//	CTransmuterSession constructor

	{
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

void CAmericaSession::OnCleanUp (void)

//	OnCleanUp
//
//	Clean up

	{
	m_Model.GetUniverse().UnregisterForNotifications(this);
	}

ALERROR CAmericaSession::OnCommand (const CString &sCmd, void *pData)

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

ALERROR CAmericaSession::OnInit (CString *retsError)

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

void CAmericaSession::OnLButtonDblClick (int x, int y, DWORD dwFlags)

//	OnLButtonDblClick
//
//	Left-button double-click

	{
	}

void CAmericaSession::OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture)

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

void CAmericaSession::OnLButtonUp (int x, int y, DWORD dwFlags)

//	OnLButtonUp
//
//	Left-button up

	{
	if (m_MapScroll.IsDragging())
		m_MapScroll.StopDrag();
	}

void CAmericaSession::OnMouseMove (int x, int y, DWORD dwFlags)

//	OnMouseMove
//
//	Mouse move

	{
	//	If we're dragging the map, then do it

	if (m_MapScroll.IsDragging())
		m_MapScroll.MouseMove(x, y);
	}

void CAmericaSession::OnObjDestroyed (SDestroyCtx &Ctx)

//	OnObjDestroyed
//
//	An object has been destroyed. Make sure we're not holding on to it.

	{
	m_Selection.OnObjDestroyed(Ctx);
	}

void CAmericaSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)

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

void CAmericaSession::OnRButtonDown (int x, int y, DWORD dwFlags)

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

void CAmericaSession::OnRButtonUp (int x, int y, DWORD dwFlags)

//	OnRButtonUp
//
//	Right-button up

	{
	}

void CAmericaSession::OnUpdate (bool bTopMost)

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

CVector CAmericaSession::ScreenToGamePos (int x, int y) const

//	ScreenToGamePos
//
//	Converts from screen coordinates to game coordinates.

	{
	return m_Model.GetPOVPos() + (g_KlicksPerPixel * CVector(x - m_xViewportCenter, m_yViewportCenter - y));
	}

void CAmericaSession::UIDismissDialog (void)

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

void CAmericaSession::UIShowDialog (CDialogDesc *pDialogDesc)

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
