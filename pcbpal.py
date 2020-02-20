class Tag:
    """ A simple class that just has a name, a dict of all its attributes, and a list of all its children tags.
        All docs that use the word "Tag" with a capital T to refer to this class
    """
    def __init__(self, name, attr, children):
        self.name = name           # a string
        self.attr = attr           # a dict
        self.children = children   # a list

class Board:
    """ A Board object should contain all data needed to trace a board """

    def __init__(self, grid, plain, libraries, designrules, elements, signals):
        """ grid:        a Tag object for the <grid>        tag.  Should not have any children
            plain:       a Tag object for the <plain>       tag.  Only include children on layers 1, 16, or 20 (might change my mind later)
            libraries:   a Tag object for the <library>     tag.  Ignore the <packages> container tag
            designrules: a Tag object for the <designrules> tag.  Should probably just be <param> Tags
            elements:    a Tag object for the <elements>    tag.  Ignore the <attribute> tags and pretend <element> tags have no children
            signals:     a Tag object for the <signals>     tag.  Ignore <contactref> tags and <signal> tags. Only look at lowest level children

            For now, ignore <description> tags and all tags that have a layer attribute that is not 1, 16, or 20
        """
        self.grid = grid
        self.plain = plain
        self.libraries = libraries
        self.designrules = designrules
        self.elements = elements
        self.signals = signals

        self.length = 0   # dimesntions of the board's bounding square
        self.width  = 0

        self.traces = []  # list of all trace line segments (<wire>) in absolute coords
        self.ends   = []  # list of all wire termination points (<pad>, <via>, <smd>) in absolute coords

        ### TODO: populate the two lists and two vars above ^ ###

        # signals and things in <plain> are already in abolute coords
        # elements need to reference their corresponding package in the correct library to obtain the coordinate offset

        # layer 20 inside <plain> gives board dimentions