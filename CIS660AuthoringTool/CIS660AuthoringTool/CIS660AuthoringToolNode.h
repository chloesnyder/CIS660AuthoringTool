#pragma once
#include <maya/MPxNode.h>

#include <maya/MFnPlugin.h>
#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>
#include <maya/MIOStream.h>

#include <sstream>
#include <maya/MGlobal.h>


#define McheckErr(stat,msg)             \
        if ( MS::kSuccess != stat ) {   \
                cerr << msg;            \
                return MS::kFailure;    \
        }

class CIS660AuthoringToolNode : public MPxNode
{
    public:
        CIS660AuthoringToolNode() {};
        virtual ~CIS660AuthoringToolNode() {};
        virtual MStatus compute(const MPlug& plug, MDataBlock& data);
        static void* creator() { return new CIS660AuthoringToolNode; }
        static MStatus initialize();

        static MTypeId id;
        static MObject time;
        static MObject width;
        static MObject height;
        static MObject size;
        static MObject outputMesh;

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

        MObject newTransform;
        MDGModifier dgModifier;

        void FILL(double x, double y, double z);

    protected:
        MObject createMesh(const MTime& time, const int& width, const int& height, const double& s, MObject& outData, MStatus& stat);
    
    private:
        void createPlane(int width, int height, double s);
};