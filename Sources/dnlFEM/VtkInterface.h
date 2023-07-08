/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlFEM_VtkInterface_h__
#define __dnlFEM_VtkInterface_h__

#include <String_.h>
#include <List.h>
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------------
// Class : VtkInterface
//
// Used to export VTK files
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class VtkInterface
{
private:
  List<short> _outputFields;
  std::ofstream _stream;
  String _fileName;

public:
  String name = "VtkInterface::_noname_"; // Name of the VTK interface

  // constructeurs
  VtkInterface(char * = NULL);
  VtkInterface(const VtkInterface &);
  ~VtkInterface();

  // Interface methods excluded from SWIG
#ifndef SWIG
  void headerWrite();
  void nodesWrite();
  void nodesNumbersWrite();
  void elementsWrite();
  void dataWrite();
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getNumberOfFields();
  short existField(short);
  void addField(short);
  void close();
  void openFile(String);
  void initFields();
  void removeField(short);
  void write();
};

#endif
