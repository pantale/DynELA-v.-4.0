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
\file ElQua4N2D.h
\brief Declaration file for the ElQua4N2D class

This file is the declaration file for the ElQua4N2D class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElQua4n2D_h__
#define __dnlElements_ElQua4n2D_h__

class Element2D;
#include <Element2D.h>

class ElQua4N2D : public Element2D
{

public:
  static const ElementData _elementData;

public:
  ElQua4N2D(long elementNumber = 1);
  ElQua4N2D(const ElQua4N2D &element);
  ~ElQua4N2D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength();
  double getArea();
};

#endif
