/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file Polygon.h
  \brief Declaration file for the Polygon class

  This file is the declaration file for the Polygon class.

  \ingroup dnlFEM
*/

#include <Tensor2.h>
#include <Polygon.h>
#include <Field.h>
#include <DynELA.h>
#include <Node.h>

//-----------------------------------------------------------------------------
Polygon::Polygon()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Polygon::~Polygon()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Polygon::init()
//-----------------------------------------------------------------------------
{
  points = 0;
}

//-----------------------------------------------------------------------------
int Polygon::numberOfPoints()
//-----------------------------------------------------------------------------
{
  return points;
}

//-----------------------------------------------------------------------------
void Polygon::add(Vec3D point)
//-----------------------------------------------------------------------------
{
  if (points >= PolygonMaxNodes)
  {
    fatalError("No left space in polygon");
  }
  vertices[points++] = point;
}

//-----------------------------------------------------------------------------
void Polygon::add(Node *node)
//-----------------------------------------------------------------------------
{
  if (points >= PolygonMaxNodes)
  {
    fatalError("No left space in polygon");
  }
  nodes[points] = node;
  vertices[points] = node->coordinates;
  points++;
}

//-----------------------------------------------------------------------------
Vec3D Polygon::getVertex(int vertex)
//-----------------------------------------------------------------------------
{
  return vertices[vertex];
}

//-----------------------------------------------------------------------------
void Polygon::computeCenter()
//-----------------------------------------------------------------------------
{
  center = 0.0;
  for (int i = 0; i < points; i++)
    center += vertices[i];
  center /= points;
}

//-----------------------------------------------------------------------------
void Polygon::computeNormal()
//-----------------------------------------------------------------------------
{
  Vec3D v1, v2;

  v1 = vertices[2] - vertices[1];
  v2 = vertices[0] - vertices[1];

  normal = v1.vectorialProduct(v2);
  normal.normalize();
}

//-----------------------------------------------------------------------------
void Polygon::resetCoordinates()
//-----------------------------------------------------------------------------
{
  center = 0.0;
  for (int i = 0; i < points; i++)
  {
    vertices[i] = nodes[i]->coordinates;
    center += vertices[i];
  }
  center /= points;
}

//-----------------------------------------------------------------------------
void Polygon::remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < points; i++)
  {
    vertices[i] = worldScale.componentsProduct(vertices[i] - center) + worldCenter;
  }
}

//-----------------------------------------------------------------------------
String Polygon::getWhitePolygonSvgCode(double width)
//-----------------------------------------------------------------------------
{
  String svgcode = "";
  String tmp1, tmp2;
  Vec3D point;
  String stringWidth;
  stringWidth.convert(width, "%g");

  // Begin polygon
  svgcode += "<polygon\n";
  svgcode += " stroke=\"black\"\n";
  svgcode += " fill=\"white\"\n";
  svgcode += " stroke-width=\"" + stringWidth + "\"\n";
  svgcode += " stroke-linejoin=\"round\"\n";
  svgcode += " points=\"";

  // polygon points
  for (int i = 0; i < points; i++)
  {
    point = getVertex(i);
    tmp1.convert(point(0));
    tmp2.convert(point(1));
    svgcode += tmp1 + "," + tmp2 + " ";
  }

  // End polygon
  svgcode += "\" />\n";

  // Returns the string
  return svgcode;
}

//-----------------------------------------------------------------------------
String Polygon::getFlatPolygonSvgCode(ColorMap &map, short field, bool stroke, double width)
//-----------------------------------------------------------------------------
{
  String svgcode = "";
  String tmp1, tmp2;
  Vec3D point;
  String stringWidth;
  stringWidth.convert(width, "%g");

  double val = 0;
  for (int i = 0; i < points; i++)
  {
    val += nodes[i]->getNodalValue(field);
  }
  val /= points;

  String col = map.getStringColor(val, true);

  // Begin polygon
  svgcode += "<polygon\n";
  svgcode += " fill=\"" + col + "\"\n";
  if (stroke)
  {
    svgcode += " stroke=\"black\"\n";
    svgcode += " stroke-width=\"" + stringWidth + "\"\n";
  }
  else
  {
    svgcode += " stroke=\"" + col + "\"\n";
    svgcode += " stroke-width=\"0.25\"\n";
  }
  svgcode += " stroke-linejoin=\"round\"\n";

  svgcode += " points=\"";

  // polygon points
  for (int i = 0; i < points; i++)
  {
    point = getVertex(i);
    tmp1.convert(point(0));
    tmp2.convert(point(1));
    svgcode += tmp1 + "," + tmp2 + " ";
  }

  // End polygon
  svgcode += "\" />\n";

  // Returns the string
  return svgcode;
}

