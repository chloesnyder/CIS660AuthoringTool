#define MNoVersionString
#define MNoPluginEntry

#include "CIS660AuthoringToolNode.h"
#include <maya/MFnInstancer.h>

MTypeId CIS660AuthoringToolNode::id(0x8000);
MObject CIS660AuthoringToolNode::time;
MObject CIS660AuthoringToolNode::treeHeight;
MObject CIS660AuthoringToolNode::mindepth;
MObject CIS660AuthoringToolNode::maxdepth;
MObject CIS660AuthoringToolNode::fpath;
MObject CIS660AuthoringToolNode::hpath;
MObject CIS660AuthoringToolNode::outputMesh;
MObject CIS660AuthoringToolNode::outPoints;

MStatus CIS660AuthoringToolNode::initialize()
{
    MFnUnitAttribute unitAttr;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numAttr;

    MStatus returnStatus;

    // init attributes

    CIS660AuthoringToolNode::time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode time attribute\n");

    CIS660AuthoringToolNode::treeHeight = numAttr.MFnNumericAttribute::create("tree_height", "th", MFnNumericData::kDouble, 1.0, &returnStatus);
    numAttr.setMin(0.0);
    numAttr.setMax(10.0);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode tree_height attribute\n");

    CIS660AuthoringToolNode::mindepth = numAttr.MFnNumericAttribute::create("mindepth", "mnd", MFnNumericData::kDouble, 0, &returnStatus);
    numAttr.setMin(-15.0);
    numAttr.setMax(0.0);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode min depth attribute\n");

    CIS660AuthoringToolNode::maxdepth = numAttr.MFnNumericAttribute::create("maxdepth", "mxd", MFnNumericData::kDouble, 0, &returnStatus);
    numAttr.setMin(0.0);
    numAttr.setMax(15.0);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode max depth attribute\n");

    CIS660AuthoringToolNode::hpath = typedAttr.MFnTypedAttribute::create("hpath", "hp", MFnData::kString, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode height path attribute\n");

    CIS660AuthoringToolNode::fpath = typedAttr.MFnTypedAttribute::create("fpath", "fp", MFnData::kString, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode foliage path attribute\n");

    CIS660AuthoringToolNode::outputMesh = typedAttr.create("outputMesh", "out", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode output mesh attribute\n");
    CHECK_MSTATUS(typedAttr.setStorable(false));

    CIS660AuthoringToolNode::outPoints = typedAttr.create("outPoints", "out_p", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringtoolNode output points attribute\n");
    CHECK_MSTATUS(typedAttr.setStorable(false));
    CHECK_MSTATUS(typedAttr.setWritable(false));
    CHECK_MSTATUS(typedAttr.setKeyable(false));
    CHECK_MSTATUS(typedAttr.setReadable(true));

    // add attributes
    returnStatus = addAttribute(CIS660AuthoringToolNode::time);
    McheckErr(returnStatus, "ERROR adding time attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::treeHeight);
    McheckErr(returnStatus, "ERROR adding tree height attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::mindepth);
    McheckErr(returnStatus, "ERROR adding min depth attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::maxdepth);
    McheckErr(returnStatus, "ERROR adding max depth attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::hpath);
    McheckErr(returnStatus, "ERROR adding height path attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::fpath);
    McheckErr(returnStatus, "ERROR adding foliage path attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR adding output mesh attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR adding output points attribute\n");

    // attribute affects output mesh
    returnStatus = attributeAffects(CIS660AuthoringToolNode::treeHeight, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (tree height affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::mindepth, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (mindepth affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::maxdepth, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (maxdepth affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::hpath, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (hpath affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::fpath, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (fpath affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::time, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (time affecting outputMesh)\n");

    // attribute affect output points
    returnStatus = attributeAffects(CIS660AuthoringToolNode::time, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (time affecting outPoints)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::treeHeight, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (tree height affecting outPoints)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::mindepth, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (mindepth affecting outPoints)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::maxdepth, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (maxdepth affecting outPoints)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::hpath, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (hpath affecting outPoints)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::fpath, CIS660AuthoringToolNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (fpath affecting outPoints)\n");

    return MS::kSuccess;
}

MStatus CIS660AuthoringToolNode::compute(const MPlug& plug, MDataBlock& data)
{

    
    MStatus returnStatus;
    if ((plug == outputMesh) || (plug == outPoints))
        {

        //get time
        MDataHandle timeData = data.inputValue(time, &returnStatus);
        McheckErr(returnStatus, "Error getting time data handle\n");
        MTime timeVal = timeData.asTime();

        //get tree hight
        MDataHandle treeHeightData = data.inputValue(treeHeight, &returnStatus);
        McheckErr(returnStatus, "Error getting tree height data handle\n");
        double treeHeightVal = treeHeightData.asDouble();

        //get min depth
        MDataHandle minDepthData = data.inputValue(mindepth, &returnStatus);
        McheckErr(returnStatus, "Error getting mindepth  data handle\n");
        double minDepthVal = minDepthData.asDouble();

        //get max depth
        MDataHandle maxDepthData = data.inputValue(maxdepth, &returnStatus);
        McheckErr(returnStatus, "Error getting maxdepth  data handle\n");
        double maxDepthVal = maxDepthData.asDouble();

        // get height path data
        MDataHandle heightPathData = data.inputValue(hpath, &returnStatus);
        McheckErr(returnStatus, "Error getting height path data handle\n");
        MString heightPathVal = heightPathData.asString();

        // get foliage path
        MDataHandle foliagePathData = data.inputValue(fpath, &returnStatus);
        McheckErr(returnStatus, "Error getting foliage path data handle\n");
        MString foliagePathVal = foliagePathData.asString();     
      
        // Mesh output
        //get output object
        MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR getting geometry data handle\n");
        MFnMeshData dataCreator;
        MObject newOutputData = dataCreator.create(&returnStatus);
        McheckErr(returnStatus, "ERROR creating outputData");
        // mesh creation
        createMesh(timeVal, 256, 256, 256, minDepthVal, maxDepthVal, heightPathVal, newOutputData, returnStatus);
        McheckErr(returnStatus, "ERROR creating mesh");
        outputHandle.set(newOutputData);
        data.setClean(plug);

        MDataHandle pointsData = data.outputValue(outPoints, &returnStatus);
        McheckErr(returnStatus, "ERROR getting out points data handle\n");
        MFnArrayAttrsData pointsAAD;
        MObject pointsObject = pointsAAD.create();
        MVectorArray positionArray = pointsAAD.vectorArray("position");
        MVectorArray rotationArray = pointsAAD.vectorArray("rotation");
        MVectorArray scaleArray = pointsAAD.vectorArray("scale");
        MDoubleArray idArray = pointsAAD.doubleArray("id");

        int width = 0;
        int height = 0;
        int size = 0;

        std::string file_path = foliagePathVal.asChar();
        if (!file_path.empty())
        {
            char const* filep = file_path.c_str();
            foliageMap.load(filep);
            height = foliageMap.height(); // assumes square iamge
            width = foliageMap.width();
            size = height;
        }

        for (int px = 0; px < width; px++)
        {
            for (int pz = 0; pz < height; pz++)
            {
                int numTreesInCell = lookUpFoliageRChannel(px, pz);
                if (numTreesInCell > 0)
                {
                    double g = lookUpFoliageGChannel(px, pz);
                    double treeHeight = treeHeightVal * g;

                    for (int i = 0; i < numTreesInCell; i++)
                        {

                        double offset = remap(((double) rand() / (double) RAND_MAX), 0.0, -treeHeight / 4.0, 1.0, treeHeight / 4.0);
                        double treeHeightOffset = treeHeight + offset;

                        // taller trees rotate more
                        double maxRotX = treeHeightOffset * (10.0/3.0);
                        double minRotX = -maxRotX;
                        double maxRotZ = treeHeightOffset * (10.0 / 3.0);
                        double minRotZ = -maxRotZ;

                        double xRot = (maxRotX - minRotX) * ((double) rand() / (double) RAND_MAX) + minRotX;
                        double zRot = (maxRotZ - minRotZ) * ((double) rand() / (double) RAND_MAX) + minRotZ;

                        // generate a percent x and percent z offset in pixel space
                        double percentX = ((double) rand() / (double) RAND_MAX) - 0.5; 
                        double percentZ = ((double) rand() / (double) RAND_MAX) - 0.5;

                    //    double percentX = remap(((double) rand() / (double) RAND_MAX), 0.0, -1.0, 1.0, 1.0);
                      //  double percentZ = remap(((double) rand() / (double) RAND_MAX), 0.0, -1.0, 1.0, 1.0);

                        // is it px, px + .5, px - .5, or pz, pz + .5, pz - .5?

                        // center of pixel + percent offset = location in screen space
                        //int x0 = (int) floor(px + .5 + percentX);
                        //int x1 = x0 + (size / width);
                        //int z0 = (int) floor(pz + .5  + percentZ);
                        //int z1 = z0 + (size / width);
                        //double x0 = (px + .5 + percentX);
                        //double x1 = x0 + (size / width);
                        //double z0 = (pz + .5 + percentZ);
                        //double z1 = z0 + (size / width);

                        double x0 = (px  + percentX);
                        double x1 = x0 + (size / width);
                        double z0 = (pz + percentZ);
                        double z1 = z0 + (size / width);

                        double h00 = lookUpHeight(x0, z0);
                        double h01 = lookUpHeight(x0, z1);
                        double h10 = lookUpHeight(x1, z0);
                        double h11 = lookUpHeight(x1, z1);

                        double h0 = lerp(h00, h01, (px + percentX) - x0);
                        double h1 = lerp(h10, h11, (px  + percentX) - x0);
                        double h = lerp(h0, h1, (pz  + percentZ) - z0);

                        float worldTreeX = remap(px  + percentX, 0.0, -size / 2.0, 255.0, size / 2.0);
                        float worldTreeZ = remap(pz + percentZ, 0.0, -size / 2.0, 255.0, size / 2.0);

                       /*double h0 = lerp(h00, h01, (px + .5 + percentX) - x0);
                       double h1 = lerp(h10, h11, (px + .5 + percentX) - x0);
                       double h = lerp(h0, h1, (pz + .5 + percentZ) - z0);

                       double worldTreeX = remap(px + .5 + percentX, 0.0, -size / 2.0, 255.0, size / 2.0);
                       double worldTreeZ = remap(pz + .5 + percentZ, 0.0, -size / 2.0, 255.0, size / 2.0);*/

               //         // generate random rotation
               //       //  double xRot = (maxRotX - minRotX) * ((double) rand() / (double) RAND_MAX) + minRotX;
               //       //  double zRot = (maxRotZ - minRotZ) * ((double) rand() / (double) RAND_MAX) + minRotZ;

               //         // randomly place a tree inside the cell
               //         double worldTreeX = (maxWorldX - minWorldX) * ((double) rand() / (double) RAND_MAX) + minWorldX;
               //         double worldTreeZ = (maxWorldZ - minWorldZ) * ((double) rand() / (double) RAND_MAX) + minWorldZ;
               //  //       worldTreeX -= .5f;
               ////         worldTreeZ -= .5f;

               //         int x0 = (int) floor(worldTreeX + (size / 2));
               //         int x1 = x0 + (size / width);
               //         int z0 = (int) floor(worldTreeZ + (size / 2));
               //         int z1 = z0 + (size / height);

               //       /*  int x0 = (int) floor(worldTreeX + 128);
               //         int x1 = x0 + 1;
               //         int z0 = (int) floor(worldTreeZ + 128);
               //         int z1 = z0 + 1;*/

               //         double h00 = lookUpHeight(x0, z0);
               //         double h01 = lookUpHeight(x0, z1);
               //         double h10 = lookUpHeight(x1, z0);
               //         double h11 = lookUpHeight(x1, z1);

               //         double h0 = lerp(h00, h01, (worldTreeX + width) - x0);
               //         double h1 = lerp(h10, h11, (worldTreeX + width/2.0) - x0);
               //         double h = lerp(h0, h1, (worldTreeZ + height/2.0) - z0);
                        if (maxDepth > 0)
                        {
                            positionArray.append(MVector(worldTreeX, (float) h + (.25 * treeHeightOffset), worldTreeZ));
                            }
                        else {
                            positionArray.append(MVector(worldTreeX, (float) h, worldTreeZ));
                            }
                        
                        rotationArray.append(MVector(xRot, 0, zRot)); // max rotation in either direction should be 5 degrees
                        scaleArray.append(MVector(treeHeightOffset * .25, treeHeightOffset * .9, treeHeightOffset * .25));
                      //  rotationArray.append(MVector(0, 0, 0));


                        idArray.append(i);
                        }
                 }
            }
        }

            
        pointsData.setMObject(pointsObject);
        McheckErr(returnStatus, "ERROR creating outpoints");
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

double CIS660AuthoringToolNode::lookUpFoliageRChannel(double x, double z)
{
    if (foliageMap.is_empty())
    {
        return 0;
    }

    int px = floor(x);
    int pz = floor(z);

    double r = foliageMap(px, pz, 0, 0);
    return r;

}

double CIS660AuthoringToolNode::lookUpFoliageGChannel(double x, double z)
{
    if (foliageMap.is_empty())
        {
        return 0;
        }

    int px = floor(x);
    int pz = floor(z);

    double g = foliageMap(px, pz, 0, 1);

    // remap g from [0, 255] to [0, 1]
    double remapG = remap(g, 0.0, 0.0, 255.0, 1.0);
    return remapG;

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

    int r = floor(heightMap(px, pz, 0, 0));
    int g = floor(heightMap(px, pz, 0, 1));
    int b = floor(heightMap(px, pz, 0, 2));

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
    //MFnMesh fnPoly;

    iarr.clear();
    faceCounts.clear();
    faceConnects.clear();

    int sx = width;
    int sy = height;
    double size = s;

    std::string file_path = heightPath.asChar();
    if(!file_path.empty())
    {
        char const* fp = file_path.c_str();
        heightMap.load(fp);
        sx = heightMap.width();
        sy = heightMap.height();
        size = sx; // assumes square images
    }

    minDepth = min_depth;
    maxDepth = max_depth;


    createPlane(sx, sy, size);
    p_gons = NULL; // Is this still true if plane isn't just 2 by 2?

    //construct point array
    pa.clear();
    for (int i = 0; i < num_verts; i++) {
        pa.append(iarr[i]);
    }

    MObject newMesh = fnPoly.create(num_verts, num_faces, pa, faceCounts, faceConnects, outData, &stat);

    return newMesh;

}
