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
  \file ElasticLaw.h
  \brief Declaration file for the ElasticLaw class

  This file is the declaration file for the ElasticLaw class.

  \ingroup dnlMaterials
*/

#ifndef __dnlMaterial_ElasticLaw_h__
#define __dnlMaterial_ElasticLaw_h__

#include <HardeningLaw.h>

class ElasticLaw : public HardeningLaw
{

public:
  ElasticLaw();
  ElasticLaw(const ElasticLaw &);
  ~ElasticLaw();

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
};

#endif
