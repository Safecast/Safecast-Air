from __future__ import print_function
import sys
import json
import json.tool
import re
import matplotlib
try:
    matplotlib.use('Qt4Agg')
except ImportError:
    pass
import scipy
import scipy.signal
import datetime
import matplotlib.dates 
import matplotlib.pyplot as plt


FLAGS = re.VERBOSE | re.MULTILINE | re.DOTALL
WHITESPACE = re.compile(r'[ \t\n\r]*', FLAGS)

class ConcatJSONDecoder(json.JSONDecoder):
    def decode(self, s, _w=WHITESPACE.match):
        s_len = len(s)
        objs = []
        end = 0
        while end != s_len:
            obj, end = self.raw_decode(s, idx=_w(s, end).end())
            end = _w(s, end).end()
            objs.append(obj)
        return objs

def loadData(filename):
    print('reading data file')
    sys.stdout.flush()
    with open(filename,'r') as f:
        dataList = json.load(f,cls=ConcatJSONDecoder)
    print('done')
    sys.stdout.flush()
    return dataList

def extractData(dataList):
    dates = extractDates(dataList)
    gasDict = extractGasData(dataList)
    tmpDict = extractTmpData(dataList)
    pmDict = extractPMData(dataList)
    gpsDict = extractGPSData(dataList)
    return dates, gasDict, tmpDict, pmDict, gpsDict

def extractDates(dataList):
    dateTimeList = [str(item['gps']['date']) for item in dataList]
    dates = [datetime.datetime.strptime(x[:-1],'%Y-%m-%dT%H:%M:%S') for x in dateTimeList]
    dates = dates[plotStartInd:]
    return dates

def extractGasData(dataList):
    gasDict = {}
    for sensor in dataList[0]['gas']:
        gas = sensor['type']
        loc = sensor['hdr'], sensor['pos']
        try:
            gasDict[gas][loc] = {'ppb': [], 'ppbFlt': []}
        except KeyError:
            gasDict[gas] = {}
            gasDict[gas][loc] = {'ppb': [], 'ppbFlt': []}

    for item in dataList:
        for sensor in item['gas']:
            gas = sensor['type']
            loc = sensor['hdr'], sensor['pos']
            gasDict[gas][loc]['ppb'].append(sensor['ppb'])
            gasDict[gas][loc]['ppbFlt'].append(sensor['ppbFlt'])
    return gasDict

def extractTmpData(dataList):
    tmpDict = {}
    for item in dataList:
        for sensor in item['tmp']:
            try:
                tmpDict[sensor['hdr']]['val'].append(sensor['valFlt'])
                tmpDict[sensor['hdr']]['valFlt'].append(sensor['valFlt'])
            except KeyError:
                tmpDict[sensor['hdr']] = {}
                tmpDict[sensor['hdr']]['val'] = [sensor['val']]
                tmpDict[sensor['hdr']]['valFlt'] = [sensor['valFlt']]
    return tmpDict

def extractPMData(dataList):
    pmDict = { 'pm1': [], 'pm2_5': [], 'pm10': []}
    for item in dataList:
        sensor = item['pm'][0]
        pmDict['pm1'].append(sensor['pm1'])
        pmDict['pm2_5'].append(sensor['pm2_5'])
        pmDict['pm10'].append(sensor['pm10'])
    return pmDict

def extractGPSData(dataList):
    gpsDict = {}
    for key in dataList[0]['gps']:
        gpsDict[key] = []
    for item in dataList:
        for key,val in item['gps'].iteritems():
            gpsDict[key].append(val)
    return gpsDict


