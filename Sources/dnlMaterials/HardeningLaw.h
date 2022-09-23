/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaterials_HardeningLaw_h__
#define __dnlMaterials_HardeningLaw_h__

#include <dnlMaths.h>

class ElementSet;

class IntegrationPoint;
class Element;
class Material;

//-----------------------------------------------------------------------------
// Class : HardeningLaw
//
// Used to manage HardeningLaw
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class HardeningLaw
{
public:
  // This enum is excluded from SWIG
#ifndef SWIG
  enum
  {
    Virtual = 0,
    Elastic,
    Elastoplastic,
    Tabular,
    JohnsonCook,
    ANN2Sig,
    ANNFortran
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

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getType();
  bool isYield();
  // bool isLinear();
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
