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

#include <BoundaryAcceleration.h>
/* #include <Node.h>
#include <NodalField.h>
 */
//-----------------------------------------------------------------------------
BoundaryAcceleration::BoundaryAcceleration(char *newName) : Boundary(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::BoundaryAcceleration(const Vec3D &newAcceleration)
//-----------------------------------------------------------------------------
{
  _acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::BoundaryAcceleration(const BoundaryAcceleration &X)
//-----------------------------------------------------------------------------
{
    std::cerr << "can't copy directely BoundaryAcceleration class" << &X << std::endl;
  exit(-1);
}

//-----------------------------------------------------------------------------
BoundaryAcceleration::~BoundaryAcceleration()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::applyInitial(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::applyConstantOnCurrentFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::applyConstantOnNewFields(Node *node, double currentTime, double timeStep)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Vec3D BoundaryAcceleration::getValue()
//-----------------------------------------------------------------------------
{
  return _acceleration;
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::setValue(const Vec3D &newAcceleration)
//-----------------------------------------------------------------------------
{
  _acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------
void BoundaryAcceleration::setValue(double accX, double accY, double accZ)
//-----------------------------------------------------------------------------
{
  _acceleration(0) = accX;
  _acceleration(1) = accY;
  _acceleration(2) = accZ;
}
