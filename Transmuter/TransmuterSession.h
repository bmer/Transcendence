//	TransmuterSession.h
//
//	CSC America
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "EditorComponents.h"

//	"Panels make up the screen."
class CPanelSession : public IHISession
	{
	public:
		CPanelSession (CHumanInterface &HI, CTransmuterModel &Model, int FocusFrame=0) : IHISession(HI),
				m_Model(Model),
				iFocusPanel(iFocusPanel)
			{ } 

		// IHISession virtuals

		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (CString *retsError);
		virtual void OnUpdate (bool bTopMost);

	private:
		CTransmuterModel &m_Model;
		TArray<CPanelSession *> m_descendentPanels;
		int iFocusPanel;
	};

class CTransmuterSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CTransmuterSession (CHumanInterface &HI, CTransmuterModel &Model, TArray<CPanelSession *> Panels);

		//	IHISession virtuals

		virtual void OnChar (char chChar, DWORD dwKeyData);
		virtual void OnCleanUp (void);
		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (CString *retsError);
		virtual void OnLButtonDblClick (int x, int y, DWORD dwFlags);
		virtual void OnLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture);
		virtual void OnLButtonUp (int x, int y, DWORD dwFlags);
		virtual void OnMouseMove (int x, int y, DWORD dwFlags);
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		virtual void OnRButtonDown (int x, int y, DWORD dwFlags);
		virtual void OnRButtonUp (int x, int y, DWORD dwFlags);
		virtual void OnUpdate (bool bTopMost);

		//	CUniverse::INotifications virtuals

		//  virtual void OnObjDestroyed (SDestroyCtx &Ctx);

	private:
		CTransmuterModel &m_Model;
		TArray<CPanelSession *> m_Panels;
	};

class CIntroSession : public IHISession
	{
	public:
		CIntroSession (CHumanInterface &HI) : IHISession(HI),
				m_iState(stateNone)
			{ }

		//	IHISession virtuals

		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (CString *retsError);
		virtual void OnPaint (CG32bitImage &Screen, const RECT &rcInvalid);
		virtual void OnUpdate (bool bTopMost);

	private:
		enum EStates
			{
			stateNone,

			stateLogo,						//	Kronosaur Productions logo
			stateTitle,						//	Transmuter title card
			stateAttract,					//	Attract screen (with main menu)
			};

		void ShowMainMenu (void);

		EStates m_iState;
	};