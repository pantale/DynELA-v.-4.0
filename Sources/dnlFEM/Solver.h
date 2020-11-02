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
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_Solver_h__
#define __dnlFEM_Solver_h__

#include <String.h>

class Model;

/* #include <upFemSolver.h>
class Model;
extern String parsedFileName;
 */
/*!
  \file Solver.h
  \brief fichier .h de definition des solveurs elements finis
  \ingroup femSolver

  Ce fichier sert à la definition de la classe Solver.

*/

/*!
  \class Solver Solver.h
  \brief Classe de definition et de manipulation des solveurs elements finis.
  \ingroup femSolver

  Cette classe sert à definir et à manipuler les solveurs disponibles au niveau d'un modele. Elle contient toutes les definitions de base concernant la gestion des temps de simulation (increment de temps, temps de debut, de fin ...) ainsi que la gestion complete des increments.

*/
class Solver
{
protected:
  double _omegaS = 2.0;
  double _solveUpToTime = 0.0;
  double _timeStepSafetyFactor = 0.9;
  int _computeTimeStepFrequency = 10;
  int _reportFrequency = 100;
  short _timeStepMethod = Courant;

public:
  enum
  {
    Courant,
    PowerIteration,
    PowerIterationUnder
  };

public:
  Model *model = NULL;       // Current model for the solver
  double timeStep = 0.0;     // Current time step for integration
  double endTime = 0.0;      // End time of the solver
  double startTime = 0.0;    // Start time of the solver
  long currentIncrement = 0; // Current increment
  long endIncrement = 0;     // Final increment
  long startIncrement = 0;   // Initial increment
  String name = "_noname_";  // Name of the solver

public:
  Solver(char *newName = NULL);
  Solver(const Solver &X);
  virtual ~Solver();

  bool timeAndIncrementsAreBetweenBounds();
  bool timeIsBetweenBounds();
  bool timeIsBetweenBounds(double time);
  double getTimeStep();
  virtual void solve(double solveUpToTime) = 0;
  void computeTimeStep(bool forceComputation = false);
  void initialize();
  void setComputeTimeStepFrequency(int frequency);
  void setIncrements(long start, long stop);
  void setModel(Model *newModel);
  void setTimes(double start_time, double end_time);
  void setTimeStepMethod(short method);
  void setTimeStepSafetyFactor(double safetyfactor);
  // member functions
  /*
  bool timeOk ();
  double getEndTime() {return endTime;}
  double getStartTime() {return startTime;}
  double getTimeStep() {return timeStep;}
  void progressWrite ();

  // virtual methods
  virtual const char* Name() = 0;
 */
};

#endif
