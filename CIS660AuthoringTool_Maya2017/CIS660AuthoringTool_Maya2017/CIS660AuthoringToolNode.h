#pragma once
#include <maya/MPxNode.h>
#include <stdlib.h>

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

#include <maya/MFnArrayAttrsData.h>
#include <map>
#include <utility>
#include <sstream>
#include <maya/MGlobal.h>
#include <random>



#include "include/CImg.h"
using namespace cimg_library;


#define McheckErr(stat,msg)             \
        if ( MS::kSuccess != stat ) {   \
                cerr << msg;            \
                return MS::kFailure;    \
        }

inline double lerp(double a, double b, double t) { return t * b + (1.f - t) * a; }

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
        static MObject treeHeight;
        static MObject mindepth;
        static MObject maxdepth;
        static MObject hpath;
        static MObject fpath;
        static MObject outputMesh;

        static MObject outPoints;

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

        MFnMesh fnPoly;
        CImg<float> heightMap;
        CImg<float> foliageMap;

        double getGrayscale(int x, int z);

        void FILL(double x, double y, double z);
        double lookUpHeight(double x, double z);
        double remap(double value, double low1, double low2, double high1, double high2);

        double lookUpFoliageRChannel(double x, double z);
        double lookUpFoliageGChannel(double x, double z);

        double minDepth;// = 0;
        double maxDepth;// = 0;

        MObject newTransform;
        MDGModifier dgModifier;

    protected:
        MObject createMesh(const MTime& time, const int& width, const int& height, const double& s,
                           const double& min_depth, const double& max_depth, const MString& heightPath, MObject& outData, MStatus& stat);

    private:
        void createPlane(int width, int height, double s);
    };