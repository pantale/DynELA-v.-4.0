/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_PolynomialFunction_h__
#define __dnlMaths_PolynomialFunction_h__

#include <List.h>
#include <Function.h>

struct PolynomialFragment
{
  int factor;
  double value;
};

//-----------------------------------------------------------------------------
// Class : PolynomialFunction
//
// Used to manage PolynomialFunction
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class PolynomialFunction : public Function
{
private:
  List<PolynomialFragment> fragment;

public:
public:
  PolynomialFunction(char *newName = NULL);
  ~PolynomialFunction();

  // Interface methods excluded from SWIG
#ifndef SWIG
  void print();
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  double get(long i);
  double getCoefficient(long i);
  double getFactor(long i);
  double getSlope(double x);
  double getValue(double x);
  long getNumberOfFragments();
  String convertToDynELASourceFile();
  void flush();
  void setFunction(long i, double val);
  void toGnuplot(String filename, double xmin, double xmax, double steps);
};

#endif
