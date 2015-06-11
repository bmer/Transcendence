//	Transmuter.h
//
//	Main header file

#ifndef INCL_TRANSMUTER
#define INCL_TRANSMUTER

#ifndef INCL_TSE
#include "TSE.h"
#endif

#ifndef INCL_TSUI
#include "TSUI.h"
#endif

class CIntroSession;
class CUIResources;
class CTransmuterController;
class CTransmuterModel;
class CTransmuterSession;

extern int g_cxScreen;
extern int g_cyScreen;
const int g_iColorDepth =					16;		//	Desired color depth

const int g_iFramesPerSecond =				30;		//	Desired frames per second
const int FRAME_RATE_COUNT =				51;		//	number of statistics to keep (for debugging)

struct SCreateTransmuter
	{
	HINSTANCE hInst;
	char *pszCommandLine;
	bool bWindowed;
	};

struct STransmuterSettings
	{
	int somePlaceHolderVariable;
	};

struct SFontTable
	{
	CG16bitFont Small;				//	10 pixels
	CG16bitFont Medium;				//	13 pixels
	CG16bitFont MediumBold;			//	13 pixels bold
	CG16bitFont MediumHeavyBold;	//	14 pixels bold
	CG16bitFont Large;				//	16 pixels
	CG16bitFont LargeBold;			//	16 pixels bold
	CG16bitFont Header;				//	19 pixels (messages, map system name, picker)
	CG16bitFont HeaderBold;			//	19 pixels bold (help page titles)
	CG16bitFont SubTitle;			//	26 pixels (crawl screen, "Loading", medium credits, ship class disp)
	CG16bitFont SubTitleBold;		//	26 pixels bold (not used)
	CG16bitFont SubTitleHeavyBold;	//	28 pixels bold (dock screen counter)
	CG16bitFont Title;				//	56 pixels (dock screen title and large credits)
	CG16bitFont LogoTitle;			//	60 pixel Blue Island

	CG16bitFont Console;			//	Fixed-width font

	CG32bitPixel rgbTitleColor;			//	Large text color
	CG32bitPixel rgbLightTitleColor;	//	A lighter text color
	CG32bitPixel rgbTextColor;			//	Color for large blocks
	CG32bitPixel rgbHelpColor;			//	Faded help text
	CG32bitPixel rgbBackground;			//	Almost black background
	CG32bitPixel rgbSectionBackground;	//	Ligher section background
	CG32bitPixel rgbSelectBackground;	//	Selection background

	CG32bitPixel rgbAltRedColor;		//	Complementary alternate color
	CG32bitPixel rgbAltRedBackground;	//	Complementary background
	CG32bitPixel rgbAltGreenColor;		//	Complementary alternate color
	CG32bitPixel rgbAltGreenBackground;	//	Complementary background
	CG32bitPixel rgbAltYellowColor;		//	Complementary alternate color
	CG32bitPixel rgbAltYellowBackground;//	Complementary background
	CG32bitPixel rgbAltBlueColor;		//	Complementary alternate color
	CG32bitPixel rgbAltBlueBackground;	//	Complementary background

	CG32bitPixel rgbItemTitle;			//	Item title color
	CG32bitPixel rgbItemRef;			//	Item reference text color
	CG32bitPixel rgbItemDesc;			//	Item description color
	CG32bitPixel rgbItemDescSelected;	//	Item description when selected
	};

struct SNewGameSettings
	{
	};

struct SAdventureSettings
	{
	CExtension *pAdventure;						//	Adventure to create
	TArray<CExtension *> Extensions;			//	List of extensions
	};

