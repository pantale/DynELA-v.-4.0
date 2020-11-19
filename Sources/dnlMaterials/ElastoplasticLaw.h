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

// TODOCXYFILE

/*!
  \file ElastoplasticLaw.h
  \brief Declaration file for the ElastoplasticLaw class

  This file is the declaration file for the ElastoplasticLaw class.

  \ingroup dnlMaterials
*/

#ifndef __dnlMaterials_ElastoplasticLaw_h__
#define __dnlMaterials_ElastoplasticLaw_h__

#include <HardeningLaw.h>

class ElastoplasticLaw : public HardeningLaw
{

  double A = 0.0;
  double B = 0.0;
  double n = 0.0;

public:
  ElastoplasticLaw();
  ElastoplasticLaw(const ElastoplasticLaw &law);
  ~ElastoplasticLaw();

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  void setParameters(double A, double B, double n);
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);

  /*  // fonctions membres
  double & Yield_Stress ();
  double & Hard_Param ();
  double & Hard_Exponent ();
  double & Fail_Strain ();
  double & Max_Stress ();

  // methodes de renseignement
  void setYieldStress(double val);
  void setHardParameter(double val);
  void setHardExponent(double val);
  String convertToDynELASourceFile(String name);

  // fonction de calcul
  double getYieldStress (IntegrationPoint * point);	// calcule la contrainte equivalente
  double getDerYieldStress (IntegrationPoint * point);	// calcule la contrainte equivalente
  double getIsotropicYieldStress (Element * element, double shift = 0.0);
  double getIsotropicYieldHardening (Element * element, double shift = 0.0);


  const char *lawName ()
  {
    return (lawName_IsoHardElastoplastic);
  }

  const char *Name ()
  {
    return (Name_IsoHardElastoplastic);
  }

  int getType ();
  int getNumberOfParameters ();
  const char *getParameterName (long ind);
  double & getParameter (long ind);

  // fonctions entree sortie
   void print (ostream & os) const;
  friend ostream & operator << (ostream & os, ElastoplasticLaw & mat);
  void toFile (FILE * pfile);
  void plot(FILE* pfile, double epsMax = 1); */
};

#endif
