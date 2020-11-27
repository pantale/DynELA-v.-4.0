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

/*
  \file Function.C
  Defines of the Function class.

  This file defines the Function class.
  \ingroup dnlMaths
*/

#include <Function.h>

/*
  Default constructor of the Function class
  - newName name of the function
*/
//-----------------------------------------------------------------------------
Function::Function(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
}

/*
  Default destructor of the Function class
*/
//-----------------------------------------------------------------------------
Function::~Function()
//-----------------------------------------------------------------------------
{
}