class CPlayerGameStats
	{
	public:
		enum EEventTypes
			{
			eventNone =							0,

			eventEnemyDestroyedByPlayer	=		1,		//	The player destroyed something important
			eventFriendDestroyedByPlayer =		2,		//	The player destroyed something important
			eventSavedByPlayer =				3,		//	The player saved something from being destroyed
			eventMajorDestroyed =				4,		//	A major object was destroyed, but not by the player
			eventMissionSuccess =				5,		//	The player successfully completed a mission
			eventMissionFailure =				6,		//	The player failed a mission
			};

		CPlayerGameStats (void);

		int CalcEndGameScore (void) const;
		void GenerateGameStats (CGameStats &Stats, CSpaceObject *pPlayer, bool bGameOver) const;
		int GetBestEnemyShipsDestroyed (DWORD *retdwUNID = NULL) const;
		CTimeSpan GetGameTime (void) const { return (!m_GameTime.IsBlank() ? m_GameTime : g_pUniverse->GetElapsedGameTime()); }
		CString GetItemStat (const CString &sStat, ICCItem *pItemCriteria) const;
		CString GetKeyEventStat (const CString &sStat, const CString &sNodeID, const CDesignTypeCriteria &Crit) const;
		CTimeSpan GetPlayTime (void) const { return (!m_PlayTime.IsBlank() ? m_PlayTime : g_pUniverse->StopGameTime()); }
		CString GetStat (const CString &sStat) const;
		DWORD GetSystemEnteredTime (const CString &sNodeID);
		int IncStat (const CString &sStat, int iInc = 1);
		void OnGameEnd (CSpaceObject *pPlayer);
		void OnItemBought (const CItem &Item, CurrencyValue iTotalPrice);
		void OnItemDamaged (const CItem &Item, int iHP);
		void OnItemFired (const CItem &Item);
		void OnItemInstalled (const CItem &Item);
		void OnItemSold (const CItem &Item, CurrencyValue iTotalPrice);
		void OnItemUninstalled (const CItem &Item);
		void OnKeyEvent (EEventTypes iType, CSpaceObject *pObj, DWORD dwCauseUNID);
		void OnObjDestroyedByPlayer (const SDestroyCtx &Ctx, CSpaceObject *pPlayer);
		void OnSystemEntered (CSystem *pSystem, int *retiLastVisit = NULL);
		void OnSystemLeft (CSystem *pSystem);
		void ReadFromStream (SLoadCtx &Ctx);
		void SetStat (const CString &sStat, const CString &sValue);
		void WriteToStream (IWriteStream *pStream);

	private:
		struct SItemTypeStats
			{
			int iCountSold;						//	Number of items sold
			CurrencyValue iValueSold;			//	Total value received for selling (credits)

			int iCountBought;					//	Number of items bought
			CurrencyValue iValueBought;			//	Total value spent buying (credits)

			int iCountInstalled;				//	Number of items currently installed on player ship
			DWORD dwFirstInstalled;				//	First time item was installed on player ship
			DWORD dwLastInstalled;				//	Last time item was installed on player ship
			DWORD dwLastUninstalled;			//	Last time item was uninstalled from player ship
			DWORD dwTotalInstalledTime;			//	Total time installed

			int iCountFired;					//	Number of times item (weapon) has been fired by player
			int iHPDamaged;						//	HP absorbed by this item type when installed on player
			};

		struct SKeyEventStats
			{
			EEventTypes iType;					//	Type of event
			DWORD dwTime;						//	When the event happened
			DWORD dwObjUNID;					//	UNID of object
			CString sObjName;					//	For unique objects (e.g., CSCs)
			DWORD dwObjNameFlags;				//	Flags for the name
			DWORD dwCauseUNID;					//	The UNID of the sovereign who caused the event
			};

		struct SKeyEventStatsResult
			{
			CString sNodeID;
			SKeyEventStats *pStats;
			bool bMarked;
			};

		struct SShipClassStats
			{
			int iEnemyDestroyed;				//	Number of enemy ships destroyed
			int iFriendDestroyed;				//	Number of friendly ships destroyed
			};

		struct SStationTypeStats
			{
			int iDestroyed;						//	Number of stations destroyed
			};

		struct SSystemStats
			{
			DWORD dwFirstEntered;				//	First time this system was entered (0xffffffff = never)
			DWORD dwLastEntered;				//	Last time this system was entered (0xffffffff = never)
			DWORD dwLastLeft;					//	Last time this system was left (0xffffffff = never)
			DWORD dwTotalTime;					//	Total time in system (all visits)
			};

		bool AddMatchingKeyEvents (const CString &sNodeID, const CDesignTypeCriteria &Crit, TArray<SKeyEventStats> *pEventList, TArray<SKeyEventStatsResult> *retList) const;
		bool FindItemStats (DWORD dwUNID, SItemTypeStats **retpStats) const;
		CString GenerateKeyEventStat (TArray<SKeyEventStatsResult> &List) const;
		SItemTypeStats *GetItemStats (DWORD dwUNID);
		bool GetMatchingKeyEvents (const CString &sNodeID, const CDesignTypeCriteria &Crit, TArray<SKeyEventStatsResult> *retList) const;
		SShipClassStats *GetShipStats (DWORD dwUNID);
		SStationTypeStats *GetStationStats (DWORD dwUNID);
		SSystemStats *GetSystemStats (const CString &sNodeID);

		int m_iScore;							//	Total score for player
		int m_iResurrectCount;					//	Number of times player has resurrected a game
		CTimeSpan m_PlayTime;					//	Total time spent playing the game
		CTimeSpan m_GameTime;					//	Total elapsed time in the game

		TMap<DWORD, SItemTypeStats> m_ItemStats;
		TMap<DWORD, SShipClassStats> m_ShipStats;
		TMap<DWORD, SStationTypeStats> m_StationStats;
		TMap<CString, SSystemStats> m_SystemStats;
		TMap<CString, TArray<SKeyEventStats>> m_KeyEventStats;

		int m_iExtraSystemsVisited;				//	For backwards compatibility
		int m_iExtraEnemyShipsDestroyed;		//	For backwards compatibility
	};

