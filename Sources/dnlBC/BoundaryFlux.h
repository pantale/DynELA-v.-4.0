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

#ifndef __dnlBC_BoundaryFlux_h__
#define __dnlBC_BoundaryFlux_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node;
 */
class BoundaryFlux : public Boundary
{

private:
  double _flux;

public:
  // constructeurs
  BoundaryFlux(char *newName = NULL);
  BoundaryFlux(double newFlux);
  BoundaryFlux(const BoundaryFlux &X);
  ~BoundaryFlux();

  // fonctions membres
  long getType() { return Boundary::Flux; }
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  double getValue();
  void setValue(double val);
};

#endif
