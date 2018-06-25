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

	ACHAR insertName[15] = ACRX_T("Arm_zone_v001");
	//ACHAR insertName[15] = ACRX_T("TestBlock01");
	
	insertFilter.restype = 2;	// Entity name
	insertFilter.resval.rstring = insertName;
	insertFilter.rbnext = NULL; // No other properties

	blockFilter.restype = 0; // Entity type
	blockFilter.resval.rstring = ACRX_T("INSERT");
	blockFilter.rbnext = &insertFilter;

	acutPrintf(ACRX_T("\nPlease select %s blocks."), insertName);
	
	if (acedSSGet(NULL, NULL, NULL, &blockFilter, ssname) != RTNORM )
	{
		acutPrintf(ACRX_T("\nSomething's wrong.\n"));
		acedSSFree(ssname);
		return;
	}
	
	long length = 0;
	
	if (acedSSLength(ssname, &length) != RTNORM)
	{
		acutPrintf(ACRX_T("\nNo blocks selectes\n"));
		acedSSFree(ssname);
		return;
	}
	
	//-----------
	ads_name ent;
	long counter = 0;
	AcDbObjectId obId = AcDbObjectId::kNull;
	for (long i = 0; i < length; i++)
	{
		if (acedSSName(ssname, i, ent) != RTNORM) continue;
		if (acdbGetObjectId(obId, ent) != Acad::eOk) continue;
		//AcDbEntity* pEnt = NULL;
		//if (acdbOpenAcDbEntity(pEnt, obId, AcDb::kForWrite) != Acad::eOk) continue;
		counter++;
	}
	acutPrintf(ACRX_T("\nThe counter is %d."), counter);
	//---------
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