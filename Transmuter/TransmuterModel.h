//	TransmuterModel.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CTransmuterModel;

struct SEditorSettings
	{
	CExtension *pEditorInstance;					//	Campaign to create
	TArray<CExtension *> Extensions;		//	Extensions to include

	CString sUsername;						//	Multiverse username of player
	};

//	Dialog Definitions ---------------------------------------------------------

class CDialogDesc
	{
	public:
		struct SActionDesc
			{
			CString sID;
			CString sLabel;
			ICCItem *pData;
			ICCItem *pCode;
			};

		CDialogDesc (CCodeChain &CC);
		~CDialogDesc (void);

		void CleanUp (void);
		inline const CString &GetDescription (void) const { return m_sDescription; }
		inline SActionDesc *GetAction (int iIndex) { return &m_Actions[iIndex]; }
		inline int GetActionCount (void) const { return m_Actions.GetCount(); }
		inline const CString &GetTitle (void) const { return m_sTitle; }
		bool InitFromItem (ICCItem *pDesc, CString *retsError);

	private:
		CCodeChain &m_CC;
		CString m_sTitle;
		CString m_sDescription;
		TArray<SActionDesc> m_Actions;
	};

//	CodeChain Primitive Libraries ----------------------------------------------

class CUILibrary : public IPrimitiveImpl
	{
	public:
		CUILibrary (CHumanInterface &HI, CTransmuterModel &Model) : 
				m_HI(HI),
				m_Model(Model)
			{ }

		virtual ICCItem *InvokeCCPrimitive (CEvalContext *pEvalCtx, ICCItem *pArgs, DWORD dwData);
		virtual bool RegisterCCPrimitives (CCodeChain &CC);

	private:
		CHumanInterface &m_HI;
		CTransmuterModel &m_Model;
	};

//	CTransmuterModel --------------------------------------------------------------

class CTransmuterModel
	{
	public:
		struct SInitDesc
			{
			SInitDesc (void) :
					bDebugMode(false),
					bForceTDB(false)
				{ }

			CString sCollectionFolder;
			TArray<CString> ExtensionFolders;
			TArray<CString> SaveFileFolders;

			bool bDebugMode;
			bool bForceTDB;
			};

		CTransmuterModel (CHumanInterface &HI, CTransmuterController *pController);

		const CVector &GetPOVPos (void) const;
		inline CUniverse &GetUniverse (void) { return m_Universe; }
		bool Init (const SInitDesc &Ctx, CString *retsError = NULL);
		bool InitBackground (CString *retsError = NULL);
		void InvokeCode (ICCItem *pCode);

	private:
		inline CString GetSaveFilePath (void) const { return (m_SaveFileFolders.GetCount() == 0 ? NULL_STR : m_SaveFileFolders[0]); }
		bool LoadUniverse (CString *retsError = NULL);

		CHumanInterface &m_HI;
		CTransmuterController *m_pController;

		CString m_sCollectionFolder;
		TArray<CString> m_ExtensionFolders;
		TArray<CString> m_SaveFileFolders;

		CUniverse m_Universe;
		SEditorSettings m_CampaignSettings;
		CGameFile m_GameFile;
		CUILibrary m_UILibrary;				//	UI CC primitives

		bool m_bDebugMode;
		bool m_bForceTDB;
	};
