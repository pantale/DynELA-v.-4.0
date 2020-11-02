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

#include <Boundary.h>

//constructeur par defaut de la classe Boundary
/*!
  Cette methode est le constructeur par defaut de la classe Boundary. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::Boundary(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  _function = NULL;
}

//constructeur par recopie de la classe Boundary
/*!
  Cette methode est le constructeur par recopie de la classe Boundary.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::Boundary(const Boundary &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely Boundary class" << &X << std::endl;
  exit(-1);
}

//destructeur de la classe Boundary
/*!
  Cette methode est le destructeur de la classe Boundary.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
Boundary::~Boundary()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Function *Boundary::getFunction()
//-----------------------------------------------------------------------------
{
  return _function;
}

//-----------------------------------------------------------------------------
void Boundary::setFunction(Function *function)
//-----------------------------------------------------------------------------
{
  _function = function;
}
