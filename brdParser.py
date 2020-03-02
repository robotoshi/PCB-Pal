# .brd File Parser, Version 1.0
# Patrick Robert Willmann - Group 13

from pcbpal import *

class fileManager:

    def __init__(self, fileName):
        self.fileName = fileName

    def loadFile(self):
        self.fileObject = open(self.fileName, "r")
        self.tempStorage = self.fileObject.read()

    def closeFile(self):
        self.fileObject.close()


class brdParser:

    def __init__(self, inputstring):
        self.inputString = inputstring
        import xml.etree.ElementTree as ET
        self.root = ET.fromstring(self.inputString)
        self.findtops()
        self.placegrid()
        self.placeplain()

    def findtops(self):
        print("Top Level Tag is: " + self.root.tag, self.root.attrib)
        # Use this technique to find various individual Element objects. This will return the first Element found!
        self.plainTop = self.root.find('.//plain')
        print(self.plainTop)
        self.gridTop = self.root.find('.//grid')
        print(self.gridTop)
        self.designRulesTop = self.root.find('.//designrules')
        print(self.designRulesTop)
        self.signalsTop = self.root.find('.//signals')
        print(self.signalsTop)
        # This is a LIST of elements - can iterate through list and still treat each item as an Element object
        self.libraryTop = self.root.findall('.//library')
        print(self.libraryTop)

    def placegrid(self):
        self.gridTag = Tag("grid")
        print("Placing... " + self.gridTag.name)
        self.gridTag.attr = self.gridTop.attrib
        print(self.gridTag.attr)
        print("... successfully placed")

    def placeplain(self):
        self.plainTag = Tag("plain")
        print("Placing... " + self.plainTag.name)
        for child in self.plainTop:
            print(child.tag, child.attrib)
            self.plainTag.children = Tag(child.tag, child.attrib)


path = "sample.brd"
# path = input("Enter the file path:\n")
fileIn = fileManager(path)
fileIn.loadFile()
parser = brdParser(fileIn.tempStorage)
fileIn.closeFile()
# board = Board(things go here yes)


