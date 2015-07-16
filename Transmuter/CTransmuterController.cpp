//	CTransmuterController.cpp
//
//	CAmericaController class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

#define FILENAME_SETTINGS						CONSTLIT("Settings.xml")
#define FOLDER_COLLECTION						CONSTLIT("Collection")
#define FOLDER_EXTENSIONS						CONSTLIT("Extensions")
#define FOLDER_GAMEWORLDS						CONSTLIT("GameWorlds")
#define FOLDER_SAVE_FILES						CONSTLIT("Games")

#define CMD_INTRO_SHOW_UI						CONSTLIT("cmdIntroShowUI")

#define CMD_MODEL_EDITOR_CREATED				CONSTLIT("cmdModelEditorCreated")
#define CMD_MODEL_INIT_DONE						CONSTLIT("cmdModelInitDone")
#define CMD_MODEL_INVOKE_CODE					CONSTLIT("cmdModelInvokeCode")

#define CMD_UI_LOAD_GAMEWORLD					CONSTLIT("cmdUILoadGameWorld")
#define CMD_UI_EXIT								CONSTLIT("cmdUIExit")
#define CMD_UI_NEW_GAMEWORLD					CONSTLIT("cmdUINewGameWorld")

#define CMD_NULL								CONSTLIT("null")

#define SETTING_DEBUG_MODE						CONSTLIT("debugMode")
#define SETTING_FORCE_TDB						CONSTLIT("forceTDB")

#define ERR_LOAD_ERROR							CONSTLIT("Error Loading Universe")
#define ERR_CANT_SHOW_CAMPAIGNS					CONSTLIT("Unable to show list of available pre-existing extensions: %s")
#define ERR_CANT_CREATE_NEW_GAMEWORLD			CONSTLIT("Unable to create new extension: %s")
#define ERR_UI_ERROR							CONSTLIT("Unable to Comply")

ALERROR CTransmuterController::OnBoot (char *pszCommandLine, SHIOptions *retOptions, CString *retsError)

//	OnBoot
//
//	Controller must initialize all option values

	{
	CString sError;

	//	Set our basic application settings

	retOptions->sAppName = CONSTLIT("Transmuter");
	retOptions->sClassName = CONSTLIT("transmuter_class");
	retOptions->sAppData = CONSTLIT("Kronosaur\\Transmuter");
	retOptions->hIcon = ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_TRANSMUTER));

	//	Set our default directory

	CString sCurDir = pathGetExecutablePath(NULL);
	::SetCurrentDirectory(sCurDir.GetASCIIZPointer());

	return NOERROR;
	}

void CTransmuterController::OnCleanUp (void)

//	OnCleanUp
//
//	Clean up

	{
	}

bool CTransmuterController::OnClose (void)

//	OnClose
//
//	User wants to close the app. We return TRUE if we wish to continue closing
//	the app; FALSE otherwise.

	{
	return true;
	}

ALERROR CTransmuterController::OnCommand (const CString &sCmd, void *pData)

//	OnCommand
//
//	Handle command

	{
	CString sError;

	//	We're done loading the universe

	if (strEquals(sCmd, CMD_MODEL_INIT_DONE))
		OnModelInitDone((CInitModelTask *)pData);


	return NOERROR;
	}

ALERROR CTransmuterController::OnInit (CString *retsError)

//	OnInit
//
//	Initialize after HI has booted.

	{
	//	Initialize the model

	CTransmuterModel::SInitDesc Ctx;
	Ctx.bDebugMode = m_Settings.GetValueBoolean(SETTING_DEBUG_MODE);
	Ctx.bForceTDB = m_Settings.GetValueBoolean(SETTING_FORCE_TDB);

	Ctx.sCollectionFolder = pathAddComponent(m_Settings.GetUserFolder(), FOLDER_COLLECTION);
	Ctx.ExtensionFolders.Insert(pathAddComponent(m_Settings.GetUserFolder(), FOLDER_EXTENSIONS));
	Ctx.SaveFileFolders.Insert(pathAddComponent(m_Settings.GetUserFolder(), FOLDER_SAVE_FILES));

	if (!m_Model.Init(Ctx, retsError))
		return ERR_FAIL;

	m_HI.ShowSession(new CTransmuterSession(m_HI, m_Model));

	return NOERROR;
	}

void CTransmuterController::OnModelInitDone (CInitModelTask *pTask)

//	OnModelInitDone
//
//	Model is done initializing.

	{
	CString sError;

	//	Check for error.

	if (pTask->GetResult(&sError))
		{
		kernelDebugLogMessage(sError);
		m_HI.OpenPopupSession(new CMessageSession(m_HI, ERR_LOAD_ERROR, sError, CMD_UI_EXIT));
		return;
		}

	//	Tell the session that we're done loading.

	m_HI.GetSession()->HICommand(CMD_INTRO_SHOW_UI);
	}

void CTransmuterController::OnShutdown (EHIShutdownReasons iShutdownCode)

//	OnShutdown
//
//	Shutdown app

	{
	}