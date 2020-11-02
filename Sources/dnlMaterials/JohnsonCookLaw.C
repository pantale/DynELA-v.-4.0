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
  \file JohnsonCookLaw.C
  \brief Definition file for the JohnsonCookLaw class

  This file is the definition file for the JohnsonCookLaw class.

  \ingroup dnlMaterials
*/

#include <JohnsonCookLaw.h>
#include <IntegrationPoint.h>
#include <Material.h>

//-----------------------------------------------------------------------------
JohnsonCookLaw::JohnsonCookLaw() : HardeningLaw()
//-----------------------------------------------------------------------------
{
  _lawType = JohnsonCook;
  _lawName = "JohnsonCook Law";
  _yieldLaw = true;
}

//-----------------------------------------------------------------------------
JohnsonCookLaw::JohnsonCookLaw(const JohnsonCookLaw &law) : HardeningLaw(law)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
JohnsonCookLaw::~JohnsonCookLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int JohnsonCookLaw::getNumberOfParameters()
//-----------------------------------------------------------------------------
{
  return 8;
}

//-----------------------------------------------------------------------------
const char *JohnsonCookLaw::getParameterName(int parameter)
//-----------------------------------------------------------------------------
{
  String retour;
  switch (parameter)
  {
  case 0:
    return "A";
    break;
  case 1:
    return "B";
    break;
  case 2:
    return "C";
    break;
  case 3:
    return "n";
    break;
  case 4:
    return "m";
    break;
  case 5:
    return "depsp0";
    break;
  case 6:
    return "Tm";
    break;
  case 7:
    return "T0";
    break;
  default:
    fatalError("JohnsonCookLaw::geConstitutiveparameterName",
               "called with parameter = %d", parameter);
  }
  return "";
}

//-----------------------------------------------------------------------------
double &JohnsonCookLaw::getParameter(int parameter)
//-----------------------------------------------------------------------------
{
  switch (parameter)
  {
  case 0:
    return A;
    break;
  case 1:
    return B;
    break;
  case 2:
    return C;
    break;
  case 3:
    return n;
    break;
  case 4:
    return m;
    break;
  case 5:
    return depsp0;
    break;
  case 6:
    return Tm;
    break;
  case 7:
    return T0;
    break;
  default:
    fatalError("JohnsonCookLaw::geConstitutiveparameterName",
               "called with parameter = %d", parameter);
  }

  // pour tromper le compilo, mais ca sert a rien car il ne passe jamais ici
  return A;
}

//-----------------------------------------------------------------------------
void JohnsonCookLaw::setParameters(double _A, double _B, double _C, double _n, double _m, double _depsp0, double _Tm, double _T0)
//-----------------------------------------------------------------------------
{
  A = _A;
  B = _B;
  C = _C;
  n = _n;
  m = _m;
  depsp0 = _depsp0;
  Tm = _Tm;
  T0 = _T0;
}

//-----------------------------------------------------------------------------
double JohnsonCookLaw::getYieldStress(double _epsp, double _depsp, double _T, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  double hardPart, viscPart, tempPart;

  // Hardening part of the Johnson-Cook law
  hardPart = A + B * pow(_epsp, n);

  // Dependence to the deformation rate
  if (_depsp > depsp0)
    viscPart = 1.0 + C * log(_depsp / depsp0);
  else
    viscPart = 1.0;

  // Dependence to the temperature if T0 < _T < Tm
  tempPart = 1.0;
  if (_T > T0)
    if (_T < Tm)
      tempPart = 1.0 - pow(((_T - T0) / (Tm - T0)), m);
    else
      tempPart = 0.0;

  // Compute and return the yield stress
  return hardPart * viscPart * tempPart;
}

// **********************************************************************
// Function to compute the Johnson-Cook yield stress
// **********************************************************************
double JohnsonCookLaw::VUMATgetYieldStress(double plasticStrain, double plasticStrainRate, double temp)
{
  double hardPart, viscPart, tempPart, yieldStress;

  // Hardening part of the Johnson-Cook law
  hardPart = A + B * pow(plasticStrain, n);

  // Dependence to the deformation rate
  if (plasticStrainRate > depsp0)
    viscPart = 1.0 + C * log(plasticStrainRate / depsp0);
  else
    viscPart = 1.0;

  // Dependence to the temperature if T0 < temp < Tm
  tempPart = 1.0;
  if (temp > T0)
    if (temp < Tm)
      tempPart = 1.0 - pow(((temp - T0) / (Tm - T0)), m);
    else
      tempPart = 0.0;

  // Compute and return the yield stress
  yieldStress = hardPart * viscPart * tempPart;
  return yieldStress;
}

// **********************************************************************
// Function to compute the Johnson-Cook hardening / plasticStrain
// **********************************************************************
double JohnsonCookLaw::VUMATyieldHardEpsp(double plasticStrain, double plasticStrainRate, double temp)
{
  double hardPart, viscPart, tempPart, yieldHardEpsp;

  // Hardening part of the Johnson-Cook law
  hardPart = n * B * pow(plasticStrain, (n - 1.0));

  // Dependence to the deformation rate
  if (plasticStrainRate > depsp0)
    hardPart = hardPart * (1.0 + C * log(plasticStrainRate / depsp0));

  // Dependence to the temperature if T0 < temp < Tm
  tempPart = 1.0;
  if (temp > T0)
    if (temp < Tm)
      tempPart = 1.0 - pow(((temp - T0) / (Tm - T0)), m);
    else
      tempPart = 0.0;

  // Compute and return the yield stress
  yieldHardEpsp = hardPart * tempPart;
  return yieldHardEpsp;
}

