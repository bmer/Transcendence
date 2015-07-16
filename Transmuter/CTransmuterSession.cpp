//	CTransmuterSession.cpp
//
//	CTransmuterSession class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CTransmuterSession::CTransmuterSession (CHumanInterface &HI, CTransmuterModel &Model) : IHISession(HI),
		m_Model(Model)
	//	CTransmuterSession constructor
	{
	}

void CTransmuterSession::OnPaint (CG32bitImage &Screen, const RECT &rcInvalid)
	{
	Screen.Fill(0, 0, Screen.GetWidth(), Screen.GetHeight(), CG32bitPixel(0, 102, 102));
	}

//	====================================================================

CTextEditorTabSession::CTextEditorTabSession (CHumanInterface &HI, CTextEditorSession &parentTextEditorSession) : IHISession(HI),
		m_parentTextEditorSession(parentTextEditorSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CTextEditorSession::CTextEditorSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CNavigationSession::CNavigationSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}

//	====================================================================

CCommandRibbonSession::CCommandRibbonSession (CHumanInterface &HI, CTransmuterSession &mainTransmuterSession) : IHISession(HI),
		m_mainTransmuterSession(mainTransmuterSession)
	//	CTransmuterSession constructor
	{
	}
