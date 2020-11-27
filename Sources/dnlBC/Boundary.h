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

#ifndef __dnlBC_Boundary_h__
#define __dnlBC_Boundary_h__

#include <dnlElements.h>

//-----------------------------------------------------------------------------
// Class : Boundary
//
// Used to manage Boundary in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  virtual long getType() = 0;
  virtual void applyInitial(Node *node, double currentTime, double timeStep) = 0;
  virtual void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep) = 0;
  virtual void applyConstantOnNewFields(Node *node, double currentTime, double timeStep) { fatalError("Pas prevu encore", "applyConstantOnNewFields %c", &node); }
  Function *getFunction();
  void setFunction(Function *function);
};

#endif