//-----------------------------------------------------------------------------
String Polygon::getInterpolatedPolygonSvgCode(ColorMap &map, int decompLevel, short field, bool stroke, double width)
//-----------------------------------------------------------------------------
{
  String svgcode = "";
  String tmp1, tmp2;
  Vec3D point;
  String stringWidth;
  stringWidth.convert(width, "%g");
  PolygonPatches _polygonPatches(decompLevel);
  PolygonPatch *patch;
  String col;

  _polygonPatches.createPatch(this, map, field);

  patch = _polygonPatches._polygonPatches.first();
  while ((patch = _polygonPatches._polygonPatches.currentUp()) != NULL)
  {
    // Begin polygon
    svgcode += "<polygon\n";

    col = map.getStringColor(patch->value, true);

    svgcode += " fill=\"" + col + "\"\n";

    svgcode += " points=\"";
    // polygon points
    for (int i = 0; i < patch->number; i++)
    {
      point = patch->coords[i];
      tmp1.convert(point(0));
      tmp2.convert(point(1));
      svgcode += tmp1 + "," + tmp2 + " ";
    }

    // End polygon
    svgcode += "\" />\n";
  }

  if (stroke)
  {
    // Begin polygon
    svgcode += "<polygon\n";
    svgcode += " stroke=\"black\"\n";
    svgcode += " fill=\"none\"\n";

    svgcode += " stroke-width=\"" + stringWidth + "\"\n";
    svgcode += " points=\"";

    // polygon points
    for (int i = 0; i < points; i++)
    {
      point = getVertex(i);
      tmp1.convert(point(0));
      tmp2.convert(point(1));
      svgcode += tmp1 + "," + tmp2 + " ";
    }

    // End polygon
    svgcode += "\" />\n";
  }

  return svgcode;
}

//-----------------------------------------------------------------------------
void Polygon::rotate(Tensor2 Mat)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < points; i++)
    vertices[i] = Mat * vertices[i];
  computeCenter();
}

//-----------------------------------------------------------------------------
PolygonPatches::PolygonPatches(int level)
//-----------------------------------------------------------------------------
{
  _decompLevel = level;
}

//-----------------------------------------------------------------------------
void PolygonPatches::createPatch(Polygon *polygon, ColorMap &map, short field)
//-----------------------------------------------------------------------------
{
  Vec3D crds[9];
  double valR[9];
  int valI[9];
  int nb = polygon->points;

  // Flush list of patches
  _polygonPatches.flush();

  for (int i = 0; i < nb; i++)
  {
    crds[i] = polygon->vertices[i];
    valR[i] = polygon->nodes[i]->getNodalValue(field);
    valI[i] = map.getIntColor(valR[i]);
  }

  if (nb == 3)
  {
    crds[3] = crds[2];
    valR[3] = valR[2];
    valI[3] = valI[2];
    nb = 4;
  }

  // construction des sous polygones
  createSubPatch(nb, crds, valR, valI, map);
}

//-----------------------------------------------------------------------------
PolygonPatch *PolygonPatches::createPolygonPatch()
//-----------------------------------------------------------------------------
{
  PolygonPatch *poly = new PolygonPatch;
  _polygonPatches << poly;
  return poly;
}

