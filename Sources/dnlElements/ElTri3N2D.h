/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElTri3n2D_h__
#define __dnlElements_ElTri3n2D_h__

#include <Element2D.h>
//-----------------------------------------------------------------------------
// Class : ElTri3N2D
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElTri3N2D : public Element2D
{

public:
  static const ElementData _elementData;

public:
  /* constructeurs */
  ElTri3N2D(long elementNumber = 1);
  ElTri3N2D(const ElTri3N2D &element);
  ~ElTri3N2D();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

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
