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

#include <BoundaryForce.h>
/* #include <Node.h>
#include <NodalField.h>
#include <Function.h> */
//constructeur par defaut de la classe BoundaryForce
/*
  Cette methode est le constructeur par defaut de la classe BoundaryForce. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce(const Vec3D &newForce)
//-----------------------------------------------------------------------------
{
  _force = newForce;
}

//constructeur par recopie de la classe BoundaryForce
/*
  Cette methode est le constructeur par recopie de la classe BoundaryForce.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::BoundaryForce(const BoundaryForce &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryForce class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryForce::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  //node->newField->_force=_force;
}

//Application d'une condition limite Imposee sur un noeud
/*
Cette methode applique des conditions aux limites imposees sur un noeud. Les conditions limites imposees sont de type vitesse, deplacement ou _force imposee.
*/
//-----------------------------------------------------------------------------
void BoundaryForce::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  /*  // l'idee est la, mais reste a le faire
  double facteur =
      1.2 * dynelaData->models.current()->currentTime / dynelaData->models.current()->solver->getEndTime();
  if (facteur > 1.0)
    facteur = 1.0;
  node->newField->_force = facteur * _force; */
  fatalError("BoundaryForce::applyConstantOnCurrentFields",
             "Pas encore implemente et teste");
}

//destructeur de la classe BoundaryForce
/*
  Cette methode est le destructeur de la classe BoundaryForce.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryForce::~BoundaryForce()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryForce::getValue()
//-----------------------------------------------------------------------------
{
  return _force;
}

//-----------------------------------------------------------------------------
void BoundaryForce::setValue(const Vec3D &newForce)
//-----------------------------------------------------------------------------
{
  _force = newForce;
}

//-----------------------------------------------------------------------------
void BoundaryForce::setValue(double forceX, double forceY, double forceZ)
//-----------------------------------------------------------------------------
{
  _force(0) = forceX;
  _force(1) = forceY;
  _force(2) = forceZ;
}
