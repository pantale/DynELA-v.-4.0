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
  \file Drawing.h
  \brief Declaration file for the Drawing class

  This file is the declaration file for the Drawing class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_Drawing_h__
#define __dnlFEM_Drawing_h__

#include <Vec3D.h>

class Polygon;

class Drawing
{
private:
  bool initialized = false;

public:
  List<Polygon *> polygons;
  Vec3D bottomLeft, topRight;
  Vec3D center;
  Vec3D worldCenter;
  Vec3D worldScale;

public:
  // constructeurs
  Drawing();
  Drawing(const Drawing &Drawing);
  ~Drawing();

  void computeBoundBox();
  void initPolygons();
  void mapToWorld();
  void resetPolygons();
  void rotate(Vec3D axis, double angle);
  void zBufferSort();
};

#endif
