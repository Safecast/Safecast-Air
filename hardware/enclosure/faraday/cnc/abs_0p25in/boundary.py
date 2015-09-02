from __future__ import print_function
import os 
import sys
from py2gcode import gcode_cmd
from py2gcode import cnc_dxf

fileName = 'layout.dxf'
feedrate = 100.0
depth = 0.28
startZ = 0.0
safeZ = 0.5
maxCutDepth = 0.10
toolDiam = 3.0/16.0 
direction = 'ccw'
cutterComp = 'outside'
startDwell = 1.0
startCond = 'minY'
layerList = ['screen_pocket', 'boundary']

prog = gcode_cmd.GCodeProg()
prog.add(gcode_cmd.GenericStart())
prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.FeedRate(feedrate))
prog.add(gcode_cmd.PathBlendMode(p=0.02,q=0.01))


innerParam = {
        'fileName'    : fileName,
        'layers'      : ['screen_pocket'],
        'depth'       : depth,
        'startZ'      : startZ,
        'safeZ'       : safeZ,
        'toolDiam'    : toolDiam,
        'direction'   : direction,
        'cutterComp'  : 'inside',
        'maxCutDepth' : maxCutDepth,
        'startDwell'  : startDwell, 
        'startCond'   : startCond,
        }
innerBoundary = cnc_dxf.DxfBoundary(innerParam)
prog.add(innerBoundary)

outerParam = {
        'fileName'    : fileName,
        'layers'      : ['boundary'],
        'depth'       : depth,
        'startZ'      : startZ,
        'safeZ'       : safeZ,
        'toolDiam'    : toolDiam,
        'direction'   : direction,
        'cutterComp'  : 'outside',
        'maxCutDepth' : maxCutDepth,
        'startDwell'  : startDwell, 
        'startCond'   : startCond,
        }
outerBoundary = cnc_dxf.DxfBoundary(outerParam)
prog.add(outerBoundary)

prog.add(gcode_cmd.ExactPathMode())

prog.add(gcode_cmd.Space())
prog.add(gcode_cmd.End(),comment=True)
baseName, dummy = os.path.splitext(__file__)
fileName = '{0}.ngc'.format(baseName)
print('generating: {0}'.format(fileName))
prog.write(fileName)
