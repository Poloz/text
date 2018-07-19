// 2013-02.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "accmd.h" //acedRegCmds
#include "rxregsvc.h" //acrxUnlockApplication
#include "SelectBlocks.h"

void runIt()
{
	selectBlocks();
}

void initApp()
{
    acedRegCmds->addCommand(ACRX_T("KZFT_CREATE_SPEC"),
                            ACRX_T("KZFT_CSPEC"),
                            ACRX_T("CSPEC"),
                            ACRX_CMD_MODAL,
                            runIt);
}

void unloadApp()
{
	acedRegCmds->removeGroup(ACRX_T("KZFT_CREATE_SPEC"));
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
    switch(msg) {
    case AcRx::kInitAppMsg:
        // Allow application to be unloaded
        acrxUnlockApplication(appId);
        // Register application as MDI aware. 
        acrxRegisterAppMDIAware(appId);
		initApp();
    break;
    case AcRx::kUnloadAppMsg:
		unloadApp();
    break;
    }
    return AcRx::kRetOK;
}