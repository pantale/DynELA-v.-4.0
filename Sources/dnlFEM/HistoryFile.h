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

#ifndef __dnlFEM_HistoryFile_h__
#define __dnlFEM_HistoryFile_h__

#include <List.h>
#include <String.h>
class Element;
class ElementSet;
class Model;
class Node;
class NodeSet;

//-----------------------------------------------------------------------------
// Class : HistoryFileItem
//
// Used to manage data inside of HistoryFile class
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class HistoryFileItem
{
  friend class HistoryFile;

protected:
  String _name;
  short _field;

public:
  // constructor
  HistoryFileItem();
  ~HistoryFileItem();

  virtual double getValue() = 0;
};
#endif

//-----------------------------------------------------------------------------
// Class : HistoryFileNodeItem
//
// Specialized class for Node Item
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class HistoryFileNodeItem : public HistoryFileItem
{
  friend class HistoryFile;

protected:
  Node *_node;

public:
  // constructor
  HistoryFileNodeItem();
  ~HistoryFileNodeItem();

  double getValue();
};
#endif

//-----------------------------------------------------------------------------
// Class : HistoryFileElementItem
//
// Specialized class for NoElementde Item
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class HistoryFileElementItem : public HistoryFileItem
{
  friend class HistoryFile;

protected:
  Element *_element;
  short _intPoint;

public:
  // constructor
  HistoryFileElementItem();
  ~HistoryFileElementItem();

  double getValue();
};
#endif

//-----------------------------------------------------------------------------
// Class : HistoryFileGlobalItem
//
// Specialized class for Global Item
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class HistoryFileGlobalItem : public HistoryFileItem
{
  friend class HistoryFile;

public:
  // constructor
  HistoryFileGlobalItem();
  ~HistoryFileGlobalItem();

  double getValue();
};
#endif

//-----------------------------------------------------------------------------
// Class : HistoryFile
//
// Time history files management class
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class HistoryFile
{
private:
  double _nextTime;
  double _saveTime;
  double _startTime;
  double _stopTime;
  FILE *_pfile;
  List<HistoryFileItem *> _items;
  String _fileName;

public:
  String name = "HistoryFile::_noname_";

public:
  // constructor
  HistoryFile(char *newName = NULL);
  HistoryFile(const HistoryFile &X);
  ~HistoryFile();

  // Interface methods excluded from SWIG
#ifndef SWIG
  void close();
  void headerWrite();
  void open();
  void save(double currentTime);
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
  double getSaveTime();
  double getStartTime();
  double getStopTime();
  String getFileName();
#endif

  void add(ElementSet *elementSet, short intPt, short field);
  void add(NodeSet *nodeSet, short field);
  void add(short field);
  void setFileName(std::string filename);
  void setSaveTime(double saveTime);
  void setSaveTime(double startTime, double stopTime, double saveTime);
};

#endif
