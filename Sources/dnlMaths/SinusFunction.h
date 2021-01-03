/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

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

//-----------------------------------------------------------------------------
// Class : SinusFunction
//
// Used to manage SinusFunction
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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

  // Interface methods excluded from SWIG
#ifndef SWIG
  void print();
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  double getCoefficient(long i);
  double getConstant();
  double getConstant(long i);
  double getPulse(long i);
  double getSlope(double x);
  double getValue(double x);
  long getNumberOfFragments();
  short getType(long i);
  String convertToDynELASourceFile();
  void setConstant(double constant);
  void setCos(double coef, double pulse, double constant = 0);
  void setSin(double coef, double pulse, double constant = 0);
  void toGnuplot(String filename, double xmin, double xmax, double steps);
};

#endif
