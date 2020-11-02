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
  \file Element3D.C
  \brief Definition file for the Element3D class

  This file is the definition file for the Element3D class.

  \ingroup dnlElements
*/

#include <Element3D.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
Element3D::Element3D(long elementNumber) : Element(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Element3D::Element3D(const Element3D &element) : Element(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un Element3D\n";
}

//-----------------------------------------------------------------------------
Element3D::~Element3D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double Element3D::getRadiusAtIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("Element3D::getRadiusAtIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

//-----------------------------------------------------------------------------
double Element3D::getRadiusAtUnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("Element3D::getRadiusAtUnderIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

//-----------------------------------------------------------------------------
bool Element3D::computeJacobian(bool reference)
//-----------------------------------------------------------------------------
{
  Node *node;
  const IntegrationPointData *integrationPointData;

  for (short intPtId = 0; intPtId < integrationPoints.getSize(); intPtId++)
  {
    // Get the current integration point
    setCurrentIntegrationPoint(intPtId);
    integrationPointData = &_elementData->integrationPoint[intPtId];

    // Initialize the Jacobian J to zero
    _integrationPoint->JxW = 0.0;

    // Computes the Jacobian
    for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
    {
      node = nodes(nodeId);
      _integrationPoint->JxW(0, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(0);
      _integrationPoint->JxW(0, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(0);
      _integrationPoint->JxW(0, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(0);
      _integrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _integrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
      _integrationPoint->JxW(1, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(1);
      _integrationPoint->JxW(2, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(2);
      _integrationPoint->JxW(2, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(2);
      _integrationPoint->JxW(2, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(2);
    }

    // Computes the det of J
    _integrationPoint->detJ = _integrationPoint->JxW.getDeterminant3x3();

    // Test of the jacobian
    if (_integrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // Computes the inverse of the Jacobian
    _integrationPoint->JxW.computeInverse3x3(_integrationPoint->detJ, _integrationPoint->invJxW);

    // Computes the derivatives of the Shape functions
    _integrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _integrationPoint->invJxW;

    // If the computation is made on the initial shape computes Initial values
    if (reference)
      _integrationPoint->detJ0 = _integrationPoint->detJ;
  }
  return true;
}

//-----------------------------------------------------------------------------
bool Element3D::computeUnderJacobian(bool reference)
//-----------------------------------------------------------------------------
{
  Node *node;
  const IntegrationPointData *integrationPointData;

  for (short intPtId = 0; intPtId < underIntegrationPoints.getSize(); intPtId++)
  {
    // Get the current integration point
    setCurrentUnderIntegrationPoint(intPtId);
    integrationPointData = &_elementData->underIntegrationPoint[intPtId];

    // Initialize the Jacobian J to zero
    _underIntegrationPoint->JxW = 0.0;

    // Computes the Jacobian
    for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
    {
      node = nodes(nodeId);
      _underIntegrationPoint->JxW(0, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(0);
      _underIntegrationPoint->JxW(0, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(0);
      _underIntegrationPoint->JxW(0, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(0);
      _underIntegrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _underIntegrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
      _underIntegrationPoint->JxW(1, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(1);
      _underIntegrationPoint->JxW(2, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(2);
      _underIntegrationPoint->JxW(2, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(2);
      _underIntegrationPoint->JxW(2, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(2);
    }

    // Computes the det of J
    _underIntegrationPoint->detJ = _underIntegrationPoint->JxW.getDeterminant3x3();

    // Test of the jacobian
    if (_underIntegrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // Computes the inverse of the Jacobian
    _underIntegrationPoint->JxW.computeInverse3x3(_underIntegrationPoint->detJ, _underIntegrationPoint->invJxW);

    // Computes the derivatives of the Shape functions
    _underIntegrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _underIntegrationPoint->invJxW;

    // If the computation is made on the initial shape computes Initial values
    if (reference)
      _underIntegrationPoint->detJ0 = _underIntegrationPoint->detJ;
  }
  return true;
}

//-----------------------------------------------------------------------------
void Element3D::computeDeformationGradient(Tensor2 &F, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long nodeId;

  // initialisation a zero de F
  F.setToUnity();

  // calcul de F
  for (nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    F(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(0);
    F(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(0);
    F(0, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(0);
    F(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(1);
    F(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(1);
    F(1, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(1);
    F(2, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(2);
    F(2, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(2);
    F(2, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(2);
  }
}

//-----------------------------------------------------------------------------
bool Element3D::checkLevel2()
//-----------------------------------------------------------------------------
{
  return true;
}

//-----------------------------------------------------------------------------
void Element3D::getV_atIntPoint(Vec3D &v, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de v
  v = 0.0;

  // calcul de v
  for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    v(0) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(0));
    v(1) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(1));
    v(2) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(2));
  }
}

//-----------------------------------------------------------------------------
void Element3D::getdV_atIntPoint(Tensor2 &dv, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de dv
  dv = 0.0;

  // calcul de dv
  for (short nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    dv(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(0);
    dv(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(0);
    dv(0, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(0);
    dv(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(1);
    dv(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(1);
    dv(1, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(1);
    dv(2, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(2);
    dv(2, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(2);
    dv(2, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(2);
  }
}

/*




//-----------------------------------------------------------------------------
void Element3D::getU_atIntPoint (Vec3D & u, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long i;

  // initialisation a zero de du
  u = 0.0;

  // calcul de du
  for (i = 0; i < nodes.getSize (); i++)
    {
      field = nodes (i)->getNodalField (time);
       u (0) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (0);
       u (1) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (1);
       u (2) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (2);
    }
}




//-----------------------------------------------------------------------------
void Element3D::getdU_atIntPoint (Tensor2 & du, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long k;

  // initialisation a zero de du
  du = 0.0;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
    {
      field = nodes (k)->getNodalField (time);
      du (0, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (0);
      du (0, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (0);
      du (0, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (0);
      du (1, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (1);
      du (1, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (1);
      du (1, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (1);
      du (2, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (2);
      du (2, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (2);
      du (2, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (2);
    }
}
*/
//-----------------------------------------------------------------------------
void Element3D::computeElasticStiffnessMatrix(bool underIntegration)
//-----------------------------------------------------------------------------
{
  short currentIntPt;
  double WxdJ;
  short i, I, j, J;
  short numberOfIntPts;
  IntegrationPointBase *currentIntPoint;

  // Initialization of the Stiffness Matrix
  stiffnessMatrix = 0;

  // Elastic behavior Matrix
  Matrix C = material->getHookeStiffnessMatrix(Material::threedimensional);

  // matrice temporaire
  Matrix CB(C.rows(), getNumberOfDimensions() * getNumberOfNodes());

  // nomre de points d'integration
  if (underIntegration)
  {
    computeUnderJacobian();
    numberOfIntPts = underIntegrationPoints.getSize();
  }
  else
  {
    numberOfIntPts = integrationPoints.getSize();
  }

  // parallelisation
  //#pragma omp parallel for private(WxdJ),shared(stiffnessMatrix)
  // boucle sur les points d'integration
  for (currentIntPt = 0; currentIntPt < numberOfIntPts; currentIntPt++)
  {
    // recuperation du point d'integration
    if (underIntegration)
    {
      currentIntPoint = getUnderIntegrationPoint(currentIntPt);
    }
    else
    {
      currentIntPoint = getIntegrationPoint(currentIntPt);
    }

    // calcul du terme d'integration numerique
    WxdJ = currentIntPoint->integrationPointData->weight * currentIntPoint->detJ;

    // calcul de C.B
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      I = getNumberOfDimensions() * i;
      double dNx = currentIntPoint->dShapeFunction(i, 0);
      double dNy = currentIntPoint->dShapeFunction(i, 1);
      double dNz = currentIntPoint->dShapeFunction(i, 2);
      CB(0, I) = (C(0, 0) * dNx + C(0, 3) * dNy + C(0, 5) * dNz);
      CB(1, I) = (C(1, 0) * dNx + C(1, 3) * dNy + C(1, 5) * dNz);
      CB(2, I) = (C(2, 0) * dNx + C(2, 3) * dNy + C(2, 5) * dNz);
      CB(3, I) = (C(3, 0) * dNx + C(3, 3) * dNy + C(3, 5) * dNz);
      CB(4, I) = (C(4, 0) * dNx + C(4, 3) * dNy + C(4, 5) * dNz);
      CB(5, I) = (C(5, 0) * dNx + C(5, 3) * dNy + C(5, 5) * dNz);
      CB(0, I + 1) = (C(0, 1) * dNy + C(0, 3) * dNx + C(0, 4) * dNz);
      CB(1, I + 1) = (C(1, 1) * dNy + C(1, 3) * dNx + C(1, 4) * dNz);
      CB(2, I + 1) = (C(2, 1) * dNy + C(2, 3) * dNx + C(2, 4) * dNz);
      CB(3, I + 1) = (C(3, 1) * dNy + C(3, 3) * dNx + C(3, 4) * dNz);
      CB(4, I + 1) = (C(4, 1) * dNy + C(4, 3) * dNx + C(4, 4) * dNz);
      CB(5, I + 1) = (C(5, 1) * dNy + C(5, 3) * dNx + C(5, 4) * dNz);
      CB(0, I + 2) = (C(0, 2) * dNz + C(0, 4) * dNy + C(0, 5) * dNx);
      CB(1, I + 2) = (C(1, 2) * dNz + C(1, 4) * dNy + C(1, 5) * dNx);
      CB(2, I + 2) = (C(2, 2) * dNz + C(2, 4) * dNy + C(2, 5) * dNx);
      CB(3, I + 2) = (C(3, 2) * dNz + C(3, 4) * dNy + C(3, 5) * dNx);
      CB(4, I + 2) = (C(4, 2) * dNz + C(4, 4) * dNy + C(4, 5) * dNx);
      CB(5, I + 2) = (C(5, 2) * dNz + C(5, 4) * dNy + C(5, 5) * dNx);
    }

    // calcul de BT [C B]
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      double dNx = currentIntPoint->dShapeFunction(i, 0);
      double dNy = currentIntPoint->dShapeFunction(i, 1);
      double dNz = currentIntPoint->dShapeFunction(i, 2);
      for (j = 0; j < getNumberOfNodes(); j++)
      {
        I = getNumberOfDimensions() * i;
        J = getNumberOfDimensions() * j;
        stiffnessMatrix(I, J) += (dNx * CB(0, J) + dNy * CB(3, J) + dNz * CB(5, J)) * WxdJ;
        stiffnessMatrix(I, J + 1) += (dNx * CB(0, J + 1) + dNy * CB(3, J + 1) + dNz * CB(5, J + 1)) * WxdJ;
        stiffnessMatrix(I, J + 2) += (dNx * CB(0, J + 2) + dNy * CB(3, J + 2) + dNz * CB(5, J + 2)) * WxdJ;
        stiffnessMatrix(I + 1, J) += (dNy * CB(1, J) + dNx * CB(3, J) + dNz * CB(4, J)) * WxdJ;
        stiffnessMatrix(I + 1, J + 1) += (dNy * CB(1, J + 1) + dNx * CB(3, J + 1) + dNz * CB(4, J + 1)) * WxdJ;
        stiffnessMatrix(I + 1, J + 2) += (dNy * CB(1, J + 2) + dNx * CB(3, J + 2) + dNz * CB(4, J + 2)) * WxdJ;
        stiffnessMatrix(I + 2, J) += (dNz * CB(2, J) + dNy * CB(4, J) + dNx * CB(5, J)) * WxdJ;
        stiffnessMatrix(I + 2, J + 1) += (dNz * CB(2, J + 1) + dNy * CB(4, J + 1) + dNx * CB(5, J + 1)) * WxdJ;
        stiffnessMatrix(I + 2, J + 2) += (dNz * CB(2, J + 2) + dNy * CB(4, J + 2) + dNx * CB(5, J + 2)) * WxdJ;
      }
    }
  }
}
