//	Transmuter.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

#include "TSE.h"
#include "TSUI.h"

class CTransmuterController;

#include "TransmuterModel.h"
#include "TransmuterSession.h"
#include "BackgroundTasks.h"

class CTransmuterController : 
		public IHIController,
		public CUniverse::IHost
	{
	public:
		CTransmuterController (void) :
				m_iState(stateNone),
				m_Model(m_HI, this)
			{ }

		//	IHICommand virtuals

		virtual ALERROR OnBoot (char *pszCommandLine, SHIOptions *retOptions, CString *retsError);
		virtual void OnCleanUp (void);
		virtual bool OnClose (void);
		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (CString *retsError);
		virtual void OnShutdown (EHIShutdownReasons iShutdownCode);

		//	IHost virtuals

		virtual const CG16bitFont *GetFont (const CString &sFont);

	private:
		enum EStates
			{
			stateNone,

			stateIntro,						//	In intro screen
			stateInGame,					//	We're playing
			};

		void OnModelInitDone (CInitModelTask *pTask);
		void OnStartEditor (bool bNewEditorInstance);

		void OnGameWorldSelected ();
		void OnGameWorldLoaded ();
		void OnGameWorldCreated ();

		EStates m_iState;					//	Current application state
		CTransmuterModel m_Model;			//	Game model
		CUserSettings m_Settings;			//	User settings
	};