#define MAX_BUTTONS					100

class CButtonBarData
	{
	public:
		CButtonBarData (void);
		~CButtonBarData (void);

		enum AlignmentTypes
			{
			alignCenter,
			alignLeft,
			alignRight,
			};

		enum StyleTypes
			{
			styleLarge,							//	128x64 button
			styleMedium,						//	64x64 button
			};

		void AddButton (DWORD dwCmdID,
						const CString &sLabel,
						const CString &sDescription,
						const CString &sKey,
						int iImageIndex,
						AlignmentTypes iAlign,
						StyleTypes iStyle = styleLarge);
		void CleanUp (void);
		int FindButton (const CString &sKey);
		inline AlignmentTypes GetAlignment (int iIndex) { return m_Buttons[iIndex].iAlign; }
		inline const RECT &GetButtonRect (int iIndex) { return m_Buttons[iIndex].rcRect; }
		inline DWORD GetCmdID (int iIndex) { return m_Buttons[iIndex].dwCmdID; }
		inline int GetCount (void) { return m_iCount; }
		inline const CString &GetDescription (int iIndex) { return m_Buttons[iIndex].sDescription; }
		inline const CG32bitImage &GetImage (void) { return m_Images; }
		void GetImageSize (int iIndex, RECT *retrcRect);
		inline int GetImageIndex (int iIndex) { return m_Buttons[iIndex].iImageIndex; }
		inline const CString &GetKey (int iIndex) { return m_Buttons[iIndex].sKey; }
		inline const CString &GetLabel (int iIndex) { return m_Buttons[iIndex].sLabel; }
		inline StyleTypes GetStyle (int iIndex) { return m_Buttons[iIndex].iStyle; }
		inline bool GetVisible (int iIndex) { return ((m_Buttons[iIndex].dwFlags & FLAG_VISIBLE) ? true : false); }
		int HitTest (int x, int y);
		ALERROR Init (void);

		inline void SetButtonRect (int iIndex, const RECT &rcRect) { m_Buttons[iIndex].rcRect = rcRect; }
		inline void SetDescription (int iIndex, const CString &sDescription) { m_Buttons[iIndex].sDescription = sDescription; }
		inline void SetLabel (int iIndex, const CString &sLabel) { m_Buttons[iIndex].sLabel = sLabel; }
		inline void SetVisible (int iIndex, bool bVisible) { if (bVisible) m_Buttons[iIndex].dwFlags |= FLAG_VISIBLE; else m_Buttons[iIndex].dwFlags &= ~FLAG_VISIBLE; }

	private:
		enum Flags
			{
			FLAG_VISIBLE = 0x00000001,
			};

		struct Entry
			{
			DWORD dwCmdID;
			CString sLabel;
			CString sDescription;
			CString sKey;
			int iImageIndex;
			AlignmentTypes iAlign;
			StyleTypes iStyle;
			DWORD dwFlags;

			RECT rcRect;
			};

		int m_iCount;
		Entry m_Buttons[MAX_BUTTONS];
		CG32bitImage m_Images;
	};

class CButtonBarDisplay
	{
	public:
		CButtonBarDisplay (void);
		~CButtonBarDisplay (void);

		void CleanUp (void);
		inline const RECT &GetRect (void) { return m_rcRect; }
		ALERROR Init (CTransmuterSession *pTmSession, 
					  CButtonBarData *pData, 
					  const RECT &rcRect);
		inline void Invalidate (void) { }
		bool OnChar (char chChar);
		bool OnKeyDown (int iVirtKey);
		bool OnLButtonDoubleClick (int x, int y);
		bool OnLButtonDown (int x, int y);
		void OnMouseMove (int x, int y);
		void Paint (CG32bitImage &Dest);
		inline void SetFontTable (const SFontTable *pFonts) { m_pFonts = pFonts; }
		void Update (void);

	private:
		void ComputeButtonRects (void);
		void ComputeButtonSize (int iIndex, int *retcxWidth, int *retcyHeight);
		int ComputeButtonWidth (CButtonBarData::AlignmentTypes iAlignment);
		void ComputeButtonRects (CButtonBarData::AlignmentTypes iAlignment, int x, int y);
		void GetImageRect (int iIndex, bool bSelected, RECT *retrcRect);
		int FindButtonAtPoint (const POINT &pt);

		CTransmuterSession *m_pTmSession;
		CButtonBarData *m_pButtons;
		RECT m_rcRect;
		const SFontTable *m_pFonts;

		int m_iSelected;
	};

