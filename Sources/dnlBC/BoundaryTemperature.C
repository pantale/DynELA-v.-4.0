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

#include <BoundaryTemperature.h>
/* #include <Node.h>
#include <NodalField.h> */

//constructeur par defaut de la classe BoundaryTemperature
/*!
  Cette methode est le constructeur par defaut de la classe BoundaryTemperature. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature(double newTemperature)
//-----------------------------------------------------------------------------
{
  _temperature = newTemperature;
}

//constructeur par recopie de la classe BoundaryTemperature
/*!
  Cette methode est le constructeur par recopie de la classe BoundaryTemperature.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::BoundaryTemperature(const BoundaryTemperature &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryTemperature class" << &X << std::endl;
  exit(-1);
}

//destructeur de la classe BoundaryTemperature
/*!
  Cette methode est le destructeur de la classe BoundaryTemperature.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryTemperature::~BoundaryTemperature()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //  node->newField->_temperature=_temperature;
  //  node->currentField->_temperature=_temperature;
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //  node->newField->_temperature=_temperature;
  //  node->currentField->_temperature=_temperature;
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double BoundaryTemperature::getValue()
//-----------------------------------------------------------------------------
{
  return _temperature;
}

//-----------------------------------------------------------------------------
void BoundaryTemperature::setValue(double newTemperature)
//-----------------------------------------------------------------------------
{
  _temperature = newTemperature;
}
