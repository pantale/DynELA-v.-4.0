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
  \file Node.h
  \brief Declaration file for the Node class

  This file is the declaration file for the Node class.

  \ingroup dnlMaterials
*/

#ifndef __dnlMaterials_HardeningLaw_h__
#define __dnlMaterials_HardeningLaw_h__

#include <dnlMaths.h>

class ElementSet;

/*!
  \file HardeningLaw.h
  \brief fichier .h de definition de la classe generique des materiaux
  \ingroup femLibrary

  Ce fichier sert à la definition la classe generique des materiaux.

*/

class IntegrationPoint;
class Element;
class Material;

/*!
  \brief Classe virtuelle de gestion des materiaux
  \ingroup femLibrary

  Classe virtuelle de gestion des materiaux pour DynELA. Cette classe regroupe les parametres de base communs à tous les types de materiaux sur DynELA, comme le module de Young, le coefficient de Poisson, ou la densite d'un materiau. Pour pouvoir utiliser un materiau sur DynELA, on utilise alors une des classes derivees de celle-ci, ce qui permet alors de definir un comportement elastique, elasto-plastique, ou tout autre type de comportement.

*/
class HardeningLaw
{
public:
#ifndef SWIG
  enum
  {
    Virtual = 0,
    Elastic,
    Elastoplastic,
    Tabular,
    JohnsonCook
  };
#endif

protected:
  bool _yieldLaw = false;
  double _dummyReal = 0.0;
  int _lawType = Virtual;
  Material *_material = NULL;
  String _lawName = "";

public:
  HardeningLaw();
  HardeningLaw(const HardeningLaw &);
  virtual ~HardeningLaw();

  int getType();
  bool isYield();
  //bool isLinear();
  String getName();
  virtual int getNumberOfParameters() = 0;
  virtual const char *getParameterName(int parameter) = 0;
  virtual double &getParameter(int parameter) = 0;
  virtual double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL) = 0;
  virtual double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL) = 0;
  void add(Material *material);

  /*
  void plotToFile(String , double  = 5);
  virtual double getIsotropicYieldStress(Element *, double  = 0.0) = 0;
  virtual double getIsotropicYieldHardeningLaw(Element *, double  = 0.0) = 0;
  virtual void plot(FILE *pfile, double  = 10) = 0;
  virtual void print(std::ostream &os) const = 0;
  virtual void toFile(FILE *pfile) = 0;
  virtual String convertToDynELASourceFile(String ); */
};

#endif
