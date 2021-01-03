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

#ifndef __dnlElements_NodeSet_h__
#define __dnlElements_NodeSet_h__

#include <dnlMaths.h>

class Node;

//-----------------------------------------------------------------------------
// Class : NodeSet
//
// Used to manage ElNode sets in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class NodeSet
{
public:
  String name;        // Public name of the NodeSet
  List<Node *> nodes; // List of the nodes

public:
  NodeSet(char *newName = NULL);   // Constructor of the NodeSet class
  NodeSet(const NodeSet &nodeSet); // Copy constructor of the NodeSet class
  ~NodeSet();                      // Destructor of the NodeSet class

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void add(Node *node);
  void setName(char *_name);
  void add(NodeSet *);
  long getSize();
  Node *getNode(long item);
  long getNumber(long item);
  void intersect(NodeSet *nodeSet);
  void substract(NodeSet *nodeSet);
  void clear();

  // fonctions membres
  /*
  void surface(ElementSet *); */
};

#endif
