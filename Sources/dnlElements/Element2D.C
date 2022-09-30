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
  \file Element2D.C
  Definition file for the Element2D class

  This file is the definition file for the Element2D class.

  \ingroup dnlElements
*/

#include <Element2D.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
Element2D::Element2D(long elementNumber) : ElementPlane(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Element2D::Element2D(const Element2D &element) : ElementPlane(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un Element2D\n";
}

//-----------------------------------------------------------------------------
Element2D::~Element2D()
//-----------------------------------------------------------------------------
{
}

// Calcul du gradient des deformations F
///*
// Cette methode effectue le calcul du gradient des deformations F au point d'integration à l'instant time. La relation utilisee pour ce calcul est donnee par:
//\f[  F=1+\frac{\partial \overrightarrow{u}}{\partial \overrightarrow{X}} \f]
//- F Tensor de retour pour le gradient des deformations F
//- time valeur permettant de selectionner le champ pour le deplacement (0 reference, 1 courant).
//
//-----------------------------------------------------------------------------
void Element2D::computeDeformationGradient(Tensor2 &F, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long nodeId;

  // initialisation a Id de F
  F.setToUnity();

  // calcul de F en ajoutant le terme du/dX
  for (nodeId = 0; nodeId < nodes.size(); nodeId++)
  {
    field = nodes(nodeId)->field(time);
    F(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(0);
    F(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(0);
    F(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(1);
    F(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(1);
  }
}

//-----------------------------------------------------------------------------
bool Element2D::checkLevel2()
//-----------------------------------------------------------------------------
{
  return true;
}

//-----------------------------------------------------------------------------
void Element2D::getdV_atIntPoint(Tensor2 &dv, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long nodeId;

  // initialisation a zero de dv
  dv = 0.0;

  // calcul de dv
  for (nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->field(time);
    dv(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(0);
    dv(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(0);
    dv(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(1);
    dv(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(1);
  }
}

/*


//-----------------------------------------------------------------------------
void Element2D::getdU_atIntPoint (Tensor2 & du, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long k;

  // initialisation a zero de du
  du = 0.;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
    {
      field = nodes (k)->field (time);
      du (0, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (0);
      du (0, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (0);
      du (1, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (1);
      du (1, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (1);
    }
}
*/
