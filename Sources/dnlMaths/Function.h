/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Function_h__
#define __dnlMaths_Function_h__

#include <String.h>
#include <dnlKernel.h>

//-----------------------------------------------------------------------------
// Class : Function
//
// Used to manage Function
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Function
{
public:
  String name = "Function::_noname_"; // name of the function

public:
  Function(char *newName = NULL);
  virtual ~Function();

  virtual double getValue(double x) = 0;
  virtual double getSlope(double x) = 0;
  virtual String convertToDynELASourceFile() = 0;

  // Interface methods excluded from SWIG
#ifndef SWIG
  virtual void print() = 0;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif
};

#endif
