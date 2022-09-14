/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file ANN2SigLaw.C
  Definition file for the ANN2SigLaw class

  This file is the definition file for the ANN2SigLaw class.

  \ingroup dnlMaterials
*/

#include <ANN2SigLaw.h>
#include <IntegrationPoint.h>
#include <Material.h>

//-----------------------------------------------------------------------------
ANN2SigLaw::ANN2SigLaw() : HardeningLaw()
//-----------------------------------------------------------------------------
{
  _lawType = ANN2Sig;
  _lawName = "ANN 2HL Sigmoid Law";
  _yieldLaw = true;
}

//-----------------------------------------------------------------------------
ANN2SigLaw::ANN2SigLaw(const ANN2SigLaw &law) : HardeningLaw(law)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ANN2SigLaw::~ANN2SigLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
int ANN2SigLaw::getNumberOfParameters()
//-----------------------------------------------------------------------------
{
  return 9;
}

//-----------------------------------------------------------------------------
const char *ANN2SigLaw::getParameterName(int parameter)
//-----------------------------------------------------------------------------
{
   String retour;
  switch (parameter)
  {
  case 0:
    return "w1";
    break;
  case 1:
    return "b1";
    break;
  case 2:
    return "w2";
    break;
  case 3:
    return "b2";
    break;
  case 4:
    return "w3";
    break;
  case 5:
    return "b3";
    break;
  case 6:
    return "logBase";
    break;
  case 7:
    return "minEntries";
    break;
  case 8:
    return "maxEntries";
    break;    
  default:
    fatalError("ANN2SigLaw::geConstitutiveparameterName",
               "called with parameter = %d", parameter);
  }
  return "";
}

//-----------------------------------------------------------------------------
double &ANN2SigLaw::getParameter(int parameter)
//-----------------------------------------------------------------------------
{
/*   switch (parameter)
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
    fatalError("ANN2SigLaw::geConstitutiveparameterName",
               "called with parameter = %d", parameter);
  }

  // pour tromper le compilo, mais ca sert a rien car il ne passe jamais ici
  return A; */
  return dummy;
}

//-----------------------------------------------------------------------------
void ANN2SigLaw::setParameters(char *filename)
//-----------------------------------------------------------------------------
{
  Matrix tmp;
  printf("%s\n",filename);

  tmp.numpyReadZ(filename, "w1");
  w1 = tmp.getTranspose();
  tmp.numpyReadZ(filename, "w2");
  w2 = tmp.getTranspose();
  tmp.numpyReadZ(filename, "w3");
  w3 = tmp.getTranspose();
  tmp.numpyReadZ(filename, "b1");
  b1 = tmp.getColumn(0);
  tmp.numpyReadZ(filename, "b2");
  b2 = tmp.getColumn(0);
  tmp.numpyReadZ(filename, "b3");
  b3 = tmp.getColumn(0);
  logBase.numpyReadZ(filename, "logBase");
  minEntries.numpyReadZ(filename, "minEntries");
  maxEntries.numpyReadZ(filename, "maxEntries");
  rangeEntries = maxEntries - minEntries;

// Check read information
/*     w1.printOut();
  w2.printOut();
  w3.printOut();
  b1.printOut();
  b2.printOut();
  b3.printOut(); 
  logBase.printOut();
  minEntries.printOut();
  maxEntries.printOut();  
  exit(0);   */
}

//-----------------------------------------------------------------------------
double ANN2SigLaw::getYieldStress(double _epsp, double _depsp, double _T, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  Vector inputData(3);

  inputData(0) = (_epsp - minEntries(0))/rangeEntries(0);
  if (_depsp > logBase(0)) {
    inputData(1) = (log(_depsp / logBase(0)) - minEntries(1)) / rangeEntries(1);
  }
  else {
    inputData(1) = 0.0;
    _depsp = logBase(0);
  }
  inputData(2) = (_T - minEntries(2)) / rangeEntries(2);

  Vector za = (-(w1 * inputData + b1)).EWExp();

  Vector zb = za.EWAddReal(1);
  
  Vector zc =( -((w2 * zb.EWInverse()) + b2)).EWExp();
  
  double y = (w3 * ((zc.EWAddReal(1)).EWInverse()))(0) + b3(0);

  double Yield = rangeEntries(3)*y + minEntries(3);

  // Compute and return the yield stress
  return Yield;
}

//-----------------------------------------------------------------------------
double ANN2SigLaw::getDerivateYieldStress(double _epsp, double _depsp, double _T, double _dtime, IntegrationPoint *intPoint)
//-----------------------------------------------------------------------------
{
  Vector inputData(3);

  inputData(0) = (_epsp - minEntries(0))/rangeEntries(0);
  if (_depsp > logBase(0)) {
    inputData(1) = (log(_depsp / logBase(0)) - minEntries(1)) / rangeEntries(1);
  }
  else {
    inputData(1) = 0.0;
    _depsp = logBase(0);
  }
  inputData(2) = (_T - minEntries(2)) / rangeEntries(2);

  Vector za = (-(w1 * inputData + b1)).EWExp();

  Vector zb = za.EWAddReal(1);
  
  Vector zc =( -((w2 * zb.EWInverse()) + b2)).EWExp();
  
  double y = (w3 * ((zc.EWAddReal(1)).EWInverse()))(0) + b3(0);

  Vector zd = zc.EWAddReal(1).EWSquare().EWInverse().EWMultiply(zc).EWMultiply(w3.getRow(0));

  Vector ze = zb.EWSquare().EWInverse().EWMultiply(za);

  Vector zf = (w2.getTranspose() * zd).EWMultiply(ze);

  Vector yd = w1.getTranspose() * zf;

  double Yield = rangeEntries(3)*y + minEntries(3);
  double dyieldDeqps1 = rangeEntries(3)/rangeEntries(0)*yd(0);
  double dyieldDeqps2 = rangeEntries(3)/rangeEntries(1)*yd(1) / _depsp;
  double dyieldDtemp = rangeEntries(3)/rangeEntries(2)*yd(2);
  double hard = dyieldDeqps1 + dyieldDeqps2 / _dtime + _material->taylorQuinney / (_material->density * _material->heatCapacity) * Yield * dyieldDtemp;

  return hard;
}
