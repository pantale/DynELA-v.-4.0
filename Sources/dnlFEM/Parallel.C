/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#include <Parallel.h>
#include <DynELA.h>

#pragma omp default none

/* #include <List.h>
#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <Field.h>
 */
//-----------------------------------------------------------------------------
Parallel::Parallel(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  // get max number of cores
  _maxThreads = omp_get_max_threads();

  // Init internal lists
  _elementsChunks = _initChunkList();
}

//-----------------------------------------------------------------------------
Parallel::~Parallel()
//-----------------------------------------------------------------------------
{
  _deleteChunkList(_elementsChunks);
}

//-----------------------------------------------------------------------------
ElementsChunk **Parallel::_initChunkList()
//-----------------------------------------------------------------------------
{
  // Init internal lists
  ElementsChunk **_chunk = new ElementsChunk *[_maxThreads];
  for (int chunk = 0; chunk < _maxThreads; chunk++)
  {
    _chunk[chunk] = new ElementsChunk;
  }
  return _chunk;
}

//-----------------------------------------------------------------------------
void Parallel::_deleteChunkList(ElementsChunk **chunkList)
//-----------------------------------------------------------------------------
{
  for (int chunk = 0; chunk < _maxThreads; chunk++)
  {
    delete _elementsChunks[chunk];
  }

  delete _elementsChunks;
}

//-----------------------------------------------------------------------------
void Parallel::setCores(int cores)
//-----------------------------------------------------------------------------
{
  _cores = cores;
  omp_set_num_threads(_cores);
  omp_set_dynamic(false);
}

//-----------------------------------------------------------------------------
void Parallel::dispatchElements(List<Element *> elements)
//-----------------------------------------------------------------------------
{
  // Set initial core
  int coreId = 0;

  // Loop over available cores
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    // Add element to chunk coreId
    _elementsChunks[coreId]->elements << elements(elementId);

    // Increase coreId id
    coreId++;
    if (coreId >= _cores)
      coreId = 0;
  }

  dynelaData->logFile << "Parallel computation elements dispatch\n";
  // display cores
  for (int core = 0; core < _cores; core++)
  {
    printf("CPU core %d - %ld elements\n", core + 1, _elementsChunks[core]->elements.getSize());
    dynelaData->logFile << "CPU core " << core + 1 << " - " << _elementsChunks[core]->elements.getSize() << " element(s)\n";
  }
}
