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


// given (x,z), finds the corresponding pixel in the image and returns the y value 
double  CIS660AuthoringToolCmd::lookUpHeight(double x, double z)
{
    //get pixel (x, z) = [R, G, B]. Convert to height
    //by converting to grayscale:( (0.3 * R) + (0.59 * G) + (0.11 * B) ) 
    int px = floor(x);
    int pz = floor(z);

    double r = heightMap(px, pz, 0, 0);
    double g = heightMap(px, pz, 0, 1);
    double b = heightMap(px, pz, 0, 2);

    double height = (.3 * r) + (.59 * g) + (.11 * b);

    return height;
}

double CIS660AuthoringToolCmd::remap(double value, double low1, double low2, double high1, double high2)
{
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
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
            // TODO: replace 0 with looking up the height value from our image that corresponds to x and z
            // remap x and z from range [-size/2.0, size/2.0] to range [0,256]
            double remapX = remap(x, (-size / 2.0), 0.0, (size / 2.0), 255.0);
            double remapZ = remap(z, (-size / 2.0), 0.0, (size / 2.0), 255.0);
            double y = lookUpHeight(remapX, remapZ);
            FILL(x, y, z);
            num_verts++;
        }
     }

    // create polys
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

    MString heightMapPath = "";

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
    if (parser.isFlagSet(HMPATH))
    {
        parser.getFlagArgument(HMPATH, 0, heightMapPath);
    }
  
    std::string file_path = heightMapPath.asChar();
    char const* fp = file_path.c_str();
   
    heightMap.load(fp); 

    createPlane(sx, sy, size);

    p_gons = NULL; 

    //construct point array
    pa.clear();
    for (int i = 0; i < num_verts; i++) {
        pa.append(iarr[i]);
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

