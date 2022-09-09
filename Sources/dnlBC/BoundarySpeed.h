/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlBC_BoundarySpeed_h__
#define __dnlBC_BoundarySpeed_h__

#include <Boundary.h>

//-----------------------------------------------------------------------------
// Class : BoundarySpeed
//
// Used to manage BoundarySpeeds in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

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
