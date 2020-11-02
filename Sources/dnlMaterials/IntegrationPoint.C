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
  \file IntegrationPoint.C
  \brief Definition file for the IntegrationPoint class

  This file is the definition file for the IntegrationPoint class.

  \ingroup dnlElements
*/

#include <IntegrationPoint.h>

//-----------------------------------------------------------------------------
IntegrationPointBase::IntegrationPointBase(int dimension, int numberOfNodes)
//-----------------------------------------------------------------------------
{
  // derivees des fonctions d'interpolation
  dShapeFunction.redim(numberOfNodes, dimension);

  // Jacobien et getInverse
  JxW.redim(dimension, dimension);
  invJxW.redim(dimension, dimension);

  R.setToUnity();
}

//-----------------------------------------------------------------------------
IntegrationPointBase::~IntegrationPointBase()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IntegrationPointBase::baseFlush()
//-----------------------------------------------------------------------------
{
  // fonctions d'interpolation
  dShapeFunction = 0.;

  // Jacobien et getInverse
  JxW = 0.;
  invJxW = 0.;
  detJ = 0.;
  //detJ0 = 0.;
}

//-----------------------------------------------------------------------------
UnderIntegrationPoint::UnderIntegrationPoint(int dimension, int numberOfNodes) : IntegrationPointBase(dimension, numberOfNodes)
//-----------------------------------------------------------------------------
{
  // initialisations
  flush();
}

//-----------------------------------------------------------------------------
UnderIntegrationPoint::~UnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void UnderIntegrationPoint::flush()
//-----------------------------------------------------------------------------
{
  // initialisations
}

//-----------------------------------------------------------------------------
IntegrationPoint::IntegrationPoint(int dimension, int numberOfNodes) : IntegrationPointBase(dimension, numberOfNodes)
//-----------------------------------------------------------------------------
{
  // initialisations
  flush();
}

//-----------------------------------------------------------------------------
IntegrationPoint::~IntegrationPoint()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void IntegrationPoint::flush()
//-----------------------------------------------------------------------------
{
  // initialisations
  plasticStrainRate = 0.0;
  //DeviatoricStress = 0.0;
  plasticStrain = 0.0;
  PlasticStrain = 0.0;
  pressure = 0.0;
  Strain = 0.0;
  StrainInc = 0.0;
  Stress = 0.0;
  yieldStress = 0.0;
}

//-----------------------------------------------------------------------------
void IntegrationPoint::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  //  os << "pressure=" << pressure << ", DeviatoricStress=" << DeviatoricStress << std::endl;
  os << "Stress=" << Stress << std::endl;
}

//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, IntegrationPoint &intPoint)
//-----------------------------------------------------------------------------
{
  intPoint.print(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const IntegrationPoint &intPoint)
//-----------------------------------------------------------------------------
{
  intPoint.write(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, IntegrationPoint &intPoint)
//-----------------------------------------------------------------------------
{
  intPoint.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void IntegrationPoint::write(std::ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  pfile.write((char *)&pressure, sizeof(double));
  pfile.write((char *)&plasticStrain, sizeof(double));

  //pfile << DeviatoricStress;
  pfile << Stress;

  pfile << Strain;
  pfile << StrainInc;
  pfile << PlasticStrain;
}

//-----------------------------------------------------------------------------
IntegrationPoint &IntegrationPoint::read(std::ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile.read((char *)&pressure, sizeof(double));
  pfile.read((char *)&plasticStrain, sizeof(double));

  //  pfile >> DeviatoricStress;
  pfile >> Stress;

  pfile >> Strain;
  pfile >> StrainInc;
  pfile >> PlasticStrain;

  return *this;
}
