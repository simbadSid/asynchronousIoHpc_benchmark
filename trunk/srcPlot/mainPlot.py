'''
Created on Apr 21, 2017

@author: kassuskley
'''

import matplotlib as mpl
from asyncore import write
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FixedLocator, FormatStrFormatter
import numpy as np
import sys

from data import Data
from util import loggerError, LIST_SEPARATOR, find, generateMappedRandomColor, stringPrecision, strPad




PLOT_LIB_LIST = ['TkAgg', 'GtkAgg', 'Agg']
for lib in PLOT_LIB_LIST:
    try:
        mpl.use(lib)
        break
    except:
        print "Failed to use library: " + lib
        if (lib == PLOT_LIB_LIST[len(PLOT_LIB_LIST)-1]):
            print "No Python graphical backend library found"











# ---------------------------------------
# Local variables
# ---------------------------------------
ARGUMENT_INPUT_HELP                 = "-help"
ARGUMENT_PLOT_TYPE                  = "-plotType="
ARGUMENT_LOG_X                      = "-logScaleX"
ARGUMENT_LOG_Y                      = "-logScaleY"
ARGUMENT_RESULT_DIM_TEXT            = "-resultDimText="
ARGUMENT_MULTIPLE_TRY               = "-multipleTry"
ARGUMENT_ALL_PROJECTION_IN_1_FRAME  = "-allProjectionIn1Frame"
ARGUMENT_MODEL_WRITE_TIME           = "-modelWriteTime="
ARGUMENT_MODEL_NB_ITERATION         = "-modelNbIteration="
ARGUMENT_LINEAR_REGRESSION          = "-linearRegression"

PLOT_TYPE_SURFACE                   = "surface"
PLOT_TYPE_CLOUD                     = "cloud"
PLOT_TYPE_BAR                       = "bar"
PLOT_TYPE_POINT                     = "point"

COLOR_DEFAULT                       = 'cyan'
COLOR_LINEAR_REGRESSION             = 'purple'
COLOR_LIST                          = ['green',                'black',                    'red',                               'purple',                               'blue']
COLOR_CORRESPONDENCE                = ['./posixGlibcIO_sleep', './posixGlibcAIO_sleep',    './posixGlibcAIO_sleep_noSignal',    './posixGlibcIO_sleep_memoryFootprint', './posixGlibcAIO_sleep_memoryFootprint']
POINT_CORRESPONDENCE                = ['p', 'x', 'o', '<', '^', '*', 'D', 'x', '|', 'H']

LABEL_LIST_FENCY                    = ["File size (Bytes)", "Time (s)",    "Total time",        "#iteration",   "Computation Time (s)", "Synchronous pattern",  "Asynchronous pattern"]
LABEL_LIST_FENCY_CORRESPONDANCE     = ["fileSize",          "time",        " Iteration + wait", "nbIteration",  "computeTime",          "./posixGlibcIO_sleep", "./posixGlibcAIO_sleep"]

RESULT_DIM_TEXT_DEFAULT             = "Time (s)"
BAR_SIZE                            = 3
LINE_WIDTH                          = 4
MARKER_SIZE                         = 14


