/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElementSet_h__
#define __dnlElements_ElementSet_h__

#include <String.h>
#include <List.h>
#include <Element.h>

//-----------------------------------------------------------------------------
// Class : ElementSet
//
// Used to manage Elements sets in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElementSet
{
public:
  String name;              // Nom public de l'ElementSet
  List<Element *> elements; // Liste des elements

public:
  // constructeurs
  ElementSet(char *newName = NULL);
  ElementSet(const ElementSet &X);
  ~ElementSet();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void add(Element *element);
  void add(ElementSet *elementSet);
  long getSize();
  void setName(char *_name);
  Element *getElement(long item);
  long getNumber(long item);
  void intersect(ElementSet *elementSet);
  void substract(ElementSet *elementSet);
  void clear();
};

#endif
