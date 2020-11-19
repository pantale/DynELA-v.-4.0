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
  \file Polygon.h
  \brief Declaration file for the Polygon class

  This file is the declaration file for the Polygon class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_Polygon_h__
#define __dnlFEM_Polygon_h__

#include <Vec3D.h>
#include <ColorMap.h>

class Node;

#define PolygonMaxNodes 8

class Polygon
{
  friend bool compareCentersXYZ(Polygon *p1, Polygon *p2);
  friend bool compareCentersYZX(Polygon *p1, Polygon *p2);
  friend bool compareCentersZXY(Polygon *p1, Polygon *p2);
  friend bool zBufferCenters(Polygon *p1, Polygon *p2);
  friend class Drawing;
  friend class PolygonPatches;

private:
  bool visible = true;
  int points = 0;
  Node *nodes[PolygonMaxNodes];
  Vec3D center;
  Vec3D normal;
  Vec3D vertices[PolygonMaxNodes];

public:
  // constructeurs
  Polygon();
  ~Polygon();

  bool isVisible() { return visible; }
  int numberOfPoints();
  String getFlatPolygonSvgCode(ColorMap &map, short field, bool stroke, double width);
  String getInterpolatedPolygonSvgCode(ColorMap &map, int decompLevel, short field, bool stroke, double width);
  String getWhitePolygonSvgCode(double width);
  Vec3D getVertex(int);
  void add(Node *node);
  void add(Vec3D point);
  void computeCenter();
  void computeNormal();
  void init();
  void remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale);
  void resetCoordinates();
  void rotate(Tensor2 Mat);
};

class PolygonPatch
{
public:
  double value;
  int colorIndex;
  int number;
  Vec3D coords[8];

public:
  // constructeurs
  PolygonPatch() {}
  ~PolygonPatch() {}
};

class PolygonPatches
{
private:
  int _decompLevel;
  List<PolygonPatch *> _polygonPatches;
  friend class Polygon;

  PolygonPatch *createPolygonPatch();
  void createSubPatch(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  void createSubPatch2(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  void reorderPoints(Vec3D *coords, int cur);

public:
  // constructeurs
  PolygonPatches(int level);
  ~PolygonPatches() {}

  void createPatch(Polygon *polygon, ColorMap &map, short field);
};

#endif
