#define MNoVersionString
#define MNoPluginEntry

#include "CIS660AuthoringToolNode.h"

MTypeId CIS660AuthoringToolNode::id(0x8000);
MObject CIS660AuthoringToolNode::time;
MObject CIS660AuthoringToolNode::width;
MObject CIS660AuthoringToolNode::height;
MObject CIS660AuthoringToolNode::outputMesh;

MStatus CIS660AuthoringToolNode::initialize()
{
    MFnUnitAttribute unitAttr;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numAttr;

    MStatus returnStatus;

    // init attributes
    CIS660AuthoringToolNode::time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode time attribute\n");

    CIS660AuthoringToolNode::width = numAttr.MFnNumericAttribute::create("width", "w", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode width attribute\n");

    CIS660AuthoringToolNode::width = numAttr.MFnNumericAttribute::create("height", "h", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode height attribute\n");

    CIS660AuthoringToolNode::outputMesh = typedAttr.create("outputMesh", "out", MFnData::kMesh, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode output mesh attribute\n");
    CHECK_MSTATUS(typedAttr.setStorable(false));

    // add attributes
    returnStatus = addAttribute(CIS660AuthoringToolNode::time);
    McheckErr(returnStatus, "ERROR adding time attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::width);
    McheckErr(returnStatus, "ERROR adding width attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::height);
    McheckErr(returnStatus, "ERROR adding height attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR adding output mesh attribute\n");

    // attribute affects
    returnStatus = attributeAffects(CIS660AuthoringToolNode::width, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (width affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::height, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (height affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::time, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (time affecting outputMesh)\n");

    return MS::kSuccess;
}

MStatus CIS660AuthoringToolNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus returnStatus;
    if(plug == outputMesh)
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
        MDataHandle heightData = data.inputValue(width, &returnStatus);
        McheckErr(returnStatus, "Error getting height data handle\n");
        int heightVal = heightData.asInt();
        
        //get output object
        MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR getting geometry data handle\n");


        MFnMeshData dataCreator;
        MObject newOutputData = dataCreator.create(&returnStatus);
        McheckErr(returnStatus, "ERROR creating outputData");

        createMesh(timeVal, widthVal, heightVal, newOutputData, returnStatus);
        McheckErr(returnStatus, "ERROR creating mesh");

        outputHandle.set(newOutputData);
        data.setClean(plug);

    }
    return MS::kSuccess;
}

MObject CIS660AuthoringToolNode::createMesh(const MTime& time, const int& width, const int& height, MObject& outData, MStatus& stat)
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

    // How do I make this just call the mel commands from AuthoringToolCmd?



   // MFnMesh meshFS;
   // MPointArray points;
   // MIntArray faceCounts;

}