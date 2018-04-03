#define MNoVersionString
#define MNoPluginEntry

#include "CIS660AuthoringToolNode.h"

MTypeId CIS660AuthoringToolNode::id(0x8000);
MObject CIS660AuthoringToolNode::time;
MObject CIS660AuthoringToolNode::width;
MObject CIS660AuthoringToolNode::height;
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
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode time attribute\n");

    CIS660AuthoringToolNode::width = numAttr.MFnNumericAttribute::create("width", "w", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode width attribute\n");

    CIS660AuthoringToolNode::height = numAttr.MFnNumericAttribute::create("height", "h", MFnNumericData::kLong, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode height attribute\n");

    CIS660AuthoringToolNode::size = numAttr.MFnNumericAttribute::create("size", "s", MFnNumericData::kDouble, 0, &returnStatus);
    McheckErr(returnStatus, "ERROR creating CIS660AuthroingToolNode size attribute\n");

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
    returnStatus = addAttribute(CIS660AuthoringToolNode::size);
    McheckErr(returnStatus, "ERROR adding size attribute\n");
    returnStatus = addAttribute(CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR adding output mesh attribute\n");

    // attribute affects
    returnStatus = attributeAffects(CIS660AuthoringToolNode::width, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (width affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::height, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (height affecting outputMesh)\n");
    returnStatus = attributeAffects(CIS660AuthoringToolNode::size, CIS660AuthoringToolNode::outputMesh);
    McheckErr(returnStatus, "ERROR in attributeAffects (size affecting outputMesh)\n");
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
        int sizeVal = sizeData.asInt();

        //get output object
        MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
        McheckErr(returnStatus, "ERROR getting geometry data handle\n");


        MFnMeshData dataCreator;
        MObject newOutputData = dataCreator.create(&returnStatus);
        McheckErr(returnStatus, "ERROR creating outputData");

        createMesh(timeVal, widthVal, heightVal, sizeVal, newOutputData, returnStatus);
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

MObject CIS660AuthoringToolNode::createMesh(const MTime& time, const int& width, const int& height, const double& s, MObject& outData, MStatus& stat)
    {

    createPlane(width, height, s);
    p_gons = NULL; // Is this still true if plane isn't just 2 by 2?

    //construct point array
    pa.clear();
    for (int i = 0; i < num_verts; i++) {
        pa.append(iarr[i]);
        }

    MFnMesh meshFS;

    MObject newMesh = meshFS.create(num_verts, num_faces, pa, faceCounts, faceConnects, outData, &stat);

    return newMesh;



    /// Probably won't need this since p_gons is null? but keep commented out since pgons may not be null
    //// If we are using polygon data then set up the face connect array
    //// here. Otherwise, the create function will do it.
    ////
    //if (NULL != p_gons) {
    //    num_face_connects = num_faces * edges_per_face;
    //    num_edges = num_face_connects / 2;

    //    for (i = 0; i<num_faces; i++)
    //        faceCounts.append(edges_per_face);

    //    for (i = 0; i<(num_faces*edges_per_face); i++)
    //        faceConnects.append(p_gons[i] - 1);
    //    }

   /*// Call the poly creation method to create the polygon
        newTransform = fnPoly.create( num_verts, num_faces, pa,
                faceCounts, faceConnects, MObject::kNullObj, &stat );
        checkErr( stat, "Could not create MFnMesh" );

        // Primitive is created so tell shape it has changed
        //
        fnPoly.updateSurface();

        dgModifier.renameNode( newTransform, "pPrimitive1" );
        dgModifier.doIt();

        //
        // Put the polygon into a shading group
        MString cmd ("sets -e -fe initialShadingGroup ");
        cmd += fnPoly.name();
        dgModifier.commandToExecute( cmd );

        MFnDagNode fnDagNode( newTransform, &stat );
        if ( MS::kSuccess == stat )
        {
                cmd = "select ";
                cmd += fnDagNode.name();
                dgModifier.commandToExecute( cmd );
        }

        dgModifier.doIt();

        return MS::kSuccess;*/

    }