# ---------------------------------------
# Local methods
# ---------------------------------------
def printHelp(progName):
    print "Usage: " + progName + ": [options] <dataFileName>"
    print "Options:"
    print "\t"+ ARGUMENT_PLOT_TYPE                  + "<type>\t\t\tTo set the type of plot.  <type> is in: "\
                                                    + "\n\t\t\t\t\t\t<"\
                                                    + "\n\t\t\t\t\t\t| " + PLOT_TYPE_SURFACE \
                                                    + "\n\t\t\t\t\t\t| <" + PLOT_TYPE_POINT + "|" + PLOT_TYPE_BAR + ">:<dim projection>:<all|list of values>"\
                                                    + "\n\t\t\t\t\t\t| " + PLOT_TYPE_CLOUD\
                                                    + "\n\t\t\t\t\t\t> "
    print "\t"+ ARGUMENT_LOG_X                      + "\t\t\t\tTo use a logarithmic scale for the abscissa plot"
    print "\t"+ ARGUMENT_LOG_Y                      + "\t\t\t\tTo use a logarithmic scale for the ordinate plot"
    print "\t"+ ARGUMENT_RESULT_DIM_TEXT            + "<text>\t\t\tTo set the text to print as a result dimension"
    print "\t"+ ARGUMENT_ALL_PROJECTION_IN_1_FRAME  + "\t\t\tTo print all the different projections in a single frame"
    print "\t"+ ARGUMENT_MULTIPLE_TRY               + "\t\t\t\tTo consider the data as average of multpile try (inly available with 2d plots)"
    print "\t"+ ARGUMENT_INPUT_HELP                 + "\t\t\t\t\tTo print the current help"
    print "\t"+ ARGUMENT_LINEAR_REGRESSION          + "\t\t\tTo print the linear regression along with the required slop"
    print "\t"+ ARGUMENT_MODEL_WRITE_TIME           + "<write time in seconds>\tTo plot the theoretical model with the given writing time"
    print "\t"+ ARGUMENT_MODEL_NB_ITERATION         + "<number of iteration>\tTo plot the theoretical model with the given number of iterations"
    exit(2)


def parseInputArg(argList):
    dataFileName            = []
    plotType                = PLOT_TYPE_SURFACE
    logX                    = False
    logY                    = False
    allProjectionIn1Frame   = False
    multipleTry             = False
    resultDimText           = RESULT_DIM_TEXT_DEFAULT
    writeTime               = None
    nbIteration             = None
    linearRegression        = False

    for i in range(1, len(argList)):
        arg = argList[i]
        if (arg == ARGUMENT_INPUT_HELP):
            printHelp(argList[0])
            exit()
        elif (arg == ARGUMENT_LOG_X):
            logX = True
        elif (arg == ARGUMENT_LOG_Y):
            logY = True
        elif (arg == ARGUMENT_ALL_PROJECTION_IN_1_FRAME):
            allProjectionIn1Frame = True
        elif (arg == ARGUMENT_MULTIPLE_TRY):
            multipleTry = True
        elif (arg == ARGUMENT_LINEAR_REGRESSION):
            linearRegression = True
        elif (arg.startswith(ARGUMENT_PLOT_TYPE)):
            plotType = arg[len(ARGUMENT_PLOT_TYPE) : ]
        elif (arg.startswith(ARGUMENT_RESULT_DIM_TEXT)):
            resultDimText = arg[len(ARGUMENT_RESULT_DIM_TEXT) : ]
        elif (arg.startswith(ARGUMENT_MODEL_WRITE_TIME)):
            writeTime = float(arg[len(ARGUMENT_MODEL_WRITE_TIME) : ])
        elif (arg.startswith(ARGUMENT_MODEL_NB_ITERATION)):
            nbIteration = int(arg[len(ARGUMENT_MODEL_NB_ITERATION) : ])
        else:
            dataFileName.append(arg)

    if (len(dataFileName) == 0):
        loggerError("Unspecified data file name", exitNow=True)

    return (dataFileName, plotType, logX, logY, resultDimText, allProjectionIn1Frame, multipleTry, writeTime, nbIteration, linearRegression)


def setPlotTypeForThesis():
    font = {'family' : 'normal',
            'weight' : 'normal',
    #        'size'   : 27}
            'size'   : 33}
    mpl.rc('font', **font)
    mpl.rcParams['axes.linewidth'] = 5
    mpl.rcParams['patch.linewidth'] = 5


def parsePlotType(plotType, data):
    if ((not plotType.startswith(PLOT_TYPE_BAR)) and (not plotType.startswith(PLOT_TYPE_POINT))):
        return (plotType, None, None)

    list        = plotType.split(":")
    plotTypeId  = list[0]
    assert((plotTypeId == PLOT_TYPE_BAR) or (plotTypeId == PLOT_TYPE_POINT))
    if (len(list) != 3):
        loggerError("Malformed input plot type (0)", param=plotType, exitNow=True)

    dimProjectionName = list[1]
    if (not data.isVariableDim(dimProjectionName)):
        loggerError("Malformed input plot type (1)", param=dimProjectionName, exitNow=True)

    if(list[2] == "all"):
        dimProjectionListValue = data.getAllUnicValueFromDim(dimProjectionName)
    else:
        dimProjectionListValue = [float(valStr) for valStr in list[2].split(LIST_SEPARATOR)]

    return (plotTypeId, dimProjectionName, dimProjectionListValue)

