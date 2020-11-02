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

#ifndef __dnlBC_BoundaryDisplacement_h__
#define __dnlBC_BoundaryDisplacement_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node; */

class BoundaryDisplacement : public Boundary
{

private:
  Vec3D _displacement;

public:
  // constructeurs
  BoundaryDisplacement(char *newName = NULL);
  BoundaryDisplacement(const Vec3D &newDisplacement);
  BoundaryDisplacement(const BoundaryDisplacement &X);
  ~BoundaryDisplacement();

  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newDisplacement);
  void setValue(double dispX, double dispY, double dispZ);

  // fonctions membres
  long getType()
  {
    return Boundary::Displacement;
  }
};
#endif
