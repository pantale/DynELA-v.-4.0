/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlFEM_Explicit_h__
#define __dnlFEM_Explicit_h__

#include <Solver.h>

#define defaultSpectralRadius 0.8182

//-----------------------------------------------------------------------------
// Class : Explicit
//
// Used to manage DynELA FEM Solvers for Explicit solve
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Explicit : public Solver
{

private:
  double _alphaF;
  double _alphaM;
  double _beta;
  double _gamma;
  double _rho_b;
  short _timeStepMethod;

  void computeChungHulbertIntegrationParameters();

public:
  // constructor
  Explicit(char *newName = NULL);
  Explicit(const Explicit &X);
  ~Explicit();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void computeDensity();
  void computePredictions();
  void endStep();
  void explicitSolve();
  void setDissipation(double dissipation);
  void solve(double solveUpToTime);
  void updateTimes();
};

#endif
