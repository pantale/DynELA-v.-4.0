/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaterials_TabularLaw_h__
#define __dnlMaterials_TabularLaw_h__

#include <HardeningLaw.h>

//-----------------------------------------------------------------------------
// Class : HardeningLaw
//
// Used to manage HardeningLaw
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class TabularLaw : public HardeningLaw
{
  DiscreteFunction *_function = NULL;

public:
  // constructeurs
  TabularLaw();
  TabularLaw(const TabularLaw &mat);
  ~TabularLaw();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  void setFunction(DiscreteFunction *function);
  DiscreteFunction *getFunction();
  String getFunctionName();
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);

  /*  // fonction de calcul
  double getYieldStress(IntegrationPoint *point);    // calcule la contrainte equivalente
  double getDerYieldStress(IntegrationPoint *point); // calcule la contrainte equivalente
  double getIsotropicYieldStress(Element *element, double shift = 0.0);
  double getIsotropicYieldHardening(Element *element, double shift = 0.0);
  String convertToDynELASourceFile(String name);

  // fonctions entree sortie
  void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, TabularLaw &mat);
  void toFile(FILE *pfile);
  void plot(FILE *pfile, double epsMax = 1); */
};

#endif
