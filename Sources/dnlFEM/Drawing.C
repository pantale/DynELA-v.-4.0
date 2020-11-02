/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file Drawing.h
  \brief Declaration file for the Drawing class

  This file is the declaration file for the Drawing class.

  \ingroup dnlFEM
*/

#include <Element.h>

#include <Drawing.h>
#include <Polygon.h>
#include <DynELA.h>

//-----------------------------------------------------------------------------
Drawing::Drawing()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Drawing::Drawing(const Drawing &Drawing)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Drawing::~Drawing()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Drawing::computeBoundBox()
//-----------------------------------------------------------------------------
{
  Vec3D coordinates = polygons.first()->vertices[0];
  bottomLeft = topRight = coordinates;

  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    for (int i = 0; i < polygon->points; i++)
    {
      coordinates = polygon->vertices[i];
      for (int j = 0; j < 3; j++)
      {
        if (coordinates(j) < bottomLeft(j))
          bottomLeft(j) = coordinates(j);
        if (coordinates(j) > topRight(j))
          topRight(j) = coordinates(j);
      }
    }
  }

  center = (topRight + bottomLeft) / 2;
}

//-----------------------------------------------------------------------------
bool compareCentersXYZ(Polygon *p1, Polygon *p2)
//-----------------------------------------------------------------------------
{
  int x1, x2;
  int prec = 1000;

  x1 = (int)(prec * p1->center(0));
  x2 = (int)(prec * p2->center(0));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  x1 = (int)(prec * p1->center(1));
  x2 = (int)(prec * p2->center(1));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  return (int)(prec * (p1->center(2)) >= (int)(prec * p2->center(2)));
}

//-----------------------------------------------------------------------------
bool compareCentersYZX(Polygon *p1, Polygon *p2)
//-----------------------------------------------------------------------------
{
  int x1, x2;
  int prec = 1000;

  x1 = (int)(prec * p1->center(1));
  x2 = (int)(prec * p2->center(1));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  x1 = (int)(prec * p1->center(2));
  x2 = (int)(prec * p2->center(2));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  return (int)(prec * (p1->center(0)) >= (int)(prec * p2->center(0)));
}

//-----------------------------------------------------------------------------
bool compareCentersZXY(Polygon *p1, Polygon *p2)
//-----------------------------------------------------------------------------
{
  int x1, x2;
  int prec = 1000;

  x1 = (int)(prec * p1->center(2));
  x2 = (int)(prec * p2->center(2));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  x1 = (int)(prec * p1->center(0));
  x2 = (int)(prec * p2->center(0));

  if (x1 < x2)
    return 0;
  if (x1 > x2)
    return 1;

  return (int)(prec * (p1->center(1)) >= (int)(prec * p2->center(1)));
}

//-----------------------------------------------------------------------------
void Drawing::initPolygons()
//-----------------------------------------------------------------------------
{
  Element *pel = dynelaData->model.elements.first();
  while ((pel = dynelaData->model.elements.currentUp()) != NULL)
  {
    for (int face = 0; face < pel->getNumberOfFaces(); face++)
    {
      // Creates a new polygon
      Polygon *polygon = new Polygon;

      // Add vertices
      for (int node = 0; node < pel->getNumberOfNodesOnFace(face); node++)
      {
        polygon->add(pel->getNodeOnFace(face, node));
      }

      // Computes center of polygon
      polygon->computeCenter();

      // Add polygon to list
      polygons << polygon;
    }
  }

  pel = dynelaData->model.elements.first();
  if (pel->getFamily() == Element::Threedimensional)
  {
    for (int pass = 0; pass < 3; pass++)
    {
      // tri préliminaire en fonction des centres
      if (pass == 0)
        polygons.sort(compareCentersXYZ);
      if (pass == 1)
        polygons.sort(compareCentersYZX);
      if (pass == 2)
        polygons.sort(compareCentersZXY);
      Polygon *p1, *p2;
      for (int i = 1; i < polygons.getSize(); i++)
      {
        p1 = polygons(i - 1);
        p2 = polygons(i);
        if (((p1->center - p2->center).getSquareNorm() < 1e-2) && (p1->points == p2->points))
        {
          int tot = 0;
          for (int j = 0; j < p1->points; j++)
          {
            for (int k = 0; k < p2->points; k++)
            {
              if (p1->nodes[j] == p2->nodes[k])
              {
                tot++;
                k = p2->points;
              }
            }
          }
          // destruction du polygone
          if (tot == p1->points)
          {
            polygons.del(i - 1, i);
            i--;
          }
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
void Drawing::resetPolygons()
//-----------------------------------------------------------------------------
{
  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->resetCoordinates();
  }
}

//-----------------------------------------------------------------------------
void Drawing::rotate(Vec3D axis, double angle)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;

  double angleRadians = angle * dnlDegToRad;

  // normalisation de l'axe
  axis.normalize();

  Mat(0, 0) = dnlSquare(axis(0)) + cos(angleRadians) * (-dnlSquare(axis(0)) + 1);
  Mat(0, 1) = axis(0) * axis(1) - axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(0, 2) = axis(0) * axis(2) + axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(1, 0) = axis(0) * axis(1) + axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(1, 1) = dnlSquare(axis(1)) + cos(angleRadians) * (-dnlSquare(axis(1)) + 1);
  Mat(1, 2) = axis(1) * axis(2) - axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 0) = axis(0) * axis(2) - axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(2, 1) = axis(1) * axis(2) + axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 2) = dnlSquare(axis(2)) + cos(angleRadians) * (-dnlSquare(axis(2)) + 1);

  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->rotate(Mat);
  }
}

//-----------------------------------------------------------------------------
void Drawing::mapToWorld()
//-----------------------------------------------------------------------------
{
  // Zbuffer diaplay
  // if (dynelaData->model.elements.first()->getFamily() == Element::Threedimensional)
  zBufferSort();

  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->remapVertices(center, worldCenter, worldScale);
  }
}

//-----------------------------------------------------------------------------
bool zBufferCenters(Polygon *p1, Polygon *p2)
//-----------------------------------------------------------------------------
{
  return p1->center(2) > p2->center(2);
}

//-----------------------------------------------------------------------------
void Drawing::zBufferSort()
//-----------------------------------------------------------------------------
{
  polygons.sort(zBufferCenters);

  Vec3D zAxis = Vec3D(0, 0, 1);
  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->computeNormal();
    polygon->visible = (polygon->normal.dotProduct(zAxis) >= 0 ? true : false);
  }
}
