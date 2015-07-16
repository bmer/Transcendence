//	EditorSessions.h
//
//	Displays for CTransmuterSession
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CContextPanel : public IAnimatron
	{
	public:
		struct SMetrics
			{
			//	Metrics

			int cxPane;
			int iCornerCut;
			int iFrameWidth;
			int iFramePadding;
			int cyCharacterImage;
			int iCommandSize;

			//	Colors

			CG32bitPixel rgbPaneBackground;

			CG32bitPixel rgbFrame;
			CG32bitPixel rgbSectionBackground;
			CG32bitPixel rgbMessageText;
			CG32bitPixel rgbTitleText;

			CG32bitPixel rgbShieldLevel;
			CG32bitPixel rgbArmorLevel;
			CG32bitPixel rgbFuelLevel;
			};

		CCommandPane (const CVisualPalette &VI, const SMetrics &Metrics, CTransmuterModel &Model);

		void ExecuteCommand (const CString &sID, const CVector &vPos);
		void ExecuteCommand (const CString &sID, CSpaceObject *pObj);

		//	IAnimatron virtuals

		virtual void GetSpacingRect (RECT *retrcRect);
		virtual void HandleLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture, bool *retbFocus);
		virtual void HandleLButtonUp (int x, int y, DWORD dwFlags);
		virtual void HandleMouseLeave (void);
		virtual void HandleMouseMove (int x, int y, DWORD dwFlags);
		virtual IAnimatron *HitTest (const CXForm &ToDest, int x, int y);
		virtual void Paint (SAniPaintCtx &Ctx);

	private:
		struct SCommandDesc
			{
			CString sID;					//	ID of the command to run
			CString sName;					//	Command name

			RECT rcRect;					//	Rect of command button (relative to pane)
			};

		struct SExtensionObject
			{
			SExtensionObject (void) :
					sExtensionObjectName("")
				{ }

			CString sExtensionObjectName;
			};

		struct SMiniStatusDesc
			{
			RECT rcRect;					//	Rect of status section
			};

		struct SProgressDesc
			{
			CString sLabel;
			int cxLevel;
			RECT rcRect;
			};

		void BufferCharacterImage (void);
		void BufferGlobalSection (void);
		void BufferMessageSection (void);
		void BufferProgressSection (void);
		void BufferStatusSection (void);
		void ExecuteCommand (const CString &sID, ICCItem *pData = NULL);
		bool HitTestCommand (int x, int y, int *retiCommand = NULL);
		void PaintBuffer (void);
		void PaintCommands (SAniPaintCtx &Ctx);
		void PaintFrame (void);
		void PaintGlobalCrewLevel (CG32bitImage &Dest, int x, int y, int iValue, const CString &sLabel);
		void PaintGlobalHeader (CG32bitImage &Dest, const RECT &rcRect, int iValue, const CString &sLabel);
		void PaintGlobalSection (SAniPaintCtx &Ctx);
		void PaintMessage (SAniPaintCtx &Ctx);
		void PaintMiniStatusLevel (CG32bitImage &Dest, int x, int y, int iLevel, CG32bitPixel rgbColor);
		void PaintProgress (SAniPaintCtx &Ctx);
		void PaintProgressRow (CG32bitImage &Dest, const SProgressDesc &Progress);
		void PaintStatus (SAniPaintCtx &Ctx);
		void RealizeCommandGroups (void);
		void RealizeCommands (int xPos, int yPos);
		void RealizeGlobalSection (int xPos, int yPos);
		void RealizeProgressSection (int xPos, int yPos);
		void RealizeSelection (void);
		void RealizeStatus (int xPos, int yPos);
		void UpdateGlobalSection (void);
		void UpdateProgress (void);

		const CVisualPalette &m_VI;
		const SMetrics m_Metrics;
		CTransmuterModel &m_Model;

		TArray<SMiniStatusDesc> m_MiniStatus;
		RECT m_rcStatusSection;

		TArray<SProgressDesc> m_Progress;
		RECT m_rcProgressSection;

		int m_iCommandPoints;
		CItemList m_Resources;
		RECT m_rcResourceSection;

		RECT m_rcCrewSection;

		CG32bitPixel m_rgbLowLevelColor;
		CG32bitPixel m_rgbNeutralLevelColor;
		CG32bitPixel m_rgbHighLevelColor;
		CG32bitPixel m_rgbPeakLevelColor;

		CG32bitImage m_Buffer;
		int m_xPane;
		int m_yPane;
		int m_cxPane;
		int m_cyPane;
		bool m_bInvalid;

		int m_iHoverCommand;
		bool m_bLButtonDown;
	};

class CDialogPane : public IAnimatron
	{
	public:
		struct SMetrics
			{
			//	Metrics

			int cxPane;
			int iCornerCut;
			int iFrameWidth;
			int iFramePadding;
			int cyCharacterImage;
			int iCommandSize;

			//	Colors

			CG32bitPixel rgbPaneBackground;
			CG32bitPixel rgbFrame;
			CG32bitPixel rgbSectionBackground;
			CG32bitPixel rgbTitleText;
			CG32bitPixel rgbDescriptionText;
			};

		CDialogPane (CHumanInterface &HI, const SMetrics &Metrics, CTransmuterModel &Model);
		~CDialogPane (void);

		void SetDescriptor (CDialogDesc *pDesc);

		//	IAnimatron virtuals

		virtual void GetSpacingRect (RECT *retrcRect);
		virtual void HandleLButtonDown (int x, int y, DWORD dwFlags, bool *retbCapture, bool *retbFocus);
		virtual void HandleLButtonUp (int x, int y, DWORD dwFlags);
		virtual void HandleMouseLeave (void);
		virtual void HandleMouseMove (int x, int y, DWORD dwFlags);
		virtual IAnimatron *HitTest (const CXForm &ToDest, int x, int y);
		virtual void Paint (SAniPaintCtx &Ctx);

	private:
		struct SCommandDesc
			{
			RECT rcRect;
			};

		bool HitTestCommand (int x, int y, int *retiCommand);
		void PaintCommands (SAniPaintCtx &Ctx);
		void Realize (void);
		void RenderBackground (void);
		void RenderDescription (void);
		void RenderTopImage (void);

		CHumanInterface &m_HI;
		const CVisualPalette &m_VI;
		const SMetrics m_Metrics;
		CTransmuterModel &m_Model;

		CDialogDesc *m_pDesc;

		int m_xPane;
		int m_yPane;
		int m_cxPane;
		int m_cyPane;

		CG32bitImage m_Background;
		RECT m_rcTopImage;
		CG32bitImage m_TopImageMask;

		RECT m_rcDescription;
		TArray<CString> m_DescLines;

		TArray<SCommandDesc> m_Commands;
		int m_iHoverCommand;
		bool m_bLButtonDown;

		bool m_bInvalid;
	};