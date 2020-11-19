/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
\file ElTet4N3D.h
\brief Declaration file for the ElTet4N3D class

This file is the declaration file for the ElTet4N3D class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElTet4n3D_h__
#define __dnlElements_ElTet4n3D_h__

#include <Element3D.h>

class ElTet4N3D : public Element3D
{

public:
  static const ElementData _elementData;

public:
  // constructeurs
  ElTet4N3D(long elementNumber = 1);
  ElTet4N3D(const ElTet4N3D &element);
  ~ElTet4N3D();

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
