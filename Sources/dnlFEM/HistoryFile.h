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

// TODOCXYFILE

/*!
  \file HistoryFile.h
  \brief Declaration file for the HistoryFile class

  This file is the declaration file for the HistoryFile class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_HistoryFile_h__
#define __dnlFEM_HistoryFile_h__

#include <List.h>
#include <String.h>
class Element;
class ElementSet;
class Model;
class Node;
class NodeSet;

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

class HistoryFileGlobalItem : public HistoryFileItem
{
  friend class HistoryFile;

public:
  // constructor
  HistoryFileGlobalItem();
  ~HistoryFileGlobalItem();

  double getValue();
};

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
  String name;

public:
  // constructor
  HistoryFile(char *newName = NULL);
  HistoryFile(const HistoryFile &X);
  ~HistoryFile();

  double getSaveTime();
  double getStartTime();
  double getStopTime();
  String getFileName();
  void add(ElementSet *elementSet, short intPt, short field);
  void add(NodeSet *nodeSet, short field);
  void add(short field);
  void headerWrite();
  void save(double currentTime);
  void setFileName(std::string filename);
  void setSaveTime(double saveTime);
  void setSaveTime(double startTime, double stopTime, double saveTime);
#ifndef SWIG
  void open();
  void close();
#endif
};

#endif