//-----------------------------------------------------------------------------
void PolygonPatches::createSubPatch2(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map)
//-----------------------------------------------------------------------------
{
  Vec3D crds[9];
  int c_min, c_max;
  PolygonPatch *patch;
  double mmin, mmax;
  int mlevels;

  c_min = valuesI[0];
  c_max = valuesI[0];
  for (int j = 1; j < points; j++)
  {
    if (valuesI[j] < c_min)
      c_min = valuesI[j];
    if (valuesI[j] > c_max)
      c_max = valuesI[j];
  }

  // polygone 1 couleur homogene
  if (c_min == c_max)
  {
    // creation du polygone
    patch = createPolygonPatch();

    // couleur remplissage
    patch->colorIndex = c_min;
    patch->value = valuesR[0];

    // ecriture du polygone
    for (int j = 0; j < points; j++)
    {
      patch->coords[j] = coords[j];
    }

    // sauvegarde du nombre
    patch->number = points;
  }
  else
  {
    double val, val_1;
    int cur_ps_l;
    int i1, i2;
    double dd1, dd2, dd3, dd4;

    for (int j = c_min; j <= c_max; j++)
    {
      // creation du polygone
      patch = createPolygonPatch();

      map.getBounds(mmin, mmax, mlevels);
      val = mmin + (mmax - mmin) / mlevels * j;

      // calcul de la zone a tracer
      val_1 = mmin + (mmax - mmin) / mlevels * (j + 1);
      patch->value = (val + val_1) / 2.0;
      cur_ps_l = 0;

      // ajout des sommets
      for (int k = 0; k < points; k++)
      {
        if (dnlBetweenValues(valuesR[k], val, val_1))
        {
          crds[cur_ps_l] = coords[k];
          cur_ps_l++;
        }
      }

      // calcul des intersections des bords
      for (int k = 0; k < points; k++)
      {

        // indices
        i1 = k;
        i2 = (k == 0 ? points - 1 : k - 1);

        if (dnlBetweenValuesExclude(val, dnlMin(valuesR[i1], valuesR[i2]), dnlMax(valuesR[i1], valuesR[i2])))
        {

          crds[cur_ps_l] = (val * (coords[i2] - coords[i1]) + valuesR[i2] * coords[i1] - valuesR[i1] * coords[i2]) / (valuesR[i2] - valuesR[i1]);
          cur_ps_l++;
        }

        if (dnlBetweenValuesExclude(val_1, dnlMin(valuesR[i1], valuesR[i2]), dnlMax(valuesR[i1], valuesR[i2])))
        {

          crds[cur_ps_l] = (val_1 * (coords[i2] - coords[i1]) + valuesR[i2] * coords[i1] - valuesR[i1] * coords[i2]) / (valuesR[i2] - valuesR[i1]);
          cur_ps_l++;
        }
      }

      // direct a 3 points
      patch->number = cur_ps_l;
      switch (cur_ps_l)
      {

      case 0:
      case 1:
      case 2:
        break;

      case 3:
        for (int k = 0; k < cur_ps_l; k++)
        {
          patch->coords[k] = crds[k];
        }
        break;
      case 4:
#define DLL(a, b, c, d)               \
  (crds[b] - crds[a]).getNorm() +     \
      (crds[c] - crds[b]).getNorm() + \
      (crds[d] - crds[c]).getNorm() + \
      (crds[a] - crds[d]).getNorm();

        dd1 = DLL(0, 1, 2, 3);
        dd2 = DLL(0, 1, 3, 2);
        dd3 = DLL(0, 2, 1, 3);
        dd4 = dnlMin3(dd1, dd2, dd3);
        if (dd4 == dd1)
        {
          patch->coords[0] = crds[0];
          patch->coords[1] = crds[1];
          patch->coords[2] = crds[2];
          patch->coords[3] = crds[3];
        }
        if (dd4 == dd2)
        {
          patch->coords[0] = crds[0];
          patch->coords[1] = crds[1];
          patch->coords[2] = crds[3];
          patch->coords[3] = crds[2];
        }
        if (dd4 == dd3)
        {
          patch->coords[0] = crds[0];
          patch->coords[1] = crds[2];
          patch->coords[2] = crds[1];
          patch->coords[3] = crds[3];
        }
        break;

      default:
        reorderPoints(crds, cur_ps_l);
        for (int k = 0; k < cur_ps_l; k++)
        {
          patch->coords[k] = crds[k];
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
void PolygonPatches::createSubPatch(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map)
//-----------------------------------------------------------------------------
{
  Vec3D crds[9];
  double valR[9];
  int valI[9];
  int c_min, c_max;
  PolygonPatch *patch;

  // le polygone est il homogene
  c_min = valuesI[0];
  c_max = valuesI[0];
  for (int j = 1; j < points; j++)
  {
    if (valuesI[j] < c_min)
      c_min = valuesI[j];
    if (valuesI[j] > c_max)
      c_max = valuesI[j];
  }

  /*
  if (c_max < 0)
  {
    // creation du polygone
    patch = createPolygonPatch();
    patch->value = map.getMin();

    // ecriture du polygone
    for (int j = 0; j < points; j++)
    {
      patch->coords[j] = coords[j];
    }

    // sauvegarde du nombre
    patch->number = points;

    return;
  }

  if (c_min > map.getLevels())
  {

    // creation du polygone
    patch = createPolygonPatch();
    patch->value = 0.0;

    // ecriture du polygone
    for (int j = 0; j < points; j++)
    {
      patch->coords[j] = coords[j];
    }

    // sauvegarde du nombre
    patch->number = points;

    return;
  }*/

  // polygone 1 couleur homogene
  if (c_min == c_max)
  {
    createSubPatch2(4, coords, valuesR, valuesI, map);
    return;
  }

  // subdivision de l'element
  _decompLevel--;

  // calcul du milieu de l'element indice 8
  coords[8] = (coords[0] + coords[1] + coords[2] + coords[3]) / 4.;
  valuesR[8] = (valuesR[0] + valuesR[1] + valuesR[2] + valuesR[3]) / 4.;
  valuesI[8] = map.getIntColor(valuesR[8]);

  // calcul de l'indice 4
  coords[4] = (coords[0] + coords[1]) / 2.;
  valuesR[4] = (valuesR[0] + valuesR[1]) / 2.;
  valuesI[4] = map.getIntColor(valuesR[4]);

  // calcul de l'indice 5
  coords[5] = (coords[1] + coords[2]) / 2.;
  valuesR[5] = (valuesR[1] + valuesR[2]) / 2.;
  valuesI[5] = map.getIntColor(valuesR[5]);

  // calcul de l'indice 6
  coords[6] = (coords[2] + coords[3]) / 2.;
  valuesR[6] = (valuesR[2] + valuesR[3]) / 2.;
  valuesI[6] = map.getIntColor(valuesR[6]);

  // calcul de l'indice 7
  coords[7] = (coords[3] + coords[0]) / 2.;
  valuesR[7] = (valuesR[3] + valuesR[0]) / 2.;
  valuesI[7] = map.getIntColor(valuesR[7]);

#define CPY(a, b)       \
  crds[a] = coords[b];  \
  valR[a] = valuesR[b]; \
  valI[a] = valuesI[b];

  CPY(0, 0);
  CPY(1, 4);
  CPY(2, 8);
  CPY(3, 7);

  if (_decompLevel == 0)
    createSubPatch2(4, crds, valR, valI, map);
  else
    createSubPatch(4, crds, valR, valI, map);

  // sub 2
  CPY(0, 4);
  CPY(1, 1);
  CPY(2, 5);
  CPY(3, 8);
  if (_decompLevel == 0)
    createSubPatch2(4, crds, valR, valI, map);
  else
    createSubPatch(4, crds, valR, valI, map);

  // sub 3
  CPY(0, 8);
  CPY(1, 5);
  CPY(2, 2);
  CPY(3, 6);
  if (_decompLevel == 0)
    createSubPatch2(4, crds, valR, valI, map);
  else
    createSubPatch(4, crds, valR, valI, map);

  // sub 4
  CPY(0, 7);
  CPY(1, 8);
  CPY(2, 6);
  CPY(3, 3);
  if (_decompLevel == 0)
    createSubPatch2(4, crds, valR, valI, map);
  else
    createSubPatch(4, crds, valR, valI, map);

  _decompLevel++;
}

//-----------------------------------------------------------------------------
void PolygonPatches::reorderPoints(Vec3D *coords, int cur)
//-----------------------------------------------------------------------------
{
  int i;
  int lp[9];
  Vec3D temp[9];
  int ord[9];
  double dd, ddmin;
  int sm, pr;

  switch (cur)
  {
  case 5:
    lp[0] = 0;
    ddmin = 100001.;
    for (lp[1] = 1; lp[1] < 5; lp[1]++)
      for (lp[2] = 1; lp[2] < 5; lp[2]++)
        for (lp[3] = 1; lp[3] < 5; lp[3]++)
          for (lp[4] = 1; lp[4] < 5; lp[4]++)
          {
            sm = 0;
            pr = 1;
            for (i = 1; i < 5; i++)
            {
              sm += lp[i];
              pr *= lp[i];
            }
            if ((sm == 10) && (pr == 24))
            {
              dd = (coords[lp[0]] - coords[lp[4]]).getNorm();
              for (i = 1; i < 5; i++)
              {
                dd += (coords[lp[i]] - coords[lp[i - 1]]).getNorm();
              }
              if (dd < ddmin)
              {
                ddmin = dd;
                memcpy(ord, lp, 5 * sizeof(int));
              }
            }
          }
    if (ddmin >= 100000.)
      fprintf(stderr, "ERREUR Reordonne_pts\n");
    for (i = 0; i < 5; i++)
      temp[i] = coords[i];
    for (i = 0; i < 5; i++)
      coords[i] = temp[ord[i]];
    break;

  case 6:
    lp[0] = 0;
    ddmin = 100001.;
    for (lp[1] = 1; lp[1] < 6; lp[1]++)
      for (lp[2] = 1; lp[2] < 6; lp[2]++)
        for (lp[3] = 1; lp[3] < 6; lp[3]++)
          for (lp[4] = 1; lp[4] < 6; lp[4]++)
            for (lp[5] = 1; lp[5] < 6; lp[5]++)
            {
              sm = 0;
              pr = 1;
              for (i = 1; i < 6; i++)
              {
                sm += lp[i];
                pr *= lp[i];
              }
              if ((sm == 15) && (pr == 120))
              {
                dd = (coords[lp[0]] - coords[lp[5]]).getNorm();
                for (i = 1; i < 6; i++)
                {
                  dd += (coords[lp[i]] - coords[lp[i - 1]]).getNorm();
                }
                if (dd < ddmin)
                {
                  ddmin = dd;
                  memcpy(ord, lp, 6 * sizeof(int));
                }
              }
            }
    if (ddmin >= 100000.)
      fprintf(stderr, "ERREUR Reordonne_pts\n");
    for (i = 0; i < 6; i++)
      temp[i] = coords[i];
    for (i = 0; i < 6; i++)
      coords[i] = temp[ord[i]];
    break;

  case 7:
    lp[0] = 0;
    ddmin = 100001.;
    for (lp[1] = 1; lp[1] < 7; lp[1]++)
      for (lp[2] = 1; lp[2] < 7; lp[2]++)
        for (lp[3] = 1; lp[3] < 7; lp[3]++)
          for (lp[4] = 1; lp[4] < 7; lp[4]++)
            for (lp[5] = 1; lp[5] < 7; lp[5]++)
              for (lp[6] = 1; lp[6] < 7; lp[6]++)
              {
                sm = 0;
                pr = 1;
                for (i = 1; i < 7; i++)
                {
                  sm += lp[i];
                  pr *= lp[i];
                }
                if ((sm == 21) && (pr == 720))
                {
                  dd = (coords[lp[0]] - coords[lp[6]]).getNorm();
                  for (i = 1; i < 7; i++)
                  {
                    dd += (coords[lp[i]] - coords[lp[i - 1]]).getNorm();
                  }
                  if (dd < ddmin)
                  {
                    ddmin = dd;
                    memcpy(ord, lp, 7 * sizeof(int));
                  }
                }
              }
    if (ddmin >= 100000.)
      fprintf(stderr, "ERREUR Reordonne_pts\n");
    for (i = 0; i < 7; i++)
      temp[i] = coords[i];
    for (i = 0; i < 7; i++)
      coords[i] = temp[ord[i]];
    break;

  case 8:
    lp[0] = 0;
    ddmin = 100001.;
    for (lp[1] = 1; lp[1] < 8; lp[1]++)
      for (lp[2] = 1; lp[2] < 8; lp[2]++)
        for (lp[3] = 1; lp[3] < 8; lp[3]++)
          for (lp[4] = 1; lp[4] < 8; lp[4]++)
            for (lp[5] = 1; lp[5] < 8; lp[5]++)
              for (lp[6] = 1; lp[6] < 8; lp[6]++)
                for (lp[7] = 1; lp[7] < 8; lp[7]++)
                {
                  sm = 0;
                  pr = 1;
                  for (i = 1; i < 8; i++)
                  {
                    sm += lp[i];
                    pr *= lp[i];
                  }
                  if ((sm == 28) && (pr == 5040))
                  {
                    dd = (coords[lp[0]] - coords[lp[7]]).getNorm();
                    for (i = 1; i < 8; i++)
                    {
                      dd += (coords[lp[i]] - coords[lp[i - 1]]).getNorm();
                    }
                    if (dd < ddmin)
                    {
                      ddmin = dd;
                      memcpy(ord, lp, 8 * sizeof(int));
                    }
                  }
                }
    if (ddmin >= 100000.)
      fprintf(stderr, "ERREUR Reordonne_pts\n");
    for (i = 0; i < 8; i++)
      temp[i] = coords[i];
    for (i = 0; i < 8; i++)
      coords[i] = temp[ord[i]];
    break;

  default:
    fatalError("ERREUR Reordonne_pts, cas %ld a implanter\n");
  }
}
