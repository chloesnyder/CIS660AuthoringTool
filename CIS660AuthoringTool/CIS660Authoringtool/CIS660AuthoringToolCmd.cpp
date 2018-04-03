#pragma once
#include "CIS660AuthoringToolCmd.h"

CIS660AuthoringToolCmd::CIS660AuthoringToolCmd() : MPxCommand()
{

}


CIS660AuthoringToolCmd::~CIS660AuthoringToolCmd()
 {

 }

void CIS660AuthoringToolCmd::FILL(double x, double  y, double z)
{
    MFloatPoint pnt((float) x, (float) y, (float) z);
    iarr.append(pnt);
}

void CIS660AuthoringToolCmd::createPlane(int width, int height, double s)
    {
    // pass in a paramter of size
    int w = width;
    int h = height;
    double size = s;
    double hSize, wSize;

    // Initialize class data
    //
    num_verts = 0;
    num_faces = 0;
    edges_per_face = 4;

    if (w < 1) w = 1;
    if (h < 1) h = 1;
    if (size < 0.0001) size = 1.0;

    wSize = size / w;
    hSize = size / h;

    // create vertices
    //
    double x, z;
    for (z = -size / 2.0; z <= size / 2.0; z += hSize)
        {
        for (x = -size / 2.0; x <= size / 2.0; x += wSize)
            {
            FILL(x, 0, z);
            num_verts++;
            }
        }

    // create polys
    //
    int v0, v1, v2, v3;
    for (int i = 0; i < h; i++)
        {
        for (int j = 0; j < w; j++)
            {
            v0 = j + (w + 1) * i;
            v1 = j + 1 + (w + 1) * i;
            v2 = j + 1 + (w + 1) * (i + 1);
            v3 = j + (w + 1) * (i + 1);

            faceConnects.append(v0);
            faceConnects.append(v3);
            faceConnects.append(v2);
            faceConnects.append(v1);
            num_faces++;
            faceCounts.append(edges_per_face);
            }
        }

    num_face_connects = num_faces * edges_per_face;
    num_edges = num_face_connects / 2;
}

MStatus CIS660AuthoringToolCmd::doIt(const MArgList& args)
{
    MStatus stat;

    // create a poly function set
    //
    MFnMesh fnPoly;

    iarr.clear();
    faceCounts.clear();
    faceConnects.clear();

    int sx = 128;
    int sy = 128;
    double size = 2.0;

    MArgDatabase parser(syntax(), args, &stat);
    if (stat != MS::kSuccess)
    {
        return stat;
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

    createPlane(sx, sy, size);

    p_gons = NULL; // Is this still true if plane isn't just 2 by 2?

    //construct point array
    pa.clear();
    for (int i = 0; i < num_verts; i++) {
        pa.append(iarr[i]);
    }

    // If we are using polygon data then set up the face connect array
    // here. Otherwise, the create function will do it.
    //
    if (NULL != p_gons) {
        num_face_connects = num_faces * edges_per_face;
        num_edges = num_face_connects / 2;

        for (int i = 0; i<num_faces; i++)
            faceCounts.append(edges_per_face);

        for (int i = 0; i<(num_faces*edges_per_face); i++)
            faceConnects.append(p_gons[i] - 1);
     }

    // Call the poly creation method to create the polygon
    newTransform = fnPoly.create(num_verts, num_faces, pa,
                                 faceCounts, faceConnects, MObject::kNullObj, &stat);
    checkErr(stat, "Could not create MFnMesh");

    // Primitive is created so tell shape it has changed
    //
    fnPoly.updateSurface();

    dgModifier.renameNode(newTransform, "pPrimitive1");
    dgModifier.doIt();

    //
    // Put the polygon into a shading group
    MString cmd("sets -e -fe initialShadingGroup ");
    cmd += fnPoly.name();
    dgModifier.commandToExecute(cmd);

    MFnDagNode fnDagNode(newTransform, &stat);
    if (MS::kSuccess == stat)
    {
        cmd = "select ";
        cmd += fnDagNode.name();
        dgModifier.commandToExecute(cmd);
    }

    dgModifier.doIt();

    MGlobal::displayInfo("CIS660AuthoringToolCmd\n");
    setResult("CIS660AuthoringToolCmd was executed\n");

    return MS::kSuccess;
  
}

//MStatus MPxNode::dependsOn(const MPlug &, const MPlug &, bool &) const
//{
//    return MS::kUnknownParameter;
//}
