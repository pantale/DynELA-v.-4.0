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
\file ElementPlane.h
\brief Declaration file for the ElementPlane class

This file is the declaration file for the ElementPlane class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElTri3n2D_h__
#define __dnlElements_ElTri3n2D_h__

#include <Element2D.h>
class ElTri3N2D : public Element2D
{

public:
  static const ElementData _elementData;

public:
  /* constructeurs */
  ElTri3N2D(long elementNumber = 1);
  ElTri3N2D(const ElTri3N2D &element);
  ~ElTri3N2D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength() { return 0; }

  /*  long numberOfUnderIntegrationPoints ()
  {
    return 1;
  }

  double getCharacteristicLength ();
  double getVolume ();
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) ;
  void computeGlob2Loc ();
  void glob2Loc (const Vec3D & point, Vec3D & local);
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const; */
};

#endif
