#pragma once
#include "CIS660AuthoringToolCmd.h"

CIS660AuthoringToolCmd::CIS660AuthoringToolCmd() : MPxCommand()
{

}


CIS660AuthoringToolCmd::~CIS660AuthoringToolCmd()
 {

 }

MStatus CIS660AuthoringToolCmd::createPlane(int x, int y)
{
    std::string createPlane;
    std::stringstream stringStream;
    stringStream << "$heightMap = `polyPlane -sx " << x << " -sy " << y << "`";
    createPlane = stringStream.str();

    MString setHeightMapNameMelCmd = "global string $heightMap";
    MString createPlaneMelCmd = createPlane.c_str();

    MGlobal::executeCommand(setHeightMapNameMelCmd);
    MGlobal::executeCommand(createPlaneMelCmd);

    return MS::kSuccess;
}

MStatus CIS660AuthoringToolCmd::doIt(const MArgList& args)
{
    MStatus status;

    int sx = 128;
    int sy = 128;
    double size = 2.0;

    MArgDatabase parser(syntax(), args, &status);
    if (status != MS::kSuccess)
    {
        return status;
    }

    if (parser.isFlagSet(WIDTH))
    {
        parser.getFlagArgument(WIDTH, 0, sx);
    }

    if (parser.isFlagSet(HEIGHT))
    {
        parser.getFlagArgument(HEIGHT, 0, sy);
    }

    if (parser.isFlagSet(SIZE))
    {
        parser.getFlagArgument(SIZE, 0, size);
    }

    status = createPlane(sx, sy);
    MGlobal::displayInfo("CIS660AuthoringToolCmd\n");
    setResult("CIS660AuthoringToolCmd was executed\n");

    return status;
}

MStatus MPxNode::dependsOn(const MPlug &, const MPlug &, bool &) const
{
    return MS::kUnknownParameter;
}



