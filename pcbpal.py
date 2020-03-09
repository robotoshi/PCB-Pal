# gcode generator, Version 1.0
# Toshi Taperek - Group 13

from typing import List, Dict


class Tag:
    """ A simple class that just has a name, a dict of all its attributes, and a list of all its children tags.
        All docs that use the word "Tag" with a capital T to refer to this class
    """

    def __init__(self, name: str, attr: Dict[str, str] = None, children: List = None):
        self.name: str = name
        self.attr: Dict[str, str] = attr or {}
        self.children: List[Tag] = children or []

    def display(self):
        print("Name: ", self.name)
        if self.attr:
            print("Attributes: ", self.attr)
        else:
            print("No attributes")
        if self.children:
            for t in self.children:
                t.display()
        else:
            print("No children")


class Board:
    """ A Board object should contain all data needed to trace a board """

    def __init__(self, grid: Tag, plain: Tag, libraries: Tag, designrules: Tag, elements: Tag, signals: Tag):
        """ :type grid:        Tag for <grid>        Should not have any children
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

        self.traces: List[Tag] = []  # list of all trace line segments (<wire>) in absolute coords
        self.ends:   List[Tag] = []  # list of all wire termination points (<pad>, <via>, <smd>) in absolute coords

        # TODO: populate the two lists and two vars above ^

        # signals and things in <plain> are already in absolute coords
        # elements need to reference their corresponding package in the correct library to obtain the coordinate offset

        # layer 20 wires inside <plain> gives board dimensions


