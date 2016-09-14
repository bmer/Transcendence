//	CGSubjugateArea.cpp
//
//	CGSubjugateArea class
//	Copyright (c) 2016 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"
#include "Transcendence.h"

#define STR_DEPLOY							CONSTLIT("Deploy")

const int COUNTERMEASURES_COUNT =			6;
const int COUNTERMEASURES_INNER_RADIUS =	94;
const int COUNTERMEASURES_ARC_ANGLE =		(360 / COUNTERMEASURES_COUNT);
const int COUNTERMEASURE_SPACING =			12;
const int COUNTERMEASURE_WIDTH =			80;

const int DAIMON_BORDER_RADIUS =			6;
const int DAIMON_COUNT =					6;
const int DAIMON_HEIGHT =					96;
const int DAIMON_WIDTH =					96;
const int DAIMON_SPACING_X =				12;
const int DAIMON_SPACING_Y =				12;

const int DMZ_WIDTH =						40;				//	Distance from outer edge of countermeasures to daimons

const int INFO_PANE_WIDTH =					200;
const DWORD INFO_PANE_HOVER_TIME =			300;

const int MOUSE_SCROLL_SENSITIVITY =		240;

const int DEPLOY_BUTTON_RADIUS =			40;

CGSubjugateArea::CGSubjugateArea (const CVisualPalette &VI, CDockScreenSubjugate &Controller) : 
		m_VI(VI),
		m_Controller(Controller),
		m_iState(stateStart),
		m_iEgo(1),
		m_iIntelligence(1),
		m_iWillpower(1),
		m_InfoPane(VI),
		m_DaimonListPainter(VI),
		m_DeployBtn(VI)

//	CGSubjugateArea constructor

	{
	int i;

	//	Initialize the countermeasure loci (these form a ring around the central
	//	core).

	int iAngle = 0;
	m_CountermeasureLoci.InsertEmpty(COUNTERMEASURES_COUNT);
	for (i = 0; i < COUNTERMEASURES_COUNT; i++)
		{
		SCountermeasureLocus &Locus = m_CountermeasureLoci[i];

		Locus.iStartAngle = iAngle;
		Locus.iArc = COUNTERMEASURES_ARC_ANGLE;
		Locus.iInnerRadius = COUNTERMEASURES_INNER_RADIUS;
		Locus.iOuterRadius = COUNTERMEASURES_INNER_RADIUS + COUNTERMEASURE_WIDTH;

		iAngle += COUNTERMEASURES_ARC_ANGLE;
		}

	//	Compute the x-offset of the upper and lower daimon rows so that they 
	//	follow the curve of the central core.

	Metric rCentralRowDist = COUNTERMEASURES_INNER_RADIUS + COUNTERMEASURE_WIDTH + DMZ_WIDTH;
	Metric rRowHeight = DAIMON_HEIGHT + DAIMON_SPACING_Y;
	Metric rTopRowDist = sqrt(rCentralRowDist * rCentralRowDist - rRowHeight * rRowHeight);

	int xCentralRow = -(int)rCentralRowDist - DAIMON_WIDTH;
	int xTopRow = -(int)rTopRowDist - DAIMON_WIDTH;

	//	Initialize the daimon loci.

	m_DaimonLoci.InsertEmpty(DAIMON_COUNT);
	m_DaimonLoci[0].xPos = xCentralRow;
	m_DaimonLoci[0].yPos = -(DAIMON_HEIGHT / 2);
	m_DaimonLoci[0].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[0].cyHeight = DAIMON_HEIGHT;

	m_DaimonLoci[1].xPos = xTopRow;
	m_DaimonLoci[1].yPos = -(DAIMON_HEIGHT / 2) - (DAIMON_HEIGHT + DAIMON_SPACING_Y);
	m_DaimonLoci[1].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[1].cyHeight = DAIMON_HEIGHT;

	m_DaimonLoci[2].xPos = xTopRow;
	m_DaimonLoci[2].yPos = (DAIMON_HEIGHT / 2) + DAIMON_SPACING_Y;
	m_DaimonLoci[2].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[2].cyHeight = DAIMON_HEIGHT;

	m_DaimonLoci[3].xPos = xCentralRow - (DAIMON_WIDTH + DAIMON_SPACING_X);
	m_DaimonLoci[3].yPos = -(DAIMON_HEIGHT / 2);
	m_DaimonLoci[3].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[3].cyHeight = DAIMON_HEIGHT;

	m_DaimonLoci[4].xPos = xTopRow - (DAIMON_WIDTH + DAIMON_SPACING_X);
	m_DaimonLoci[4].yPos = -(DAIMON_HEIGHT / 2) - (DAIMON_HEIGHT + DAIMON_SPACING_Y);
	m_DaimonLoci[4].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[4].cyHeight = DAIMON_HEIGHT;

	m_DaimonLoci[5].xPos = xTopRow - (DAIMON_WIDTH + DAIMON_SPACING_X);
	m_DaimonLoci[5].yPos = (DAIMON_HEIGHT / 2) + DAIMON_SPACING_Y;
	m_DaimonLoci[5].cxWidth = DAIMON_WIDTH;
	m_DaimonLoci[5].cyHeight = DAIMON_HEIGHT;

	//	Compute some colors

	m_rgbCountermeasureBack = CG32bitPixel(255, 127, 127, 66);		//	H:0   S:50 B:100
	m_rgbDaimonBack = CG32bitPixel(229, 161, 229, 66);				//	H:300 S:30 B:90
	}

