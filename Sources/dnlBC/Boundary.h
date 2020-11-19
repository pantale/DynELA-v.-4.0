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

// TODOCXYFILE

/*!
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#ifndef __dnlBC_Boundary_h__
#define __dnlBC_Boundary_h__

#include <dnlElements.h>

/* class Node;
class NodeSet;
class Function; */

class Boundary
{
public:
  enum
  {
    Speed,
    Acceleration,
    Displacement,
    Restrain,
    Force,
    Temperature,
    Flux,
    Convection
  };

protected:
  Function *_function; // Fonction discrete associee si elle existe

public:
  List<NodeSet *> nodeSet;
  // List<bool> typeSet;
  String name; // Public name of the BoundaryCondition

public:
  Boundary(char *newName = NULL);
  Boundary(const Boundary &X);
  virtual ~Boundary();

  virtual long getType() = 0;
  virtual void applyInitial(Node *node, double currentTime, double timeStep) = 0;
  virtual void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep) = 0;
  virtual void applyConstantOnNewFields(Node *node, double currentTime, double timeStep) { fatalError("Pas prevu encore", "applyConstantOnNewFields %c", &node); }
  Function *getFunction();
  void setFunction(Function *function);
};

#endif
