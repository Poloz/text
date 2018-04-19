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

AcDbObjectId createLine()
{
    AcGePoint3d startPt(4.0, 2.0, 0.0);
    AcGePoint3d endPt(10.0, 7.0, 0.0);
    AcDbLine *pLine = new AcDbLine(startPt, endPt);
    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);
    pBlockTable->close();
    AcDbObjectId lineId;
    pBlockTableRecord->appendAcDbEntity(lineId, pLine);
    pBlockTableRecord->close();
    pLine->close();
    return lineId;
}

AcDbObjectId createCircle()
{
    AcGePoint3d center(9.0, 3.0, 0.0);
    AcGeVector3d normal(0.0, 0.0, 1.0);
    AcDbCircle *pCirc = new AcDbCircle(center, normal, 2.0);
    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);
    pBlockTable->close();
    AcDbObjectId circleId;
    pBlockTableRecord->appendAcDbEntity(circleId, pCirc);
    pBlockTableRecord->close();
    pCirc->close();
    return circleId;
}

void createNewLayer()
{
    AcDbLayerTable *pLayerTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pLayerTable, AcDb::kForWrite);

    AcDbLayerTableRecord *pLayerTableRecord =
        new AcDbLayerTableRecord;
    pLayerTableRecord->setName(ACRX_T("ASDK_MYLAYER"));

	// Defaults are used for other properties of 
	// the layer if they are not otherwise specified.

	pLayerTable->add(pLayerTableRecord);
    pLayerTable->close();
    pLayerTableRecord->close();
}

void createGroup(AcDbObjectIdArray& objIds, TCHAR* pGroupName)
{
    AcDbGroup *pGroup = new AcDbGroup(pGroupName);
	
	// Put the group in the group dictionary which resides
    // in the named object dictionary.
    //
    AcDbDictionary *pGroupDict;
    acdbHostApplicationServices()->workingDatabase()
        ->getGroupDictionary(pGroupDict, AcDb::kForWrite);

    AcDbObjectId pGroupId;
    pGroupDict->setAt(pGroupName, pGroup, pGroupId);
    pGroupDict->close();

    // Now that the group has been added, it has an ObjectID.
	// This is important since the group will become a persistent
	// reactor for the added entities...
    for (int i = 0; i < objIds.length(); i++) {
        pGroup->append(objIds[i]);
    }

	pGroup->close();
}

Acad::ErrorStatus changeColor(AcDbObjectId entId, Adesk::UInt16 newColor)
{
    AcDbEntity *pEntity;
    acdbOpenObject(pEntity, entId,
        AcDb::kForWrite);

    pEntity->setColorIndex(newColor);
    pEntity->close();

    return Acad::eOk;
}

void selectBlocks()
{
	ads_name ssname;
	// Get the current PICKFIRST or ask user for a selection
	acedSSGet(NULL, NULL, NULL, NULL, ssname); 
}

void runIt()
{
	selectBlocks();
    //createNewLayer();
	//AcDbObjectIdArray idArr;
	// create a line and circle and add them to the objectId
    // array
    //
    //idArr.append(createLine());
    //idArr.append(createCircle());
	// change circle color to red
    //
    //changeColor(idArr.last(), 1);
	// put the line and circle in a group named
    // "ASDK_TEST_GROUP"
    //
    //createGroup(idArr, ACRX_T("ASDK_TEST_GROUP"));
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

// Simple acrxEntryPoint code. Normally intialization and cleanup
// (such as registering and removing commands) should be done here.
//

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