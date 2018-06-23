// 2013-02.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "rxregsvc.h"
#include "acutads.h"
#include "dbid.h"
#include "dbsymtb.h"
#include "dbapserv.h"
#include "dbents.h"
#include <dbgroup.h>
#include <dbapserv.h>
#include <dbsymtb.h>
#include <aced.h>
#include "adscodes.h" //RTNORM
#include "acedads.h"


void selectBlocks()
{
	ads_name ssname;
	struct resbuf filter;
	ACHAR sbuf[15] = ACRX_T("Arm_zone_v001");
	filter.restype = 0;	// Entity name
	filter.resval.rstring = sbuf;
	filter.rbnext = NULL; // No other properties
	// Get the current PICKFIRST or ask user for a selection
	acutPrintf(ACRX_T("\nPlease select Arm_zone_v001 blocks."));
	acedSSGet(NULL, NULL, NULL, NULL, ssname);
	//acedSSGet(ACRX_T("X"), NULL, NULL, &filter, ssname); //
	long length = 0;
	if (acedSSLength(ssname, &length) != RTNORM)
	{
		acutPrintf(ACRX_T("\nLeaving..."));
		acedSSFree(ssname);
		return;
	}
	ads_name ent;
	long counter = 0;
	AcDbObjectId obId = AcDbObjectId::kNull;
	for (long i = 0; i < length; i++)
	{
		if (acedSSName(ssname, i, ent) != RTNORM) continue;
		if (acdbGetObjectId(obId, ent) != Acad::eOk) continue;
		AcDbEntity* pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, obId, AcDb::kForWrite) != Acad::eOk) continue;
		counter++;
	}
	acutPrintf(ACRX_T("The counter is:%n.", counter));
}

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
	acedRegCmds->removeGroup(ACRX_T("ASDK_MAKE_ENTS"));
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
    switch(msg) {
    case AcRx::kInitAppMsg:
        // Allow application to be unloaded
        // Without this statement, AutoCAD will
        // not allow the application to be unloaded
        // except on AutoCAD exit.
        //
        acrxUnlockApplication(appId);
        // Register application as MDI aware. 
        // Without this statement, AutoCAD will
        // switch to SDI mode when loading the
        // application.
        //
        acrxRegisterAppMDIAware(appId);
        acutPrintf(ACRX_T("\nExample Application Loaded"));
		initApp();
    break;
    case AcRx::kUnloadAppMsg:
		unloadApp();
        //acutPrintf(ACRX_T("\nExample Application Unloaded"));
    break;
    }
    return AcRx::kRetOK;
}