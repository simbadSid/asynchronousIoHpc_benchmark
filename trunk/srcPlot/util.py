'''
Created on Mar 24, 2017

@author: kassuskley
'''

from __future__ import print_function
import os
import sys


# ---------------------------------------
# Global parameters
# ---------------------------------------
LIST_SEPARATOR      = ','
DEFAULT_COMMENT     = "#"


# ---------------------------------------
# Global methods
# ---------------------------------------
def loggerError(msg, param=None, exitNow=False):
    print ("\n\n***********************************", file=sys.stderr)
    if (param == None):
        print (msg, file=sys.stderr)
    else:
        print (msg + ": " + param, file=sys.stderr)
    print ("***********************************", file=sys.stderr)

    if (exitNow == True):
        exit(12)


def findIndexInList(list, valueToLook):
    for i in xrange(len(list)):
        if (list[i] == valueToLook):
            return i
    return None


def copyExceptInd(list, id):
    assert(id >= 0)
    assert(id < len(list))
    return list[:id] + list[id+1:]


def equal (a, b):
    return (a == b)
def find(scalar, vector, equalFunc=equal):
    for i in xrange(len(vector)):
        if (equalFunc(scalar, vector[i])):
            return i
    return -1


# ---------------------------------------
# Data file parser
# ---------------------------------------
def isEndOfFile(fic):
    return (fic.tell() == os.fstat(fic.fileno()).st_size)


def nextMeaningfullLine(fic, commentString=DEFAULT_COMMENT, raiseExceptionIfNon=True):
    while (not isEndOfFile(fic)):
        res = fic.readline().strip()
        if ((commentString != None) and (res.startswith(commentString))):
            continue
        elif (res == "\n" or res == ""):
            continue
        else:
            return res
    if (raiseExceptionIfNon):
        raise Exception("No useful string found in the file " + fic.name)
    else:
        return None


def nextArrayLine(fic, separator=LIST_SEPARATOR):
    return nextMeaningfullLine(fic).split(separator)


def strPad(strInput, nbrPadChar, endChar=None, padChar=' '):
    res = str(strInput)
    if (len(res) > nbrPadChar):
        return res[:nbrPadChar]
    for i in xrange(nbrPadChar - len(str(strInput))):
        res += padChar
    if (endChar != None):
        res += endChar
    return res

def generateMappedRandomColor():
# TODO change to a result mapped to an f(input)
    import numpy as np
    col = np.random.rand(3,1)

def stringPrecision(alpha, precision):
    strAlpha    = str(alpha)
    strAlphaList= strAlpha.split('e')
    if (len(strAlphaList) > 1):
        expo = 'e'+strAlphaList[1]
    else:
        expo = None
        
    strAlphaList= strAlpha.split('.')
    if (len(strAlphaList) > 1):
        strAlpha = strAlphaList[0] + '.' + (strAlphaList[1])[:precision]

    if (not expo == None):
        strAlpha = strAlpha + expo
    return strAlpha

