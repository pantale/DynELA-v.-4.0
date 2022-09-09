/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElQua4n2D_h__
#define __dnlElements_ElQua4n2D_h__

class Element2D;
#include <Element2D.h>

//-----------------------------------------------------------------------------
// Class : ElQua4N2D
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElQua4N2D : public Element2D
{

public:
  static const ElementData _elementData;

public:
  ElQua4N2D(long elementNumber = 1);
  ElQua4N2D(const ElQua4N2D &element);
  ~ElQua4N2D();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength();
  double getArea();
};

#endif
