/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file Explicit.h
  \brief Declaration file for the Explicit class

  This file is the declaration file for the Explicit class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_Explicit_h__
#define __dnlFEM_Explicit_h__

#include <Solver.h>

#define defaultSpectralRadius 0.8182

/*!
  \brief Classe de definition et de manipulation des solveurs elements finis.
  \ingroup dnlFEM

  Cette classe sert à definir et à manipuler les solveurs disponibles au niveau d'un modele. Elle contient toutes les definitions de base concernant la gestion des temps de simulation (increment de temps, temps de debut, de fin ...) ainsi que la gestion complete des increments.
*/
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

  void computeDensity();
  void computePredictions();
  void endStep();
  void explicitSolve();
  void setDissipation(double dissipation);
  void solve(double solveUpToTime);
  void updateTimes();
};

#endif
