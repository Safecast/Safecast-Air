from __future__ import print_function
import os
import sys
from py2gcode import gcode_cmd
from py2gcode import cnc_dxf

fileName = 'pm_bracket.dxf'

feedrate = 200.0
stockThickness = 0.25

prog = gcode_cmd.GCodeProg()
prog.add(gcode_cmd.GenericStart())
prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.FeedRate(feedrate))

param = {
        'fileName'    : fileName,
        'layers'      : ['bend'],
        'depth'       : 0.7*stockThickness,
        'startZ'      : 0.0,
        'safeZ'       : 0.5,
        'toolDiam'    : 0.25,
        'direction'   : 'ccw',
        'cutterComp'  : None,
        'maxCutDepth' : 0.03,
        'startDwell'  : 2.0,
        'startCond'   : 'minX',
        }
boundary = cnc_dxf.DxfBoundary(param)
prog.add(boundary)

prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.End(),comment=True)

baseName, ext = os.path.splitext(__file__)
outputFileName = '{0}.ngc'.format(baseName)
print('output writen to: {0}'.format(outputFileName))
prog.write(outputFileName)

