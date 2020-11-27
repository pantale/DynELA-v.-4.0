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

#ifndef __dnlBC_BoundaryCondition_h__
#define __dnlBC_BoundaryCondition_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundaryCondition
//
// Used to manage BoundaryConditions in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class BoundaryCondition
{

protected:
  List<Boundary *> constant; // Liste des conditions aux limites constantes
  List<Boundary *> initial;  // Liste des conditions aux limites initiales

public:
  // constructeurs
  BoundaryCondition();
  BoundaryCondition(const BoundaryCondition &X);
  ~BoundaryCondition();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  bool applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  bool applyInitial(Node *node, double currentTime, double timeStep);
  bool existConstant();
  bool existInitial();
  Boundary *getConstant(long i);
  Boundary *getInitial(long i);
  /* Boundary *isConstantCondition(long value);
  Boundary *isInitialCondition(long value); */
  long getNumberOfConstants();
  long getNumberOfInitials();
  void addConstant(Boundary *bc);
  void addInitial(Boundary *bc);
};
#endif
