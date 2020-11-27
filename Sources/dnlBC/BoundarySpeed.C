/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file NodeSet.h
  Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#include <BoundarySpeed.h>
/* #include <Node.h>
#include <NodalField.h>
#include <Function.h> */

//constructeur par defaut de la classe BoundarySpeed
/*
  Cette methode est le constructeur par defaut de la classe BoundarySpeed. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed(const Vec3D &newSpeed)
//-----------------------------------------------------------------------------
{
  // stores the prescribed speed
  _speed = newSpeed;
}

//constructeur par recopie de la classe BoundarySpeed
/*
  Cette methode est le constructeur par recopie de la classe BoundarySpeed.
  \warning La recopie n'est pas prevue ici, cette methode genere une erreur d'execution

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::BoundarySpeed(const BoundarySpeed &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely BoundarySpeed class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
void BoundarySpeed::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  // apply the curent speed on the material and grid speeds
  node->currentField->speed = _speed;
  node->newField->speed = _speed;

  // acceleration nulle
  node->currentField->acceleration = 0.0;
  node->newField->acceleration = 0.0;
}

//Application d'une condition limite Imposee en vitesse sur un noeud
/*
  Cette methode applique des conditions aux limites imposees en vitesse sur un noeud. Deux cas se presentent selon la nature de la vitesse imposee (constante ou variable dans le temps).
  Dans le cas oe la vitesse imposee est constante dans le temps, on applique simplement dans la direction contrainte une acceleration nulle et une vitesse imposee constante egale a la vitesse de la CL.
  Dans le cas oe la CL est variable dans le temps, on utilise les relations suivantes \f[ \stackrel{\bullet }{v_{0}}=\frac{\overline{v}(t+\Delta t)-\overline{v}(t)}{\Delta t} \f] et  \f[  v_{0}=\overline{v}(t) \f] relations dans lesquelles  \f$ \overline{v}(t) \f$ represente la valeur de la vitesse imposee a l'instant t definie a partir de la fonction discrete.
  - node Noeud sur lequel la condition limite est appliquee

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
void BoundarySpeed::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  /*   // si c'est constant
  if (_function == NULL)
  {
    // acceleration nulle
    node->currentField->acceleration = 0.0;

    // vitesse imposee
    node->currentField->speed = _speed;

    // deplacement constant
    node->currentField->displacement = node->currentField->speed * timeStep;
  }
  else
  {
    // acceleration non nulle
    node->currentField->acceleration = (_function->getValue(currentTime + timeStep) - _function->getValue(currentTime)) * _speed / timeStep;

    // vitesse imposee
    node->currentField->speed = _function->getValue(currentTime) * _speed;

    // deplacement constant
    node->currentField->displacement = node->currentField->speed * timeStep;
  } */
  exit(0);
  for (int i = 0; i < 3; i++)
  {
    // si on a une composante de vitesse imposee
    if (_speed(i) != 0.)
    {
      // si c'est constant
      if (_function == NULL)
      {
        // acceleration nulle
        node->currentField->acceleration(i) = 0.0;

        // vitesse imposee
        node->currentField->speed(i) = _speed(i);
      }
      else
      {
        // acceleration non nulle
        node->currentField->acceleration(i) = (_function->getValue(currentTime + timeStep) - _function->getValue(currentTime)) * _speed(i) / timeStep;

        // vitesse imposee
        node->currentField->speed(i) = _function->getValue(currentTime) * _speed(i);
      }
    }
  }
}

//-----------------------------------------------------------------------------
void BoundarySpeed::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
  /*   // si c'est constant
  if (_function == NULL)
  {
    // acceleration nulle
 //   node->currentField->acceleration = 0.0;
    node->newField->acceleration = 0.0;

    // vitesse imposee
 //   node->currentField->speed = _speed;
    node->newField->speed = _speed;

    // deplacement constant
  //  node->currentField->displacement = node->currentField->speed * timeStep;
    node->newField->displacement = node->newField->speed * timeStep;
  }
  else
  {
    // acceleration non nulle
  // node->currentField->acceleration = (_function->getValue(currentTime + timeStep) - _function->getValue(currentTime)) * _speed / timeStep;
    node->newField->acceleration = node->currentField->acceleration;

    // vitesse imposee
 //   node->currentField->speed = _function->getValue(currentTime) * _speed;
    node->newField->speed = _function->getValue(currentTime + timeStep) * _speed;

    // deplacement constant
 //   node->currentField->displacement = node->currentField->speed * timeStep;
    node->newField->displacement = node->newField->speed * timeStep;
  } */
  for (short direction = 0; direction < 3; direction++)
  {
    // si on a une composante de vitesse imposee
    if (_speed(direction) != 0.0)
    {
      // si c'est constant
      if (_function == NULL)
      {
        // acceleration nulle
        node->currentField->acceleration(direction) = 0.0;
        node->newField->acceleration(direction) = 0.0;

        // vitesse imposee
        node->currentField->speed(direction) = _speed(direction);
        node->newField->speed(direction) = _speed(direction);

        // deplacement constant
        node->currentField->displacement(direction) = node->currentField->speed(direction) * timeStep;
        node->newField->displacement(direction) = node->newField->speed(direction) * timeStep;
      }
      else
      {
        // acceleration non nulle
        node->currentField->acceleration(direction) = (_function->getValue(currentTime + timeStep) - _function->getValue(currentTime)) * _speed(direction) / timeStep;
        node->newField->acceleration(direction) = node->currentField->acceleration(direction);

        // vitesse imposee
        node->currentField->speed(direction) = _function->getValue(currentTime) * _speed(direction);
        node->newField->speed(direction) = _function->getValue(currentTime + timeStep) * _speed(direction);

        // deplacement constant
        node->currentField->displacement(direction) = node->currentField->speed(direction) * timeStep;
        node->newField->displacement(direction) = node->newField->speed(direction) * timeStep;
      }
    }
  }
}

//destructeur de la classe BoundarySpeed
/*
  Cette methode est le destructeur de la classe BoundarySpeed.

  \since DynELA 0.9.1
*/
//-----------------------------------------------------------------------------
BoundarySpeed::~BoundarySpeed()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundarySpeed::getValue()
//-----------------------------------------------------------------------------
{
  return _speed;
}

//-----------------------------------------------------------------------------
void BoundarySpeed::setValue(const Vec3D &newSpeed)
//-----------------------------------------------------------------------------
{
  _speed = newSpeed;
}

//-----------------------------------------------------------------------------
void BoundarySpeed::setValue(double speedX, double speedY, double speedZ)
//-----------------------------------------------------------------------------
{
  _speed(0) = speedX;
  _speed(1) = speedY;
  _speed(2) = speedZ;
}
