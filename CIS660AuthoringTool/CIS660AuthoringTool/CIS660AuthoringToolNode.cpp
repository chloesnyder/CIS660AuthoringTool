#define MNoVersionString
#define MNoPluginEntry

#include "CIS660AuthoringToolNode.h"

MTypeId CIS660AuthoringToolNode::id(0x8000);
MObject CIS660AuthoringToolNode::time;
MObject CIS660AuthoringToolNode::width;
MObject CIS660AuthoringToolNode::height;
MObject CIS660AuthoringToolNode::mindepth;
MObject CIS660AuthoringToolNode::maxdepth;
MObject CIS660AuthoringToolNode::hpath;
MObject CIS660AuthoringToolNode::size;
MObject CIS660AuthoringToolNode::outputMesh;

MStatus CIS660AuthoringToolNode::initialize()
{
    MFnUnitAttribute unitAttr;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numAttr;

    MStatus returnStatus;

    // init attributes
    CIS660AuthoringToolNode::time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode time attribute\n");

    CIS660AuthoringToolNode::width = numAttr.MFnNumericAttribute::create("width", "w", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode width attribute\n");

    CIS660AuthoringToolNode::height = numAttr.MFnNumericAttribute::create("height", "h", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode height attribute\n");

    CIS660AuthoringToolNode::size = numAttr.MFnNumericAttribute::create("size", "s", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode size attribute\n");

    CIS660AuthoringToolNode::mindepth = numAttr.MFnNumericAttribute::create("mindepth", "mnd", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode min depth attribute\n");

    CIS660AuthoringToolNode::maxdepth = numAttr.MFnNumericAttribute::create("maxdepth", "mxd", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode max depth attribute\n");

    CIS660AuthoringToolNode::hpath = typedAttr.MFnTypedAttribute::create("hpath", "hp", MFnData::kString, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode height path attribute\n");

    CIS660AuthoringToolNode::outputMesh = typedAttr.create("outputMesh", "out", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode output mesh attribute\n");
    CHECK_MSTATUS(typedAttr.setStorable(false));

    // add attributes
    returnStatus = addAttribute(CIS660AuthoringToolNode::time);
    McheckErr(returnStatus, "ERROR adding time attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::width);
    McheckErr(returnStatus, "ERROR adding width attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::height);
    McheckErr(returnStatus, "ERROR adding height attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::size);
    McheckErr(returnStatus, "ERROR adding size attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::mindepth);
    McheckErr(returnStatus, "ERROR adding min depth attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::maxdepth);
    McheckErr(returnStatus, "ERROR adding max depth attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::hpath);
    McheckErr(returnStatus, "ERROR adding height path attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR adding output mesh attribute\n");

    // attribute affects
    returnStatus = attributeAffects(CIS660AuthoringToolNode::width, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (width affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::height, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (height affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::size, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (size affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::mindepth, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (mindepth affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::maxdepth, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (maxdepth affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::hpath, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (hpath affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::time, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (time affecting outputMesh)\n");

    return MS::kSuccess;
}

MStatus CIS660AuthoringToolNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus returnStatus;
    if (plug == outputMesh)
        {
        //get time
        MDataHandle timeData = data.inputValue(time, &returnStatus);
        McheckErr(returnStatus, "Error getting time data handle\n");
        MTime timeVal = timeData.asTime();

        //get width
        MDataHandle widthData = data.inputValue(width, &returnStatus);
        McheckErr(returnStatus, "Error getting width data handle\n");
        int widthVal = widthData.asInt();

        //get height
        MDataHandle heightData = data.inputValue(height, &returnStatus);
        McheckErr(returnStatus, "Error getting height data handle\n");
        int heightVal = heightData.asInt();

        //get size
        MDataHandle sizeData = data.inputValue(size, &returnStatus);
        McheckErr(returnStatus, "Error getting size  data handle\n");
        double sizeVal = sizeData.asDouble();

        //get min depth
        MDataHandle minDepthData = data.inputValue(mindepth, &returnStatus);
        McheckErr(returnStatus, "Error getting mindepth  data handle\n");
        double minDepthVal = minDepthData.asDouble();

        //get max depth
        MDataHandle maxDepthData = data.inputValue(maxdepth, &returnStatus);
        McheckErr(returnStatus, "Error getting maxdepth  data handle\n");
        double maxDepthVal = maxDepthData.asDouble();

        MDataHandle heightPathData = data.inputValue(hpath, &returnStatus);
        McheckErr(returnStatus, "Error getting height path data handle\n");
        MString heightPathVal = heightPathData.asString();

        //get output object
        MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR getting geometry data handle\n");

        MFnMeshData dataCreator;
        MObject newOutputData = dataCreator.create(&returnStatus);
        McheckErr(returnStatus, "ERROR creating outputData");

        createMesh(timeVal, widthVal, heightVal, sizeVal, minDepthVal, maxDepthVal, heightPathVal, newOutputData, returnStatus);
        McheckErr(returnStatus, "ERROR creating mesh");

        outputHandle.set(newOutputData);
        data.setClean(plug);

        }
    return MS::kSuccess;
}

void CIS660AuthoringToolNode::FILL(double x, double  y, double z)
{
    MFloatPoint pnt((float) x, (float) y, (float) z);
    iarr.append(pnt);
}

void CIS660AuthoringToolNode::createPlane(int width, int height, double s)
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

double CIS660AuthoringToolNode::remap(double value, double low1, double low2, double high1, double high2)
{
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

// given (x,z), finds the corresponding pixel in the image and returns the y value 
double  CIS660AuthoringToolNode::lookUpHeight(double x, double z)
{
    if (heightMap.is_empty())
    {
        return 0;
    }

    //get pixel (x, z) = [R, G, B]. Convert to height
    //by converting to grayscale:( (0.3 * R) + (0.59 * G) + (0.11 * B) ) 
    int px = floor(x);
    int pz = floor(z);

    double r = heightMap(px, pz, 0, 0);
    double g = heightMap(px, pz, 0, 1);
    double b = heightMap(px, pz, 0, 2);

    double height = (.3 * r) + (.59 * g) + (.11 * b);

    //remap height to be in a certain range based on slider
    double maxHeight = (.3 * 255) + (.58 * 255) + (.11 * 255); // 252.45 is max height
    double minHeight = 0;
    height = remap(height, minHeight, minDepth, maxHeight, maxDepth);

    return height;
}

MObject CIS660AuthoringToolNode::createMesh(const MTime& time, const int& width, const int& height, const double& s, 
                                            const double& min_depth, const double& max_depth, const MString& heightPath, MObject& outData, MStatus& stat)
{

    // create a poly function set
    //
    MFnMesh fnPoly;

    iarr.clear();
    faceCounts.clear();
    faceConnects.clear();

    int sx = 128;
    int sy = 128;
    double size = 2.0;

    std::string file_path = heightPath.asChar();
    if(!file_path.empty())
    {
        char const* fp = file_path.c_str();
        heightMap.load(fp);
    }

    minDepth = min_depth;
    maxDepth = max_depth;


    createPlane(width, height, s);
    p_gons = NULL; // Is this still true if plane isn't just 2 by 2?

    //construct point array
    pa.clear();
    for (int i = 0; i < num_verts; i++) {
        pa.append(iarr[i]);
    }

    MObject newMesh = fnPoly.create(num_verts, num_faces, pa, faceCounts, faceConnects, outData, &stat);

    return newMesh;

}
