/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file Field.C
  Definition of the Finite Elements fields.

  This file defines the Finite Elements fields.
  \ingroup dnlKernel
*/

#include <Field.h>
#include <Errors.h>

#define NAME_VEC3D(NAME) NAME, NAME "X", NAME "Y", NAME "Z"
#define NAME_TENSOR2(NAME) NAME, NAME "XX", NAME "XY", NAME "XZ", NAME "YX", NAME "YY", NAME "YZ", NAME "ZX", NAME "ZY", NAME "ZZ"

const char *const Field::vtkNames[] = {
    NAME_VEC3D("displacement"),
    NAME_VEC3D("displacementIncrement"),
    NAME_VEC3D("initialNodeCoordinate"),
    NAME_VEC3D("nodeCoordinate"),
    NAME_VEC3D("speed"),
    NAME_VEC3D("speedIncrement"),
    "density",
    "plasticStrainRate",
    "energy",
    "energyIncrement",
    "internalEnergy",
    "plasticStrain",
    "gamma",
    "gammaCumulate",
    "initialTemperature",
    "mass",
    "vonMises",
    "pressure",
    "temperature",
    "timeStep",
    "realTimeStep",
    "kineticEnergy",
    "yield",
    NAME_TENSOR2("Strain"),
    NAME_TENSOR2("StrainIncrement"),
    NAME_TENSOR2("PlasticStrain"),
    NAME_TENSOR2("PlasticStrainIncrement"),
    NAME_TENSOR2("DeviatoricStress"),
    NAME_TENSOR2("Stress"), "ENDFIELDS"};

/*
  Default constructor of the Field class.
*/
//-----------------------------------------------------------------------------
Field::Field()
//-----------------------------------------------------------------------------
{
  if (vtkNames[ENDFIELDS] != "ENDFIELDS")
    fatalError("Field::Field", "Lists of fields doesn't contain the same number of elements");
}

/*
  Default destructor of the Field class.
*/
//-----------------------------------------------------------------------------
Field::~Field()
//-----------------------------------------------------------------------------
{
}

/*
  Returns a string defining the VTK name of the field.
  - field identification number of the field.
  Return : identification name of the field
*/
//-----------------------------------------------------------------------------
String Field::getVtklabel(short field)
//-----------------------------------------------------------------------------
{
  return vtkNames[field];
}

/*
  Returns the identification number of a given field specified by its name.
  - name identification name of the field.
  Return : identification number of the field
*/
//-----------------------------------------------------------------------------
short Field::getField(String name)
//-----------------------------------------------------------------------------
{
  short fieldId = 0;
  while (fieldId != ENDFIELDS)
  {
    if (vtkNames[fieldId] == name)
      return fieldId;
    fieldId += 1;
  }
  return -1;
}

/*
  Returns the type of a given field specified by its identification number.
  - field identification number of the field.
  Return : 0 if the field is a scalar, 1 if the field is a Vec3D and 2 if it's a tensor.
*/
//-----------------------------------------------------------------------------
short Field::getType(short field)
//-----------------------------------------------------------------------------
{
  switch (field)
  {
  // Those guys are all Vec3D
  case Field::displacement:
  case Field::displacementIncrement:
  case Field::initialNodeCoordinate:
  case Field::nodeCoordinate:
  case Field::speed:
  case Field::speedIncrement:
    return 1;
    break;

    // And those guys are Tensor2
  case Field::Strain:
  case Field::StrainInc:
  case Field::PlasticStrain:
  case Field::PlasticStrainInc:
  case Field::DeviatoricStress:
  case Field::Stress:
    return 2;
    break;
  }

  // Default type is a scalar
  return 0;
}
