/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
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
  ElQua4NAx(long elementNumber = 1);
  ElQua4NAx(const ElQua4NAx &element);
  ~ElQua4NAx();

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
