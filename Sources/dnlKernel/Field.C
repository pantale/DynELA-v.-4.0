/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
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
    NAME_VEC3D("coords"),
    //NAME_VEC3D("coords0"),
    NAME_VEC3D("disp"),
    NAME_VEC3D("dispInc"),
    NAME_VEC3D("speed"),
    NAME_VEC3D("speedInc"),
    "density",
    //"energy",
    //"energyInc",
    "gamma",
    "gammaCumulate",
    "internalEnergy",
    "inelasticEnergy",
    "kineticEnergy",
    "mass",
    "plasticStrain",
    "plasticStrainRate",
    "pressure",
    "realTimeStep",
    "T",
    //"T0",
    "timeStep",
    "vonMises",
    "yield",
    //NAME_TENSOR2("DeviatoricStress"),
    NAME_TENSOR2("PlasticStrain"),
    NAME_TENSOR2("PlasticStrainInc"),
    NAME_TENSOR2("Strain"),
    NAME_TENSOR2("StrainInc"),
    NAME_TENSOR2("Stress"),
    "ENDFIELDS"};

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
  case Field::disp:
  case Field::dispInc:
  //case Field::coords0:
  case Field::coords:
  case Field::speed:
  case Field::speedInc:
    return 1;
    break;

    // And those guys are Tensor2
  case Field::Strain:
  case Field::StrainInc:
  case Field::PlasticStrain:
  case Field::PlasticStrainInc:
  //case Field::DeviatoricStress:
  case Field::Stress:
    return 2;
    break;
  }

  // Default type is a scalar
  return 0;
}
