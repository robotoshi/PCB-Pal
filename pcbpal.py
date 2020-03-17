# gcode generator, Version 1.0
# Toshi Taperek - Group 13
from __future__ import annotations

from typing import List, Dict
import sys
import signal
from math import sqrt, isclose


class Tag:
    """ A simple class that just has a name, a dict of all its attributes, and a list of all its children tags.
        All docs that use the word "Tag" with a capital T to refer to this class
    """

    def __init__(self, name: str, attr: Dict[str, str] = None, children: List[Tag] = None):
        self.name: str = name
        self.attr: Dict[str, str] = attr or {}
        self.children: List[Tag] = children or []

    def __str__(self):
        output = "<" + self.name + ">\t"
        if self.attr:
            output += "Attributes: " + str(self.attr) + "\n"
        else:
            output += "No attributes\n"

        if self.children:
            for child in self.children:
                output += str(child).replace("<", "\t<")
        return output


class Endpoint:
    """ A representation of an ordered pair of xy coordinates.  Could be either end of a wire, or a single endpoint.
        If it is one end of a 2-ended segment (i.e. a wire), the member variable "partner" will reference the other
        endpoint associated with the segment, or self otherwise.
    """

    def __init__(self, x: float, y: float, tag: Tag, partner: Endpoint = None, isStart: bool = True):
        self.x: float = x
        self.y: float = y
        self.tag: Tag = tag
        self.isStart: bool = isStart
        # = '1' if isStart else '2'
        self.partner: Endpoint = partner or self

    @property
    def partner(self):
        return self.__partner

    @partner.setter
    def partner(self, p):
        self.__partner = p

    def distFrom(self, x: float, y: float) -> float:
        return sqrt((x - self.x) ** 2 + (y - self.y) ** 2)

    # def distFrom(self, other: Endpoint) -> float:
    #     return self.distFrom(other.x, other.y)

    def generateGCodes(self) -> List[str]:
        if self.tag.name == 'wire':
            codes = ["G1 X " + str(self.partner.x) + " Y " + str(self.partner.y)]
        else:
            raise NotImplementedError
        return codes

    def isAt(self, x: float, y: float):
        # TODO: pick good tolerances
        return isclose(x, self.x, rel_tol=0.05) and isclose(y, self.y, rel_tol=0.05)

    def __str__(self):
        return "(" + str(self.x) + ", " + str(self.y) + ") to " + \
               "(" + str(self.partner.x) + ", " + str(self.partner.y) + ")"

    def __eq__(self, rhs):
        return self.x == rhs.x and self.y == rhs.y


class Board:
    """ A Board object should contain all data needed to trace a board """

    def __init__(self, grid: Tag, plain: Tag, libraries: Tag, designrules: Tag, elements: Tag, signals: Tag):
        """ Create a board object from
            :type grid:        Tag for <grid>        Should not have any children
            :type plain:       Tag for <plain>       Only include children on layers 1, 16, or 20 (might change my mind later)
            :type libraries:   Tag for <library>     Ignore the <packages> container tag
            :type designrules: Tag for <designrules> Should probably just be <param> Tags
            :type elements:    Tag for <elements>    Ignore the <attribute> tags and pretend <element> tags have no children
            :type signals:     Tag for <signals>     Ignore <contactref> tags and <signal> tags. Only look at lowest level children

            For now, ignore <description> tags and all tags that have a layer attribute that is not 1, 16, or 20
        """
        self.grid:        Tag = grid
        self.plain:       Tag = plain
        self.libraries:   Tag = libraries
        self.designrules: Tag = designrules
        self.elements:    Tag = elements
        self.signals:     Tag = signals

        self.length: float = 0  # dimensions of the board's bounding square
        self.width:  float = 0

        self.traces: List[Endpoint] = []  # list of all trace endpoints

        # TODO: populate the list and two vars above ^

        # signals and things in <plain> are already in absolute coords
        # elements need to reference their corresponding package in the correct library to obtain the coordinate offset

        # layer 20 wires inside <plain> gives board dimensions

    def __str__(self):
        return "length: " + str(self.length) + "\n" + \
               "width: " + str(self.width) + "\n" + \
               "traces:\n" + str(self.traces) + "\n"

    def __repr__(self):
        return self.__str__() + "\n" + str(self.grid) + str(self.plain) + str(self.libraries) + \
               str(self.designrules) + str(self.elements) + str(self.signals)


def eprint(*args, **kwargs):
    """Print to stderr"""
    print(*args, file=sys.stderr, **kwargs)


def openfile(name):
    """Open a file with the given name and return the handle. Exit on failure."""
    try:
        return open(name)
    except FileNotFoundError:
        eprint("No such file.  Exiting...")
        exit(2)


def signal_handler(sig, frame):
    """Handle a keyboard interrupt signal (SIGINT or Ctrl-C) and suppress the stacktrace."""
    print('\n\nReceived Ctrl-C.  Exiting...')
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)
