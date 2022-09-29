/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file NodeSet.h
  Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#include <BoundaryCondition.h>
/* #include <Boundary.h>
#include <Node.h> */

//constructeur par defaut de la classe BoundaryCondition
/*
  Cette methode est le constructeur par defaut de la classe BoundaryCondition. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition()
//-----------------------------------------------------------------------------
{
}

//constructeur par recopie de la classe BoundaryCondition
/*
  Cette methode est le constructeur par recopie de la classe BoundaryCondition.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition(const BoundaryCondition &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryCondition class" << &X << std::endl;
  exit(-1);
}

//destructeur de la classe BoundaryCondition
/*
  Cette methode est le destructeur de la classe BoundaryCondition.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryCondition::~BoundaryCondition()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool BoundaryCondition::existConstant()
//-----------------------------------------------------------------------------
{
  return (constant.size() != 0);
}

//-----------------------------------------------------------------------------
bool BoundaryCondition::existInitial()
//-----------------------------------------------------------------------------
{
  return (initial.size() != 0);
}

//-----------------------------------------------------------------------------
void BoundaryCondition::addConstant(Boundary *bc)
//-----------------------------------------------------------------------------
{
  // verification
#ifdef VERIF_alloc
  if (bc == NULL)
    fatalError("BoundaryCondition::addConstant",
               "Error due to a null pointer assignment");
#endif

  constant << bc;
}

//-----------------------------------------------------------------------------
void BoundaryCondition::addInitial(Boundary *bc)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_alloc
  if (bc == NULL)
    fatalError("BoundaryCondition::addInitial",
               "Error due to a null pointer assignment");
#endif

  initial << bc;
}

//-----------------------------------------------------------------------------
Boundary *BoundaryCondition::getInitial(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
  if (i >= initial.size())
    fatalError("BoundaryCondition::getInitial", "Out of bounds error");
#endif

  return initial(i);
}

//-----------------------------------------------------------------------------
Boundary *BoundaryCondition::getConstant(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
  if (i >= constant.size())
    fatalError("BoundaryCondition::getConstant", "Out of bounds error");
#endif

  return constant(i);
}

//-----------------------------------------------------------------------------
bool BoundaryCondition::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  short conditionId;

  // if no initial condition found return false
  if (!existInitial() && !existConstant())
    return false;

  // apply the initial conditions
  for (conditionId = 0; conditionId < initial.size(); conditionId++)
    initial(conditionId)->applyInitial(node, currentTime, timeStep);

  for (conditionId = 0; conditionId < constant.size(); conditionId++)
    constant(conditionId)->applyInitial(node, currentTime, timeStep);

  // return true
  return true;
}

//Application d'une condition limite Imposee sur un noeud
/*
Cette methode applique des conditions aux limites imposees sur un noeud. Les conditions limites imposees sont de type vitesse, deplacement ou force imposee.
*/
//-----------------------------------------------------------------------------
bool BoundaryCondition::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  // if no constant condition found return false
  if (!existConstant())
    return false;

  // apply the constant conditions
  for (short i = 0; i < constant.size(); i++)
  {
    constant(i)->applyConstantOnCurrentFields(node, currentTime, timeStep);
  }

  // return true
  return true;
}

//-----------------------------------------------------------------------------
bool BoundaryCondition::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  // if no constant condition found return false
  if (!existConstant())
    return false;

  // apply the constant conditions
  for (short boundaryId = 0; boundaryId < constant.size(); boundaryId++)
  {
    constant(boundaryId)->applyConstantOnNewFields(node, currentTime, timeStep);
  }

  // return true
  return true;
}

/* //-----------------------------------------------------------------------------
Boundary *BoundaryCondition::isConstantCondition(long value)
//-----------------------------------------------------------------------------
{
  long j;
  Boundary *pb;

  if (existConstant())
  {
    for (j = 0; j < getNumberOfConstants(); j++)
    {
      pb = getConstant(j);

      if (pb->getType() == value)
        return pb;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Boundary *BoundaryCondition::isInitialCondition(long value)
//-----------------------------------------------------------------------------
{
  long j;
  Boundary *pb;

  if (existInitial())
  {
    for (j = 0; j < getNumberOfInitials(); j++)
    {
      pb = getInitial(j);

      if (pb->getType() == value)
        return pb;
    }
  }
  return NULL;
} */

//-----------------------------------------------------------------------------
long BoundaryCondition::getNumberOfConstants()
//-----------------------------------------------------------------------------
{
  return constant.size();
}

//-----------------------------------------------------------------------------
long BoundaryCondition::getNumberOfInitials()
//-----------------------------------------------------------------------------
{
  return initial.size();
}
