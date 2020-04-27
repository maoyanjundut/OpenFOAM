#!/usr/bin/python
import re
import os 
from pylab import *

pathname = os.path.abspath('./Data')
readPath = os.path.join(pathname,'ProbeVOF')
a = os.listdir(readPath)

# Sorting
remove = []
for i in range(len(a)):
    if (a[i].rfind('.')+1): # Includes point
        remove.append(i)
remove.reverse()
for i in remove:
    a.pop(i)
a.sort(lambda a,b: cmp(int(a.split('F')[1]), float(b.split('F')[1])))

# Plot  
index = 0
indexFig = 0
# read the theory wave surface
fileR_theory = open(os.path.join(pathname,'ThoryFocusWaveSurface.txt'))
data_theory = fileR_theory.read()
fileR_theory.close
data_theory = data_theory.split('\n')
x1 = []
y1 = []

for i in range(len(data_theory)-1):
    line_2 = data_theory[i]
    line_2 = line_2.split(' ')
    x1.append(float(line_2[0]))
    y1.append(float(line_2[1]))

for gauge in a:
    index = index + 1
    if index >= 4 or index == 1:
        index = 1
        indexFig = indexFig + 1
        figure(num=indexFig)
        
    subplots_adjust(hspace=0.6)
    
    fileR = open(os.path.join(readPath,gauge), 'r')
    data = fileR.read()
    fileR.close()
    data = data.split('\n')
    x = []
    y = []
    for i in range(len(data)-1):
        line = data[i]
        line = line.split(' ') 
        x.append(float(line[0]))
        y.append(float(line[1]))
  
    subplot(3,1,index)
    plot(x,y,'r-',x1,y1,'g.')
    xlabel('t (s)')    
    ylabel('$h + \eta$ (m)')
    title(gauge)
    savefig('./Data/surfaceElevation%s'%indexFig)# this joints the string and varibles
show()