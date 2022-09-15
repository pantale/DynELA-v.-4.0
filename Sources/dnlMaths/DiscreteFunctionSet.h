/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_DiscreteFunctionSet_h__
#define __dnlMaths_DiscreteFunctionSet_h__

#include <DiscreteFunction.h>

//-----------------------------------------------------------------------------
// Class : DiscreteFunctionSet
//
// Used to manage DiscreteFunctionSet
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class DiscreteFunctionSet
{
  List <DiscreteFunction *> curves;

public:
  DiscreteFunctionSet();
  DiscreteFunctionSet(const DiscreteFunctionSet &X);
  ~DiscreteFunctionSet();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void read(String);
  void write(String);
  DiscreteFunction *get(String);
  DiscreteFunction *get(long);
  void add(DiscreteFunction *);
  void remove(long);
  void remove(String);
  long getSize();
};

#endif
