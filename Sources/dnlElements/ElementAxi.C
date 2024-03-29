/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file ElementAxi.C
  Definition file for the ElementAxi class

  This file is the definition file for the ElementAxi class.

  \ingroup dnlElements
*/

#include <ElementAxi.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
ElementAxi::ElementAxi(long elementNumber) : ElementPlane(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElementAxi::ElementAxi(const ElementAxi &element) : ElementPlane(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un ElementAxi\n";
}

//-----------------------------------------------------------------------------
ElementAxi::~ElementAxi()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double ElementAxi::getRadiusAtIntegrationPoint()
//-----------------------------------------------------------------------------
{
  // Initialize currentRadius to zero
  double currentRadius = 0.0;

  // Computes currentRadius
  for (short nodeId = 0; nodeId < nodes.size(); nodeId++)
  {
    currentRadius += _integrationPoint->integrationPointData->shapeFunction(nodeId) * nodes(nodeId)->coords(0);
  }

  return currentRadius;
}

//-----------------------------------------------------------------------------
double ElementAxi::getRadiusAtUnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
  // Initialize currentRadius to zero
  double currentRadius = 0.0;

  // Computes currentRadius
  for (short nodeId = 0; nodeId < nodes.size(); nodeId++)
  {
    currentRadius += _underIntegrationPoint->integrationPointData->shapeFunction(nodeId) * nodes(nodeId)->coords(0);
  }

  return currentRadius;
}

//-----------------------------------------------------------------------------
void ElementAxi::computeDeformationGradient(Tensor2 &F, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  double Vr = 0.0;

  // Initialize F to zero
  F.setToUnity();

  // calcul de F
  for (short nodeId = 0; nodeId < nodes.size(); nodeId++)
  {
    field = nodes(nodeId)->field(time);
    F(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->u(0);
    F(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->u(0);
    F(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->u(1);
    F(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->u(1);
    Vr += _integrationPoint->integrationPointData->shapeFunction(nodeId) * field->u(0);
  }

  // calcul F terme axisymetrique
  F(2, 2) += Vr / _integrationPoint->radius;
}

//-----------------------------------------------------------------------------
bool ElementAxi::checkLevel2()
//-----------------------------------------------------------------------------
{
  // check for a negative value of x coordinate
  for (short nodeId = 0; nodeId < nodes.size(); nodeId++)
  {
    if (nodes(nodeId)->coords(0) < 0.)
    {
      if (nodes(nodeId)->coords(0) > -1e-8)
      {
        warning("Negative r value in an axisymetric element",
                "Element %d has an r value of %10.3E\nThis has been changed to 0.00\n",
                number, nodes(nodeId)->coords(0));
        nodes(nodeId)->coords(0) = 0.;
      }
      else
      {
        std::cerr << "Error in element " << number << "\nr coordinate (" << nodes(nodeId)->coords(0) << ") of the node " << nodes(nodeId)->number << " is negative\n";
        exit(-1);
      }
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
void ElementAxi::getdV_atIntPoint(Tensor2 &dv, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  Vec3D v;

  // Initialize dv to zero
  dv = 0.;

  // calcul de dv
  for (long nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->field(time);
    dv(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(0);
    dv(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(0);
    dv(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(1);
    dv(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(1);
  }

  // calcul du terme axisymetrique
  getV_atIntPoint(v, time);
  dv(2, 2) = v(0) / _integrationPoint->radius;
}

/*


//-----------------------------------------------------------------------------
void ElementAxi::getdU_atIntPoint (Tensor2 & du, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long k;
  double v;
  double R;

  // Initialize du to zero
  du = 0.;
  v = 0.;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
    {
      field = nodes (k)->field (time);
      du (0, 0) += _integrationPoint->dShapeFunction (k, 0) * field->u (0);
      du (0, 1) += _integrationPoint->dShapeFunction (k, 1) * field->u (0);
      du (1, 0) += _integrationPoint->dShapeFunction (k, 0) * field->u (1);
      du (1, 1) += _integrationPoint->dShapeFunction (k, 1) * field->u (1);
       v += _integrationPoint->integrationPointData->shapeFunction (k) * field->u (0);
    }

  // calcul du terme axisymetrique
  R=_integrationPoint->radius;
  du (2, 2) = v / R;
}
*/
