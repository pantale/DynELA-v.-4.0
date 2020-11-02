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

#ifndef __dnlElements_ElHex8n3D_h__
#define __dnlElements_ElHex8n3D_h__

#include <Element3D.h>
class Element3D;

class ElHex8N3D : public Element3D
{

public:
  static const ElementData _elementData;

public:
  // constructeurs
  ElHex8N3D(long elementNumber = 1);
  ElHex8N3D(const ElHex8N3D &el);
  ~ElHex8N3D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength();
  double getVolume();

  /*  long numberOfUnderIntegrationPoints ()
  {
    return 1;
  }

  // fonctions de calcul
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) ;
  void computeGlob2Loc ();
  void glob2Loc (const Vec3D & point, Vec3D & local);
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const;

  // fonctions entree sortie
  void print (ostream & os) const;
  friend ostream & operator << (ostream & os, ElHex8N3D & element); */
};

#endif
