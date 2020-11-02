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
  \file TabularLaw.C
  \brief Definition file for the TabularLaw class

  This file is the definition file for the TabularLaw class.

  \ingroup dnlMaterials
*/

#include <TabularLaw.h>

//-----------------------------------------------------------------------------
TabularLaw::TabularLaw() : HardeningLaw()
//-----------------------------------------------------------------------------
{
  _lawType = Tabular;
  _lawName = "Tabular Law";
  _yieldLaw = true;
}

//-----------------------------------------------------------------------------
TabularLaw::TabularLaw(const TabularLaw &mat) : HardeningLaw(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
TabularLaw::~TabularLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int TabularLaw::getNumberOfParameters()
//-----------------------------------------------------------------------------
{
  return 0;
}

//-----------------------------------------------------------------------------
const char *TabularLaw::getParameterName(int parameter)
//-----------------------------------------------------------------------------
{
  return "";
}

//-----------------------------------------------------------------------------
double &TabularLaw::getParameter(int parameter)
//-----------------------------------------------------------------------------
{
  return _dummyReal;
}

//-----------------------------------------------------------------------------
void TabularLaw::setFunction(DiscreteFunction *function)
//-----------------------------------------------------------------------------
{
  _function = function;
}

//-----------------------------------------------------------------------------
DiscreteFunction *TabularLaw::getFunction()
//-----------------------------------------------------------------------------
{
  return (_function);
}

//-----------------------------------------------------------------------------
String TabularLaw::getFunctionName()
//-----------------------------------------------------------------------------
{
  return _function->name;
}

//-----------------------------------------------------------------------------
double TabularLaw::getYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return _function->getValue(plasticStrain);
}

//-----------------------------------------------------------------------------
double TabularLaw::getDerivateYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return _function->getSlope(plasticStrain);
}

/*
//-----------------------------------------------------------------------------
void TabularLaw::plot(FILE *pfile, double epsMax)
//-----------------------------------------------------------------------------
{
  for (double evpl = 0; evpl < epsMax; evpl += (evpl / 5.0) + 1.0 / 1000.0)
  {
    double val = _function->getValue(evpl);
    fprintf(pfile, "%lf, %lf\n", evpl, val);
  }
}

//-----------------------------------------------------------------------------
double TabularLaw::getIsotropicYieldStress(Element *element, double shift)
//-----------------------------------------------------------------------------
{
  double plasticStrain = element->_integrationPoint->plasticStrain + dnlSqrt23 * shift;
  return _function->getValue(plasticStrain);
}

//-----------------------------------------------------------------------------
double TabularLaw::getIsotropicYieldHardening(Element *element, double shift)
//-----------------------------------------------------------------------------
{
  double plasticStrain = element->_integrationPoint->plasticStrain + dnlSqrt23 * shift;
  return _function->getSlope(plasticStrain);
}

//-----------------------------------------------------------------------------
double TabularLaw::getYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return _function->getValue(point->plasticStrain);
}

//-----------------------------------------------------------------------------
double TabularLaw::getDerYieldStress(IntegrationPoint *point)
//-----------------------------------------------------------------------------
{
  return _function->getSlope(point->plasticStrain);
}

//-----------------------------------------------------------------------------
void TabularLaw::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "Elastic Plastic Tabular law\n";
  //  Material::print (os);
  os << "Function=" << _function->name << endl;
  //   os << "A="<<A<<endl;
  //   os << "B="<<B<<endl;
  //   os << "n="<<n<<endl;
  //   os << "EpsM="<<EpsM<<endl;
  //   os << "SigM="<<SigM<<endl;
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, TabularLaw &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void TabularLaw::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  fprintf(pfile, "FUNCTION = %s\n", _function->name.chars());
  //   fprintf(pfile,"    STRESS      PARAMETER    EXPONENT\n");
  //   fprintf(pfile,"  %8.5E  %8.5E  %8.5E\n",A,B,n);
  //   fprintf(pfile,"\n");
}

//-----------------------------------------------------------------------------
void TabularLaw::setFunction(DiscreteFunction *fu)
//-----------------------------------------------------------------------------
{
  _function = fu;
}

//-----------------------------------------------------------------------------
String TabularLaw::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setFunction(%s);\n", name.chars(), _function->name.chars());
  str += sstr;

  return str;
}
 */
