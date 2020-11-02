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

#ifndef __dnlBC_BoundaryCondition_h__
#define __dnlBC_BoundaryCondition_h__

#include <Boundary.h>

/* class Boundary;
class Node; */

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