class CCommandLineDisplay
	{
	public:
		CCommandLineDisplay (void);CTransmuterSession *m_pTmSession;
		~CCommandLineDisplay (void);

		void CleanUp (void);
		inline void ClearInput (void) { m_sInput = NULL_STR; m_bInvalid = true; }
		inline const CString &GetInput (void) { return m_sInput; }
		inline int GetOutputLineCount (void) { return GetOutputCount(); }
		inline const RECT &GetRect (void) { return m_rcRect; }
		ALERROR Init (CTransmuterSession *pTmSession, const RECT &rcRect);
		void Input (const CString &sInput);
		void InputBackspace (void);
		void InputEnter (void);
		void InputLastLine (void);
		void OnKeyDown (int iVirtKey, DWORD dwKeyState);
		void Output (const CString &sOutput, CG32bitPixel rgbColor = CG32bitPixel::Null());
		void Paint (CG32bitImage &Dest);
		inline void SetFontTable (const SFontTable *pFonts) { m_pFonts = pFonts; }

	private:
		enum Constants
			{
			MAX_LINES = 80,
			};

		void AppendOutput (const CString &sLine, CG32bitPixel rgbColor);
		const CString &GetOutput (int iLine);
		CG32bitPixel GetOutputColor (int iLine);
		int GetOutputCount (void);
		void Update (void);

		CTransmuterSession *m_pTmSession;
		const SFontTable *m_pFonts;
		RECT m_rcRect;

		CString m_Output[MAX_LINES + 1];
		CG32bitPixel m_OutputColor[MAX_LINES + 1];
		int m_iOutputStart;
		int m_iOutputEnd;
		CString m_sInput;
		CString m_sLastLine;

		CG32bitImage m_Buffer;
		bool m_bInvalid;
		int m_iCounter;
		RECT m_rcCursor;
	};

