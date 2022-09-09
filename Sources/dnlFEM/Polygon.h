/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlFEM_Polygon_h__
#define __dnlFEM_Polygon_h__

#include <Vec3D.h>
#include <ColorMap.h>

class Node;

#define PolygonMaxNodes 8

//-----------------------------------------------------------------------------
// Class : Polygon
//
// Used to manage Polygons for SVG files
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
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
#endif // from #if !defined(SWIG)

//-----------------------------------------------------------------------------
// Class : PolygonPatch
//
// Used to manage PolygonPatch for SVG files
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
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
#endif // from #if !defined(SWIG)

//-----------------------------------------------------------------------------
// Class : PolygonPatches
//
// Used to manage PolygonPatches for SVG files
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
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
#endif // from #if !defined(SWIG)

#endif
