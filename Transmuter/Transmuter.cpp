//	Transmuter.cpp
//
//	Transmuter
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

int WINAPI WinMain (HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine, 
                    int nCmdShow)

//	WinMain
//
//	Main Windows entry-point

	{
	//	Initialize the kernel

	if (!::kernelInit(KERNEL_FLAG_INTERNETS))
		{
		::MessageBox(NULL, "Unable to initialize Alchemy.", "Human Interface", MB_OK);
		return 0;
		}

	//	Create the controller

	IHIController *pController = new CTransmuterController;

	//	Run

	CHumanInterface::Run(pController, hInstance, nCmdShow, lpCmdLine);

	//	Done

	kernelCleanUp();

	return 0;
	}
