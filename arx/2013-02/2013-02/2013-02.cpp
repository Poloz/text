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
#include "dbobjptr.h"
#include "dbdynblk.h"
#include <atlstr.h> 

void selectBlocks()
{
	ads_name ssname;
	struct resbuf blockFilter;//, insertFilter;

	ACHAR insertName[15] = ACRX_T("Arm_zone_v001");
	//ACHAR insertName[15] = ACRX_T("TestBlock01");
	
	//insertFilter.restype = 2;	// Entity name
	//insertFilter.resval.rstring = insertName;
	//insertFilter.rbnext = NULL; // No other properties

	blockFilter.restype = 0; // Entity type
	blockFilter.resval.rstring = ACRX_T("INSERT");
	blockFilter.rbnext = NULL; //&insertFilter;

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
	
	ads_name ent;
	long counter = 0;
	AcDbObjectId obId = AcDbObjectId::kNull;
	for (long i = 0; i < length; i++)
	{
		if (acedSSName(ssname, i, ent) != RTNORM) {acutPrintf(ACRX_T("\nCannot get entity from selection.\n")); continue;};
		if (acdbGetObjectId(obId, ent) != Acad::eOk) {acutPrintf(ACRX_T("\nCannot get ObjectId from entity.\n")); continue;};
		//AcDbEntity* pEnt = NULL;
		//if (acdbOpenAcDbEntity(pEnt, obId, AcDb::kForRead) != Acad::eOk) {acutPrintf(ACRX_T("\nCannot open for read.\n")); continue;};
		AcDbObjectPointer<AcDbBlockReference> pBlkRef (obId, AcDb::kForRead);
		if (pBlkRef.openStatus() != Acad::eOk) {acutPrintf(ACRX_T("\nCannot open for read.\n")); continue;};
		//AcDbObjectId idBlkTblRec = pBlkRef->blockTableRecord();
		AcDbDynBlockReference dynBlk(obId);
		if (dynBlk.isDynamicBlock()) 
			{
				AcDbBlockTableRecordPointer pBTR(dynBlk.dynamicBlockTableRecord(),AcDb::kForRead);
				if (pBTR.openStatus() == Acad::eOk)
				{
					const ACHAR *blkName = NULL; pBTR->getName(blkName);
					acutPrintf(ACRX_T("\nDynamic block with name: \"%s\""), blkName);
				} else {continue;}
				AcDbDynBlockReferencePropertyArray blkPropAry;
				dynBlk.getBlockProperties(blkPropAry);
				Acad::ErrorStatus err;
				AcDbDynBlockReferenceProperty blkProp;
				for(long lIndex=0L ; lIndex<blkPropAry.length() ; ++lIndex){
					blkProp = blkPropAry[lIndex];
					if (wcscmp(blkProp.propertyName().kACharPtr(), L"ÄËÈÍÀ") == 0){
						acutPrintf(ACRX_T("\n%s : "),blkProp.propertyName());
					    AcDbEvalVariant currentValue = blkProp.value();
					    //acutPrintf(ACRX_T("\n Restype %d"),currentValue.restype);
					    acutPrintf(ACRX_T("%g\n"),currentValue.resval.rreal);
					}
				}
				//http://arxdummies.blogspot.com/2005/03/class-6-entities.html
				// First we will look into BlockReference entity looking for
				// all non-constant attributes which are stored into block insertions
				AcDbObjectIterator *pIter = pBlkRef->attributeIterator() ;
				//acutPrintf("\nBlock's Non-constant Attributes:");
				while ( !pIter->done () ) {
					AcDbEntity *pAttEnt = NULL;
					AcDbObjectId idAtt = pIter->objectId();
					if (acdbOpenObject(pAttEnt,idAtt,AcDb::kForRead) == Acad::eOk) {
						if (pAttEnt->isKindOf(AcDbAttribute::desc())) {
							// Here you can read Attribute information
							AcDbAttribute* pAtt = AcDbAttribute::cast(pAttEnt);
							//CString msg;
							//msg.Format("\nTAG:%s - VALUE:%s",pAtt->tag(),pAtt->textString());
							//acutPrintf(msg);
							if (wcscmp(pAtt->tag(), L"ÄÈÀÌ") == 0) {
								acutPrintf(ACRX_T("\nTAG:%s - VALUE:%s"),pAtt->tag(),pAtt->textString());
							}
						}
						pAttEnt->close();
					}
					pIter->step();
				}
				delete pIter ;				
			}
		counter++;
	}
	acutPrintf(ACRX_T("\nThe counter is %d."), counter);
	acedSSFree(ssname);
}


void runIt()
{
	selectBlocks();
	//PrintBlockName();
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