/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*
  \file Field.h
  Declaration of the Finite Elements fields.

  This file declares the Finite Elements fields.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_Field_h__
#define __dnlKernel_Field_h__

#include <String.h>

#define FIELD_VEC3D(NAME) NAME, NAME##X, NAME##Y, NAME##Z
#define FIELD_TENSOR2(NAME) NAME, NAME##XX, NAME##XY, NAME##XZ, NAME##YX, NAME##YY, NAME##YZ, NAME##ZX, NAME##ZY, NAME##ZZ

/*
  Informations about nodal and element fields

This class is used to store and manipulate information about nodal and element fields for the DynELA Finite Element code.
  \ingroup dnlKernel
*/
//-----------------------------------------------------------------------------
// Class : Explicit
//
// Used to manage DynELA FEM Solvers for Explicit solve
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Field
{
public:
  static const char *const vtkNames[]; // List of names associated to the fields for the VTK output file

public:
  enum FieldLabel
  {
    FIELD_VEC3D(coords),             // -> NODE : Coordinates of a node
    //FIELD_VEC3D(coords0),            // Initial coords of a node
    FIELD_VEC3D(disp),               // -> NODE : Displacement of a node
    FIELD_VEC3D(dispInc),            // -> NodalField : Increment of the disp
    FIELD_VEC3D(speed),              // -> NodalField : Speed of a node
    FIELD_VEC3D(speedInc),           // -> NodalField : Increment of the speed of a node
    density,                         // ->INT PT : Material density
    //energy,                          // Total energy
    //energyInc,                       // Increment of the total energy
    gamma,                           // ->INT PT : Gamma radial return value
    gammaCumulate,                   // ->INT PT : Cumulative value of gamma values
    internalEnergy,                  // ->INT PT : Internal energy
    inelasticEnergy,                 // ->INT PT : Inelastic energy
    kineticEnergy,                   // -> GLOBAL : Kinetic energy of the current model
    mass,                            // -> NODE : Nodal Mass
    plasticStrain,                   // ->INT PT : Equivalent plastic strain
    plasticStrainRate,               // ->INT PT : Equivalent plastic strain rate
    pressure,                        // ->INT PT : Pressure
    realTimeStep,                    // -> GLOBAL : Real timeStep taking into account the reducing due to saves
    T,                               // ->INT PT : Temperature
    //T0,                              // Initial Temperature
    timeStep,                        // -> GLOBAL : TimeStep
    vonMises,                        // von Mises equivalent stress
    yieldStress,                     // ->INT PT : Maximum yield Stress of the point
    //FIELD_TENSOR2(DeviatoricStress), // Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$
    FIELD_TENSOR2(PlasticStrain),    // ->INT PT : Plastic Strain tensor
    FIELD_TENSOR2(PlasticStrainInc), // ->INT PT : Increment of the Plastic Strain tensor
    FIELD_TENSOR2(Strain),           // ->INT PT : Strain tensor
    FIELD_TENSOR2(StrainInc),        // ->INT PT : Increment of the Strain tensor
    FIELD_TENSOR2(Stress),           // ->INT PT : Stress tensor
    ENDFIELDS
  };

public:
  Field();
  ~Field();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  short getField(String name);
  short getType(short field);
  String getVtklabel(short field);
};

