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
\file ElementPlane.h
\brief Declaration file for the ElementPlane class

This file is the declaration file for the ElementPlane class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElementPlane_h__
#define __dnlElements_ElementPlane_h__

#include <Element.h>

class ElementPlane : public Element
{

public:
  ElementPlane(long elementNumber = 1);
  ElementPlane(const ElementPlane &element);
  ~ElementPlane();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
  double getCharacteristicLength() = 0;
  double getRadiusAtIntegrationPoint();
  double getRadiusAtUnderIntegrationPoint();
  bool computeJacobian(bool reference = false);
  bool computeUnderJacobian(bool reference = false);
  void getV_atIntPoint(Vec3D &v, short time);
  void computeElasticStiffnessMatrix(bool underIntegration = false);

  /*
  void getU_atIntPoint (Vec3D & u, short time);
  long numberOfUnderIntegrationPoints () = 0;
  double getVolume () = 0;
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc () = 0;
  void glob2Loc (const Vec3D & point, Vec3D & local) = 0; */
};

#endif