class CTransmuterSession : public IAniCommand
	{
	public:
		CTransmuterSession (HWND hWnd, CTransmuterController *pTC);

		void Animate (CG32bitImage &TheScreen, CGameSession *pSession, bool bTopMost);

		void OnSomePlaceholderAction(int *pSomePlaceHolderVariable);

		//	CUniverse::IHost
		virtual void ConsoleOutput (const CString &sLine);
		virtual IPlayerController *CreatePlayerController (void);
		virtual IShipController *CreateShipController (const CString &sController);
		virtual void DebugOutput (const CString &sLine);
		virtual void GameOutput (const CString &sLine);
		virtual const CG16bitFont *GetFont (const CString &sFont);

		//	IAniCommand
		virtual void OnAniCommand (const CString &sID, const CString &sEvent, const CString &sCmd, DWORD dwData);

	private:
		enum SessionState
			{
			gSomePlaceholderVariable,
			};

		struct SPreferences
			{
			bool bMusicOn;
			int iSoundVolume;

			bool bModified;
			};

		void OnAccountChanged (const CMultiverseModel &Multiverse);
		void OnCommandIntro (const CString &sCmd, void *pData);
		void OnDblClickIntro (int x, int y, DWORD dwFlags);
		void OnCharIntro (char chChar, DWORD dwKeyData);
		void OnKeyDownIntro (int iVirtKey, DWORD dwKeyData);
		void OnLButtonDownIntro (int x, int y, DWORD dwFlags);
		void OnLButtonUpIntro (int x, int y, DWORD dwFlags);
		void OnMouseMoveIntro (int x, int y, DWORD dwFlags);
		void PaintDlgButton (const RECT &rcRect, const CString &sText);
		void SetAccountControls (const CMultiverseModel &Multiverse);
		void SetDebugOption (void);

		void SetMusicOption (void);

		void OnKeyDownHelp (int iVirtKey, DWORD dwKeyData);
		void PaintHelpScreen (void);

		void CleanUpDisplays (void);
		void ClearDebugLines (void);
		void ComputeScreenSize (void);
		ALERROR InitDisplays (void);
		void LoadPreferences (void);
		void PaintDebugLines (void);
		void PaintFrameRate (void);
		void PaintLRS (void);
		void PaintMainScreenBorder (void);

		void ReportCrash (void);
		void ReportCrashEvent (CString *retsMessage);

		void SavePreferences (void);
		void ShowErrorMessage (const CString &sError);

		LONG WMActivateApp (bool bActivate);
		LONG WMChar (char chChar, DWORD dwKeyData);
		LONG WMClose (void);
		LONG WMCreate (CString *retsError);
		LONG WMDestroy (void);
		LONG WMDisplayChange (int iBitDepth, int cxWidth, int cyHeight);
		LONG WMKeyDown (int iVirtKey, DWORD dwKeyData);
		LONG WMKeyUp (int iVirtKey, DWORD dwKeyData);
		LONG WMLButtonDblClick (int x, int y, DWORD dwFlags);
		LONG WMLButtonDown (int x, int y, DWORD dwFlags);
		LONG WMLButtonUp (int x, int y, DWORD dwFlags);
		LONG WMMouseMove (int x, int y, DWORD dwFlags);
		LONG WMMove (int x, int y);
		LONG WMSize (int cxWidth, int cyHeight, int iSize);

		CTransmuterController *m_pTC;

		//	General
		CString m_sVersion;
		CString m_sCopyright;

		//	hWnds
		HWND m_hWnd;

		//	DirectX
		CUIResources m_UIRes;
		SFontTable m_Fonts;
		CReanimator m_Reanimator;

		//	Main game structures
		RECT m_rcScreen;					//	Rect of entire screen
		RECT m_rcMainScreen;				//	Rect of main (1024x768) area
		RECT m_rcWindow;					//	Rect of main window in screen coordinates
		RECT m_rcWindowScreen;				//	Rect of screen within window

		CG32bitImage m_LRS;					//	Long-range scan
		RECT m_rcLRS;						//	Rect on screen where LRS goes
		CG32bitImage *m_pLargeHUD;			//	Background LRS image
		CG32bitImage *m_pSRSSnow;			//	SRS snow image
		CG32bitImage *m_pLRSBorder;			//	LRS border

		CCommandLineDisplay m_DebugConsole;	//	CodeChain debugging console

		CGameStats m_LastStats;				//	Last game stats

		//	DirectX debugging
		int m_iFrameCount;					//	Number of frames so far
		int m_iFrameTime[FRAME_RATE_COUNT];	//	Last n frame times (in milliseconds)
		int m_iPaintTime[FRAME_RATE_COUNT];	//	Last n paint times (in milliseconds)
		int m_iUpdateTime[FRAME_RATE_COUNT];//	Last n update times (in milliseconds)
		int m_iBltTime[FRAME_RATE_COUNT];	//	Last n BltToScreen times (in milliseconds)
		int m_iStartAnimation;

		//	Debug info
#ifdef DEBUG
		CString m_DebugLines[DEBUG_LINES_COUNT];			//	Debug lines
		int m_iDebugLinesStart;
		int m_iDebugLinesEnd;
#endif
		CString m_sCrashInfo;				//	Saved crash info
		CTextFileLog m_GameLog;

	friend LONG APIENTRY MainWndProc (HWND hWnd, UINT message, UINT wParam, LONG lParam);
	friend class CIntroShipController;
	friend class CGameSession;
	friend class CIntroSession;
	friend class CTransmuterController;
	friend class CTransmuterModel;
	};

//	Key mapping class ---------------------------------------------------------

const DWORD INVALID_VIRT_KEY = 0xFFFFFFFF;

