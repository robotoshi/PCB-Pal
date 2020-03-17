# .brd File Parser, Version 1.0
# Patrick Robert Willmann - Group 13

from pcbpal import *
import xml.etree.ElementTree as ET


class FileManager:

    def __init__(self, filename):
        self.fileName = filename

    def loadFile(self):
        self.fileObject = open(self.fileName, "r")
        self.tempStorage = self.fileObject.read()

    def closeFile(self):
        self.fileObject.close()


class BrdParser:

    """ .somethingTop nomenclature refers to the xml.etree element dictionary
        .somethingTag refers to the output dictionary which will be passed to the final Board object
    """

    def __init__(self, inputstring):
        self.inputString = inputstring
        self.root = ET.fromstring(self.inputString)
        print("\nTop Level Tag is: " + self.root.tag, self.root.attrib)
        self.gridTop = self.root.find('.//grid')
        print(self.gridTop)
        self.plainTop = self.root.find('.//plain')
        print(self.plainTop)
        self.designRulesTop = self.root.find('.//designrules')
        print(self.designRulesTop)
        self.signalsTop = self.root.find('.//signals')
        print(self.signalsTop)
        self.elementsTop = self.root.find('.//elements')
        print(self.elementsTop)
        self.librariesTop = self.root.find('.//libraries')
        print(self.librariesTop)

        self.gridTag = Tag("grid")
        self.plainTag = Tag("plain")
        self.designRulesTag = Tag("designrules")
        self.signalsTag = Tag("signals")
        self.elementsTag = Tag("elements")
        self.librariesTag = Tag("libraries")

        self.place(self.gridTop, self.gridTag)
        self.place(self.plainTop, self.plainTag)
        self.place(self.designRulesTop, self.designRulesTag)
        self.place(self.signalsTop, self.signalsTag)
        self.place(self.elementsTop, self.elementsTag)
        self.place(self.librariesTop, self.librariesTag)

    def place(self, elementIn: ET.Element, tag: Tag):
        print("Placing... ", elementIn.tag)
        tag.attr = elementIn.attrib
        if tag.attr:
            print(tag.attr)
        else:
            print("No Attributes")
        for child in elementIn:
            tag.children.append(Tag(child.tag, child.attrib))
            print(tag.children[-1].name, tag.children[-1].attr)
            if len(list(child)):
                self.place(child, tag.children[-1])
        print("Successfully placed: ", tag.name)


def createBoard(path) -> Board:
    fileIn = FileManager(path)
    fileIn.loadFile()
    parser = BrdParser(fileIn.tempStorage)
    fileIn.closeFile()
    board = Board(parser.gridTag, parser.plainTag, parser.librariesTag, parser.designRulesTag, parser.elementsTag, parser.signalsTag)
    return board


if __name__== "__main__":
    filepath = input("Enter the file path:\n")
    # path = "sample.brd"
    board = createBoard(filepath)
    print(board)
