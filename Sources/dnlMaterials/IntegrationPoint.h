/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_IntegrationPoint_h__
#define __dnlElements_IntegrationPoint_h__

#include <dnlMaths.h>

struct IntegrationPointData;
//-----------------------------------------------------------------------------
// Class : IntegrationPointBase
//
// Used to manage IntegrationPointBase
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class IntegrationPointBase
{

public:
  double detJ;                                      // Determinant du Jacobien
  double detJ0;                                     // Determinant du Jacobien de la configuration de reference
  double radius = 1;                                // Radius at the current integration point
  Matrix dShapeFunction;                            // Matrice des derivees des fonctions d'interpolation par rapport aux coordonnees physiques
  Matrix invJxW;                                    // Inverse du Jacobien de l'element au point d'integration
  Matrix JxW;                                       // Jacobien de l'element au point d'integration
  Tensor2 R;                                        // Rotation tensor for the integration pount
  const IntegrationPointData *integrationPointData; // Data storage for the integration point data

public:
  IntegrationPointBase(int dimension, int numberOfNodes);
  virtual ~IntegrationPointBase();
  virtual void flush() = 0;
  void baseFlush();
};

//-----------------------------------------------------------------------------
// Class : UnderIntegrationPoint
//
// Used to manage UnderIntegrationPoint
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class UnderIntegrationPoint : public IntegrationPointBase
{

public:
  UnderIntegrationPoint(int dimension, int numberOfNodes);
  ~UnderIntegrationPoint();
  void flush();
};

//-----------------------------------------------------------------------------
// Class : IntegrationPoint
//
// Used to manage IntegrationPoint
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class IntegrationPoint : public IntegrationPointBase
{

public:
  double yieldStress = 0.0;       // parametre d'ecrouissage
  double plasticStrain = 0.0;     // parametre d'ecrouissage
  double plasticStrainRate = 0.0; // parametre d'ecrouissage
  double pressure = 0.0;          // partie spherique du tenseur des contraintes
  double gamma = 0.0;             // state variable for VUMAT gamma
  double gammaCumulate = 0.0;     // state variable for VUMAT gamma
  double temperature = 0.0;       // state variable for VUMAT temperature
  double internalEnergy = 0.0;    //
  double inelasticEnergy = 0.0;   //
  double density;                 //
  SymTensor2 Strain;              // tenseur des deformations
  SymTensor2 StrainInc;           // increment du tenseur des deformations
  SymTensor2 PlasticStrain;       // tenseur des deformations plastiques
  SymTensor2 PlasticStrainInc;    // increment du tenseur des deformations plastiques
  SymTensor2 Stress;              // tenseur des contraintes

public:
  IntegrationPoint(int dimension, int numberOfNodes);
  ~IntegrationPoint();

  // Interface methods excluded from SWIG
#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, IntegrationPoint &intPoint);
  friend std::ofstream &operator<<(std::ofstream &, const IntegrationPoint &intPoint);
  friend std::ostream &operator<<(std::ostream &, IntegrationPoint &intPoint);
  IntegrationPoint &read(std::ifstream &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  void flush();
};

#endif
