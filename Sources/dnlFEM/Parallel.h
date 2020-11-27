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

#ifndef __dnlFEM_Parallel_h__
#define __dnlFEM_Parallel_h__

#include <dnlKernel.h>
#include <omp.h>

class Element;

//-----------------------------------------------------------------------------
// Class : ElementsChunk
//
// Used to manage parallel element chunk
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class ElementsChunk
{
public:
  List<Element *> elements; // List of the elements of the chunk

public:
  ElementsChunk() {}
  ~ElementsChunk() {}
};
#endif

//-----------------------------------------------------------------------------
// Class : class Parallel
//
//  Used to manage parallel computation in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Parallel
{

private:
  int _cores = 1;
  int _maxThreads = 1;
  ElementsChunk **_elementsChunks; // Elements chunks

private:
  ElementsChunk **_initChunkList();
  void _deleteChunkList(ElementsChunk **chunkList);

public:
  String name = "Parallel::noname_";

  // constructeurs
  Parallel(char *newName = NULL);
  ~Parallel();

  // Interface methods excluded from SWIG
#ifndef SWIG
  ElementsChunk *getElementsOfCore(int core);
  ElementsChunk *getElementsOfCurrentCore();
  void dispatchElements(List<Element *> elementList);
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getCores();
  void setCores(int cores);
};

//-----------------------------------------------------------------------------
inline int Parallel::getCores()
//-----------------------------------------------------------------------------
{
  return _cores;
}

//-----------------------------------------------------------------------------
inline ElementsChunk *Parallel::getElementsOfCore(int core)
//-----------------------------------------------------------------------------
{
  return _elementsChunks[core];
}

//-----------------------------------------------------------------------------
inline ElementsChunk *Parallel::getElementsOfCurrentCore()
//-----------------------------------------------------------------------------
{
  return _elementsChunks[omp_get_thread_num()];
}

#endif
