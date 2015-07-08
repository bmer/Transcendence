//	BackgroundTasks.h
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CInitModelTask : public IHITask
	{
	public:
		CInitModelTask (CHumanInterface &HI, 
						CTransmuterModel &Model) : IHITask(HI), 
				m_Model(Model)
			{ }

		//	IHITask virtuals

		virtual ALERROR OnExecute (ITaskProcessor *pProcessor, CString *retsResult) { if (!m_Model.InitBackground(retsResult)) return ERR_FAIL; return NOERROR; }

	private:
		CTransmuterModel &m_Model;
	};