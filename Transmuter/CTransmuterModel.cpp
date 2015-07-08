//	CTransmuterModel.cpp
//
//	CTransmuterModel class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

const DWORD UNID_DEFAULT_CAMPAIGN =					0x02110000;

CTransmuterModel::CTransmuterModel (CHumanInterface &HI, CTransmuterController *pController) :
		m_HI(HI),
		m_pController(pController),
		m_UILibrary(HI, *this)

//	CAmericaModel constructor

	{
	}

bool CTransmuterModel::Init (const SInitDesc &Ctx, CString *retsError)

//	Init
//
//	Initialize

	{
	//	Save file path

	m_SaveFileFolders = Ctx.SaveFileFolders;

	//	Make sure the first path (default) exists so we can save files there.

	if (m_SaveFileFolders.GetCount() == 0)
		{
		if (retsError) *retsError = CONSTLIT("No save file path defined.");
		return false;
		}

	if (!pathExists(m_SaveFileFolders[0]))
		{
		if (!pathCreate(m_SaveFileFolders[0]))
			{
			if (retsError) *retsError = strPatternSubst(CONSTLIT("Cannot create default save file path: %s."), m_SaveFileFolders[0]);
			return false;
			}
		}

	//	Other folders

	m_sCollectionFolder = Ctx.sCollectionFolder;
	m_ExtensionFolders = Ctx.ExtensionFolders;

	//	Options

	m_bDebugMode = Ctx.bDebugMode;
	m_bForceTDB = Ctx.bForceTDB;

	return true;
	}

bool CTransmuterModel::InitBackground (CString *retsError)

//	InitBackground
//
//	Initialize the universe in the background.

	{
	//	Load the universe

	if (!LoadUniverse(retsError))
		return false;

	return true;
	}

void CTransmuterModel::InvokeCode (ICCItem *pCode)

//	InvokeCode
//
//	Invoke code from a command

	{
	CCodeChainCtx Ctx;

	//	Run code

	ICCItem *pResult = Ctx.Run(pCode);
	if (pResult->IsError())
		kernelDebugLogMessage(pResult->GetStringValue());

	Ctx.Discard(pResult);
	}

bool CTransmuterModel::LoadUniverse (CString *retsError)

//	LoadUniverse
//
//	Load the universe

	{
	try
		{
		//	Make sure the universe know about our various managers

		m_Universe.SetDebugMode(m_bDebugMode);
		m_Universe.SetSoundMgr(&m_HI.GetSoundMgr());

		//	Load the Transcendence Data Definition file that describes the universe.

		CUniverse::SInitDesc Ctx;
		Ctx.sFilespec = CONSTLIT("America.tdb");
		Ctx.sSourceFilespec = CONSTLIT("..\\AmericaCore\\America.xml");
		Ctx.sCollectionFolder = m_sCollectionFolder;
		Ctx.ExtensionFolders = m_ExtensionFolders;
		Ctx.pHost = m_pController;
		Ctx.bDebugMode = m_bDebugMode;
		Ctx.dwAdventure = UNID_DEFAULT_CAMPAIGN;
		Ctx.bDefaultExtensions = true;
		Ctx.bForceTDB = m_bForceTDB;

		if (m_Universe.Init(Ctx, retsError) != NOERROR)
			return false;

		//	Initialize TSUI CodeChain primitives

		if (m_HI.InitCodeChainPrimitives(m_Universe.GetCC()) != NOERROR)
			{
			if (retsError) *retsError = CONSTLIT("Unable to load CodeChain primitives.");
			return false;
			}

		//	Initialize application primitives

		if (!m_UILibrary.RegisterCCPrimitives(m_Universe.GetCC()))
			{
			if (retsError) *retsError = CONSTLIT("Unable to load CSC America primitives.");
			return false;
			}

		return true;
		}
	catch (...)
		{
		if (retsError) *retsError = CONSTLIT("Crash loading universe.");
		return false;
		}
	}

bool CTransmuterModel::LoadNewGameWorld (const SCampaignSettings &CampaignSettings, CString *retsError)

//	StartNewCampaign
//
//	Starts a new campaign.

	{
	m_CampaignSettings = CampaignSettings;

	//	New game

	CString sFilename = pathAddComponent(GetSaveFilePath(), m_GameFile.GenerateFilename(m_CampaignSettings.sPlayerName));
	if (m_GameFile.Create(sFilename, m_CampaignSettings.sUsername) != NOERROR)
		{
		if (retsError) *retsError = strPatternSubst(CONSTLIT("Unable to create save file: %s."), sFilename);
		return false;
		}

	//	If we're signed in and if the adventure/extension combination is
	//	registered, then we have a registered game.

	if (!m_CampaignSettings.sUsername.IsBlank() && !m_Universe.InDebugMode() && m_Universe.GetDesignCollection().IsRegisteredGame())
		m_Universe.SetRegistered(true);

	//	Create a controller for the player

	CAmericaPlayer *pPlayer = new CAmericaPlayer;
	if (!pPlayer->Init(CampaignSettings, retsError))
		return false;

	//	Start the campaign

	if (m_Universe.InitAdventure(pPlayer, retsError) != NOERROR)
		return false;

	return true;
	}

bool CAmericaModel::StartNewCampaignBackground (CString *retsError)

//	StartNewCampaignBackground
//
//	Continues creating the campaign in the background.

	{

	return true;
	}