def plotModel(ax, writeTime, nbIteration):
    maxC        = 3
    maxT        = 13
    ax.plot([0,         writeTime], [nbIteration*writeTime, nbIteration*writeTime], "--", linewidth=LINE_WIDTH, color='cyan', label="Asynchronous model (C << W)")
    ax.plot([writeTime, maxC],      [nbIteration*writeTime, nbIteration*maxC],      "--", linewidth=LINE_WIDTH, color='blue', label="Asynchronous model (C >> W)")
    ax.plot([writeTime, writeTime], [0,                     maxT],                  "--", linewidth=LINE_WIDTH, color='purple',label="Write time (" + str(writeTime) + ")") 


def plotModel_hpc(ax, writeTime, nbIteration, nbIoDevice=1):
    maxC        = 5
    ax.plot([0,                     writeTime/nbIoDevice],  [(nbIteration-1)*writeTime/nbIoDevice + writeTime,    nbIteration*writeTime/nbIoDevice+writeTime],  "--", linewidth=LINE_WIDTH, color='cyan',  label="Theoretical model (C << W)")
    ax.plot([writeTime/nbIoDevice,  maxC],                  [nbIteration*writeTime/nbIoDevice + writeTime,        nbIteration*maxC+writeTime],                  "--", linewidth=LINE_WIDTH, color='blue',  label="Theoretical model (C >> W)") 
    ax.plot([writeTime/nbIoDevice,  writeTime/nbIoDevice],  [0,                                         200],                               "--", linewidth=LINE_WIDTH, color='purple',label="Write time / nb IO devices") 


def projectionPlotHeader(data, dataCompare, dimProjectionName, dimProjectionValue, ax, fig, writeTime=None, nbIteration=None, plotStyleForThesis=False):
    if ((writeTime <> None) and (nbIteration <> None)):
        plotModel(ax, writeTime, nbIteration)
    plt.legend()
    frameTitle = data.getBenchmarkPatternInfo()
    if (not dataCompare == None):
        frameTitle += " VS " + dataCompare.getBenchmarkPatternInfo()
    fig.canvas.set_window_title(frameTitle)
    # Shrink current axis by 20%
    ax = plt.subplot(111)
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
#    ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
#    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.06),fancybox=True, shadow=True, ncol=3)
#    plt.grid()
#    plt.legend(bbox_to_anchor=(0.02, 1.2), loc='upper left')  # Put the legend on the left
    plt.show(block=False)


def fencyLabel(originalText):
    for i in xrange(len(LABEL_LIST_FENCY)):
        if (originalText == LABEL_LIST_FENCY_CORRESPONDANCE[i]):
            return LABEL_LIST_FENCY[i]

    return originalText


def linearRegression(rowDataX, rowDataY):
    n    = len(rowDataX)
    xMean    = 0
    yMean    = 0
    for i in xrange(n):
        xMean += rowDataX[i]
        yMean += rowDataY[i]
    xMean /= n
    yMean /= n

    betaNominator   = 0
    betaDenominator = 0
    for i in xrange(n):
        betaNominator    += (rowDataX[i] - xMean)*(rowDataY[i] - yMean)
        betaDenominator    += (rowDataX[i] - xMean)*(rowDataX[i] - xMean)
    beta = betaNominator / betaDenominator
    alpha= yMean - beta*xMean

    SSE = 0.
    SST = 0.
    for i in xrange(n):
        errorKnowingX		= rowDataY[i] - (alpha + rowDataX[i] * beta)
        errorNotKnowingX	= rowDataY[i] - yMean
        SSE = SSE + (errorKnowingX		* errorKnowingX)
        SST = SST + (errorNotKnowingX	* errorNotKnowingX)

    coefDet = 1. - (SSE / SST)
    return (alpha, beta, coefDet)


