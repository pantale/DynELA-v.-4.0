#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

# Parameters of the model
nbreSaves = 20
stopTime = 1e-3
nbrePoints = 1000
bottomWidth = 6.35
topWidth = 6.413
heigh = 26.67
nbElementsWidth = 10
nbElementsHeigh = 40
factor = 1 / 2
displacement = 7

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

dxWidth = bottomWidth / nbElementsWidth
dxHeigh = heigh / nbElementsHeigh
ddx = (topWidth - bottomWidth) / bottomWidth / heigh
speed = displacement / stopTime

# Creates the main Object
model = dnl.DynELA('BarNecking')

# Creates the Nodes
nbNodes = 1
nsAll = dnl.NodeSet('NS_All')
dxHeigh /= (1 / factor + 1) / 2
y = 0
for j in range (nbElementsHeigh + 1):
    for i  in range (nbElementsWidth + 1):
        model.createNode(nbNodes, i * dxWidth * (1 + ddx * y), y, 0)
        model.add(nsAll, nbNodes)
        nbNodes += 1
    if (j == nbElementsHeigh/2): dxHeigh /= factor
    y += dxHeigh
nbNodes -= 1
print('Number of nodes created:', model.getNodesNumber())

# Creates the Elements
model.setDefaultElement(dnl.Element.ElQua4NAx)
nbElements = 1
esAll = dnl.ElementSet('ES_All')
for j in range (nbElementsHeigh):
    for i in range (nbElementsWidth):
        n1 = (i + (j * (nbElementsWidth + 1)) + 1)
        n2 = (i + (j * (nbElementsWidth + 1)) + 2)
        n3 = (i + ((j + 1) * (nbElementsWidth + 1)) + 2)
        n4 = (i + ((j + 1) * (nbElementsWidth + 1)) + 1)
        model.createElement(nbElements, n1, n2, n3, n4)
        model.add(esAll, nbElements)
        nbElements += 1
nbElements -= 1
print('Number of elements created:', model.getElementsNumber())

topNS = dnl.NodeSet('NS_Top')
model.add(topNS, nbNodes-nbElementsWidth, nbNodes)

bottomNS = dnl.NodeSet('NS_Bottom')
model.add(bottomNS, 1, nbElementsWidth + 1)

axisNS = dnl.NodeSet('NS_Axis')
model.add(axisNS, 1, nbNodes, nbElementsWidth + 1)

histRad = dnl.NodeSet('NS_HistRadius')
model.add(histRad, 1 + nbElementsWidth)

histES = dnl.ElementSet('ES_Hist')
model.add(histES, 1)

# Creates the hardening law
hardLaw = dnl.JohnsonCookLaw()
hardLaw.setParameters(A, B, C, n, m, depsp0, Tm, T0)

# Creates the material
steel = dnl.Material('Steel')
steel.setHardeningLaw(hardLaw)
steel.youngModulus = young
steel.poissonRatio = poisson
steel.density = density
steel.heatCapacity = heatCapacity
steel.taylorQuinney = taylorQuinney
steel.initialTemperature = T0

# Finaly link the material to the structure
model.add(steel, esAll)

# Declaration of a boundary condition for bottom line
bottomBC = dnl.BoundaryRestrain('BC_bottom')
bottomBC.setValue(0, 1, 1)
model.attachConstantBC(bottomBC, bottomNS)

# Declaration of a boundary condition for axis line
axisBC = dnl.BoundaryRestrain('BC_axis')
axisBC.setValue(1, 0, 1)
model.attachConstantBC(axisBC, axisNS)

# Declaration of a ramp function to apply the load
ramp = dnl.RampFunction('constantFunction')
ramp.setFunction(dnl.RampFunction.Constant, 0, stopTime)

# Declaration of a boundary condition for top line
speedBC = dnl.BoundarySpeed('BC_speed')
speedBC.setValue(0, displacement, 0)
speedBC.setFunction(ramp)
model.attachConstantBC(speedBC, topNS)

solver = dnl.Explicit('Solver')
solver.setTimes(0, stopTime)
model.add(solver)
solver.setTimeStepMethod(solver.PowerIteration)
#solver.setTimeStepSafetyFactor(0.6)
model.setSaveTimes(0, stopTime, stopTime / nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile('vonMisesHistory')
vonMisesHist.setFileName('vonMises.plot')
vonMisesHist.add(histES, 0, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime / nbrePoints)
model.add(vonMisesHist)

plasticStrainHist = dnl.HistoryFile('plasticStrainHistory')
plasticStrainHist.setFileName('plasticStrain.plot')
plasticStrainHist.add(histES, 0, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime / nbrePoints)
model.add(plasticStrainHist)

temperatureHist = dnl.HistoryFile('temperatureHistory')
temperatureHist.setFileName('temperature.plot')
temperatureHist.add(histES, 0, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime / nbrePoints)
model.add(temperatureHist)

internalEnergyHist = dnl.HistoryFile('internalEnergyHistory')
internalEnergyHist.setFileName('internalEnergy.plot')
internalEnergyHist.add(histES, 0, dnl.Field.internalEnergy)
internalEnergyHist.setSaveTime(stopTime / nbrePoints)
model.add(internalEnergyHist)

radiusHist = dnl.HistoryFile('radiusHistory')
radiusHist.setFileName('radius.plot')
radiusHist.add(histRad, dnl.Field.nodeCoordinateX)
radiusHist.setSaveTime(stopTime / nbrePoints)
model.add(radiusHist)

dtHist = dnl.HistoryFile('dtHistory')
dtHist.setFileName('dt.plot')
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime / nbrePoints)
model.add(dtHist)

keHist = dnl.HistoryFile('keHistory')
keHist.setFileName('ke.plot')
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime / nbrePoints)
model.add(keHist)

# Parallel computation
model.parallel.setCores(4)

model.solve()

svg = dnl.SvgInterface('SVG')
svg.setTitleDisplay(False)
svg.setLegendPosition(350, 150)
svg.write('temperatureCP.svg', dnl.Field.temperature)
svg.write('vonMisesCP.svg', dnl.Field.vonMises)

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
