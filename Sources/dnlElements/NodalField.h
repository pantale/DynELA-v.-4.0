/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_NodalField_h__
#define __dnlElements_NodalField_h__

#include <dnlMaths.h>

//-----------------------------------------------------------------------------
// Class : NodalField
//
// Used to manage Nodal fields in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class NodalField
{

public:
  // double density = 0.0;        // Mass density at the current node \f$ \rho \f$
  // double densityInc = 0.0;     // Increment of the mass density at the current node \f$ d\rho \f$
  // double energy = 0.0;           // Internal energy at the current node \f$ e \f$
  // double energyInc = 0.0;        // Increment of the internal energy at the current node \f$ de \f$
  // Vec3D flux = 0.0;            // Thermal flux at the current node \f$ \overrightarrow{q} \f$
  Vec3D u = 0;         // Increment of the disp at the current node \f$ d\overrightarrow{d} \f$
  Vec3D speed = 0;        // Material speed at the current node \f$ \overrightarrow{v} \f$
  Vec3D acceleration = 0; // Increment of the material speed at the current node \f$ \overrightarrow{v} \f$

public:
  NodalField();
  NodalField(const NodalField &X);
  ~NodalField();

  // Interface methods excluded from SWIG
#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, NodalField &);
  friend std::ofstream &operator<<(std::ofstream &, const NodalField &);
  friend std::ostream &operator<<(std::ostream &, const NodalField &);
  NodalField &read(std::ifstream &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  long objectSize();
  void resetValues();
};

#endif