class CEditorKeys
	{
	public:
		enum Keys
			{
			keyError =					-1,
			keyNone =					0,

			keyThrustForward =			16,	//	Up and Down
			keyRotateLeft =				18,	//	left
			keyRotateRight =			19,	//	right
			keyFireWeapon =				20,	//	Ctrl and Space
			keyFireMissile =			21,	//	Shift and Return
			keyNextMissile =			22,	//	Tab
			keyShowHelp =				23,	//	F1
			keyShowGameStats =			24,	//	F2
			keyReserved3 =				25,	//	F3
			keyReserved4 =				26,	//	F4
			keyReserved5 =				27,	//	F5
			keyReserved6 =				28,	//	F6
			keyVolumeUp =				29,	//	F7
			keyVolumeDown =				30,	//	F8
			keyShowConsole =			31,	//	F9

			keySpecial1 =				32,	//	'1'
			keySpecial2 =				33,	//	'2'
			keySpecial3 =				34,	//	'3'
			keySpecial4 =				35,	//	'4'
			keySpecial5 =				36,	//	'5'
			keySpecial6 =				37,	//	'6'
			keySpecial7 =				38,	//	'7'
			keySpecial8 =				39,	//	'8'
			keySpecial9 =				40,	//	'9'
			keySpecial0 =				41,	//	'0'

			keyEnableAllDevices =		42,
			keyDisableAllDevices =		43,
			keyEnableAllDevicesToggle =	44,
			keyEnableDeviceToggle00 =	45,
			keyEnableDeviceToggle01 =	46,
			keyEnableDeviceToggle02 =	47,
			keyEnableDeviceToggle03 =	48,
			keyEnableDeviceToggle04 =	49,
			keyEnableDeviceToggle05 =	50,
			keyEnableDeviceToggle06 =	51,
			keyEnableDeviceToggle07 =	52,
			keyEnableDeviceToggle08 =	53,
			keyEnableDeviceToggle09 =	54,
			keyEnableDeviceToggle10 =	55,
			keyEnableDeviceToggle11 =	56,
			keyEnableDeviceToggle12 =	57,
			keyEnableDeviceToggle13 =	58,
			keyEnableDeviceToggle14 =	59,
			keyEnableDeviceToggle15 =	60,
			keyEnableDeviceToggle16 =	61,
			keyEnableDeviceToggle17 =	62,
			keyEnableDeviceToggle18 =	63,
			keyEnableDeviceToggle19 =	64,
			keyEnableDeviceToggle20 =	65,
			keyEnableDeviceToggle21 =	66,
			keyEnableDeviceToggle22 =	67,
			keyEnableDeviceToggle23 =	68,
			keyEnableDeviceToggle24 =	69,
			keyEnableDeviceToggle25 =	70,
			keyEnableDeviceToggle26 =	71,
			keyEnableDeviceToggle27 =	72,
			keyEnableDeviceToggle28 =	73,
			keyEnableDeviceToggle29 =	74,
			keyEnableDeviceToggle30 =	75,
			keyEnableDeviceToggle31 =	76,

			keyTargetPrevFriendly =		77,
			keyTargetPrevEnemy =		78,
			keyPrevWeapon =				79,
			keyPrevMissile =			80,
			keyShowGalacticMap =		81,

			keyCount =					82,
			};

		CEditorKeys (void);

		CEditorKeys::Keys GetEditorCommand (const CString &sCmd) const;
		inline Keys GetEditorCommand (DWORD dwVirtKey) const { return m_iMap[(dwVirtKey < 256 ? dwVirtKey : 0)]; }
		DWORD GetKey (const CString &sKey) const;
		char GetKeyIfChar (Keys iCommand) const;

		ALERROR ReadFromXML (CXMLElement *pDesc);
		ALERROR WriteAsXML (IWriteStream *pOutput);

	private:
		Keys m_iMap[256];
	};

//	Game settings class -------------------------------------------------------

class IExtraSettingsHandler
	{
	public:
		virtual ALERROR OnLoadSettings (CXMLElement *pDesc, bool *retbModified) { return NOERROR; }
		virtual ALERROR OnSaveSettings (IWriteStream *pOutput) { return NOERROR; }
	};

