import sys
import os
from pcbpal import *
import brdParser
from typing import List


def wireToStr(trace: Tag):
    return "(" + trace.attr['x1'] + ", " + trace.attr['y1'] + ") to " + \
           "(" + trace.attr['x2'] + ", " + trace.attr['y2'] + ")"


def idRemove(list: List, item):
    for i, val in enumerate(list):
        if val is item:
            del list[i]
            return
    raise ValueError


# sys.stdout = open(os.devnull, 'w')
# board = brdParser.createBoard("sample.brd")
# sys.stdout = sys.__stdout__
# print(repr(board))



wires = [
    Tag("wire", {'x1': '34.0868', 'y1': '14.1732', 'x2': '34.036', 'y2': '14.224', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '35.052', 'y1': '15.24', 'x2': '35.052', 'y2': '15.367', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '34.036', 'y1': '14.224', 'x2': '35.052', 'y2': '15.24', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '33.9852', 'y1': '14.1732', 'x2': '34.163', 'y2': '14.1732', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '32.258', 'y1': '12.446', 'x2': '33.9852', 'y2': '14.1732', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '37.719', 'y1': '17.78', 'x2': '37.846', 'y2': '17.78', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '35.052', 'y1': '15.367', 'x2': '37.465', 'y2': '17.78', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '37.465', 'y1': '17.78', 'x2': '37.719', 'y2': '17.78', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '29.972', 'y1': '10.287', 'x2': '29.972', 'y2': '10.414', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '28.448', 'y1': '8.89', 'x2': '28.448', 'y2': '7.874', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '29.972', 'y1': '10.414', 'x2': '28.448', 'y2': '8.89', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '13.352', 'y1': '45.864', 'x2': '13.352', 'y2': '46.143', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '10.541', 'y1': '43.053', 'x2': '13.352', 'y2': '45.864', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '10.541', 'y1': '23.876', 'x2': '10.541', 'y2': '43.053', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '19.685', 'y1': '14.732', 'x2': '10.541', 'y2': '23.876', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '29.845', 'y1': '10.287', 'x2': '29.972', 'y2': '10.287', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '19.685', 'y1': '14.732', 'x2': '25.4', 'y2': '14.732', 'width': '0.4064', 'layer': '16'}),
    Tag("wire", {'x1': '25.4', 'y1': '14.732', 'x2': '29.845', 'y2': '10.287', 'width': '0.4064', 'layer': '16'})
]

endpoints: List[Endpoint] = []

for trace in wires:  # board.traces:
    if trace.name != 'wire':
        continue
    end1 = Endpoint(float(trace.attr['x1']), float(trace.attr['y1']), trace)
    end2 = Endpoint(float(trace.attr['x2']), float(trace.attr['y2']), trace, end1, False)
    end1.partner = end2
    endpoints.extend([end1, end2])


gcodes: List[str] = []
current_x = 0.0
current_y = 0.0
penDown = False

while len(endpoints) > 0:
    nearest = None
    min_dist = float('inf')
    for point in endpoints:
        dist = point.distFrom(current_x, current_y)
        if dist < min_dist:
            min_dist = dist
            nearest = point

    if not nearest.isAt(current_x, current_y):
        if penDown:
            penDown = False
            gcodes.append("M5")
        gcodes.append("G0 X " + str(nearest.x) + " Y " + str(nearest.y))
        current_x = nearest.x
        current_y = nearest.y

    if not penDown:
        penDown = True
        gcodes.append("M3")

    gcodes.extend(nearest.generateGCodes())
    current_x = nearest.partner.x
    current_y = nearest.partner.y
    idRemove(endpoints, nearest.partner)
    idRemove(endpoints, nearest)

for code in gcodes:
    print(code)
