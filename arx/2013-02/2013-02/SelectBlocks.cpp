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
#include <string> // std::stod - string to double
#include "SetAttribute.h" // SetAttribute()

void selectBlocks()
{	
	ads_name ssname;
	struct resbuf blockFilter;//, insertFilter;

	const ACHAR *prefix = ACRX_T("Arm_");

	blockFilter.restype = 0; // Entity type
	blockFilter.resval.rstring = ACRX_T("INSERT"); //We are selecting block insertions
	blockFilter.rbnext = NULL; //No next property

	acutPrintf(ACRX_T("\nPlease select blocks."));
	
	if (acedSSGet(NULL, NULL, NULL, &blockFilter, ssname) != RTNORM ) //So we are selecting blocks insertions
	{
		acutPrintf(ACRX_T("\nSomething's wrong.\n"));
		acedSSFree(ssname);
		return;
	}
	
	long length = 0;	
	if (acedSSLength(ssname, &length) != RTNORM) //If no blocks selected - we are out
	{
		acutPrintf(ACRX_T("\nNo blocks selectes\n"));
		acedSSFree(ssname);
		return;
	}	
	ads_name ent;
	long counter = 0;
	AcDbObjectId obId = AcDbObjectId::kNull;
	//ssname contains some blocks. Now we are iterating through them
	for (long i = 0; i < length; i++)
	{
		if (acedSSName(ssname, i, ent) != RTNORM) {acutPrintf(ACRX_T("\nCannot get entity from selection.\n")); continue;};
		if (acdbGetObjectId(obId, ent) != Acad::eOk) {acutPrintf(ACRX_T("\nCannot get ObjectId from entity.\n")); continue;};
		AcDbObjectPointer<AcDbBlockReference> pBlkRef (obId, AcDb::kForRead);
		if (pBlkRef.openStatus() != Acad::eOk) {acutPrintf(ACRX_T("\nCannot open for read.\n")); delete pBlkRef; continue;};
		AcDbDynBlockReference dynBlk(obId);
		if (dynBlk.isDynamicBlock()) //We need only dynamic blocks, right?
			{
				AcDbBlockTableRecordPointer pBTR(dynBlk.dynamicBlockTableRecord(),AcDb::kForRead);
				if (pBTR.openStatus() == Acad::eOk) //Can we open block for read?
				{
					const ACHAR *blkName = NULL; 
					wchar_t tmp[5];

					pBTR->getName(blkName);					
					//acutPrintf(ACRX_T("\nDEBUG blkName: %s"), blkName);
					wcsncpy_s(tmp, blkName, 4);
					wcscat_s(tmp, L"\0");
					//acutPrintf(ACRX_T("\nDEBUG tmp: %s"), tmp);
					if (wcscmp(tmp, L"Arm_") != 0) { //Block name is not starting with "Arm_"
						delete pBTR;
						pBlkRef->close();
						//acutPrintf(ACRX_T("\nDEBUG Block name is not starting with \"Arm_\"."));
						continue;
					}
					//acutPrintf(ACRX_T("\nDynamic block with name: \"%s\""), blkName);
				} else { //Cannot open block for read
					delete pBTR;
					pBlkRef->close(); 
					continue;
				}

				double LocalIncrement = 0;
				double LocalWidth = 0;
				double LocalQuantity = 0;

				acutPrintf(ACRX_T("\nDEBUG: -------------"));

				AcDbDynBlockReferencePropertyArray blkPropAry;
				dynBlk.getBlockProperties(blkPropAry);
				//Acad::ErrorStatus err;
				AcDbDynBlockReferenceProperty blkProp;
				for(long lIndex=0L ; lIndex<blkPropAry.length() ; ++lIndex){
					blkProp = blkPropAry[lIndex];
					if (wcscmp(blkProp.propertyName().kACharPtr(), L"ØÈÐÈÍÀ") == 0){
						//acutPrintf(ACRX_T("\n%s : "),blkProp.propertyName());
					    AcDbEvalVariant currentValue = blkProp.value();
					    //acutPrintf(ACRX_T("\n Restype %d"),currentValue.restype);
						LocalWidth = currentValue.resval.rreal;
					    acutPrintf(ACRX_T("\nDEBUG: Øèðèíà: %.2f"), LocalWidth);
					}
				}

				//http://arxdummies.blogspot.com/2005/03/class-6-entities.html
				// First we will look into BlockReference entity looking for
				// all non-constant attributes which are stored into block insertions
				AcDbObjectIterator *pIter = pBlkRef->attributeIterator();
				
				while (!pIter->done()) {
					AcDbEntity *pAttEnt = NULL;
					AcDbObjectId idAtt = pIter->objectId();
					if (acdbOpenObject(pAttEnt,idAtt,AcDb::kForWrite) == Acad::eOk) {
						//We need this entity open for writing
						if (pAttEnt->isKindOf(AcDbAttribute::desc())) {
							AcDbAttribute* pAtt = AcDbAttribute::cast(pAttEnt);
							//CString msg;
							//msg.Format("\nTAG:%s - VALUE:%s",pAtt->tag(),pAtt->textString());
							//acutPrintf(msg);
							/**
							if (wcscmp(pAtt->tag(), L"ÄÈÀÌ") == 0) {
								acutPrintf(ACRX_T(" %s:%s"),pAtt->tag(),pAtt->textString());
							}
							if (wcscmp(pAtt->tag(), L"ÏÎÇ") == 0) {
								acutPrintf(ACRX_T("\n%s:%s"),pAtt->tag(),pAtt->textString());
							}
							**/
							if (wcscmp(pAtt->tag(), L"ØÀÃ") == 0) {
								acutPrintf(ACRX_T("\nDEBUG: %s:%s"),pAtt->tag(),pAtt->textString());
								LocalIncrement = std::stod(pAtt->textString());
								LocalQuantity = 1 + (LocalWidth / LocalIncrement);
								acutPrintf(ACRX_T("\nDEBUG: width: %.4f"), LocalWidth);
								acutPrintf(ACRX_T("\nDEBUG: increment: %.4f"), LocalIncrement);
								acutPrintf(ACRX_T("\nDEBUG: quantity: %.4f"), LocalQuantity);
								//SetAttribute(LocalQuantity, L"ÊÎË", LocalQuantity);
							}
							//Very bad thing happens here. The thing is - KOL attribute iterated after SHAG
							//and we think that KOL may be safely updated because of this thing
							//To get rid of this behavior here should be another iteration cycle
							//In the first iteration cycle LocalQuantity is calculated
							//In the second - KOL updated.
							//But here's temporary fix. For years and years...
							if (wcscmp(pAtt->tag(), L"ÊÎË") == 0) {
								acutPrintf(ACRX_T("\nDEBUG: %s:%s"),pAtt->tag(),pAtt->textString());
								acutPrintf(ACRX_T("\nDEBUG: Updating to quantity: %.4f"), LocalQuantity);
							}
						}
						pAttEnt->close();
					}
					pIter->step();
				} //Finished all iterations
				delete pIter;			
				pBTR->close();
			} //Finished "if dynamic block" block
		counter++;
		pBlkRef->close();
	}
	acutPrintf(ACRX_T("\nThe counter is %d.\n"), counter);
	acedSSFree(ssname);
}