class CEditorSettings
	{
	public:
		enum Options
			{
			//	Game play options
			playerName,						//	Default player name

			//	Installation options
			useTDB,							//	Force use of .TDB
			noAutoUpdate,					//	Do not auto upgrade the game

			//	Video options
			forceDirectX,					//	Use DirectX to access screen directly
			forceNonDirectX,				//	Use GDI to draw to screen
			forceExclusive,					//	Blt directly to primary surface
			forceNonExclusive,				//	Blt using cooperative mode
			windowedMode,					//	Use a window (instead of full screen)
			useBackgroundBlt,				//	Blt in the background
			force1024Res,					//	Force 1024x768 resolution
			force600Res,					//	Force 1024x600 resolution
			graphicsQuality,				//	SFX vs performance
			no3DSystemMap,					//	3D system map projection

			//	Sounds options
			noSound,						//	No sound (either music or sound effects)
			noMusic,						//	TRUE if we should play music
			soundVolume,					//	FX volume level
			musicVolume,					//	Music volume level
			musicPath,						//	Path to music folder (may be NULL_STR)

			//	Debug options
			debugMode,						//	Program in debug mode
			debugGame,						//	Game instance in debug mode
			noDebugLog,						//	Do not write out a debug log
			debugVideo,						//	Write out video information
			noCrashPost,					//	Do not post crash log to Multiverse
			noCollectionDownload,			//	Do not automatically download collection
			debugSoundtrack,				//	Soundtrack debugging UI

			//	Constants
			OPTIONS_COUNT = 34,
			};

		CEditorSettings (IExtraSettingsHandler *pExtra = NULL) : m_pExtra(pExtra) { }

		inline const CString &GetAppDataFolder (void) const { return m_sAppData; }
		inline bool GetBoolean (int iOption) const { return m_Options[iOption].bValue; }
		inline const TArray<CString> &GetExtensionFolders (void) const { return m_ExtensionFolders; }
		inline const CString &GetInitialSaveFile (void) const { return m_sSaveFile; }
		inline int GetInteger (int iOption) const { return m_Options[iOption].iValue; }
		inline const CEditorKeys &GetKeyMap (void) const { return m_KeyMap; }
		inline const CString &GetString (int iOption) const { return m_Options[iOption].sValue; }
		ALERROR Load (const CString &sFilespec, CString *retsError = NULL);
		ALERROR ParseCommandLine (char *pszCmdLine);
		ALERROR Save (const CString &sFilespec);
		inline void SetBoolean (int iOption, bool bValue, bool bModifySettings = true) { SetValueBoolean(iOption, bValue, bModifySettings); if (bModifySettings) m_bModified = true; }
		inline void SetDefaultExtensions (DWORD dwAdventure, bool bDebugMode, const TArray<DWORD> &List) { m_Extensions.SetList(dwAdventure, bDebugMode, List); m_bModified = true; }
		inline void SetInteger (int iOption, int iValue, bool bModifySettings = true) { SetValueInteger(iOption, iValue, bModifySettings); if (bModifySettings) m_bModified = true; }
		inline void SetModified (void) { m_bModified = true; }
		inline void SetSettingsHandler (IExtraSettingsHandler *pExtra) { m_pExtra = pExtra; }
		inline void SetString (int iOption, const CString &sValue, bool bModifySettings = true) { SetValueString(iOption, sValue, bModifySettings); if (bModifySettings) m_bModified = true; }

	private:
		struct SOption
			{
			bool bValue;					//	Current boolean value
			int iValue;						//	Current integer value
			CString sValue;					//	Current string value

			CString sSettingsValue;			//	Value of this option in settings file
											//	NOTE: This may differ from the current value
											//	above if we don't want to save a setting
			};

		void LoadFromRegistry (void);
		void SetValue (int iOption, const CString &sValue, bool bSetSettings = false);
		void SetValueBoolean (int iOption, bool bValue, bool bSetSettings = false);
		void SetValueInteger (int iOption, int iValue, bool bSetSettings = false);
		void SetValueString (int iOption, const CString &sValue, bool bSetSettings = false);

		IExtraSettingsHandler *m_pExtra;	//	Additional settings handler
		SOption m_Options[OPTIONS_COUNT];	//	Options
		CEditorKeys m_KeyMap;					//	Key map
		CExtensionListMap m_Extensions;		//	Default extensions

		CString m_sAppData;					//	Location of Settings.xml
		TArray<CString> m_SaveFileFolders;	//	List of folders for save files (1st is default)
		TArray<CString> m_ExtensionFolders;//	List of folders for extensions (may be empty)

		CString m_sSaveFile;				//	Optional save file to open on game start

		bool m_bModified;					//	If TRUE, we need to save out settings
	};

//	Transmuter data model class --------------------------------------------

class CTransmuterModel
	{
	public:
		CTransmuterModel (CHumanInterface &HI);
		~CTransmuterModel (void) { }

		ALERROR InitTransmuter (const STransmuterSettings &Settings, CString *retsError);

		void OnSomePlaceholderEvent (int *somePlaceHolderVariable);

	private:
		enum States
			{
			stateUnknown,

			stateCreatingNewGame,					//	Creating a new game in the background
			stateInGame,							//	In normal play
			statePlayerInGateOldSystem,				//	Player removed from the system and in a stargate
			statePlayerInGateNewSystem,				//	Player not yet in the new system
			statePlayerInResurrect,					//	Player removed from the system, but about to be resurrected
			statePlayerDestroyed,					//	Player ship is no longer in universe,
													//		but we're still running
			statePlayerInEndGame,					//	Player hit end game condition
			stateGameOver,							//	GameFile closed
			};

		CString CalcEpitaph (SDestroyCtx &Ctx);
		void CalcStartingPos (CShipClass *pStartingShip, DWORD *retdwMap, CString *retsNodeID, CString *retsPos);
		ALERROR CreateAllSystems (const CString &sStartNode, CSystem **retpStartingSystem, CString *retsError);
		void GenerateGameStats (CGameStats *retStats, bool bGameOver = false);
		inline CString GetSaveFilePath (void) const { return (m_SaveFileFolders.GetCount() == 0 ? NULL_STR : m_SaveFileFolders[0]); }
		ALERROR LoadGameStats (const CString &sFilespec, CGameStats *retStats);
		ALERROR LoadHighScoreList (CString *retsError = NULL);
		ALERROR LoadUniverse (const CString &sCollectionFolder, const TArray<CString> &ExtensionFolders, CString *retsError = NULL);
		void MarkGateFollowers (CSystem *pSystem);
		ALERROR SaveGameStats (const CGameStats &Stats, bool bGameOver = false, bool bEndGame = false);
		void TransferGateFollowers (CSystem *pOldSystem, CSystem *pSystem, CSpaceObject *pStargate);

		CHumanInterface &m_HI;
		States m_iState;

		SFileVersionInfo m_Version;
		TArray<CString> m_SaveFileFolders;			//	List of all save file folders (first is the default)
		bool m_bDebugMode;							//	Game in debug mode (or next game should be in debug mode)
		bool m_bForceTDB;							//	Use TDB even if XML files exist
		bool m_bNoSound;							//	No sound
		bool m_bNoMissionCheckpoint;				//	Do not save game on mission accept

		CEditorSessionFile m_EditorSessionFile;
		CUniverse m_Universe;
	};

