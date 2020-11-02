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

/*!
  \file ElQua4NAx.h
  \brief Declaration file for the ElQua4NAx class

  This file is the declaration file for the ElQua4NAx class.
  \ingroup dnlElements
*/

#ifndef __dnlElements_ElQua4nAx_h__
#define __dnlElements_ElQua4nAx_h__

class ElementAxi;
#include <ElementAxi.h>

/*!
  \brief Management and manipulation of 4 nodes Axisymmetric element

  \ingroup dnlElements
*/
class ElQua4NAx : public ElementAxi
{

public:
  static const ElementData _elementData; //!< Database storage for the Element

public:
  ElQua4NAx(long = 1);
  ElQua4NAx(const ElQua4NAx &);
  ~ElQua4NAx();

  void getShapeFunctionAtPoint(Vector &, const Vec3D &) const;
  void getDerShapeFunctionAtPoint(Matrix &, const Vec3D &) const;
  double getCharacteristicLength();
  double getArea();
};

#endif
