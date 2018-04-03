#pragma once
#ifndef CreateCIS660AuthoringToolCmd_H_
#define CreateCIS660AuthoringToolCmd_H_

#include <maya/MPxCommand.h>
#include <string>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <list>
#include <sstream>

#define WIDTH "-w"
#define WIDTH_LONG "-width"
#define HEIGHT "-h"
#define HEIGHT_LONG "-height"

class CIS660AuthoringToolCmd : public MPxCommand
{
public:
    virtual ~CIS660AuthoringToolCmd();
    static void* creator() { return new CIS660AuthoringToolCmd(); }
    MStatus doIt(const MArgList& args);
    MStatus createPlane(int x, int y);

    static MSyntax newSyntax() {
        MSyntax syntax;
        syntax.addFlag(WIDTH, WIDTH_LONG, MSyntax::kLong);
        syntax.addFlag(HEIGHT, HEIGHT_LONG, MSyntax::kLong);
    }
};

#endif