////
//// Copyright (C) 
//// 
//// File: CIS660AuthoringToolCmd.cpp
////
//// MEL Command: CIS660AuthoringTool
////
//// Author: Maya Plug-in Wizard 2.0
////
//
//// Includes everything needed to register a simple MEL command with Maya.
//// 
//
//
//#include <maya/MSimple.h>
//
//#include <maya/MIOStream.h>
//#include <math.h>
//
//#include <maya/MPxCommand.h>
//#include <maya/MString.h>
//#include <maya/MArgList.h>
//#include <maya/MGlobal.h>
//#include <maya/MPoint.h>
//#include <maya/MIntArray.h>
//#include <maya/MDoubleArray.h>
//#include <maya/MFloatPoint.h>
//#include <maya/MFloatPointArray.h>
//#include <maya/MDagPath.h>
//#include <maya/MFnPlugin.h>
//#include <maya/MFnMesh.h>
//
//#ifndef M_PI
//#define M_PI            3.14159265358979323846  /* pi */
//#endif
//
//
//// reference for creating a plane: http://download.autodesk.com/us/maya/2010help/API/poly_primitive_cmd_8cpp-example.html
//
//// Macro for error checking
//#define checkErr(stat,msg)                                      \
//    if ( MS::kSuccess != stat ) {                       \
//        displayError(msg);                                      \
//                return stat;                                            \
//        }
//
//// Class definition //
//
//class CIS660AuthoringTool : public MPxCommand
//    {
//    public:
//        CIS660AuthoringTool() {};
//        virtual                 ~CIS660AuthoringTool();
//
//        MStatus                 doIt(const MArgList& args);
//        MStatus                 undoIt();
//
//        inline bool             isUndoable() const { return true; };
//        static void*    creator();
//
//    public:
//        inline void             FILL(double x, double y, double z);
//        void                    createPlane();
//
//        // What sort of shape we're making
//        int shapeFlag;
//
//        // Misc. primitive data
//        //
//        int num_verts;                  // Number of vertices of polygon
//        int num_faces;                  // Number of faces on polygon
//        int num_edges;                  // Number of edges on polygon
//        int edges_per_face;             // Number of edges (or verticies) per face
//        int num_face_connects;  // Number of elements in face connect array
//        int *p_gons;                    // Pointer to static array of face connects
//        MFloatPointArray iarr;
//        MFloatPointArray pa;
//        MIntArray faceCounts;
//        MIntArray faceConnects;
//
//        MObject newTransform;
//        MDGModifier dgModifier;
//    };
//
//// class implementation
//
//CIS660AuthoringTool::~CIS660AuthoringTool() {}
//
//void* CIS660AuthoringTool::creator()
//    {
//    return new CIS660AuthoringTool();
//    }
//
//void CIS660AuthoringTool::FILL(double x, double y, double z)
//    {
//    MFloatPoint pnt((float) x, (float) y, (float) z);
//    iarr.append(pnt);
//    }
//
//void CIS660AuthoringTool::createPlane()
//    {
//    int w = 2;
//    int h = 2;
//    double size = 2.0;
//    double hSize, wSize;
//
//    // Initialize class data
//    //
//    num_verts = 0;
//    num_faces = 0;
//    edges_per_face = 4;
//
//    if (w < 1) w = 1;
//    if (h < 1) h = 1;
//    if (size < 0.0001) size = 1.0;
//
//    wSize = size / w;
//    hSize = size / h;
//
//    // create vertices
//    //
//    double x, z;
//    for (z = -size / 2.0; z <= size / 2.0; z += hSize)
//        {
//        for (x = -size / 2.0; x <= size / 2.0; x += wSize)
//            {
//            FILL(x, 0, z);
//            num_verts++;
//            }
//        }
//
//    // create polys
//    //
//    int v0, v1, v2, v3;
//    for (int i = 0; i < h; i++)
//        {
//        for (int j = 0; j < w; j++)
//            {
//            v0 = j + (w + 1) * i;
//            v1 = j + 1 + (w + 1) * i;
//            v2 = j + 1 + (w + 1) * (i + 1);
//            v3 = j + (w + 1) * (i + 1);
//
//            faceConnects.append(v0);
//            faceConnects.append(v3);
//            faceConnects.append(v2);
//            faceConnects.append(v1);
//            num_faces++;
//            faceCounts.append(edges_per_face);
//            }
//        }
//
//    num_face_connects = num_faces * edges_per_face;
//    num_edges = num_face_connects / 2;
//    }
//
//MStatus CIS660AuthoringTool::doIt(const MArgList& args)
//    {
//    MStatus stat;
//
//    // create a poly function set
//    MFnMesh fnPoly;
//    iarr.clear();
//    faceCounts.clear();
//    faceConnects.clear();
//
//    createPlane();
//    p_gons = NULL;
//
//    //cosntruct the point array
//    pa.clear();
//    int i;
//
//    for (i = 0; i<num_verts; i++)
//        pa.append(iarr[i]);
//
//    // If we are using polygon data then set up the face connect array
//    // here. Otherwise, the create function will do it.
//    //
//    if (NULL != p_gons) {
//        num_face_connects = num_faces * edges_per_face;
//        num_edges = num_face_connects / 2;
//
//        for (i = 0; i<num_faces; i++)
//            faceCounts.append(edges_per_face);
//
//        for (i = 0; i<(num_faces*edges_per_face); i++)
//            faceConnects.append(p_gons[i] - 1);
//        }
//
//    // Call the poly creation method to create the polygon
//    newTransform = fnPoly.create(num_verts, num_faces, pa,
//                                 faceCounts, faceConnects, MObject::kNullObj, &stat);
//    checkErr(stat, "Could not create MFnMesh");
//
//    // Primitive is created so tell shape it has changed
//    //
//    fnPoly.updateSurface();
//
//    dgModifier.renameNode(newTransform, "pPrimitive1");
//    dgModifier.doIt();
//
//    //
//    // Put the polygon into a shading group
//    MString cmd("sets -e -fe initialShadingGroup ");
//    cmd += fnPoly.name();
//    dgModifier.commandToExecute(cmd);
//
//    MFnDagNode fnDagNode(newTransform, &stat);
//    if (MS::kSuccess == stat)
//        {
//        cmd = "select ";
//        cmd += fnDagNode.name();
//        dgModifier.commandToExecute(cmd);
//        }
//
//    dgModifier.doIt();
//
//    return MS::kSuccess;
//
//    return stat;
//    }
//
//MStatus CIS660AuthoringTool::undoIt()
//    {
//    MStatus status;
//    MDGModifier dgModifier;
//
//    MFnDagNode dagNode(newTransform, &status);
//    checkErr(status, "Could not get dagNode for transform");
//
//    MObject mesh = dagNode.child(0, &status);
//    checkErr(status, "Could not get dagNode's child");
//
//    MFnDependencyNode meshNode(mesh);
//    status = dgModifier.deleteNode(mesh);
//    checkErr(status, "Could not delete mesh");
//
//    dgModifier.deleteNode(newTransform);
//    checkErr(status, "Could not delete transform");
//
//    return dgModifier.doIt();
//    }
//
//MStatus initializePlugin(MObject obj)
//    {
//    MStatus   status;
//    MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");
//
//    status = plugin.registerCommand("CIS660AuthoringToolTest", CIS660AuthoringTool::creator);
//    if (!status) {
//        status.perror("registerCommand");
//        return status;
//        }
//
//    status = plugin.registerUI("CIS660AuthoringToolCreateUI", "CIS660AuthoringToolDeleteUI");
//    if (!status) {
//        status.perror("registerUI");
//        return status;
//    }
//
//    return status;
//    }
//
//MStatus uninitializePlugin(MObject obj)
//    {
//    MStatus   status;
//    MFnPlugin plugin(obj);
//
//    status = plugin.deregisterCommand("CIS660AuthoringToolTest");
//    if (!status) {
//        status.perror("deregisterCommand");
//        return status;
//        }
//
//    return status;
//    }