CGSubjugateArea::~CGSubjugateArea (void)

//	CGSubjugateArea destructor

	{
	}


void CGSubjugateArea::AddCountermeasure (CItemType *pItem)

//	AddCountermeasure
//
//	Adds a countermeasure for the artifact to deploy

	{
	SCountermeasureEntry *pEntry = m_CountermeasureList.Insert();
	pEntry->pCountermeasure = pItem;
	}

void CGSubjugateArea::AddDaimon (CItemType *pItem)

//	AddDaimon
//
//	Adds a daimon for the player to deploy

	{
	m_DaimonList.Add(pItem);
	}

void CGSubjugateArea::ArtifactSubdued (void)

//	ArtifactSubdued
//
//	This is called when the artifact is successfully subjugated

	{
	m_iState = stateSuccess;
	m_Controller.OnCompleted(true);
	}

void CGSubjugateArea::Command (ECommands iCommand, void *pData)

//	Command
//
//	Execute a command

	{
	switch (iCommand)
		{
		case cmdSelectNextDaimon:
			SelectDaimon(m_DaimonList.GetSelection() + 1);
			break;

		case cmdSelectPrevDaimon:
			SelectDaimon(m_DaimonList.GetSelection() - 1);
			break;
		}
	}

void CGSubjugateArea::DeployDaimon (void)

//	DeployDaimon
//
//	Deploy the currently selected daimon.

	{
	ArtifactSubdued();
	}

void CGSubjugateArea::HideInfoPane (void)

//	HideInfoPane
//
//	Hides the info pane if it is showing

	{
	if (m_InfoPaneSel.iType != selectNone)
		{
		m_InfoPane.Hide();
		m_InfoPaneSel = SSelection();
		Invalidate();
		}
	}

bool CGSubjugateArea::HitTest (int x, int y, SSelection &Sel) const

//	HitTest
//
//	Returns TRUE if the point is over an active region.
//	NOTE: x,y are paint coordinates (i.e., screen relative).

	{
	//	Only works if we're active

	if (!IsActive())
		{
		Sel = SSelection();
		return false;
		}

	//	Check the list of daimons (NOTE: We could return iIndex of -1 if we
	//	are over the list area, but not on an actual item).

	else if (m_DaimonListPainter.HitTest(x, y, &Sel.iIndex))
		Sel.iType = selectDaimonList;

	//	Check deploy button

	else if (m_DeployBtn.HitTest(x, y))
		Sel.iType = selectDeployBtn;

	//	Check countermeasures

	else if (HitTestCountermeasureLoci(x, y, &Sel.iIndex))
		Sel.iType = selectCountermeasureLoci;

	//	Otherwise, nothing

	else
		{
		Sel = SSelection();
		return false;
		}

	//	Success!

	return true;
	}

bool CGSubjugateArea::HitTestCountermeasureLoci (int x, int y, int *retiIndex) const

//	HitTestCountermeasureLoci
//
//	Returns TRUE if the point is over a countermeasure loci.

	{
	int i;

	//	Convert mouse position to radial coordinates relative to the center of
	//	the core.

	Metric rRadius;
	int iAngle = VectorToPolar(CVector(x - m_xCenter, m_yCenter - y), &rRadius);
	int iRadius = (int)rRadius;

	//	See if we're inside any loci

	for (i = 0; i < m_CountermeasureLoci.GetCount(); i++)
		{
		const SCountermeasureLocus &Locus = m_CountermeasureLoci[i];

		if (iAngle >= Locus.iStartAngle && iAngle < (Locus.iStartAngle + Locus.iArc)
				&& iRadius >= Locus.iInnerRadius && iRadius < Locus.iOuterRadius)
			{
			if (retiIndex)
				*retiIndex = i;

			return true;
			}
		}

	return false;
	}

bool CGSubjugateArea::IsCommandValid (ECommands iCommand, void *pData) const

