#include "Instancer.h"


MTypeId instancerNode::id(0x9000);
MObject instancerNode::time;
MObject instancerNode::width;
MObject instancerNode::height;
MObject instancerNode::mindepth;
MObject instancerNode::maxdepth;
MObject instancerNode::hpath;
MObject instancerNode::size;

MObject instancerNode::outPoints;
MObject instancerNode::inNumPoints;

double  instancerNode::lookUpHeight(double x, double z)
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

double  instancerNode::remap(double value, double low1, double low2, double high1, double high2)
{
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}


MStatus instancerNode::initialize()
    {
    MFnUnitAttribute unitAttr;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numAttr;

    MStatus returnStatus;

    // init attributes

    instancerNode::time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode time attribute\n");

    instancerNode::width = numAttr.MFnNumericAttribute::create("width", "w", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode width attribute\n");

    instancerNode::height = numAttr.MFnNumericAttribute::create("height", "h", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode height attribute\n");

    instancerNode::size = numAttr.MFnNumericAttribute::create("size", "s", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode size attribute\n");

    instancerNode::mindepth = numAttr.MFnNumericAttribute::create("mindepth", "mnd", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode min depth attribute\n");

    instancerNode::maxdepth = numAttr.MFnNumericAttribute::create("maxdepth", "mxd", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode max depth attribute\n");

    instancerNode::hpath = typedAttr.MFnTypedAttribute::create("hpath", "hp", MFnData::kString, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode height path attribute\n");

    instancerNode::inNumPoints = numAttr.MFnNumericAttribute::create("inputPoints", "ip", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringToolNode inputPoints attribute\n");

    instancerNode::outPoints = typedAttr.create("outPoints", "out_p", MFnArrayAttrsData::kDynArrayAttrs, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthoringtoolNode output points attribute\n");
    CHECK_MSTATUS(typedAttr.setStorable(false));
    CHECK_MSTATUS(typedAttr.setReadable(true));
    CHECK_MSTATUS(typedAttr.setKeyable(false));
    CHECK_MSTATUS(typedAttr.setWritable(false));

    // add attributes
    returnStatus = addAttribute(instancerNode::time);
    McheckErr(returnStatus, "ERROR adding time attribute\n");
    returnStatus = addAttribute(instancerNode::width);
    McheckErr(returnStatus, "ERROR adding width attribute\n");
    returnStatus = addAttribute(instancerNode::height);
    McheckErr(returnStatus, "ERROR adding height attribute\n");
    returnStatus = addAttribute(instancerNode::size);
    McheckErr(returnStatus, "ERROR adding size attribute\n");
    returnStatus = addAttribute(instancerNode::mindepth);
    McheckErr(returnStatus, "ERROR adding min depth attribute\n");
    returnStatus = addAttribute(instancerNode::maxdepth);
    McheckErr(returnStatus, "ERROR adding max depth attribute\n");
    returnStatus = addAttribute(instancerNode::hpath);
    McheckErr(returnStatus, "ERROR adding height path attribute\n");
    returnStatus = addAttribute(instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR adding output mesh attribute\n");
    returnStatus = addAttribute(instancerNode::inNumPoints);
    McheckErr(returnStatus, "ERROR adding in points attribute\n");


    // attribute affects
    returnStatus = attributeAffects(instancerNode::width, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (width affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::height, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (height affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::size, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (size affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::mindepth, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (mindepth affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::maxdepth, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (maxdepth affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::hpath, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (hpath affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::time, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (time affecting outputMesh)\n");
    returnStatus = attributeAffects(instancerNode::inNumPoints, instancerNode::outPoints);
    McheckErr(returnStatus, "ERROR in attributeAffects (inNumPoints affecting outPoints)\n");


    return MS::kSuccess;
}

MStatus instancerNode::compute(const MPlug& plug, MDataBlock& data)
    {


    MStatus returnStatus;
    if (plug == outPoints)
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

        // get num points
        MDataHandle inNumPointsData = data.inputValue(inNumPoints, &returnStatus);
        McheckErr(returnStatus, "Error getting num points data handle\n");
        int inNumPointsVal = inNumPointsData.asInt();

        //get output object
        MDataHandle outputPointsHandle = data.outputValue(outPoints, &returnStatus);
        McheckErr(returnStatus, "ERROR getting geometry data handle\n");

        MFnArrayAttrsData pointDataCreator;
        MObject newOutPointData = pointDataCreator.create(&returnStatus);
        McheckErr(returnStatus, "ERROR creating newOutPointData");

        //random point distribution
        createInstancesOfObject(timeVal, widthVal, heightVal, sizeVal, minDepthVal, maxDepthVal, inNumPointsVal, newOutPointData, returnStatus);
        McheckErr(returnStatus, "ERROR creating outpoints");

        outputPointsHandle.set(newOutPointData);
        data.setClean(plug);

        }

    return MS::kSuccess;
}

MObject instancerNode::createInstancesOfObject(const MTime& time, const int& width, const int& height, const double& s,
                                                         const double& min_depth, const double& max_depth, const int& in_num_points, MObject& newOutPointData, MStatus& stat)
    {

    // Fix what's happening with pointsData https://github.com/chloesnyder/CIS660HW3/blob/master/randomNode.py
    //data.outputValue(newOutPointData);

    MFnArrayAttrsData pointsAAD;
    MObject pointsObject = pointsAAD.create();
    MVectorArray positionArray = pointsAAD.vectorArray("position");
    MDoubleArray idArray = pointsAAD.doubleArray("id");

    // loop to fill the arrays
    for (int i = 0; i < in_num_points; i++)
        {
        // randomly generate an x coord and z coord within [0, width] and [0, height], then remap
        // look up the height in the image for y coord
        int rx = rand() % width;
        int rz = rand() % height;
        double remapX = remap(rx, (-s / 2.0), 0.0, (s / 2.0), 255.0);
        double remapZ = remap(rz, (-s / 2.0), 0.0, (s / 2.0), 255.0);
        double y = lookUpHeight(remapX, remapZ);

        positionArray.append(MVector(remapX, y, remapZ));
        idArray.append(i);
        }

    newOutPointData = pointsObject;

    return newOutPointData;
    }