//	CTransmuterController class --------------------------------------------
//
//	COMMANDS
//
//	uiShowHelp						Show help session

class CTransmuterController : public IHIController, public IExtraSettingsHandler, public CUniverse::IHost
	{
	public:
		CTransmuterController (void) : 
				m_iState(stateNone),
				m_iBackgroundState(stateIdle),
				m_Model(m_HI),
				m_bUpgradeDownloaded(false)
			{ }

		inline const CEditorKeys &GetKeyMap (void) const { return m_Settings.GetKeyMap(); }
		inline CTransmuterModel &GetModel (void) { return m_Model; }
		inline CMultiverseModel &GetMultiverse (void) { return m_Multiverse; }
		inline bool GetOptionBoolean (int iOption) { return m_Settings.GetBoolean(iOption); }
		inline int GetOptionInteger (int iOption) { return m_Settings.GetInteger(iOption); }
		inline CCloudService &GetService (void) { return m_Service; }
		inline CEditorSettings &GetSettings (void) { return m_Settings; }
		inline CSoundtrackManager &GetSoundtrack (void) { return m_Soundtrack; }
		void SetOptionBoolean (int iOption, bool bValue);
		void SetOptionInteger (int iOption, int iValue);

		//	IHICommand virtuals
		virtual ALERROR OnBoot (char *pszCommandLine, SHIOptions *retOptions, CString *retsError);
		virtual void OnCleanUp (void);
		virtual bool OnClose (void);
		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (CString *retsError);
		virtual void OnShutdown (EHIShutdownReasons iShutdownCode);

		//	IExtraSettingsHandler
		virtual ALERROR OnLoadSettings (CXMLElement *pDesc, bool *retbModified);
		virtual ALERROR OnSaveSettings (IWriteStream *pOutput);

	private:
		enum States
			{
			stateNone,
			};

		enum BackgroundStates
			{
			stateIdle,

			stateSignedIn,					//	Signed in and idle
			stateLoadingCollection,			//	Waiting for collection to load
			stateDownloadingUpgrade,		//	Downloading an upgrade to the game
			stateDownloadingCatalogEntry,	//	Downloading a TDB file
			stateLoadingNews,				//	Waiting for news to load
			stateDownloadingResource,		//	Downloading a resource file
			};

		void CleanUpUpgrade (void);
		bool CheckAndRunUpgrade (void);
		void DisplayMultiverseStatus (const CString &sStatus, bool bError = false);
		bool InstallUpgrade (CString *retsError);
		bool IsUpgradeReady (void);
		bool RequestCatalogDownload (const TArray<CMultiverseCatalogEntry *> &Downloads);
		bool RequestResourceDownload (const TArray<CMultiverseFileRef> &Downloads);
		ALERROR WriteUpgradeFile (IMediaType *pData, CString *retsError);

		States m_iState;
		BackgroundStates m_iBackgroundState;
		CTransmuterModel m_Model;

		CCloudService m_Service;
		CMultiverseModel m_Multiverse;
		CSoundtrackManager m_Soundtrack;
		bool m_bUpgradeDownloaded;

		CEditorSettings m_Settings;
	};

//	Utility functions

void AnimateMainWindow (HWND hWnd);
void CopyGalacticMapToClipboard (HWND hWnd, CGalacticMapPainter *pPainter);
void CopyGameStatsToClipboard (HWND hWnd, const CGameStats &GameStats);
const CG16bitFont &GetFontByName (const SFontTable &Fonts, const CString &sFontName);
ALERROR InitCodeChainExtensions (CCodeChain &CC);
CString TransPath (const CString &sPath);

//	Animation functions

//	Inlines

inline CEditorSessionFile &CTransmuterSession::GetEditorSessionFile (void)
	{
	return m_pTC->GetModel().GetGameFile();
	}

inline CTransmuterModel &CTransmuterSession::GetModel (void)
	{
	return m_pTC->GetModel();
	}

inline CGameSettings &CTransmuterSession::GetSettings (void)
	{
	return m_pTC->GetSettings();
	}

#include "BackgroundTasks.h"
#include "Sessions.h"

#endif