//	IsCommandValid
//
//	Returns TRUE if the command can be executed right now.

	{
	switch (iCommand)
		{
		case cmdSelectNextDaimon:
			return (m_DaimonList.GetSelection() < m_DaimonList.GetCount() - 1);

		case cmdSelectPrevDaimon:
			return (m_DaimonList.GetSelection() > 0);

		default:
			return false;
		}
	}

bool CGSubjugateArea::LButtonDoubleClick (int x, int y)

//	LButtonDoubleClick
//
//	Handle mouse

	{
	return LButtonDown(x, y);
	}

bool CGSubjugateArea::LButtonDown (int x, int y)

//	LButtonDown
//
//	Handle mouse

	{
	//	We store everything in paint coordinates, so we need to convert

	GetParent()->ConvertToPaintCoords(x, y);

	//	See what we clicked on.

	if (!HitTest(x, y, m_Clicked))
		return false;

	//	Handle the click (if necessary)

	switch (m_Clicked.iType)
		{
		case selectDaimonList:
			if (m_Clicked.iIndex != -1)
				SelectDaimon(m_Clicked.iIndex);
			break;
		}

	return true;
	}

void CGSubjugateArea::LButtonUp (int x, int y)

//	LButtonUp
//
//	Handle mouse

	{
	//	We store everything in paint coordinates, so we need to convert

	GetParent()->ConvertToPaintCoords(x, y);

	//	Remember where we clicked, but undo our internal state in case we
	//	need to return early.

	SSelection Clicked = m_Clicked;
	m_Clicked = SSelection();

	//	See where we are now

	SSelection Released;
	if (!HitTest(x, y, Released))
		return;

	//	Handle it if we released at an active point

	switch (Released.iType)
		{
		case selectDeployBtn:
			//	If we clicked down on the button and released over it, then we
			//	do it.

			if (Clicked.iType == selectDeployBtn)
				DeployDaimon();
			break;
		}
	}

void CGSubjugateArea::MouseEnter (void)

//	MouseEnter
//
//	Mouse has entered area

	{
	}

void CGSubjugateArea::MouseLeave (void)

//	MouseLeave
//
//	Mouse has left area

	{
	m_Hover = SSelection();
	HideInfoPane();
	}

void CGSubjugateArea::MouseMove (int x, int y)

//	MoveMose
//
//	Mouse has moved within the area.

	{
	//	We store everything in paint coordinates, so we need to convert

	GetParent()->ConvertToPaintCoords(x, y);

	//	Hit test (store the result in m_Hover).

	HitTest(x, y, m_Hover);

	//	Close the info pane, if necessary

	if (m_Hover != m_InfoPaneSel)
		HideInfoPane();
	}

void CGSubjugateArea::MouseWheel (int iDelta, int x, int y, DWORD dwFlags)

//	MouseWheel
//
//	Handle mouse scrolling

	{
	//	We store everything in paint coordinates, so we need to convert

	GetParent()->ConvertToPaintCoords(x, y);

	//	Hit test (store the result in m_Hover).

	SSelection Select;
	if (!HitTest(x, y, Select))
		return;

	//	Handle it

	switch (Select.iType)
		{
		case selectDaimonList:
			{
			int iChange = (-iDelta / MOUSE_SCROLL_SENSITIVITY);
			if (iChange == 0)
				iChange = -Sign(iDelta / (MOUSE_SCROLL_SENSITIVITY / 8));

			SelectDaimon(m_DaimonList.GetSelection() + iChange);
			break;
			}
		}
	}

void CGSubjugateArea::OnSetRect (void)

//	OnSetRect
//
//	The rectangle has been set, so we can compute some metrics

	{
	RECT rcRect = GetParent()->GetPaintRect(GetRect());

	//	We split the rect in thirds

	int cxThird = RectWidth(rcRect) / 3;
	int xCol1 = rcRect.left;
	int xCol2 = xCol1 + cxThird;
	int xCol3 = rcRect.right - cxThird;

	//	Center line (vertically)

	int yRectCenter = rcRect.top + (RectHeight(rcRect) / 2);

	//	The core is at the center of the 3rd column

	m_xCenter = xCol3 + (cxThird / 2);
	m_yCenter = yRectCenter;

	//	The list of daimons in hand is in the first column

	m_rcHand.left = xCol1;
	m_rcHand.top = rcRect.top;
	m_rcHand.right = xCol1 + cxThird;
	m_rcHand.bottom = rcRect.bottom;

	m_DaimonListPainter.SetList(m_DaimonList);
	m_DaimonListPainter.SetRect(m_rcHand);

	//	Position the deploy button at the center of the boundary between 
	//	columns 1 and 2.

	m_DeployBtn.SetPos(xCol2 + DEPLOY_BUTTON_RADIUS, yRectCenter);
	m_DeployBtn.SetLabel(STR_DEPLOY);
	}

