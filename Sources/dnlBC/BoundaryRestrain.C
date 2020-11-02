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

#include <BoundaryRestrain.h>
/* #include <Node.h>
#include <NodalField.h> */

//constructeur par defaut de la classe BoundaryRestrain
/*!
  Cette methode est le constructeur par defaut de la classe BoundaryRestrain. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain(const Vec3D &newRestrain)
//-----------------------------------------------------------------------------
{
  _restrain = newRestrain;
}

//constructeur par recopie de la classe BoundaryRestrain
/*!
  Cette methode est le constructeur par recopie de la classe BoundaryRestrain.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::BoundaryRestrain(const BoundaryRestrain &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryRestrain class" << &X << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//Application d'une condition limite Imposee en deplacement sur un noeud
/*!
Cette methode applique une condition aux limites imposee en vitesse sur un noeud. Dans la direction de la condition imposee, la vitesse, le deplacement et l'acceleration materielle du noeud sont imposes nuls.
*/
//-----------------------------------------------------------------------------
void BoundaryRestrain::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  if (_restrain(0) > 0)
  {
    node->currentField->displacement(0) = 0.0;
    node->currentField->speed(0) = 0.0;
    node->currentField->acceleration(0) = 0.0;
    //  node->newField->acceleration(0) = 0.0;
  }
  if (_restrain(1) > 0)
  {
    node->currentField->displacement(1) = 0.0;
    node->currentField->speed(1) = 0.0;
    node->currentField->acceleration(1) = 0.0;
    //   node->newField->acceleration(1) = 0.0;
  }
  if (_restrain(2) > 0)
  {
    node->currentField->displacement(2) = 0.0;
    node->currentField->speed(2) = 0.0;
    node->currentField->acceleration(2) = 0.0;
    //   node->newField->acceleration(2) = 0.0;
  }
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  if (_restrain(0) > 0)
  {
    node->currentField->displacement(0) = 0.0;
    node->newField->displacement(0) = 0.0;
    node->currentField->speed(0) = 0.0;
    node->newField->speed(0) = 0.0;
    node->currentField->acceleration(0) = 0.0;
    node->newField->acceleration(0) = 0.0;
  }
  if (_restrain(1) > 0)
  {
    node->currentField->displacement(1) = 0.0;
    node->newField->displacement(1) = 0.0;
    node->currentField->speed(1) = 0.0;
    node->newField->speed(1) = 0.0;
    node->currentField->acceleration(1) = 0.0;
    node->newField->acceleration(1) = 0.0;
  }
  if (_restrain(2) > 0)
  {
    node->currentField->displacement(2) = 0.0;
    node->newField->displacement(2) = 0.0;
    node->currentField->speed(2) = 0.0;
    node->newField->speed(2) = 0.0;
    node->currentField->acceleration(2) = 0.0;
    node->newField->acceleration(2) = 0.0;
  }
}

//destructeur de la classe BoundaryRestrain
/*!
  Cette methode est le destructeur de la classe BoundaryRestrain.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryRestrain::~BoundaryRestrain()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryRestrain::getValue()
//-----------------------------------------------------------------------------
{
  return _restrain;
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::setValue(const Vec3D &newRestrain)
//-----------------------------------------------------------------------------
{
  _restrain = newRestrain;
}

//-----------------------------------------------------------------------------
void BoundaryRestrain::setValue(double restrainX, double restrainY, double restrainZ)
//-----------------------------------------------------------------------------
{
  _restrain(0) = restrainX;
  _restrain(1) = restrainY;
  _restrain(2) = restrainZ;
}
