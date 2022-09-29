/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <Parallel.h>
#include <DynELA.h>

#pragma omp default none
/*
  Constructor of the Parallel class
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

/*
  Destructor of the Parallel class
*/
//-----------------------------------------------------------------------------
Parallel::~Parallel()
//-----------------------------------------------------------------------------
{
  _deleteChunkList(_elementsChunks);
}

/*
  Init the list of elements by chunk
*/
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

/*
  Delete the list of elements by chunk
*/
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

/*
  Set the number of cores to use for parallel solving
  - cores : Number of cores to use
*/
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
  for (long elementId = 0; elementId < elements.size(); elementId++)
  {
    // Add element to chunk coreId
    _elementsChunks[coreId]->elements << elements(elementId);

    // Increase coreId id
    coreId++;
    if (coreId >= _cores)
      coreId = 0;
  }

  dynelaData->logFile << "Parallel computation elements dispatch\n";

  // display list of elements/cores
  for (int core = 0; core < _cores; core++)
  {
    printf("CPU core %d - %ld elements\n", core + 1, _elementsChunks[core]->elements.size());
    dynelaData->logFile << "CPU core " << core + 1 << " - " << _elementsChunks[core]->elements.size() << " element(s)\n";
  }
}
