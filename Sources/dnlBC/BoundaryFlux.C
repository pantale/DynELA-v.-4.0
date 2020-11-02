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

#include <BoundaryFlux.h>
/* #include <Node.h>
#include <NodalField.h>
 */
//constructeur par defaut de la classe BoundaryFlux
/*!
  Cette methode est le constructeur par defaut de la classe BoundaryFlux. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux(double fr)
//-----------------------------------------------------------------------------
{
  _flux = fr;
}

//constructeur par recopie de la classe BoundaryFlux
/*!
  Cette methode est le constructeur par recopie de la classe BoundaryFlux.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::BoundaryFlux(const BoundaryFlux &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryFlux class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//destructeur de la classe BoundaryFlux
/*!
  Cette methode est le destructeur de la classe BoundaryFlux.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryFlux::~BoundaryFlux()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryFlux::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //  node->newField->flux = _flux;
  //  node->currentField->flux = _flux;
}

//-----------------------------------------------------------------------------
void BoundaryFlux::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //  node->newField->flux = _flux;
  // node->currentField->flux = _flux;
}

//-----------------------------------------------------------------------------
void BoundaryFlux::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //  node->newField->flux = _flux;
  //  node->currentField->flux = _flux;
}

//-----------------------------------------------------------------------------
double BoundaryFlux::getValue()
//-----------------------------------------------------------------------------
{
  return _flux;
}

//-----------------------------------------------------------------------------
void BoundaryFlux::setValue(double newFlux)
//-----------------------------------------------------------------------------
{
  _flux = newFlux;
}
