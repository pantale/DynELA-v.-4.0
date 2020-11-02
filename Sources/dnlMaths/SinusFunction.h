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

#ifndef __dnlMaths_SinusFunction_h__
#define __dnlMaths_SinusFunction_h__

#include <List.h>
#include <Function.h>

struct SinusFragment
{
  double pulse;
  double factor;
  double constant;
  char type;
};

class SinusFunction : public Function
{
private:
  List<SinusFragment> fragment;
  double constant;

public:
  enum
  {
    none,
    Sin,
    Cos
  };

public:
  SinusFunction(char *newName = NULL);
  ~SinusFunction();

  double getValue(double x);
  long getNumberOfFragments();
  void setSin(double coef, double pulse, double constant = 0);
  void setCos(double coef, double pulse, double constant = 0);
  void setConstant(double constant);
  short getType(long i);
  double getPulse(long i);
  double getCoefficient(long i);
  double getConstant(long i);
  double getConstant();
  double getSlope(double x);
  void toGnuplot(String filename, double xmin, double xmax, double steps);
  String convertToDynELASourceFile();

#ifndef SWIG
  void print();
#endif
};

#endif
