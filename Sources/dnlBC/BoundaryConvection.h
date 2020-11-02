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

#ifndef __dnlBC_BoundaryConvection_h__
#define __dnlBC_BoundaryConvection_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node; */

class BoundaryConvection : public Boundary
{

public:
  double _hc;
  double _Tf;

public:
  // constructeurs
  BoundaryConvection(char *newName = NULL);
  BoundaryConvection(double newhc, double newTf);
  BoundaryConvection(const BoundaryConvection &X);
  ~BoundaryConvection();

  // fonctions membres
  long getType() { return Boundary::Convection; }
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(double newhc, double newTf);
};

#endif
