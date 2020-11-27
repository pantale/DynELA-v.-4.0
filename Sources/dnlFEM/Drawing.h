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

#ifndef __dnlFEM_Drawing_h__
#define __dnlFEM_Drawing_h__

#include <Vec3D.h>

class Polygon;

//-----------------------------------------------------------------------------
// Class : Drawing
//
// Used to manage Drawing for SVG files
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
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
#endif // from #if !defined(SWIG)

#endif