# -----------------------------------------------------------------------------
if __name__ == '__main__':

    filename = sys.argv[1]
    plotStartInd = 0 
    pmFiltSize = 7 
    figSize = (11.02,8.27)
    saveToFile = False
    showPlots = True

    # Load data and extract items of interest
    dataList = loadData(filename)
    dates, gasDict, tmpDict, pmDict, gpsDict  = extractData(dataList)
    print('total hours: {0}'.format(len(dates)/60.0))

    # Apply median filter to PM data
    pm1 = pmDict['pm1'][plotStartInd:]
    pm2_5 = pmDict['pm2_5'][plotStartInd:]
    pm10 = pmDict['pm10'][plotStartInd:]

    pm1Flt = scipy.signal.medfilt(pm1,pmFiltSize)
    pm2_5Flt = scipy.signal.medfilt(pm2_5,pmFiltSize)
    pm10Flt = scipy.signal.medfilt(pm10,pmFiltSize)


    hourLoc = matplotlib.dates.HourLocator(interval=5)
    formatter = matplotlib.dates.DateFormatter('%Y/%m/%d %H:%M')

    # Plot gas data
    figCnt = 0
    for i, gas in enumerate(gasDict):
        figCnt += 1
        fig = plt.figure(figCnt,figsize=figSize)
        figName = 'figure{0}.png'.format(figCnt)
        for j, loc in enumerate(sorted(gasDict[gas])):
            ax = plt.subplot(2,1,j+1)

            ppb = gasDict[gas][loc]['ppb'][plotStartInd:]
            ppbFlt = gasDict[gas][loc]['ppbFlt'][plotStartInd:]

            hdlRaw, = plt.plot(dates,ppb,'b')
            plt.grid('on')
            plt.ylabel('(ppb)')

            hdlFlt, = plt.plot(dates, ppbFlt,'r')
            plt.ylabel('(ppb)')
            ax.xaxis.set_major_locator(hourLoc)
            ax.xaxis.set_major_formatter(formatter)

            labels = ax.get_xticklabels()
            if j == 0:
                plt.title('{0} Sensors'.format(gas))
                for x in labels:
                    x.set_visible(False)
                plt.figlegend((hdlRaw,hdlFlt),('raw', 'filtered (lowpass)'),'upper right')
            if j == len(gasDict[gas]):
                plt.setp(labels,rotation=30)

            locStr = 'location: hdr {0}, pos {1}'.format(*loc)
            plt.annotate(locStr,xy=(0.01,0.925),xycoords='axes fraction')

            plt.grid('on')
        fig.autofmt_xdate()
        if saveToFile:
            plt.savefig(figName)


    # Plot tmp data
    figCnt += 1
    fig = plt.figure(figCnt,figsize=figSize)
    figName = 'figure{0}.png'.format(figCnt)
    for i, loc in enumerate(tmpDict):
        ax = plt.subplot(2,1,i+1)
        val = tmpDict[loc]['val'][plotStartInd:]
        valFlt = tmpDict[loc]['valFlt'][plotStartInd:]

        hdlRaw, = plt.plot(dates, val,'b')
        plt.ylabel('(C)')
        plt.grid('on')

        hdlFlt, = plt.plot(dates,valFlt,'r')
        plt.ylabel('(C)')
        ax.xaxis.set_major_locator(hourLoc)
        ax.xaxis.set_major_formatter(formatter)

        labels = ax.get_xticklabels()
        if i == 0:
            plt.title('Temperature Sensors')
            for x in labels:
                x.set_visible(False)
            plt.figlegend((hdlRaw,hdlFlt),('raw', 'filtered (lowpass)'),'upper right')
        if i == len(tmpDict)-1:
            plt.setp(labels,rotation=30)

        locStr = 'sensor location: hdr {0}'.format(loc)
        plt.annotate(locStr,xy=(0.01,0.925),xycoords='axes fraction')
        plt.grid('on')
    fig.autofmt_xdate()
    if saveToFile:
        plt.savefig(figName)

    # Plot partical counter data
    figCnt += 1
    fig = plt.figure(figCnt,figsize=figSize)
    figName = 'figure{0}.png'.format(figCnt)

    ax = plt.subplot(3,1,1)
    hdlRaw, = plt.plot(dates, pm1,'b')
    hdlFlt, = plt.plot(dates, pm1Flt,'r')
    plt.title('Particle Counter')
    plt.ylabel(r'PM1 ($\mu g / m^3$)')
    ax.xaxis.set_major_locator(hourLoc)
    ax.xaxis.set_major_formatter(formatter)
    plt.grid('on')
    labels = ax.get_xticklabels()
    for x in labels:
        x.set_visible(False)
    plt.figlegend((hdlRaw,hdlFlt),('raw', 'filtered (median)'),'upper right')

    ax = plt.subplot(3,1,2)
    plt.plot(dates, pm2_5,'b')
    plt.plot(dates, pm2_5Flt,'r')
    plt.ylabel(r'PM2.5 ($\mu g / m^3$)')
    ax.xaxis.set_major_locator(hourLoc)
    ax.xaxis.set_major_formatter(formatter)
    plt.grid('on')
    labels = ax.get_xticklabels()
    for x in labels:
        x.set_visible(False)

    ax = plt.subplot(3,1,3)
    plt.plot(dates, pm10,'b')
    plt.plot(dates, pm10Flt,'r')
    plt.ylabel(r'PM10 ($\mu g / m^3$)')
    ax.xaxis.set_major_locator(hourLoc)
    ax.xaxis.set_major_formatter(formatter)
    plt.grid('on')
    labels = ax.get_xticklabels()
    plt.setp(labels,rotation=30)
    fig.autofmt_xdate()
    if saveToFile:
        plt.savefig(figName)

    # Gas sensor Overlay plots 
    if 0:
        colorList = ('b','r')
        for gas in gasDict:
            figCnt += 1
            fig = plt.figure(figCnt,figsize=figSize)
            figName = 'figure{0}.png'.format(figCnt)
            ax = plt.subplot(1,1,1)
            hdlList = []
            locStrList = []
            for i, loc in enumerate(gasDict[gas]):
                ppb = gasDict[gas][loc]['ppbFlt']
                ppb = ppb[plotStartInd:]
                hdl, = plt.plot(dates,ppb,colorList[i])
                hdlList.append(hdl)
                plt.title('{0} Sensors Overlay'.format(gas))
                locStr = 'sensor (hdr{0}, pos{1}'.format(*loc)
                locStrList.append(locStr)
            
            plt.ylabel('(ppb)')
            ax.xaxis.set_major_locator(hourLoc)
            ax.xaxis.set_major_formatter(formatter)
            plt.grid('on')
            labels = ax.get_xticklabels()
            plt.setp(labels,rotation=30)
            fig.autofmt_xdate()
            plt.figlegend(hdlList,locStrList, 'upper right')
            if saveToFile:
                plt.savefig(figName)



    # Regression between O3 data sets and overlay plot
    if 0:
        o3DataSet0 = gasDict['O3'][(0,1)]['ppbFlt']
        o3DataSet1 = gasDict['O3'][(1,1)]['ppbFlt']
        o3Fit = scipy.polyfit(o3DataSet0, o3DataSet1, 1)
        o3DataSet0Adj = scipy.polyval(o3Fit,o3DataSet0) 
        o3DataSet0 = o3DataSet0[plotStartInd:]
        o3DataSet1 = o3DataSet1[plotStartInd:]
        o3DataSet0Adj = o3DataSet0Adj[plotStartInd:] 

        figCnt += 1
        fig = plt.figure(figCnt,figsize=figSize)
        figName = 'figure{0}.png'.format(figCnt)

        ax = plt.subplot(2,1,1)
        h1, = plt.plot(dates,o3DataSet1,'b')
        h0, = plt.plot(dates,o3DataSet0,'r')
        plt.ylabel('(ppb)')
        plt.grid('on')
        labels = ax.get_xticklabels()
        for x in labels:
            x.set_visible(False)
        plt.title('O3 Sensors  - Overlay and regression')

        ax = plt.subplot(2,1,2)
        h3, = plt.plot(dates,o3DataSet1,'b')
        h4, = plt.plot(dates,o3DataSet0Adj,'g')
        plt.ylabel('(ppb)')
        plt.figlegend((h1,h0,h4),('sensor (hdr0, pos1)', 'sensor (hdr1, pos1)', 'sensor (hdr1, pos1) regression'), 'center right')
        plt.grid('on')
        ax.xaxis.set_major_locator(hourLoc)
        ax.xaxis.set_major_formatter(formatter)
        plt.grid('on')
        labels = ax.get_xticklabels()
        plt.setp(labels,rotation=30)
        fig.autofmt_xdate()
        if saveToFile:
            plt.savefig(figName)

    if showPlots:
        plt.show()

