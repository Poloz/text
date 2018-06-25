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
	struct resbuf blockFilter, insertFilter;

	//ACHAR insertName[15] = ACRX_T("Arm_zone_v001");
	ACHAR insertName[15] = ACRX_T("TestBlock01");
	ACHAR entityname[15] = ACRX_T("INSERT");
	
	insertFilter.restype = 0;	// Entity name
	insertFilter.resval.rstring = insertName;
	insertFilter.rbnext = NULL; // No other properties

	blockFilter.restype = 0;
	blockFilter.resval.rstring = insertName;
	blockFilter.rbnext = &insertFilter;
	
	// Get the current PICKFIRST or ask user for a selection
	acutPrintf(ACRX_T("\nPlease select %s blocks."), insertName);
	//acedSSGet(NULL, NULL, NULL, NULL, ssname);
	
	acedSSGet(ACRX_T("X"), NULL, NULL, &blockFilter, ssname); //
	
	long length = 0;
	if (acedSSLength(ssname, &length) != RTNORM)
	{
		acutPrintf(ACRX_T("\nLeaving..."));
		acedSSFree(ssname);
		return;
	}
	/**
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
	**/
	acutPrintf(ACRX_T("\nLooks OK..."));
	acedSSFree(ssname);
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