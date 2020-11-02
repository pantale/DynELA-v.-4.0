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
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#include <BoundaryConvection.h>
/* #include <Node.h>
#include <NodalField.h> */

//constructeur par defaut de la classe BoundaryConvection
/*!
  Cette methode est le constructeur par defaut de la classe BoundaryConvection. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection(const double hc_, const double tf_)
//-----------------------------------------------------------------------------
{
  _hc = hc_;
  _Tf = tf_;
}

//-----------------------------------------------------------------------------
BoundaryConvection::BoundaryConvection(const BoundaryConvection &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryConvection class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//destructeur de la classe BoundaryConvection
/*!
  Cette methode est le destructeur de la classe BoundaryConvection.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryConvection::~BoundaryConvection()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryConvection::getValue()
//-----------------------------------------------------------------------------
{
  Vec3D v;
  v(0) = _hc;
  v(1) = _Tf;
  return v;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::setValue(double x, double y)
//-----------------------------------------------------------------------------
{
  _hc = x;
  _Tf = y;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //node->newField->force=force;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //     node->newField->hconv=_hc;
  //    node->currentField->Tfluid=_Tf;
}

//-----------------------------------------------------------------------------
void BoundaryConvection::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}
