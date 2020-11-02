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
  \file VtkInterface.h
  \brief Declaration file for the VtkInterface class

  This file is the declaration file for the VtkInterface class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_VtkInterface_h__
#define __dnlFEM_VtkInterface_h__

#include <String.h>
#include <List.h>
#include <iostream>
#include <fstream>

class VtkInterface
{
private:
  List<short> _outputFields;
  std::ofstream _stream;
  String _fileName;

public:
  String name = "_noname_"; // Name of the VTK interface

  // constructeurs
  VtkInterface(char *newName = NULL);
  VtkInterface(const VtkInterface &VtkInterface);
  ~VtkInterface();

  int getNumberOfFields();
  short existField(short field);
  void addField(short field);
  void close();
  void open(String fileName);
  void initFields();
  void removeField(short field);
  void write();

#ifndef SWIG
  void headerWrite();
  void nodesWrite();
  void nodesNumbersWrite();
  void elementsWrite();
  void dataWrite();
#endif
};

#endif
