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

#include <BoundaryDisplacement.h>
/* #include <Node.h>
#include <NodalField.h>
#include <Function.h> */

//constructeur par defaut de la classe BoundaryDisplacement
/*
  Cette methode est le constructeur par defaut de la classe BoundaryDisplacement. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement(const Vec3D &newDisplacement)
//-----------------------------------------------------------------------------
{
  _disp = newDisplacement;
}

//constructeur par recopie de la classe BoundaryDisplacement
/*
  Cette methode est le constructeur par recopie de la classe BoundaryDisplacement.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundaryDisplacement::BoundaryDisplacement(const BoundaryDisplacement &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundaryDisplacement class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//Application d'une condition limite Imposee en deplacement sur un noeud
/*
Cette methode applique une condition aux limites imposee en vitesse sur un noeud. Dans la direction de la condition imposee, la vitesse, le deplacement et l'acceleration materielle du noeud sont imposes nuls.
*/
//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  fatalError("BoundaryDisplacement::applyConstantOnNewFields", "not yet implemented\n");
  for (int i = 0; i < 3; i++)
  {
    // si on a une composante de vitesse imposee
    if (_disp(i) != 0.)
    {

      // si c'est constant
      if (_function == NULL)
      {
        // deplacement constant
        node->field0->u(i) = _disp(i);
        node->field1->u(i) = _disp(i);

        // vitesse imposee
        node->field0->speed(i) = _disp(i) / timeStep;
        node->field1->speed(i) = _disp(i) / timeStep;

        // acceleration nulle
        //     node->field0->acceleration(i) = 2.0 * _disp(i) / dnlSquare(timeStep);
        //    node->field1->acceleration(i) = 2.0 * _disp(i) / dnlSquare(timeStep);
        node->field0->acceleration(i) = 0.0;
        node->field1->acceleration(i) = 0.0;
      }
      else
      {
        // deplacement constant
        node->field0->u(i) = _disp(i) * _function->getValue(currentTime);
        node->field1->u(i) = _disp(i) * _function->getValue(currentTime + timeStep);

        // vitesse imposee
        node->field0->speed(i) = _disp(i) / timeStep * _function->getValue(currentTime);
        node->field1->speed(i) = _disp(i) / timeStep * _function->getValue(currentTime + timeStep);

        // acceleration non nulle
        node->field0->acceleration(i) = 2.0 * _disp(i) / dnlSquare(timeStep) * _function->getValue(currentTime);
        node->field1->acceleration(i) = 2.0 * _disp(i) / dnlSquare(timeStep) * _function->getValue(currentTime + timeStep);
      }
    }
  }
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  fatalError("BoundaryDisplacement::applyConstantOnCurrentFields", "Old one\n");
}

//-----------------------------------------------------------------------------
BoundaryDisplacement::~BoundaryDisplacement()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryDisplacement::getValue()
//-----------------------------------------------------------------------------
{
  return _disp;
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::setValue(const Vec3D &newDisplacement)
//-----------------------------------------------------------------------------
{
  _disp = newDisplacement;
}

//-----------------------------------------------------------------------------
void BoundaryDisplacement::setValue(double dispX, double dispY, double dispZ)
//-----------------------------------------------------------------------------
{
  _disp(0) = dispX;
  _disp(1) = dispY;
  _disp(2) = dispZ;
}
