/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file NodalField.h
  \brief Declaration file for the NodalField class

  This file is the declaration file for the NodalField class. A NodalField contains all Finite Element data for each node of the structure.

  \ingroup dnlElements
*/

#ifndef __dnlElements_NodalField_h__
#define __dnlElements_NodalField_h__

#include <dnlMaths.h>

class NodalField
{

public:
  // double density = 0.0;        // Mass density at the current node \f$ \rho \f$
  // double densityInc = 0.0;     // Increment of the mass density at the current node \f$ d\rho \f$
  //double energy = 0.0;           // Internal energy at the current node \f$ e \f$
  //double energyInc = 0.0;        // Increment of the internal energy at the current node \f$ de \f$
  // Vec3D flux = 0.0;            // Thermal flux at the current node \f$ \overrightarrow{q} \f$
  // Vec3D force = 0.0;           // External force vector the current node \f$ \overrightarrow{f_e} \f$
  Vec3D displacement = 0.0; // Increment of the displacement at the current node \f$ d\overrightarrow{d} \f$
  Vec3D speed = 0.0;        // Material speed at the current node \f$ \overrightarrow{v} \f$
  Vec3D acceleration = 0.0; // Increment of the material speed at the current node \f$ \overrightarrow{v} \f$

public:
  NodalField();
  NodalField(const NodalField &X);
  ~NodalField();

#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, NodalField &);
  friend std::ofstream &operator<<(std::ofstream &, const NodalField &);
  friend std::ostream &operator<<(std::ostream &, const NodalField &);
  NodalField &read(std::ifstream &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  long objectSize();
  void resetValues();
};

#endif
