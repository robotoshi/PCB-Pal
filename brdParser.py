# .brd File Parser, Version 1.0
# Patrick Robert Willmann - Group 13 

class fileManager: 

    def __init__ (self, fileName):
        self.fileName = fileName
    
    def loadFile (self):
        self.fileObject = open (self.fileName, "r")
        self.tempStorage = self.fileObject.read()
    
    def closeFile (self):
        self.fileObject.close()

class brdParser:

    def __init__ (self, inputString):
        self.inputString = inputString
        import xml.etree.ElementTree as ET
        root = ET.fromstring(self.inputString)
        



path = input("Enter the file path:\n")
fileIn = fileManager(path)
fileIn.loadFile()
## print (fileIn.tempStorage)