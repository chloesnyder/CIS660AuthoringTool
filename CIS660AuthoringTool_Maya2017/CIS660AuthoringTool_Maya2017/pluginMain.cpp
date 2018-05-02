#pragma once
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>
#include <list>

#include "CIS660AuthoringToolNode.h"



MStatus initializePlugin(MObject obj)
    {
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin(obj, "MyPlugin", "1.0", "Any");

 
    plugin.setName("CIS660AuthoringTool");

    status = plugin.registerNode("CIS660AuthoringToolNode", CIS660AuthoringToolNode::id, CIS660AuthoringToolNode::creator, CIS660AuthoringToolNode::initialize);
    if (!status) {
        char buffer2[2048];
        sprintf_s(buffer2, 2048, "error");
        MGlobal::displayInfo(buffer2);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        }

    char buffer[2048];
    std::string s = plugin.loadPath().asChar();
    sprintf_s(buffer, 2048, "source \"%s/CIS660AuthoringToolMelScript\";", plugin.loadPath().asChar());
    MGlobal::executeCommand(buffer, true);

    return status;
    }

MStatus uninitializePlugin(MObject obj)
    {
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(CIS660AuthoringToolNode::id);
    if (!status) {
        CHECK_MSTATUS_AND_RETURN_IT(status);
        }

    return status;
    }