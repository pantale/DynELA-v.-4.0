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
  \file ElementPlane.C
  \brief Definition file for the ElementPlane class

  This file is the definition file for the ElementPlane class.

  \ingroup dnlElements
*/

#include <ElementPlane.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(long elementNumber) : Element(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(const ElementPlane &element) : Element(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un ElementPlane\n";
}

//-----------------------------------------------------------------------------
ElementPlane::~ElementPlane()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool ElementPlane::computeJacobian(bool reference)
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
      _integrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _integrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
    }

    // Computes the det of J
    _integrationPoint->detJ = _integrationPoint->JxW.getDeterminant2x2();

    // Test of the jacobian
    if (_integrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // Computes the inverse of the Jacobian
    _integrationPoint->JxW.computeInverse2x2(_integrationPoint->detJ, _integrationPoint->invJxW);

    // Computes the derivatives of the Shape functions
    _integrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _integrationPoint->invJxW;

    // Computes the radius at the integration point for axisymmetric element
    if (getFamily() == Element::Axisymetric)
      _integrationPoint->radius = getRadiusAtIntegrationPoint();

    // If the computation is made on the initial shape computes Initial values
    if (reference)
    {
      _integrationPoint->detJ0 = _integrationPoint->detJ;

      // If the element is axisymmetric
      if (getFamily() == Element::Axisymetric)
        _integrationPoint->detJ0 = _integrationPoint->detJ * _integrationPoint->radius;
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
bool ElementPlane::computeUnderJacobian(bool reference)
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
      _underIntegrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _underIntegrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
    }

    // Computes the det of J
    _underIntegrationPoint->detJ = _underIntegrationPoint->JxW.getDeterminant2x2();

    // Test of the jacobian
    if (_underIntegrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // Computes the inverse of the Jacobian
    _underIntegrationPoint->JxW.computeInverse2x2(_underIntegrationPoint->detJ, _underIntegrationPoint->invJxW);

    // Computes the derivatives of the Shape functions
    _underIntegrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _underIntegrationPoint->invJxW;

    // Computes the radius at the integration point for axisymmetric element
    if (getFamily() == Element::Axisymetric)
      _underIntegrationPoint->radius = getRadiusAtUnderIntegrationPoint();

    // If the computation is made on the initial shape computes Initial values
    if (reference)
    {
      _underIntegrationPoint->detJ0 = _underIntegrationPoint->detJ;

      // If the element is axisymmetric
      if (getFamily() == Element::Axisymetric)
        _underIntegrationPoint->detJ0 = _underIntegrationPoint->detJ * _underIntegrationPoint->radius;
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
void ElementPlane::getV_atIntPoint(Vec3D &v, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de v
  v = 0.0;

  // calcul de v courant
  for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    v(0) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(0));
    v(1) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(1));
  }
}

//-----------------------------------------------------------------------------
void ElementPlane::computeElasticStiffnessMatrix(bool underIntegration)
//-----------------------------------------------------------------------------
{
  short currentIntPt;
  double WxdJ, R;
  short i, I, j, J;
  short numberOfIntPts;
  IntegrationPointBase *currentIntPoint;

  // Initialization of the Stiffness Matrix
  stiffnessMatrix = 0;

  // Elastic behavior Matrix
  Matrix C;
  if (getFamily() == Element::Axisymetric)
    C = material->getHookeStiffnessMatrix(Material::axisymetric);
  else
    C = material->getHookeStiffnessMatrix(Material::planeStrain);

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
      // calcul du terme d'integration numerique
      WxdJ = currentIntPoint->integrationPointData->weight * currentIntPoint->detJ;
      if (getFamily() == Element::Axisymetric)
      {
        //setCurrentUnderIntegrationPoint(currentIntPt);
        R = currentIntPoint->radius;
        WxdJ *= dnl2PI * R;
      }
    }
    else
    {
      currentIntPoint = getIntegrationPoint(currentIntPt);
      // calcul du terme d'integration numerique
      WxdJ = currentIntPoint->integrationPointData->weight * currentIntPoint->detJ;
      if (getFamily() == Element::Axisymetric)
      {
        // setCurrentIntegrationPoint(currentIntPt);
        R = currentIntPoint->radius;
        WxdJ *= dnl2PI * R;
      }
    }

    // calcul de C.B
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      double dNx = currentIntPoint->dShapeFunction(i, 0);
      double dNy = currentIntPoint->dShapeFunction(i, 1);
      double NoverR = currentIntPoint->integrationPointData->shapeFunction(i) / R;
      I = getNumberOfDimensions() * i;
      CB(0, I) = (C(0, 0) * dNx + C(0, 2) * dNy);
      CB(1, I) = (C(1, 0) * dNx + C(1, 2) * dNy);
      CB(2, I) = (C(2, 0) * dNx + C(2, 2) * dNy);
      CB(0, I + 1) = (C(0, 1) * dNy + C(0, 2) * dNx);
      CB(1, I + 1) = (C(1, 1) * dNy + C(1, 2) * dNx);
      CB(2, I + 1) = (C(2, 1) * dNy + C(2, 2) * dNx);
      if (getFamily() == Element::Axisymetric)
      {
        CB(3, I) = (C(3, 0) * dNx + C(3, 2) * dNy);
        CB(3, I + 1) = (C(3, 1) * dNy + C(3, 2) * dNx);
        CB(0, I) += C(0, 3) * NoverR;
        CB(1, I) += C(1, 3) * NoverR;
        CB(2, I) += C(2, 3) * NoverR;
        CB(3, I) += C(3, 3) * NoverR;
      }
    }

    // calcul de BT [C B]
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      double dNx = currentIntPoint->dShapeFunction(i, 0);
      double dNy = currentIntPoint->dShapeFunction(i, 1);
      double NoverR = currentIntPoint->integrationPointData->shapeFunction(i) / R;
      for (j = 0; j < getNumberOfNodes(); j++)
      {
        I = getNumberOfDimensions() * i;
        J = getNumberOfDimensions() * j;
        stiffnessMatrix(I, J) += (dNx * CB(0, J) + dNy * CB(2, J)) * WxdJ;
        stiffnessMatrix(I, J + 1) += (dNx * CB(0, J + 1) + dNy * CB(2, J + 1)) * WxdJ;
        stiffnessMatrix(I + 1, J) += (dNy * CB(1, J) + dNx * CB(2, J)) * WxdJ;
        stiffnessMatrix(I + 1, J + 1) += (dNy * CB(1, J + 1) + dNx * CB(2, J + 1)) * WxdJ;
        if (getFamily() == Element::Axisymetric)
        {
          stiffnessMatrix(I, J) += CB(3, J) * NoverR * WxdJ;
          stiffnessMatrix(I, J + 1) += CB(3, J + 1) * NoverR * WxdJ;
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
double ElementPlane::getRadiusAtIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("ElementPlane::getRadiusAtIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

//-----------------------------------------------------------------------------
double ElementPlane::getRadiusAtUnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("ElementPlane::getRadiusAtUnderIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

/*
//-----------------------------------------------------------------------------
void ElementPlane::getU_atIntPoint (Vec3D & u, short time)
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
    }
}
*/
