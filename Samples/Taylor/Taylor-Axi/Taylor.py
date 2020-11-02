#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

# Parameters of the model
nbreSaves = 20
stopTime = 80.0e-6
nbrePoints = 250
width = 3.2
heigh = 32.4
nbElementsWidth = 5
nbElementsHeigh = 50
speed = 287000

# Material parameters
young = 206000
poisson = 0.3
density = 7.83e-09
heatCapacity = 4.6e+08
taylorQuinney = 0.9
A = 806.0
B = 614.0
C = 0.0089
n = 0.168
m = 1.1
depsp0 = 1.0
Tm = 1540.0
T0 = 20.0

dxWidth = width/(nbElementsWidth);
dxHeigh = heigh/(nbElementsHeigh);

# Creates the main Object
model = dnl.DynELA("Taylor")

# Creates the Nodes
nbNodes = 1
allNS = dnl.NodeSet("NS_All")
for j in range (nbElementsHeigh+1):
    for i  in range (nbElementsWidth+1):
        model.createNode(nbNodes, i*dxWidth, j*dxHeigh, 0.00)
        model.add(allNS, nbNodes)
        nbNodes += 1
nbNodes -= 1
print("Number of nodes created:", model.getNodesNumber())

# Creates the Elements
model.setDefaultElement(dnl.Element.ElQua4NAx)
nbElements = 1
allES = dnl.ElementSet("ES_All")
for j in range (nbElementsHeigh):
    for i in range (nbElementsWidth):
        n1 = (i+(j*(nbElementsWidth+1))+1)
        n2 = (i+(j*(nbElementsWidth+1))+2)
        n3 = (i+((j+1)*(nbElementsWidth+1))+2)
        n4 = (i+((j+1)*(nbElementsWidth+1))+1)
        model.createElement(nbElements, n1, n2, n3, n4)
        model.add(allES, nbElements)
        nbElements += 1
nbElements -= 1
print("Number of elements created:", model. getElementsNumber())

bottomNS = dnl.NodeSet("NS_Bottom")
model.add(bottomNS, 1, nbElementsWidth+1)

axisNS = dnl.NodeSet("NS_Axis")
model.add(axisNS, 1, nbNodes, nbElementsWidth+1)

histRad = dnl.NodeSet("NS_HistRadius")
model.add(histRad, 1 + nbElementsWidth)

histHei = dnl.NodeSet("NS_HistHeight")
model.add(histHei, nbNodes - nbElementsWidth)

histES = dnl.ElementSet("ES_Hist")
model.add(histES, 1)

# Creates the hardening law
hardLaw = dnl.JohnsonCookLaw()
hardLaw.setParameters(A, B, C, n, m, depsp0, Tm, T0)

# Creates the material
steel = dnl.Material("Steel")
steel.setHardeningLaw(hardLaw)
steel.youngModulus = young
steel.poissonRatio = poisson
steel.density = density
steel.heatCapacity = heatCapacity
steel.taylorQuinney = taylorQuinney
steel.initialTemperature = T0
model.add(steel, allES)

# Declaration of a boundary condition for bottom line
bottomBC = dnl.BoundaryRestrain('BC_bottom')
bottomBC.setValue(0, 1, 1)
model.attachConstantBC(bottomBC, bottomNS)

# Declaration of a boundary condition for axis line
axisBC = dnl.BoundaryRestrain('BC_axis')
axisBC.setValue(1, 0, 1)
model.attachConstantBC(axisBC, axisNS)

# Declaration of the initial speed
speedBC = dnl.BoundarySpeed('BC_speed')
speedBC.setValue(0, -speed, 0)
model.attachInitialBC(speedBC, allNS)

solver = dnl.Explicit("Solver")
solver.setTimes(0, stopTime)
model.add(solver)
solver.setComputeTimeStepFrequency(1)
solver.setTimeStepMethod(solver.PowerIteration)
#solver.setTimeStepSafetyFactor(1.09)
model.setSaveTimes(0, stopTime, stopTime/nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile("vonMisesHistory")
vonMisesHist.setFileName(dnl.String("vonMises.plot"))
vonMisesHist.add(histES, 0, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime / nbrePoints)
model.add(vonMisesHist)

plasticStrainHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainHist.setFileName(dnl.String("plasticStrain.plot"))
plasticStrainHist.add(histES, 0, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime / nbrePoints)
model.add(plasticStrainHist)

temperatureHist = dnl.HistoryFile("temperatureHistory")
temperatureHist.setFileName(dnl.String("temperature.plot"))
temperatureHist.add(histES, 0, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime / nbrePoints)
model.add(temperatureHist)

radiusHist = dnl.HistoryFile("radiusHistory")
radiusHist.setFileName(dnl.String("radius.plot"))
radiusHist.add(histRad, dnl.Field.nodeCoordinateX)
radiusHist.setSaveTime(stopTime / nbrePoints)
model.add(radiusHist)

heightHist = dnl.HistoryFile("heightHistory")
heightHist.setFileName(dnl.String("height.plot"))
heightHist.add(histHei, dnl.Field.nodeCoordinateY)
heightHist.setSaveTime(stopTime / nbrePoints)
model.add(heightHist)

dtHist = dnl.HistoryFile("dtHistory")
dtHist.setFileName(dnl.String("dt.plot"))
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime / nbrePoints)
model.add(dtHist)

keHist = dnl.HistoryFile("keHistory")
keHist.setFileName(dnl.String("ke.plot"))
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime / nbrePoints)
model.add(keHist)

# Parallel computation
model.parallel.setCores(4)

model.solve()

finalRadius = model.getNodeByNum(1+nbElementsWidth).coordinates(0)
finalHeight = model.getNodeByNum(nbNodes-nbElementsWidth).coordinates(1)
f=open('results.txt','w')
f.write('final radius : ' + str(finalRadius) + '\n')
f.write('final height : ' + str(finalHeight) + '\n')
f.close()

svg = dnl.SvgInterface("SVG")
svg.setTitleDisplay(False)
svg.setLegendPosition(350, 150)
svg.write(dnl.String("temperatureCP.svg"),dnl.Field.temperature)
svg.write(dnl.String("vonMisesCP.svg"),dnl.Field.vonMises)
svg.write(dnl.String("plasticStrainCP.svg"),dnl.Field.plasticStrain)

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