def plotLinearRegression(X, Z, plt):
#    X = X[2:]
#    Z = Z[2:]
    col=COLOR_LINEAR_REGRESSION
    (alpha, beta, coefDet) = linearRegression(X, Z)
    lineZ = [(alpha + X[i] * beta) for i in xrange(len(X))]

    strAlpha    = stringPrecision(alpha, 1)
    strBeta     = stringPrecision(beta, 1)
    strCoefDet  = stringPrecision(coefDet, 3)
    title1		= "Linear regression        : y      = " + strAlpha + " + x " + strBeta
    title2		= "Coef. of determination: R^2 = " + str(strCoefDet)
    plt.plot(X, lineZ, '--', linewidth=LINE_WIDTH, label=title1 + "\n" + title2, color=col)

    for i in xrange(len(X)):
        x       = X[i]
        z       = Z[i]
        zReg    = lineZ[i]
        plt.plot([x, x], [z, zReg], '--', linewidth=LINE_WIDTH, color=col)


# ---------------------------------------
# Plotting methods (per chart page)
# ---------------------------------------
def plotCloud(X, Y, Z, fig, X_label, Y_label, Z_label):
    ax = plt.scatter(X,Y, s=75, c=Z)
    fig.colorbar(ax, label=Z_label)
    plt.xlabel(X_label)
    plt.ylabel(Y_label)


def plotSurface(X, Y, Z, fig, X_label, Y_label, Z_label):
    ax = fig.gca(projection='3d')
    X,Y = np.meshgrid(X, Y)
    surf= ax.plot_surface(X, Y, Z, rstride=10, cstride=10, cmap=cm.jet, linewidth=0, antialiased=False)
    fig.colorbar(surf, label=Z_label)
#TODO
#   ax.set_zlim3d(-1.01, 1.01)
#TODO end
    ax.w_zaxis.set_major_locator(LinearLocator(10))
    ax.w_zaxis.set_major_formatter(FormatStrFormatter('%.03f'))
    ax.set_xlabel(X_label)
    ax.set_ylabel(Y_label)
    ax.set_zlabel(Z_label)

def plotPoint(X, Z, Z_error, fig, ax, X_label, Z_label, legend, barSize, logX, logY, legendExtra="", pointType=0, generateRandomColor=False, linearRegression=False, linearReg=False):
# TODO to remove
#    if (legend.startswith("./posixGlibcIO") and legendExtra != "computeTime(0.0001)"):
#        return
# TODO end to remove

# TODO to remove
    if (legendExtra == 'Iterations'):
        return
# TODO end to remove

    pointType = POINT_CORRESPONDENCE[pointType]
    if (generateRandomColor):
        col = generateMappedRandomColor()
    else:
        col = find (legend, COLOR_CORRESPONDENCE)
        if (col < 0):
            col = COLOR_DEFAULT
        else:
            col = COLOR_LIST[col]
#    col = 'green'
    legend      = fencyLabel(legend)
    legendExtra = fencyLabel(legendExtra)
    legend      = legend + " (" + legendExtra + ")"
#    legend = "Write time"
#    legend = "Write time (saturation method)"
#    legend = "Asynchronous I/O request time"
    ax.plot(X, Z, "-"+pointType, color=col, label=legend, linewidth=LINE_WIDTH, markersize=MARKER_SIZE)

    if (linearReg == True):    
        plotLinearRegression(X, Z, plt)

    if (Z_error != None):
#        ax.fill_between(X, Z_error[1], Z_error[0], color=col, alpha=0.1)
        Z_error_max = [(Z_error[0][i] - Z[i]) for i in xrange(len(Z))]
        Z_error_min = [(Z[i] - Z_error[1][i]) for i in xrange(len(Z))]
        ax.errorbar(X, Z, yerr=[Z_error_min, Z_error_max], color=col, linewidth=LINE_WIDTH)
        
    Z_label = fencyLabel(Z_label)
    X_label = fencyLabel(X_label)
    ax.set_ylabel(Z_label)
    ax.set_xlabel(X_label)
    if (logX):
        ax.set_xscale('log')
    if (logY):
        ax.set_yscale('log')


