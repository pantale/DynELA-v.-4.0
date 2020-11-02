#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov  7 14:14:48 2019

@author: pantale
"""

import os
iconsDir = os.environ['DYNELA'] + '/Samples/Flags'

import subprocess
import shutil
import numpy as np
import argparse

class Benchmark:
    def __init__(self, name):
        self.Benchmark = name
        self.tolerance = 1e-1

    def setPlotFiles(self, _files):
        self.files = _files
        
    def setTolerance(self, tol):
        self.tolerance = tol
        
    def run(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--run", action="store_true")
        parser.add_argument("--reference", action="store_true")
        args = parser.parse_args()
        if (args.run):
            self.runCompare()
        if (args.reference):
            self.setReference()
    
    def runBench(self):
        print("Run the " + self.Benchmark + " benchmark\n")
        bashCommand = "python3 " + self.Benchmark + ".py"
        benchRun = subprocess.run(bashCommand, shell=True, check=True)
        return benchRun.returncode

    def runCompare(self):
        diff = False
        shutil.copy2(iconsDir + '/run-flag.png', '_bench.png')
        benchRun = self.runBench()
        if (benchRun == 0):
            print("Success\n")
            for file in self.files:
                try:
                    data1 = np.genfromtxt(file)
                    data0 = np.genfromtxt(file + '.ref')
                    if ((np.abs(data1-data0)).max() > self.tolerance):
                        print("Found differences in result file " +file+"\n")
                        print("  Delta is " + str((np.abs(data1-data0)).max())+"\n")
                        diff = True
                except:
                   print("Unable to check:", file, "vs", file + ".ref -> Ignored\n")
            if (diff == False):
                print("Benchmark is OK\n")
                shutil.copy2(iconsDir + '/ok-flag.png', '_bench.png')
                return 0
            else:
                print("Delta founds in solution\n")
                shutil.copy2(iconsDir + '/ko-flag.png', '_bench.png')
                return 1
        else:
            print("Process failed\n")
            shutil.copy2(iconsDir + '/ko-flag.png', '_bench.png')
            return -1
        
    def setReference(self):
        for file in self.files:
            shutil.copy2(file, file + '.ref')
