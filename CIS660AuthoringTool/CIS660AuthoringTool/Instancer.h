#pragma once
#include <maya/MPxNode.h>
#include <stdlib.h>
#include <maya/MTime.h>
#include <maya/MVectorArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MIOStream.h>

#include <maya/MFnArrayAttrsData.h>

#include <sstream>
#include <maya/MGlobal.h>



#include "include/CImg.h"
using namespace cimg_library;


#define McheckErr(stat,msg)             \
        if ( MS::kSuccess != stat ) {   \
                cerr << msg;            \
                return MS::kFailure;    \
        }



class instancerNode  : public MPxNode
    {
    public:
        instancerNode() {};
        virtual ~instancerNode() {};
        virtual MStatus compute(const MPlug& plug, MDataBlock& data);
        static void* creator() { return new instancerNode; }
        static MStatus initialize();

        static MTypeId id;
        static MObject time;
        static MObject width;
        static MObject height;
        static MObject mindepth;
        static MObject maxdepth;
        static MObject hpath;
        static MObject size;

        static MObject outPoints;
        static MObject inNumPoints;

        CImg<float> heightMap;

        double lookUpHeight(double x, double z);
        double remap(double value, double low1, double low2, double high1, double high2);

        double minDepth = 0;
        double maxDepth = 100;

        MObject createInstancesOfObject(const MTime& time, const int& width, const int& height, const double& s,
                                                                            const double& min_depth, const double& max_depth, const int& in_num_points, MObject& newOutPointData, MStatus& stat);
    };

