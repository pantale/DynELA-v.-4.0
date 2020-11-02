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

#ifndef __dnlBC_BoundarySpeed_h__
#define __dnlBC_BoundarySpeed_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node; */

class BoundarySpeed : public Boundary
{

private:
  Vec3D _speed; // Vecteur contenant la vitesse du point

public:
  // constructeurs
  BoundarySpeed(char *newName = NULL);
  BoundarySpeed(const Vec3D &newSpeed);
  BoundarySpeed(const BoundarySpeed &X);
  ~BoundarySpeed();

  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newSpeed);
  void setValue(double speedX, double speedY, double speedZ);

  // fonctions membres
  long getType()
  {
    return Boundary::Speed;
  }
};

#endif
