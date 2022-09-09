/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElTet4n3D_h__
#define __dnlElements_ElTet4n3D_h__

#include <Element3D.h>

//-----------------------------------------------------------------------------
// Class : ElTet4N3D
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElTet4N3D : public Element3D
{

public:
  static const ElementData _elementData;

public:
  // constructeurs
  ElTet4N3D(long elementNumber = 1);
  ElTet4N3D(const ElTet4N3D &element);
  ~ElTet4N3D();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength() { return 0; }

  /*  // fonction de manipulation
  long numberOfUnderIntegrationPoints ()
  {
    return 1;
  }

  // fonctions de calcul
  double getCharacteristicLength(); // calcule et renvoie la longueur caracteristique
  double getVolume();
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) ;
   void computeGlob2Loc();
  void glob2Loc(const Vec3D& point,Vec3D& local);
  //void getIntgtoNodes(Vector& N,const Vec3D& point) const; */
};

#endif
