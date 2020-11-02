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

#ifndef __dnlBC_BoundaryRestrain_h__
#define __dnlBC_BoundaryRestrain_h__

#include <Boundary.h>
/* #include <Vec3D.h>
class Node;
 */
class BoundaryRestrain : public Boundary
{

private:
  Vec3D _restrain;

public:
  // constructeurs
  BoundaryRestrain(char *newName = NULL);
  BoundaryRestrain(const Vec3D &newRestrain);
  BoundaryRestrain(const BoundaryRestrain &X);
  ~BoundaryRestrain();

  void applyConstantOnNewFields(Node *node, double currentTime, double timeStep);
  void applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep);
  void applyInitial(Node *node, double currentTime, double timeStep);
  Vec3D getValue();
  void setValue(const Vec3D &newRestrain);
  void setValue(double restrainX, double restrainY, double restrainZ);

  // fonctions membres
  long getType()
  {
    return Boundary::Restrain;
  }
};

#endif
