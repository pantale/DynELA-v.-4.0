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
  \file ElastoplasticLaw.C
  \brief Definition file for the ElastoplasticLaw class

  This file is the definition file for the ElastoplasticLaw class.

  \ingroup dnlMaterials
*/

#include <ElastoplasticLaw.h>
#include <IntegrationPoint.h>

//-----------------------------------------------------------------------------
ElastoplasticLaw::ElastoplasticLaw() : HardeningLaw()
//-----------------------------------------------------------------------------
{
  _lawType = Elastoplastic;
  _lawName = "Elastoplastic Law";
  _yieldLaw = true;
}

//-----------------------------------------------------------------------------
ElastoplasticLaw::ElastoplasticLaw(const ElastoplasticLaw &mat) : HardeningLaw(mat)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElastoplasticLaw::~ElastoplasticLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int ElastoplasticLaw::getNumberOfParameters()
//-----------------------------------------------------------------------------
{
  return 3;
}

//-----------------------------------------------------------------------------
const char *ElastoplasticLaw::getParameterName(int parameter)
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
    return "n";
    break;
  default:
    fatalError("ElastoplasticLaw::geConstitutiveParameterName",
               "called with parameter = %d", parameter);
  }
  return "";
}

//-----------------------------------------------------------------------------
double &ElastoplasticLaw::getParameter(int parameter)
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
    return n;
    break;
  default:
    fatalError("ElastoplasticLaw::geConstitutiveParameterName",
               "called with parameter = %d", parameter);
  }

  // pour tromper le compilo, mais ca sert a rien car il ne passe jamais ici
  return A;
}

//-----------------------------------------------------------------------------
void ElastoplasticLaw::setParameters(double _A, double _B, double _n)
//-----------------------------------------------------------------------------
{
  A = _A;
  B = _B;
  n = _n;
}

//-----------------------------------------------------------------------------
double ElastoplasticLaw::getYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return (A + B * pow(plasticStrain, n));
}

//-----------------------------------------------------------------------------
double ElastoplasticLaw::getDerivateYieldStress(double plasticStrain, double plasticStrainRate, double T, double dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  return (n * B * pow(plasticStrain, (n - 1.0)));
}

/*

//-----------------------------------------------------------------------------
void ElastoplasticLaw::plot(FILE* pfile, double epsMax)
//-----------------------------------------------------------------------------
{
  for (double evpl=0;evpl < epsMax; evpl+= (evpl/5.0)+1.0/1000.0)
    {
      double val = (A + B * pow (evpl, n));
      fprintf(pfile,"%lf, %lf\n",evpl,val);
    }
}


//-----------------------------------------------------------------------------
void ElastoplasticLaw::print (ostream & os) const
//-----------------------------------------------------------------------------
{
  os << "Elastic Plastic law\n";
//  Material::print (os);
  os << "A=" << A << endl;
  os << "B=" << B << endl;
  os << "n=" << n << endl;
//  os << "EpsM=" << EpsM << endl;
//  os << "SigM=" << SigM << endl;
}

//-----------------------------------------------------------------------------
ostream & operator << (ostream & os, ElastoplasticLaw & mat)
//-----------------------------------------------------------------------------
{
  mat.print (os);
  return os;
}

//-----------------------------------------------------------------------------
void ElastoplasticLaw::toFile (FILE * pfile)
//-----------------------------------------------------------------------------
{
  fprintf (pfile, "     YIELD      HARDENING    HARDENING\n");
  fprintf (pfile, "    STRESS      PARAMETER    EXPONENT\n");
  fprintf (pfile, "  %8.5E  %8.5E  %8.5E\n", A, B, n);
  fprintf (pfile, "\n");
}

//-----------------------------------------------------------------------------
String ElastoplasticLaw::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str="";

  sprintf(sstr,"%s.setYieldStress(%12.7E);\n",name.chars(),A);
  str+=sstr;
  sprintf(sstr,"%s.setHardParameter(%12.7E);\n",name.chars(),B);
  str+=sstr;
  sprintf(sstr,"%s.setHardExponent(%12.7E);\n",name.chars(),n);
  str+=sstr;

  return str;
}
 */
