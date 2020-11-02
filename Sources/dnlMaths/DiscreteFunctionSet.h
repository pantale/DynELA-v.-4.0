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
  \file DiscreteFunctionSet.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function set class.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_DiscreteFunctionSet_h__
#define __dnlMaths_DiscreteFunctionSet_h__

#include <DiscreteFunction.h>

/*!
  \brief This is a set of discrete functions.

  This class is used to manage sets of discrete function set and group them. This is a usefull class for the \b DynELA \b curves application.
  \ingroup dnlMaths
*/
class DiscreteFunctionSet
{
  List<DiscreteFunction *> curves;

public:
  DiscreteFunctionSet();
  DiscreteFunctionSet(const DiscreteFunctionSet &X);
  ~DiscreteFunctionSet();

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
