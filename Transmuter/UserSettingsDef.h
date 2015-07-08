//	UserSettingsDef.h
//
//	User settings definition.
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

CUserSettings::SOptionDef g_UserSettings[] =
	{
		//	Installation options

		{	"useTDB",					CUserSettings::optionBoolean,	"false",	0	},
		{	"noAutoUpdate",				CUserSettings::optionBoolean,	"false",	0	},

		//	Video options
		{	"forceDirectX",				CUserSettings::optionBoolean,	"false",	0	},
		{	"forceNonDirectX",			CUserSettings::optionBoolean,	"false",	0	},
		{	"forceExclusive",			CUserSettings::optionBoolean,	"false",	0	},
		{	"forceNonExclusive",		CUserSettings::optionBoolean,	"false",	0	},
		{	"windowedMode",				CUserSettings::optionBoolean,	"false",	0	},
		{	"useBackgroundBlt",			CUserSettings::optionBoolean,	"true",		0	},
		{	"force1024",				CUserSettings::optionBoolean,	"false",	0	},
		{	"force600",					CUserSettings::optionBoolean,	"false",	0	},

		//	Sounds options
		{	"noSound",					CUserSettings::optionBoolean,	"false",	0	},
		{	"noMusic",					CUserSettings::optionBoolean,	"false",	0	},
		{	"soundVolume",				CUserSettings::optionInteger,	"7",		0	},
		{	"musicVolume",				CUserSettings::optionInteger,	"7",		0	},
		{	"musicPath",				CUserSettings::optionString,	"",			0	},

		//	Debug options
		{	"debugMode",				CUserSettings::optionBoolean,	"false",	0	},
		{	"debugGame",				CUserSettings::optionBoolean,	"false",	0	},
		{	"noDebugLog",				CUserSettings::optionBoolean,	"false",	0	},
		{	"debugVideo",				CUserSettings::optionBoolean,	"false",	0	},
		{	"noCrashPost",				CUserSettings::optionBoolean,	"false",	0	},
		{	"noCollectionDownload",		CUserSettings::optionBoolean,	"false",	0	},
		{	"debugSoundtrack",			CUserSettings::optionBoolean,	"false",	0	},

		{	NULL,						CUserSettings::optionBoolean,	NULL,	0 }
	};