def plotBar(X, Z, Z_error, fig, ax, X_label, Z_label, legend, barSize, logX, logY, legendExtra="", transparency=1, generateRandomColor=False):
    if (generateRandomColor):
        col = generateMappedRandomColor()
    else:
        col = find (legend, COLOR_CORRESPONDENCE)
        if (col < 0):
            col = COLOR_DEFAULT
        else:
            col = COLOR_LIST[col]
    legend = legend + " (" + legendExtra + ")"
    bar = ax.bar(X, Z,  barSize, color=color, label=legend, alpha=transparency, yerr=Z_error)
    ax.set_ylabel(Z_label)
    ax.set_xlabel(X_label)
    if (logX):
        ax.set_xscale('log')
    if (logY):
        ax.set_yscale('log')
        


# ---------------------------------------
# Plotting methods (loop on all data)
# ---------------------------------------
def plotData_projection(data, plotType, dimProjectionName, dimProjectionListValue, logX, logY, allProjectionIn1Frame, resultDimText, multipleTry, dataCompare=None, writeTime=None, nbIteration=None, plotStyleForThesis=False, linearReg=False):
    pointType   =0
    if (allProjectionIn1Frame):
        fig     = plt.figure()
        ax      = fig.gca()
    for dimProjectionValue in dimProjectionListValue:
        (X_list, Z_list, Z_error_list, X_label_list) = data.getVariableDimVect_projection(dimProjectionName, dimProjectionValue)
        assert(len(X_list) == len(X_label_list))
        if (dataCompare != None):
            if (not data.isSameFixedDim(dataCompare)):
                loggerError("The 2 given data files have different fixed dimension", exitNow=False)
                return
            (X_list_cmp, Z_list_cmp, Z_error_list_cmp, X_label_list_cmp) = dataCompare.getVariableDimVect_projection(dimProjectionName, dimProjectionValue)
            assert(len(X_list_cmp) == len(X_label_list_cmp))
            assert(X_label_list == X_label_list_cmp)
#TODO            shift = float(min((X_list_cmp[0][1] - X_list_cmp[0][0]), (X_list[0][1] - X_list[0][0])))/3
        transparency=1.
        if (allProjectionIn1Frame):
            resultName  = dimProjectionName + "(" + str(dimProjectionValue) + ")"
        if (not allProjectionIn1Frame):
            fig = plt.figure()
            ax  = fig.gca()
        for resultId in xrange(data.getNbResultDim()):
            if (not allProjectionIn1Frame):
                resultName  = data.getResultDimName(resultId)
            Z           = Z_list[resultId]
            Z_error     = Z_error_list[resultId]
            if (plotType == PLOT_TYPE_BAR):
                plotBar(X_list[0], Z, Z_error, fig, ax, X_label_list[0], resultDimText, data.getBenchmarkPatternInfo(), BAR_SIZE, logX, logY, legendExtra=resultName, transparency=transparency, generateRandomColor=allProjectionIn1Frame)
                if (dataCompare != None):
                    Z_cmp       = Z_list_cmp[resultId]
                    Z_cmp_error = Z_error_list_cmp[resultId]
                    plotBar([x + shift for x in X_list_cmp[0]], Z_cmp, Z_cmp_error, fig, ax, X_label_list_cmp[0], resultDimText, dataCompare.getBenchmarkPatternInfo(), BAR_SIZE, logX, logY, legendExtra=resultName, transparency=transparency, generateRandomColor=allProjectionIn1Frame)
            elif (plotType == PLOT_TYPE_POINT):
                plotPoint(X_list[0], Z, Z_error, fig, ax, X_label_list[0], resultDimText, data.getBenchmarkPatternInfo(), BAR_SIZE, logX, logY, legendExtra=resultName, pointType=pointType, generateRandomColor=allProjectionIn1Frame, linearReg=linearReg)
                pointType = (pointType + 1) % len(POINT_CORRESPONDENCE)
                if (dataCompare != None):
                    Z_cmp       = Z_list_cmp[resultId]
                    Z_cmp_error = Z_error_list_cmp[resultId]
                    plotPoint(X_list_cmp[0], Z_cmp, Z_cmp_error, fig, ax, X_label_list_cmp[0], resultDimText, dataCompare.getBenchmarkPatternInfo(), BAR_SIZE, logX, logY, legendExtra=resultName, pointType=pointType, generateRandomColor=allProjectionIn1Frame, linearReg=linearReg)
                    pointType = (pointType + 1) % len(POINT_CORRESPONDENCE)
            else:
                loggerError("Unknown 2d plot type", param=plotType, exitNow=True)
            transparency = transparency / 2.

        if (not allProjectionIn1Frame):
            projectionPlotHeader(data, dataCompare, dimProjectionName, dimProjectionValue, ax, fig, writeTime, nbIteration)
    if (allProjectionIn1Frame):
        projectionPlotHeader(data, dataCompare, dimProjectionName, dimProjectionValue, ax, fig, writeTime, nbIteration)


