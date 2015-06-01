//	TransMuter.h
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

struct STransMuterHIOptions
	{
	STransMuterHIOptions (void) :
			sAppName(CONSTLIT("TransMuter")),
			sClassName(CONSTLIT("transmuter_class")),
			sAppData(CONSTLIT("Kronosaur\\TransMuter")),
			hIcon(NULL),
			m_cxScreenDesired(1024),
			m_cyScreenDesired(768),
			m_iColorDepthDesired(16),
			m_bWindowedMode(false),
			m_bMultiMonitorMode(false),
			m_bForceDX(false),
			m_bForceNonDX(false),
			m_bForceExclusiveMode(false),
			m_bForceNonExclusiveMode(false),
			m_bForceScreenSize(false),
			m_iSoundVolume(DEFAULT_SOUND_VOLUME),
			m_bDebugVideo(false)
		{ }

	//	App options
	CString sAppName;					//	Human readable app name (e.g., "Transcendence")
	CString sClassName;					//	Window class name (e.g., "transcendence_class")
	CString sAppData;					//	Path under AppData (e.g., "Kronosaur\Transcendence")
	HICON hIcon;						//	Application icon

	//	Display options
	int m_cxScreenDesired;				//	Ignored if not WindowedMode
	int m_cyScreenDesired;				//	Ignored if not WindowedMode
	int m_iColorDepthDesired;
	bool m_bWindowedMode;
	bool m_bMultiMonitorMode;			//	If TRUE, screen spans all monitors

	bool m_bForceDX;
	bool m_bForceNonDX;
	bool m_bForceExclusiveMode;
	bool m_bForceNonExclusiveMode;
	bool m_bForceScreenSize;

	//	Sound options
	int m_iSoundVolume;
	CString m_sMusicFolder;				//	Path to music folder

	//	Debug options
	bool m_bDebugVideo;
	};

//	Data Model -----------------------------------------------------------------

class CTransMuterModel
	{
	public:
		CTransMuterModel (CHumanInterface &HI) : m_HI(HI) { }
		~CTransMuterModel (void) { }

		void CleanUp (void) { }
		inline const CString &GetCopyright (void) { return m_Version.sCopyright; }
		inline const CString &GetProductName (void) { return m_Version.sProductName; }
		inline CUniverse &GetUniverse (void) { return m_Universe; }
		inline const CString &GetVersion (void) { return m_Version.sProductVersion; }
		ALERROR Init (void);
		ALERROR LoadUniverse (CString *retsError = NULL);

	private:
		CHumanInterface &m_HI;

		SFileVersionInfo m_Version;
		CUniverse m_Universe;
	};

//	Controller -----------------------------------------------------------------

class CTransMuterController : public IHIController
	{
	public:
		CTransMuterController (CHumanInterface &HI) : IHIController(HI),
				m_Model(HI)
			{ }

		//	IHICommand virtuals
		virtual ALERROR OnBoot (char *pszCommandLine, SHIOptions &Options);
		virtual void OnCleanUp (void);
		virtual ALERROR OnCommand (const CString &sCmd, void *pData = NULL);
		virtual ALERROR OnInit (void);

	private:
		CTransMuterModel m_Model;
	};

//	Background Tasks -----------------------------------------------------------

class CLoadUniverseTask : public IHITask
	{
	public:
		CLoadUniverseTask (CHumanInterface &HI, CTransMuterModel &Model) : IHITask(HI), m_Model(Model) { }

		//	IHITask virtuals
		virtual ALERROR OnExecute (ITaskProcessor *pProcessor, CString *retsResult) { return m_Model.LoadUniverse(retsResult); }

	private:
		CTransMuterModel &m_Model;
	};

#include "Sessions.h"

#endif