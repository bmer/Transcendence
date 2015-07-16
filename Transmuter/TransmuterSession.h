//	TransmuterSession.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CTransmuterSession;
class CTextEditorSession;
class CTextEditorTabSession;
class CNavigationSession;
class CCommandRibbonSession;

class CTransmuterSession : public IHISession, public CUniverse::INotifications
	{
	public:
		CTransmuterSession (CHumanInterface &HI, CTransmuterModel m_Model);

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

	private:
		CTextEditorSession *m_textEditorSession;
		CNavigationSession *m_navigationSession;
		CCommandRibbonSession *m_commandRibbonSession;
		CTransmuterModel m_Model;
		int iFocus;
	};

class CTextEditorSession : public IHISession
	{
	public:
		CTextEditorSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession);

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

	private:
		CTransmuterSession &m_mainTransmuterSession;
		TArray<CTextEditorTabSession *> m_textEditorTabSessions;
		int iFocusTab;
	};

class CTextEditorTabSession : public IHISession
	{
	public:
		CTextEditorTabSession(CHumanInterface &HI, CTextEditorSession &parentTextEditorSession);


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
	private:
		CTextEditorSession &m_parentTextEditorSession;
	};

class CNavigationSession : public IHISession
	{
	public:
		CNavigationSession (CHumanInterface &HI, CTransmuterSession &m_mainTransmuterSession);

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

	private:
		CTransmuterSession &m_mainTransmuterSession;
	};

class CCommandRibbonSession : public IHISession
	{
	public:
		CCommandRibbonSession (CHumanInterface &HI, CTransmuterSession &m_mainTransmuterSession);

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

	private:
		CTransmuterSession &m_mainTransmuterSession;
	};