def plotData_3d(data, plotType):
    X = data.getVariableDimVect(0)
    Y = data.getVariableDimVect(1)
    for resultName in data.getResultDimNameVect():
        print "\n\t=========================="
        print "\tPlot (" + plotType + ") " + str(resultName)
        Z   = data.getResultDimVect(resultName)
        fig = plt.figure()
        if (plotType == PLOT_TYPE_SURFACE):
            plotSurface(X, Y, Z, fig, data.getVariableDimName(0), data.getVariableDimName(1), resultName)
        elif (plotType == PLOT_TYPE_CLOUD):
            plotCloud(X, Y, Z, fig, data.getVariableDimName(0), data.getVariableDimName(1), resultName)
        else:
            loggerError("Unknown 3d plot type", param=plotType, exitNow=True)
        plt.title(data.getHeader(left=False),   loc='right')
        plt.title(data.getHeader(left=True),    loc='left')
        fig.canvas.set_window_title(str(data.getBenchmarkPatternInfo()))
        plt.grid()
        plt.show(block=False)


def plotData(data, plotType, logX, logY, allProjectionIn1Frame, resultDimText, multipleTry, dataCompare=None, writeTime=None, nbIteration=None, plotStyleForThesis=False, linearReg=False):
    LR = linearRegression
#    plt.hold(True)  # Allows to add subsequent plots
    (plotType, dimProjectionName, dimProjectionListValue) = parsePlotType(plotType, data)
    if ((plotType == PLOT_TYPE_BAR) or (plotType == PLOT_TYPE_POINT)):
        plotData_projection(data, plotType, dimProjectionName, dimProjectionListValue, logX, logY, allProjectionIn1Frame, resultDimText, multipleTry, dataCompare=dataCompare, writeTime=writeTime, nbIteration=nbIteration, plotStyleForThesis=plotStyleForThesis, linearReg=linearReg)
    else:
#TODO add the log scal option
        plotData_3d(data, plotType)


# ---------------------------------------
# Main method

# ---------------------------------------
if __name__ == "__main__":
    (dataFileName, plotType, logX, logY, resultDimText, allProjectionIn1Frame, multipleTry, writeTime, nbIteration, linearReg)  = parseInputArg(sys.argv)

    plotStyleForThesis=False
#    setPlotTypeForThesis()
#    plotStyleForThesis=True

    dataList = [Data() for f in dataFileName]
    for i in xrange(len(dataFileName)):
        print "\t Parsing the data file: " + dataFileName[i]
        dataList[i].parseAndSet(dataFileName[i], multipleTry=multipleTry)
#        assert (dataList[i].getNbVariableDim() == 2)

    nbFile = len(dataFileName)
    if (nbFile == 1):
        print dataList[0].toString()
        plotData(dataList[0], plotType, logX, logY, allProjectionIn1Frame, resultDimText, multipleTry, writeTime=writeTime, nbIteration=nbIteration, plotStyleForThesis=plotStyleForThesis, linearReg=linearReg)
    else:
        for i in xrange(nbFile-1):
#            print dataList[i].toString()
            for j in range(i+1, nbFile):
                plotData(dataList[i], plotType, logX, logY, allProjectionIn1Frame, resultDimText, multipleTry, dataCompare=dataList[j], writeTime=writeTime, nbIteration=nbIteration, plotStyleForThesis=plotStyleForThesis, linearReg=linearReg)
#        print dataList[nbFile-1].toString()
    plt.show()