// **********************************************************************
// Function to compute the Johnson-Cook hardening / plasticStrainRate
// **********************************************************************
double JohnsonCookLaw::VUMATyieldHardDepsp(double plasticStrain, double plasticStrainRate, double temp)
{
  double hardPart, viscPart, tempPart, yieldHardDepsp;

  // Hardening part of the Johnson-Cook law
  hardPart = 0.0;

  // Dependence to the deformation rate
  if (plasticStrainRate > depsp0)
    hardPart = (A + B * pow(plasticStrain, n)) * C / plasticStrainRate;

  // Dependence to the temperature if T0 < temp < Tm
  tempPart = 1.0;
  if (temp > T0)
    if (temp < Tm)
      tempPart = 1.0 - pow(((temp - T0) / (Tm - T0)), m);
    else
      tempPart = 0.0;

  // Compute and return the yield stress
  yieldHardDepsp = hardPart * tempPart;
  return yieldHardDepsp;
}

// **********************************************************************
// Function to compute the Johnson-Cook hardening / T
// **********************************************************************
double JohnsonCookLaw::VUMATyieldHardTemp(double plasticStrain, double plasticStrainRate, double temp)
{
  double hardPart, viscPart, tempPart, yieldHardTemp;

  // Hardening part of the Johnson-Cook law
  hardPart = A + B * pow(plasticStrain,
                         n);
  // Dependence to the deformation rate
  if (plasticStrainRate > depsp0)
    viscPart = 1.0 + C * log(plasticStrainRate / depsp0);
  else
    viscPart = 1.0;

  // Dependence to the temperature if T0 < temp < Tm
  tempPart = 0.0;
  if ((temp > T0) and (temp < Tm))
    tempPart = -m * (pow(((temp - T0) / (Tm - T0)), (m))) / (temp - T0);

  // Compute and return the yield stress
  yieldHardTemp = hardPart *
                  viscPart * tempPart;
  return yieldHardTemp;
}

// **********************************************************************
// Function to compute the Johnson-Cook hardening
// **********************************************************************
//-----------------------------------------------------------------------------
double JohnsonCookLaw::VUMATgetDerivateYieldStress(double plasticStrain, double plasticStrainRate, double temp, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  double yield = VUMATgetYieldStress(plasticStrain, plasticStrainRate, temp);
  double hardEpsp = VUMATyieldHardEpsp(plasticStrain, plasticStrainRate, temp);
  double hardDepsp = VUMATyieldHardDepsp(plasticStrain, plasticStrainRate, temp);
  double hardTemp = VUMATyieldHardTemp(plasticStrain, plasticStrainRate, temp);
  double hard = hardEpsp + hardDepsp / _dtime + _material->taylorQuinney / (_material->density * _material->heatCapacity) * yield * hardTemp;
  return hard;
}

//-----------------------------------------------------------------------------
double JohnsonCookLaw::getDerivateYieldStress(double _epsp, double _depsp, double _T, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  double hard;
  double tempPart1 = 0.0;

  // Hardening part of the Johnson-Cook law
  double hardPart0 = A + B * pow(_epsp, n);

  // Dependence to the deformation rate
  double viscPart0 = 1.0;
  if (_depsp > depsp0)
    viscPart0 = 1.0 + C * log(_depsp / depsp0);

  // Dependence to the temperature if T0 < _T < Tm
  double tempPart0 = 1.0;
  if (_T > T0)
    if (_T < Tm)
    {
      tempPart1 = pow(((_T - T0) / (Tm - T0)), m);
      tempPart0 = 1.0 - tempPart1;
    }
    else
      tempPart0 = 0.0;

  // Function to compute the Johnson-Cook hardening / _epsp
  hard = n * B * pow(_epsp, (n - 1.0)) * viscPart0 * tempPart0;

  // Function to compute the Johnson-Cook hardening / _depsp
  if (_depsp > depsp0)
  {
    hard += hardPart0 * C * tempPart0 / (_depsp * _dtime);
  }

  // Function to compute the Johnson-Cook hardening / T
  if ((_T > T0) and (_T < Tm))
  {
    hard -= m * _material->taylorQuinney * hardPart0 * hardPart0 * viscPart0 * viscPart0 * tempPart0 * tempPart1 / (_material->density * _material->heatCapacity * (_T - T0));
  }

  return hard;
}

//-----------------------------------------------------------------------------
double JohnsonCookLaw::getNumericalDerivateYieldStress(double _epsp, double _depsp, double _T, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  double deltaEpsp = 1.0e-8;
  double deltaDepsp = 1.0e-8;
  double deltaTemp = 1.0e-8;

  double epspForward = _epsp + deltaEpsp;
  double depspForward = _depsp + deltaDepsp;
  double tempForward = _T + deltaTemp;

  double yield = getYieldStress(_epsp, _depsp, _T, _dtime, intPoint);
  double yieldHardEpsp = (getYieldStress(epspForward, _depsp, _T, _dtime, intPoint) - yield) / deltaEpsp;
  double yieldHardDepsp = (getYieldStress(_epsp, depspForward, _T, _dtime, intPoint) - yield) / deltaDepsp;
  double yieldHardTemp = (getYieldStress(_epsp, _depsp, tempForward, _dtime, intPoint) - yield) / deltaTemp;
  return yieldHardEpsp + yieldHardDepsp / _dtime + _material->taylorQuinney / (_material->density * _material->heatCapacity) * yield * yieldHardTemp;
}
