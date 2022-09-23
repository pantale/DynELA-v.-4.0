/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaterials_ANN2SigLaw_h__
#define __dnlMaterials_ANN2SigLaw_h__

#include <HardeningLaw.h>

/*
  \class ANN2SigLaw
  Johnson-Cook hardening law.

  The general formulation of the Johnson-Cook constitutive law have the following form:
  \f[ \sigma^{y}=\left(_A+B\overline{\varepsilon^{p}}^{n}\right)\left[1+C\ln\left(\frac{\stackrel{\bullet}{\overline{\varepsilon^{p}}}}{\stackrel{\bullet}{\overline{\varepsilon_{0}}}}\right)\right]\left[1-\left(\frac{T-T_{0}}{T_{m}-T_{0}}\right)^{m}\right] \f]_______

  In this approach, a modified version is used to compute the yield stress of the material at the current point:
  \f[ \sigma^{y}=\left(_A+B\overline{\varepsilon^{p}}^{n}\right)\sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})\sigma_{T}^{y}(T) \f]_______

  where the dependence on deformation rate \f$ \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}}) \f$
  is defined by the following:
  \f[ \left\{ \begin{array}{lll}
  \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})=1+C\ln\left(\frac{\stackrel{\bullet}{\overline{\varepsilon^{p}}}}{\stackrel{\bullet}{\overline{\varepsilon_{0}}}}\right) & \mbox{if} & \stackrel{\bullet}{\overline{\varepsilon^{p}}}\geq\stackrel{\bullet}{\overline{\varepsilon_{0}}}\\
  \sigma_{\varepsilon^{p}}^{y}(\stackrel{\bullet}{\overline{\varepsilon^{p}}})=1 & \mbox{if} & \stackrel{\bullet}{\overline{\varepsilon^{p}}}<\stackrel{\bullet}{\overline{\varepsilon_{0}}}
  \end{array}\right.\f]
  and the dependence on temperature \f$ \sigma_{T}^{y}(T)\f$ is defined by the following:
  \f[ \left\{ \begin{array}{lll}
  \sigma_{T}^{y}(T)=1-\left(\frac{T-T_{0}}{T_{m}-T_{0}}\right)^{m} & \mbox{if} & T_{0}\leq T\leq T_{m}\\
  \sigma_{T}^{y}(T)=1 & \mbox{if} & T<T_{0}\\
  \sigma_{T}^{y}(T)=0 & \mbox{if} & T\geq T_{m}
  \end{array}\right.\f]

  \ingroup femLibrary
  \see Material
  \since DynELA 3.0.0
*/
//-----------------------------------------------------------------------------
// Class : ANN2SigLaw
//
// Used to manage ANN2SigLaw
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ANN2SigLaw : public HardeningLaw
{
  // double A = 0.0;
  // double B = 0.0;
  // double C = 0.0;
  // double n = 0.0;
  // double m = 0.0;
  // double depsp0 = 0.0;
  // double T0 = 0.0;
  // double Tm = 0.0;
  double dummy;
  Matrix w1, w2, w3;
  Vector b1, b2, b3;
  Vector logBase, minEntries, maxEntries, rangeEntries;

public:
  ANN2SigLaw();
  ANN2SigLaw(const ANN2SigLaw &law);
  ~ANN2SigLaw();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  void setParameters(char *filename);
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
};

#endif