// Specific documentation zone of the file for Doxygen
/* \var Field::disp
     Displacement of a node
    \var Field::dispX
     Displacement of a node along the X direction
    \var Field::dispY
     Displacement of a node along the Y direction
    \var Field::dispZ
     Displacement of a node along the Z direction

    \var Field::dispInc
     Increment of the disp
    \var Field::dispIncX
     Increment of the disp along the X direction
    \var Field::dispIncY
     Increment of the disp along the Y direction
    \var Field::dispIncZ
     Increment of the disp along the Z direction

    \var Field::coords0
     Initial coordinate of a node - NO SENSE !
    \var Field::coords0X
     Initial coordinate X of a node
    \var Field::coords0Y
     Initial coordinate Y of a node
    \var Field::coords0Z
     Initial coordinate Z of a node

    \var Field::coords
     Coordinate of a node - NO SENSE !
    \var Field::coordsX
     Coordinate X of a node
    \var Field::coordsY
     Coordinate Y of a node
    \var Field::coordsZ
     Coordinate Z of a node

    \var Field::speed
     Speed of a node
    \var Field::speedX
     Speed of a node along the X direction
    \var Field::speedY
     Speed of a node along the Y direction
    \var Field::speedZ
     Speed of a node along the Z direction

    \var Field::speedInc
     Increment of the speed of a node
    \var Field::speedIncX
     Increment of the speed of a node along the X direction
    \var Field::speedIncY
     Increment of the speed of a node along the Y direction
    \var Field::speedIncZ
     Increment of the speed of a node along the Z direction

    \var Field::Strain
     Strain tensor norm
    \var Field::StrainXX
     Strain tensor component XX
    \var Field::StrainXY
     Strain tensor component XY
    \var Field::StrainXZ
     Strain tensor component XZ
    \var Field::StrainYX
     Strain tensor component YX
    \var Field::StrainYY
     Strain tensor component YY
    \var Field::StrainYZ
     Strain tensor component YZ
    \var Field::StrainZX
     Strain tensor component ZX
    \var Field::StrainZY
     Strain tensor component ZY
    \var Field::StrainZZ
     Strain tensor component ZZ

    \var Field::StrainInc
     Increment of the Strain tensor norm
    \var Field::StrainIncXX
     Increment of the Strain tensor component XX
    \var Field::StrainIncXY
     Increment of the Strain tensor component XY
    \var Field::StrainIncXZ
     Increment of the Strain tensor component XZ
    \var Field::StrainIncYX
     Increment of the Strain tensor component YX
    \var Field::StrainIncYY
     Increment of the Strain tensor component YY
    \var Field::StrainIncYZ
     Increment of the Strain tensor component YZ
    \var Field::StrainIncZX
     Increment of the Strain tensor component ZX
    \var Field::StrainIncZY
     Increment of the Strain tensor component ZY
    \var Field::StrainIncZZ
     Increment of the Strain tensor component ZZ

    \var Field::PlasticStrain
     Plastic Strain tensor norm
    \var Field::PlasticStrainXX
     Plastic Strain tensor component XX
    \var Field::PlasticStrainXY
     Plastic Strain tensor component XY
    \var Field::PlasticStrainXZ
     Plastic Strain tensor component XZ
    \var Field::PlasticStrainYX
     Plastic Strain tensor component YX
    \var Field::PlasticStrainYY
     Plastic Strain tensor component YY
    \var Field::PlasticStrainYZ
     Plastic Strain tensor component YZ
    \var Field::PlasticStrainZX
     Plastic Strain tensor component ZX
    \var Field::PlasticStrainZY
     Plastic Strain tensor component ZY
    \var Field::PlasticStrainZZ
     Plastic Strain tensor component ZZ

    \var Field::PlasticStrainInc
     Increment of the Plastic Strain tensor norm
    \var Field::PlasticStrainIncXX
     Increment of the Plastic Strain tensor component XX
    \var Field::PlasticStrainIncXY
     Increment of the Plastic Strain tensor component XY
    \var Field::PlasticStrainIncXZ
     Increment of the Plastic Strain tensor component XZ
    \var Field::PlasticStrainIncYX
     Increment of the Plastic Strain tensor component YX
    \var Field::PlasticStrainIncYY
     Increment of the Plastic Strain tensor component YY
    \var Field::PlasticStrainIncYZ
     Increment of the Plastic Strain tensor component YZ
    \var Field::PlasticStrainIncZX
     Increment of the Plastic Strain tensor component ZX
    \var Field::PlasticStrainIncZY
     Increment of the Plastic Strain tensor component ZY
    \var Field::PlasticStrainIncZZ
     Increment of the Plastic Strain tensor component ZZ

    \var Field::DeviatoricStress
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ norm
    \var Field::DeviatoricStressXX
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component XX
    \var Field::DeviatoricStressXY
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component XY
    \var Field::DeviatoricStressXZ
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component XZ
    \var Field::DeviatoricStressYX
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component YX
    \var Field::DeviatoricStressYY
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component YY
    \var Field::DeviatoricStressYZ
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component YZ
    \var Field::DeviatoricStressZX
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component ZX
    \var Field::DeviatoricStressZY
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component ZY
    \var Field::DeviatoricStressZZ
     Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$ component ZZ

    \var Field::Stress
     Stress tensor norm
    \var Field::StressXX
     Stress tensor component XX
    \var Field::StressXY
     Stress tensor component XY
    \var Field::StressXZ
     Stress tensor component XZ
    \var Field::StressYX
     Stress tensor component YX
    \var Field::StressYY
     Stress tensor component YY
    \var Field::StressYZ
     Stress tensor component YZ
    \var Field::StressZX
     Stress tensor component ZX
    \var Field::StressZY
     Stress tensor component ZY
    \var Field::StressZZ
     Stress tensor component ZZ
  */

#endif
