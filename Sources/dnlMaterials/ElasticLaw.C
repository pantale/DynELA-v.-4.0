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
  \file ElasticLaw.C
  Definition file for the ElasticLaw class

  This file is the definition file for the ElasticLaw class.

  \ingroup dnlMaterials
*/

#include <ElasticLaw.h>

//-----------------------------------------------------------------------------
ElasticLaw::ElasticLaw() : HardeningLaw()
//-----------------------------------------------------------------------------
{
  _lawType = Elastic;
  _lawName = "Elastic Law";
  _yieldLaw = false;
}

//-----------------------------------------------------------------------------
ElasticLaw::ElasticLaw(const ElasticLaw &mat) : HardeningLaw(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElasticLaw::~ElasticLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int ElasticLaw::getNumberOfParameters()
//-----------------------------------------------------------------------------
{
  return 0;
}

//-----------------------------------------------------------------------------
const char *ElasticLaw::getParameterName(int parameter)
//-----------------------------------------------------------------------------
{
  return "";
}

//-----------------------------------------------------------------------------
double &ElasticLaw::getParameter(int parameter)
//-----------------------------------------------------------------------------
{
  return _dummyReal;
}

//-----------------------------------------------------------------------------
double ElasticLaw::getYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return 0.0;
}

//-----------------------------------------------------------------------------
double ElasticLaw::getDerivateYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return 0.0;
}
