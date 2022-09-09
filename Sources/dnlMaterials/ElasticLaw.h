/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaterial_ElasticLaw_h__
#define __dnlMaterial_ElasticLaw_h__

#include <HardeningLaw.h>

//-----------------------------------------------------------------------------
// Class : ElasticLaw
//
// Used to manage ElasticLaw
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElasticLaw : public HardeningLaw
{

public:
  ElasticLaw();
  ElasticLaw(const ElasticLaw &);
  ~ElasticLaw();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
};

#endif
