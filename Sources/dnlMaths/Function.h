/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file Function.h
  \brief Declaration of the Function class.

  This file declares the Function class.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_Function_h__
#define __dnlMaths_Function_h__

#include <String.h>

/*!
  \brief Function class

This class is used to store and manipulate information about Functions for the DynELA Finite Element code.
  \ingroup dnlMaths
*/
class Function
{
public:
  String name; //!< name of the function

public:
  Function(char *newName = NULL);
  virtual ~Function();

  virtual double getValue(double x) = 0;
  virtual double getSlope(double x) = 0;
  virtual String convertToDynELASourceFile() = 0;
#ifndef SWIG
  virtual void print() = 0;
#endif
};

#endif
