/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_RampFunction_h__
#define __dnlMaths_RampFunction_h__

#include <Function.h>

//-----------------------------------------------------------------------------
// Class : RampFunction
//
// Used to manage RampFunction
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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

  // Interface methods excluded from SWIG
#ifndef SWIG
  void print();
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  double getSlope(double x);
  double getValue(double x);
  short getType(long i);
  String convertToDynELASourceFile();
  void setFunction(short newType, double newMin, double newMax);
  void toGnuplot(String filename, long steps = 250);
};

#endif
