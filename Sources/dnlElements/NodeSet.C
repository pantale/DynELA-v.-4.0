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

#include <NodeSet.h>
#include <Node.h>

//-----------------------------------------------------------------------------
NodeSet::NodeSet(char *_name)
//-----------------------------------------------------------------------------
{
  if (_name != NULL)
    setName(_name);
}

//-----------------------------------------------------------------------------
NodeSet::NodeSet(const NodeSet &nodeSet)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely NodeSet class" << &nodeSet << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
NodeSet::~NodeSet()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void NodeSet::setName(char *_name)
//-----------------------------------------------------------------------------
{
  name = _name;
}

//Add a new node to the current NodeSet
/*!
  This method adds an existing node to a NodeSet. The node is directly specified by its pointer, no verification is performed by this method.  \param nd pointeur sur un noeud
*/
//-----------------------------------------------------------------------------
void NodeSet::add(Node *node)
//-----------------------------------------------------------------------------
{
  nodes << node;
}

//Add new nodes to the current NodeSet
/*!
   This method adds an existing NodeSet and a NodeSet.
  \param NodeSet to be merged
*/
//-----------------------------------------------------------------------------
void NodeSet::add(NodeSet *nodesSet)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < nodesSet->nodes.getSize(); i++)
  {
    nodes << nodesSet->nodes(i);
  }
}

//nombre de noeuds dans la liste
/*!
  Cette methode renvoie le nombre de noeuds dans un NodeSet
  \return nombre de noeuds de la liste

*/
//-----------------------------------------------------------------------------
long NodeSet::getSize()
//-----------------------------------------------------------------------------
{
  return nodes.getSize();
}

//recupere un pointeur sur le ieme noeud d'un node set
/*!
  Cette methode renvoie un pointeur sur le ieme noeud d'une liste de noeuds contenue dans un NodeSet.
  \param i place du noeud dans la liste.
  \return pointeur sur le noeud

*/
//-----------------------------------------------------------------------------
Node *NodeSet::getNode(long nodeNumber)
//-----------------------------------------------------------------------------
{
  return (nodes(nodeNumber));
}

//recupere le numero du ieme noeud d'un node set
/*!
  Cette methode renvoie le numero du ieme noeud d'une liste de noeuds contenue dans un NodeSet.
  \param i place du noeud dans la liste.
  \return pointeur sur le noeud

*/
//-----------------------------------------------------------------------------
long NodeSet::getNumber(long item)
//-----------------------------------------------------------------------------
{
  return (nodes(item)->number);
}

//-----------------------------------------------------------------------------
void NodeSet::intersect(NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  bool ok;
  for (long i = nodes.getSize() - 1; i >= 0; i--)
  {
    ok = false;
    for (long j = 0; j < nodeSet->nodes.getSize(); j++)
    {
      if (nodes(i) == nodeSet->nodes(j))
      {
        ok = true;
        break;
      }
    }
    if (!ok)
      nodes.del(i);
  }
}

//-----------------------------------------------------------------------------
void NodeSet::substract(NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  for (long i = nodes.getSize() - 1; i >= 0; i--)
  {
    for (long j = 0; j < nodeSet->nodes.getSize(); j++)
    {
      if (nodes(i) == nodeSet->nodes(j))
      {
        nodes.del(i);
        break;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void NodeSet::clear()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

/*
//-----------------------------------------------------------------------------
void NodeSet::surface(ElementSet *eset)
//-----------------------------------------------------------------------------
{
  List<LateralSurface *> lat;
  Element *pel;
  long i, j, k;
  LateralSurface *sr1;
  LateralSurface *sr2;

  for (i = 0; i < eset->getSize(); i++)
  {
    // get the element
    pel = eset->elements(i);

    for (j = 0; j < pel->getNumberOfSideFaces(); j++)
    {
      // create surfaces
      LateralSurface *sr = new LateralSurface;

      for (k = 0; k < pel->getNumberOfNodesOnSideFace(j); k++)
      {
        sr->tab[k] = pel->getNodeOnSideFace(j, k)->number;
      }

      // sort the table
      qsort(sr->tab, pel->getNumberOfNodesOnSideFace(j), sizeof(long), compareIndice);

      sr->pel = pel;
      sr->face = j;

      lat << sr;
    }
  }

  // sort the whole list
  lat.sort(compareLateralSurfaces);

  // now detect doubles
  bool ok;
  for (i = 1; i < lat.getSize(); i++)
  {
    sr1 = lat(i - 1);
    sr2 = lat(i);
    long sdd1 = sr1->pel->getNumberOfNodesOnSideFace(sr1->face);
    long sdd2 = sr2->pel->getNumberOfNodesOnSideFace(sr2->face);

    if (sdd1 == sdd2)
    {
      ok = true;
      for (j = 0; j < sdd1; j++)
      {
        if ((sr1->tab[j] != sr2->tab[j]) || (sr1->pel->material != sr2->pel->material))
        {
          ok = false;
          break;
        }
      }
      if (ok)
      {
        delete lat(i - 1);
        delete lat(i);
        lat.del(i - 1, i);
        i--;
      }
    }
  }

  nodes.flush();

  // creer la liste des noeuds
  for (i = 0; i < lat.getSize(); i++)
  {
    sr1 = lat(i);
    for (j = 0; j < sr1->pel->getNumberOfNodesOnSideFace(sr1->face); j++)
    {
      nodes << sr1->pel->getNodeOnSideFace(sr1->face, j);
    }
  }

  // sort the list
  nodes.sort();

  // suppress doubles
  for (i = 1; i < nodes.getSize(); i++)
  {
    if (nodes(i - 1) == nodes(i))
    {
      nodes.del(i - 1, i - 1);
      i--;
    }
  }

  // detruire la liste des surfaces temporaires
  for (i = 0; i < lat.getSize(); i++)
  {
    delete lat(i);
  }

  lat.flush();
}
*/
