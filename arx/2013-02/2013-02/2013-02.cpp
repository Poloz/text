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

	const ACHAR *prefix = ACRX_T("Arm_");

	blockFilter.restype = 0; // Entity type
	blockFilter.resval.rstring = ACRX_T("INSERT");
	blockFilter.rbnext = NULL;
	acutPrintf(ACRX_T("\nPlease select blocks."));
	
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
		AcDbObjectPointer<AcDbBlockReference> pBlkRef (obId, AcDb::kForRead);
		if (pBlkRef.openStatus() != Acad::eOk) {acutPrintf(ACRX_T("\nCannot open for read.\n")); delete pBlkRef; continue;};
		AcDbDynBlockReference dynBlk(obId);
		if (dynBlk.isDynamicBlock()) 
			{
				AcDbBlockTableRecordPointer pBTR(dynBlk.dynamicBlockTableRecord(),AcDb::kForRead);
				if (pBTR.openStatus() == Acad::eOk)
				{
					const ACHAR *blkName = NULL; pBTR->getName(blkName);
					wchar_t tmp[5];
					//acutPrintf(ACRX_T("\nDEBUG blkName: %s"), blkName);
					wcsncpy(tmp, blkName, 4);
					wcscat(tmp, L"\0");
					//acutPrintf(ACRX_T("\nDEBUG tmp: %s"), tmp);
					if (wcscmp(tmp, L"Arm_") != 0) {
						delete pBTR;
						pBlkRef->close();
						acutPrintf(ACRX_T("\nContinued."));
						continue;
					}
					//acutPrintf(ACRX_T("\nDynamic block with name: \"%s\""), blkName);
				} else {
					delete pBTR;
					pBlkRef->close(); 
					continue;
				}
				//http://arxdummies.blogspot.com/2005/03/class-6-entities.html
				// First we will look into BlockReference entity looking for
				// all non-constant attributes which are stored into block insertions
				AcDbObjectIterator *pIter = pBlkRef->attributeIterator();
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
								//acutPrintf(ACRX_T("\nTAG:%s - VALUE:%s"),pAtt->tag(),pAtt->textString());
								acutPrintf(ACRX_T(" %s:%s"),pAtt->tag(),pAtt->textString());
							}
							if (wcscmp(pAtt->tag(), L"ÏÎÇ") == 0) {
								//acutPrintf(ACRX_T("\nTAG:%s - VALUE:%s"),pAtt->tag(),pAtt->textString());
								acutPrintf(ACRX_T("\n%s:%s"),pAtt->tag(),pAtt->textString());
							}
							if (wcscmp(pAtt->tag(), L"ÊÎË") == 0) {
								//acutPrintf(ACRX_T("\nTAG:%s - VALUE:%s"),pAtt->tag(),pAtt->textString());
								acutPrintf(ACRX_T(" %s:%s"),pAtt->tag(),pAtt->textString());
							}
						}
						pAttEnt->close();
					}
					pIter->step();
				}
				delete pIter ;
				
				AcDbDynBlockReferencePropertyArray blkPropAry;
				dynBlk.getBlockProperties(blkPropAry);
				//Acad::ErrorStatus err;
				AcDbDynBlockReferenceProperty blkProp;
				for(long lIndex=0L ; lIndex<blkPropAry.length() ; ++lIndex){
					blkProp = blkPropAry[lIndex];
					if (wcscmp(blkProp.propertyName().kACharPtr(), L"ÄËÈÍÀ") == 0){
						//acutPrintf(ACRX_T("\n%s : "),blkProp.propertyName());
					    AcDbEvalVariant currentValue = blkProp.value();
					    //acutPrintf(ACRX_T("\n Restype %d"),currentValue.restype);
					    acutPrintf(ACRX_T(" %.0f"),currentValue.resval.rreal);
					}
				}
				pBTR->close();
			}
		counter++;
		pBlkRef->close();
	}
	acutPrintf(ACRX_T("\nThe counter is %d.\n"), counter);
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