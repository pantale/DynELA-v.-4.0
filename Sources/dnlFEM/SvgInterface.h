/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlFEM_SvgInterface_h__
#define __dnlFEM_SvgInterface_h__

#include <String.h>
#include <List.h>
#include <iostream>
#include <fstream>
#include <Vec3D.h>
#include <Polygon.h>
#include <ColorMap.h>

#define maxPolygonPatchLevel 4

struct SvgRotate
{
  Vec3D axis;
  double angle;
};

//-----------------------------------------------------------------------------
// Class : SvgInterface
//
// Used to export SVG files
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class SvgInterface
{
private:
  bool _autoRangeValues = true;
  bool _dataInfosDisplay = false;
  bool _initialized = false;
  bool _legendDisplay = true;
  bool _meshDisplay = true;
  bool _rotate = false;
  bool _titleDisplay = true;
  const Vec3D _svgTopRight = Vec3D(0, 0, 0);
  const Vec3D _svgBottomRight = Vec3D(1600, 1600, 0);
  double _height = 16;
  double _meshThickness = 1;
  double _scale = 1;
  double _scaleRatio = 0.95;
  double _width = 16;
  int _dataInfosX = 50;
  int _dataInfosY = 1200;
  int _legendX = 30;
  int _legendY = 30;
  int _patchDecompLevel = 1;
  int _titleX = 50;
  int _titleY = 1550;
  List<SvgRotate *> _rotateList;
  std::ofstream _stream;
  String _fileName;
  Vec3D _svgCenter;

  // Interface methods excluded from SWIG
#ifndef SWIG
  void closeSvgFile();
  void dataInfoWrite();
  void filledRectangleWrite(int x1, int y1, int x2, int y2, String col);
  void flatPolygonsWrite();
  void headerWrite();
  void initDrawing();
  void initSvgFile(String fileName);
  void interpolatedPolygonsWrite();
  void legendWrite();
  void lineWrite(int x1, int y1, int x2, int y2, double width = 1);
  void meshWrite();
  void rectangleWrite(int x1, int y1, int x2, int y2, String col = "black", double width = 1);
  void tailWrite();
  void textWrite(Vec3D location, String text, int size = 30, String color = "black");
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

public:
  ColorMap colorMap;
  short field;
  String name = "SvgInterface::_noname_"; // Default name of the SVG interface

  // constructeurs
  SvgInterface(char *newName = NULL);
  SvgInterface(const SvgInterface &SvgInterface);
  ~SvgInterface();

  void resetView();
  void rotate(char axis, double angle);
  void rotate(Vec3D axis, double angle);
  void setAutoRangeValues(bool autoRangeValues);
  void setInfoDisplay(bool display);
  void setInfoPosition(int x, int y);
  void setLegendDisplay(bool display);
  void setLegendPosition(int x, int y);
  void setMeshDisplay(bool display);
  void setMeshThickness(double thickness);
  void setPatchLevel(int level);
  void setTitleDisplay(bool display);
  void setTitlePosition(int x, int y);
  void write(std::string fileName, short field = -1);
};

#endif
