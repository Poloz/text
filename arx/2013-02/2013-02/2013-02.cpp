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
	ads_name ssname; //selection set name
	struct resbuf entityFilter, blockNameFilter; //mask for the selection
	ACHAR entityName[10] = ACRX_T("INSERT");
	ACHAR blockName[15] = ACRX_T("Arm_zone_v001");

	blockNameFilter.restype = 2; //blockname
	blockNameFilter.resval.rstring = blockName;
	blockNameFilter.rbnext = NULL; //no next properties

	entityFilter.restype = 0; //entity name
	entityFilter.resval.rstring = entityName;
	entityFilter.rbnext = &blockNameFilter; //next property
		
	// Get the current PICKFIRST or ask user for a selection
	acutPrintf(ACRX_T("\nPlease select %s blocks."), blockName);
	//acedSSGet(NULL, NULL, NULL, NULL, ssname); //getting current selection set, if null - asking user
	if (acedSSGet(NULL, NULL, NULL, &entityFilter, ssname) != RTNORM)
	{
		acutPrintf(ACRX_T("\nNo selection.\n"));
		//acedSSFree(ssname); //freeing selection set
		//acutRelRb(&entityFilter);
		//acutRelRb(&blockNameFilter);
		return;
	}

	/**
	ads_name ent;
	int counter = 0;
	AcDbObjectId obId = AcDbObjectId::kNull;
	for (long i = 0; i < length; i++)
	{
		if (acedSSName(ssname, i, ent) != RTNORM) continue;
		if (acdbGetObjectId(obId, ent) != Acad::eOk) continue;
		AcDbEntity* pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, obId, AcDb::kForWrite) != Acad::eOk) continue;
		counter++;
	}
	acutPrintf(ACRX_T("The counter is:%d.", counter));
	**/
	acutPrintf(ACRX_T("\nSome blocks were selected.\n"));
	acedSSFree(ssname);
	acutRelRb(&entityFilter);
	acutRelRb(&blockNameFilter);
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
        //acutPrintf(ACRX_T("\nArm_Spec v0.1 loaded\n"));
		initApp();
    break;
    case AcRx::kUnloadAppMsg:
		unloadApp();
    break;
    }
    return AcRx::kRetOK;
}