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

// TODOCXYFILE

/*!
  \file PolynomialFunction.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_RampFunction_h__
#define __dnlMaths_RampFunction_h__

//#include <dnlKernel.h>
#include <Function.h>

class RampFunction : public Function
{
private:
  short _type;
  double _min, _max;
  double _arg[10];

public:
  enum
  {
    Unknown,
    Linear,
    Sinus,
    Constant,
    Erf,
    Soft
  };

public:
  RampFunction(char *newName = NULL);
  ~RampFunction();

  double getValue(double x);
  double getSlope(double x);
  void setFunction(short newType, double newMin, double newMax);
  short getType(long i);
  void toGnuplot(String filename, long steps = 250);
  String convertToDynELASourceFile();

#ifndef SWIG
  void print();
#endif
};

#endif
