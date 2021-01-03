/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElQua4nAx_h__
#define __dnlElements_ElQua4nAx_h__

class ElementAxi;
#include <ElementAxi.h>

//-----------------------------------------------------------------------------
// Class : ElQua4NAx
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElQua4NAx : public ElementAxi
{

public:
  static const ElementData _elementData; // Database storage for the Element

public:
  ElQua4NAx(long = 1);
  ElQua4NAx(const ElQua4NAx &);
  ~ElQua4NAx();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void getShapeFunctionAtPoint(Vector &, const Vec3D &) const;
  void getDerShapeFunctionAtPoint(Matrix &, const Vec3D &) const;
  double getCharacteristicLength();
  double getArea();
};

#endif
