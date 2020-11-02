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

#ifndef __dnlElements_ElTet10n3D_h__
#define __dnlElements_ElTet10n3D_h__

#include <Element3D.h>

class ElTet10N3D : public Element3D
{

public:
  static const ElementData _elementData;

public:
  // constructeurs
  ElTet10N3D(long elementNumber = 1);
  ElTet10N3D(const ElTet10N3D &el);
  ~ElTet10N3D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength() { return 0; }

  /*  // fonction de manipulation
  long numberOfUnderIntegrationPoints ()
  {
    return 1;
  }

  // fonctions de calcul
  double getVolume();
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) ;
  void computeGlob2Loc();
  void glob2Loc(const Vec3D& point,Vec3D& local);
  //void getIntgtoNodes(Vector& N,const Vec3D& point) const; */
};

#endif
