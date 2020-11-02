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

#ifndef __dnlBC_BoundaryForce_h__
#define __dnlBC_BoundaryForce_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node; */

class BoundaryForce : public Boundary
{

private:
  Vec3D _force;

public:
  // constructeurs
  BoundaryForce(char *newName = NULL);
  BoundaryForce(const Vec3D &newForce);
  BoundaryForce(const BoundaryForce &X);
  ~BoundaryForce();

  // fonctions membres
  long getType()
  {
    return Boundary::Force;
  }
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newForce);
  void setValue(double forceX, double forceY, double forceZ);
};

#endif
