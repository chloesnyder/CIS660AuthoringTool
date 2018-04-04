#pragma once
#ifndef CreateCIS660AuthoringToolCmd_H_
#define CreateCIS660AuthoringToolCmd_H_

#include <maya/MPxCommand.h>
#include <string>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <list>
#include <sstream>

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <iostream>
#include <math.h>

#include "CImg.h"
using namespace cimg_library;

#define WIDTH "-w"
#define WIDTH_LONG "-width"
#define HEIGHT "-h"
#define HEIGHT_LONG "-height"
#define SIZE "-s"
#define SIZE_LONG "-size"
#define HMPATH "-hp"
#define HMPATH_LONG "-hpath"

#define McheckErr(stat,msg)             \
        if ( MS::kSuccess != stat ) {   \
                cerr << msg;            \
                return MS::kFailure;    \
        }

#define checkErr(stat,msg)                                      \
    if ( MS::kSuccess != stat ) {                       \
        displayError(msg);                                      \
                return stat;                                            \
        }


class CIS660AuthoringToolCmd : public MPxCommand
{
public:
    CIS660AuthoringToolCmd();
    virtual ~CIS660AuthoringToolCmd();
    static void* creator() { return new CIS660AuthoringToolCmd(); }
    MStatus doIt(const MArgList& args);
    void createPlane(int width, int height, double s);

    // Poly primitive
    int num_verts;                  // Number of vertices of polygon
    int num_faces;                  // Number of faces on polygon
    int num_edges;                  // Number of edges on polygon
    int edges_per_face;             // Number of edges (or verticies) per face
    int num_face_connects;  // Number of elements in face connect array
    int *p_gons;                    // Pointer to static array of face connects
    MFloatPointArray iarr;
    MFloatPointArray pa;
    MIntArray faceCounts;
    MIntArray faceConnects;

    CImg<float> heightMap;
  
    MObject newTransform;
    MDGModifier dgModifier;

    void FILL(double x, double y, double z);
    double lookUpHeight(double x, double z);
    double remap(double value, double low1, double low2, double high1, double high2);

   
    static MSyntax newSyntax() {
        MSyntax syntax;
        syntax.addFlag(WIDTH, WIDTH_LONG, MSyntax::kLong);
        syntax.addFlag(HEIGHT, HEIGHT_LONG, MSyntax::kLong);
        syntax.addFlag(SIZE, SIZE_LONG, MSyntax::kDouble);
        syntax.addFlag(HMPATH, HMPATH_LONG, MSyntax::kString);
        return syntax;
    }
};

#endif