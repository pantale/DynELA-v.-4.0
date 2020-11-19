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
\file ElementAxi.h
\brief Declaration file for the ElementAxi class

This file is the declaration file for the ElementAxi class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElementAxi_h__
#define __dnlElements_ElementAxi_h__

#include <ElementPlane.h>

class ElementAxi : public ElementPlane
{

public:
  ElementAxi(long elementNumber = 1);
  ElementAxi(const ElementAxi &X);
  ~ElementAxi();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
  double getRadiusAtIntegrationPoint();
  double getRadiusAtUnderIntegrationPoint();
  void computeDeformationGradient(Tensor2 &F, short time);
  bool checkLevel2();
  void getdV_atIntPoint(Tensor2 &dv, short time);

  /*  void getdU_atIntPoint (Tensor2 & du, short time);
  void computeDeformationGradient (Tensor2 & du, short time);
  long numberOfUnderIntegrationPoints () = 0;
  double getCharacteristicLength () = 0;
  double getVolume () = 0;
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc () = 0;
  void glob2Loc (const Vec3D & point, Vec3D & local) = 0; */
};

#endif
