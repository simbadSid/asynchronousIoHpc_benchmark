'''
Created on Apr 21, 2017

@author: kassuskley
'''

import sys
from util import *
import re



# ---------------------------------------
# Local variables
# ---------------------------------------
ARGUMENT_INPUT_HELP             = "-help"
ARGUMENT_INPUT_DIVIDE           = "-divide="
ARGUMENT_INPUT_DIVIDE_AND_ROW   = "-divideAndRow="

KEY_WORLD                       = "(id=XXXXXXX)"


# ---------------------------------------
# Local methods
# ---------------------------------------
def printHelp(progName):
    print "Usage: " + progName + ": [options] <cube_dump outputFileName> <list of cube_dump entry id>"
    print "Options:"
    print "\t" + ARGUMENT_INPUT_DIVIDE          + "<factor>\tTo divide each result by the given factor"
    print "\t" + ARGUMENT_INPUT_DIVIDE_AND_ROW  + "<factor>\tTo divide each result by the given factor and add both the result anf the row data"
    print "\t" + ARGUMENT_INPUT_HELP            + "\t\t\tTo print the current help"
    exit(1)


def parseInputArg(argList):
    divideFacor = None
    divideAndRow= None
    nbOption    = 0
    for arg in argList[1:]:
        if (arg == ARGUMENT_INPUT_HELP):
            printHelp(argList[0])
            exit()
        elif (arg.startswith(ARGUMENT_INPUT_DIVIDE)):
            nbOption += 1
            divideFacor = float(arg[len(ARGUMENT_INPUT_DIVIDE):])
            divideAndRow= False
        elif (arg.startswith(ARGUMENT_INPUT_DIVIDE_AND_ROW)):
            nbOption += 1
            divideFacor = float(arg[len(ARGUMENT_INPUT_DIVIDE_AND_ROW):])
            divideAndRow= True
        else:
            break

    if (len(argList)-nbOption < 3):
        printHelp(argList[0])

    cubeDumpFileName    = argList[1+nbOption]
    idList              = argList[1+nbOption+1:]

    assert(cubeDumpFileName != None)
    return (cubeDumpFileName, idList, divideFacor, divideAndRow)


##
# \brief Parse a line of the cube_dump result file.
# \returns The value corresponding to one of the id into idList
# \Warning: this function is very specific to the type of output returned by cube_dump.   It needs to be adapted at each new cube_dump version 
#TODO Find a more reliable way to do it
def findKeyValue(line, idList):
    array = line.split("(id=")
    if (len(array) != 2):
        return (None, None)
    array = array[1].split(")")
    if (len(array) != 2):
        return (None, None)
    
    
    
    for i in idList:
        if (i == array[0]):
            res = 0
            valueList = array[1].split()
            for val in valueList:
                res += float(val)
            return (i, res)
    return (None, None)


def parseCubeDumpFile(cubeDumpFileName, idList):
    res = []

    try:
        cubeDumpFile = open(cubeDumpFileName, 'r')
    except:
        loggerError(msg="Failed to open the cube_dump result file", param=cubeDumpFileName, exitNow=True)
    while(not isEndOfFile(cubeDumpFile)):
        line            = nextMeaningfullLine(cubeDumpFile, raiseExceptionIfNon=False)
        (id, value)     = findKeyValue(line, idList)
        if (value != None):
            res.append((id, value))

    if (len(idList) != len(res)):
        loggerError(msg="Failed while parsing the cube_dump result file"+cubeDumpFileName+ ".\nCan't find all the results corresponding to the id",
                     param="Looking for " + str(idList) + " and found " + str(res), exitNow=True)

    cubeDumpFile.close()
    resSorted = []
    for i in idList:
        found = False
        for (id, value) in res:
            if (id == i):
                resSorted.append(value)
                found = True
                break
        if (not found):
            loggerError(msg="The id \"" + str(id) + "\" in the cube_dump result file has multiple lines", param=cubeDumpFileName, exitNow=True)
        
    return resSorted

# ---------------------------------------
# Main method
# ---------------------------------------
if __name__ == "__main__":
    (cubeDumpFileName, idList, divideFacor, divideAndRow)   = parseInputArg(sys.argv)
    keyValueList_row                                        = parseCubeDumpFile(cubeDumpFileName, idList)
    if (divideFacor == None):
        keyValueList = keyValueList_row
    else:
        if (divideAndRow == True):
            keyValueList = []
            for val in keyValueList_row:
                keyValueList.append(val)
                keyValueList.append(val/divideFacor)
        else:
            keyValueList = [val/divideFacor for val in keyValueList_row]

    for i in xrange(len(keyValueList)):
        keyValue = keyValueList[i]
        sys.stdout.write(str(keyValue))
        if (i < len(keyValueList)-1):
            sys.stdout.write(", ")
    sys.stdout.write("\n")
    sys.stdout.flush()