void CGSubjugateArea::Paint (CG32bitImage &Dest, const RECT &rcRect)

//	Paint
//
//	Paint

	{
	int i;

//	Dest.Fill(rcRect.left, rcRect.top, RectWidth(rcRect), RectHeight(rcRect), CG32bitPixel(128, 128, 255, 0x40));

	//	Paint the central core animation

	m_AICorePainter.Paint(Dest, m_xCenter, m_yCenter);

	//	Paint the deployed countermeasures

	for (i = 0; i < m_CountermeasureLoci.GetCount(); i++)
		PaintCountermeasureLocus(Dest, m_CountermeasureLoci[i]);

	//	Paint the deployed daimons

	for (i = 0; i < m_DaimonLoci.GetCount(); i++)
		PaintDaimonLocus(Dest, m_DaimonLoci[i]);

	//	Paint the available daimons

	m_DaimonListPainter.Paint(Dest);

	//	Paint the deploy button

	CDaimonButtonPainter::EStates iDeployBtnState;
	if (!IsActive())
		iDeployBtnState = CDaimonButtonPainter::stateDisabled;
	else if (g_pHI->IsLButtonDown() && m_Hover.iType == selectDeployBtn && m_Clicked.iType == selectDeployBtn)
		iDeployBtnState = CDaimonButtonPainter::stateDown;
	else if (!g_pHI->IsLButtonDown() && m_Hover.iType == selectDeployBtn)
		iDeployBtnState = CDaimonButtonPainter::stateHover;
	else
		iDeployBtnState = CDaimonButtonPainter::stateNormal;

	m_DeployBtn.Paint(Dest, iDeployBtnState);

	//	Paint the info pane on top of everything

	m_InfoPane.Paint(Dest);
	}

void CGSubjugateArea::PaintCountermeasureLocus (CG32bitImage &Dest, const SCountermeasureLocus &Locus) const

//	PaintCountermeasureLocus
//
//	Paints a deployed countermeasure.

	{
	CGDraw::Arc(Dest,
			m_xCenter,
			m_yCenter,
			Locus.iInnerRadius,
			Locus.iStartAngle,
			Locus.iStartAngle + Locus.iArc,
			COUNTERMEASURE_WIDTH,
			m_rgbCountermeasureBack,
			CGDraw::blendNormal,
			COUNTERMEASURE_SPACING / 2,
			CGDraw::ARC_INNER_RADIUS);
	}

void CGSubjugateArea::PaintDaimonLocus (CG32bitImage &Dest, const SDaimonLocus &Locus) const

//	PaintDaimonLocus
//
//	Paints a deployed daimon

	{
	CGDraw::RoundedRect(Dest,
			m_xCenter + Locus.xPos,
			m_yCenter + Locus.yPos,
			Locus.cxWidth,
			Locus.cyHeight,
			DAIMON_BORDER_RADIUS,
			m_rgbDaimonBack);
	}

void CGSubjugateArea::SelectDaimon (int iNewSelection)

//	SelectDaimon
//
//	Selects the given daimon

	{
	int iOldSelection = m_DaimonList.GetSelection();
	iNewSelection = m_DaimonList.SetSelection(iNewSelection);

	if (iOldSelection != iNewSelection)
		{
		g_pUniverse->PlaySound(NULL, g_pUniverse->FindSound(UNID_DEFAULT_SELECT));
		m_DaimonListPainter.OnSelectionChanged(iOldSelection, iNewSelection);
		Invalidate();
		}
	}

void CGSubjugateArea::Update (void)

//	Update
//
//	Update

	{
	//	Update all our components

	if (m_DaimonListPainter.Update())
		Invalidate();

	m_DeployBtn.Update();

	//	See if we need to show the info pane

	if (GetScreen()->GetTimeSinceMouseMove() >= INFO_PANE_HOVER_TIME
			&& m_InfoPaneSel != m_Hover)
		{
		POINT pt;
		GetScreen()->GetMousePos(&pt);
		int x = pt.x;
		int y = pt.y;
		GetParent()->ConvertToPaintCoords(x, y);

		switch (m_Hover.iType)
			{
			case selectCountermeasureLoci:
				m_InfoPane.SetTitle(strPatternSubst("Countermeasure %d", m_Hover.iIndex + 1));
				m_InfoPane.SetDescription(CONSTLIT("This is a countermeasure that counters your measure."));
				m_InfoPane.Show(x, y, INFO_PANE_WIDTH, GetPaintRect());
				m_InfoPaneSel = m_Hover;
				Invalidate();
				break;
			}
		}
	}