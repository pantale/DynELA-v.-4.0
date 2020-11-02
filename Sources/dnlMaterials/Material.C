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
  \file Material.C
  \brief Definition file for the Material class

  This file is the definition file for the Material class.

  \ingroup dnlMaterials
*/

#include <Material.h>
#include <HardeningLaw.h>

//-----------------------------------------------------------------------------
Material::Material(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
}

//-----------------------------------------------------------------------------
Material::Material(const Material &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Material::~Material()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool Material::checkValues()
//-----------------------------------------------------------------------------
{
  // check poisson value
  if ((poissonRatio < 0.) || (poissonRatio > 0.5))
    fatalError("Materials integrity check",
               "material %s have a poisson value of %lf\n", name.chars(),
               poissonRatio);

  // check Young modulus
  if (youngModulus <= 0.)
    fatalError("Materials integrity check",
               "material %s have a young value of %lf\n", name.chars(), youngModulus);

  // check density
  if (density <= 0.)
    fatalError("Materials integrity check",
               "material %s have a density value of %lf\n", name.chars(),
               density);

  return true;
}

//-----------------------------------------------------------------------------
void Material::computeHookeTensor()
//-----------------------------------------------------------------------------
{
  long i, j, k, l;
  double mu, lambda;

  lambda = getLambda();   // poissonRatio * youngModulus / ((1. - 2. * poissonRatio) * (1. + poissonRatio));
  mu = getShearModulus(); // youngModulus / (2. * (1. + poissonRatio));

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        for (l = 0; l < 3; l++)
        {
          _hookeTensor(i, j, k, l) = lambda * (dnlKronecker(i, j) * dnlKronecker(k, l)) + mu * (dnlKronecker(i, k) * dnlKronecker(j, l) + dnlKronecker(i, l) * dnlKronecker(j, k));
        }
}

//-----------------------------------------------------------------------------
void Material::setColor(double red, double green, double blue)
//-----------------------------------------------------------------------------
{
  _color(0) = red;
  _color(1) = green;
  _color(2) = blue;
}

//-----------------------------------------------------------------------------
Matrix Material::getHookeStiffnessMatrix(short type)
//-----------------------------------------------------------------------------
{
  Matrix matrix;

  switch (type)
  {
  case planeStrain:
    matrix.redim(3, 3);
    matrix = 0;
    matrix(0, 0) = matrix(1, 1) = 1 - poissonRatio;
    matrix(2, 2) = 1 - 2 * poissonRatio;
    //matrix(2, 2) = (1 - 2 * poissonRatio) / 2;
    matrix(1, 0) = matrix(0, 1) = poissonRatio;
    matrix *= youngModulus / ((1 + poissonRatio) * (1 - 2 * poissonRatio));
    break;

  case planeStress:
    matrix.redim(3, 3);
    matrix = 0;
    matrix(0, 0) = 1;
    matrix(1, 1) = 1;
    matrix(2, 2) = 1 - poissonRatio;
    //matrix(2, 2) = (1 - poissonRatio) / 2;
    matrix(1, 0) = matrix(0, 1) = poissonRatio;
    matrix *= youngModulus / (1 - dnlSquare(poissonRatio));
    break;

  case axisymetric:
    matrix.redim(4, 4);
    matrix = 0;
    matrix(0, 0) = matrix(1, 1) = matrix(3, 3) = 1 - poissonRatio;
    matrix(2, 2) = 1 - 2 * poissonRatio;
    //matrix(2, 2) = (1 - 2 * poissonRatio) / 2;
    matrix(1, 0) = matrix(0, 1) = matrix(3, 0) = matrix(0, 3) = matrix(3, 1) = matrix(1, 3) = poissonRatio;
    matrix *= youngModulus / ((1 + poissonRatio) * (1 - 2 * poissonRatio));
    break;

  case threedimensional:
    matrix.redim(6, 6);
    matrix = 0;
    matrix(0, 0) = matrix(1, 1) = matrix(2, 2) = 1 - poissonRatio;
    matrix(3, 3) = matrix(4, 4) = matrix(5, 5) = 1 - 2 * poissonRatio;
    //matrix(3, 3) = (1 - 2 * poissonRatio) / 2;
    //matrix(4, 4) = (1 - 2 * poissonRatio) / 2;
    //matrix(5, 5) = (1 - 2 * poissonRatio) / 2;
    matrix(0, 1) = matrix(1, 0) = matrix(0, 2) = matrix(2, 0) = matrix(1, 2) = matrix(2, 1) = poissonRatio;
    matrix *= youngModulus / ((1 + poissonRatio) * (1 - 2 * poissonRatio));
    break;

  default:
    fatalError("Material::getHookeStiffnessMatrix", "Matrix not computed for this case");
  }
  return matrix;
}

//-----------------------------------------------------------------------------
void Material::setHardeningLaw(HardeningLaw *hardeningLaw)
//-----------------------------------------------------------------------------
{
  _hardening = hardeningLaw;
  hardeningLaw->add(this);
}

//-----------------------------------------------------------------------------
HardeningLaw *Material::getHardeningLaw() const
//-----------------------------------------------------------------------------
{
  return _hardening;
}

//-----------------------------------------------------------------------------
bool Material::isYield()
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->isYield());
  return false;
}

/*
//-----------------------------------------------------------------------------
void Material::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "material: " << name << endl;
  os << "youngModulus= " << youngModulus << endl;
  os << "nu=" << poissonRatio << endl;
  os << "density=" << density << endl;
  os << "heatCapacity=" << heatCapacity << endl;
  os << "initialTemperature=" << initialTemperature << endl;

  // complement d'ecriture de la partie ecrouissage isotrope
  if (_hardening != NULL)
    _hardening->print(os);
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Material &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void Material::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // fprintf(pfile,"MATERIAL %4d\n",);
  fprintf(pfile, "MATERIAL %s\n", name.chars());
  fprintf(pfile, "    YOUNG'S      POISSON'S    DENSITY\n");
  fprintf(pfile, "    MODULUS        RATIO\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E\n\n", youngModulus, poissonRatio, density);
  fprintf(pfile, "    INITIAL       HEAT      DILATATION     THERMAL\n");
  fprintf(pfile, "  TEMPERATURE   CAPACITY       COEFF     CONDUCTIVITY\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E  %8.5E\n\n", initialTemperature, heatCapacity, dilatation, thermalConductivity);

  // complement d'ecriture de la partie ecrouissage isotrope
  if (_hardening != NULL)
    _hardening->toFile(pfile);
}

//-----------------------------------------------------------------------------
String Material::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setHeatCoefficient(%12.7E);\n", name.chars(), heatCoeff());
  str += sstr;
  sprintf(sstr, "%s.setDilatation(%12.7E);\n", name.chars(), dilat());
  str += sstr;
  sprintf(sstr, "%s.setInitTemperature(%12.7E);\n", name.chars(), initTemp());
  str += sstr;
  sprintf(sstr, "%s.setConductivity(%12.7E);\n", name.chars(), conductivity());
  str += sstr;
  sprintf(sstr, "%s.setYoung(%12.7E);\n", name.chars(), young());
  str += sstr;
  sprintf(sstr, "%s.setPoisson(%12.7E);\n", name.chars(), nu());
  str += sstr;
  sprintf(sstr, "%s.setDensity(%12.7E);\n", name.chars(), density());
  str += sstr;
  sprintf(sstr, "%s.setColor(%12.7E, %12.7E, %12.7E);\n", name.chars(), _color(0), _color(1), _color(2));
  str += sstr;

  // attach the isotropic hardening part to the material if exist
  if (_hardening != NULL)
  {
    sprintf(sstr, "%s.setHardening(%s);\n", name.chars(), _hardening->name.chars());
    str += sstr;
  }

  return str;
}



//-----------------------------------------------------------------------------
double Material::getYieldStress(IntegrationPoint *pfield)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getYieldStress(pfield));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getDerYieldStress(IntegrationPoint *pfield)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getDerYieldStress(pfield));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getIsotropicYieldStress(Element *el, double shift)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getIsotropicYieldStress(el, shift));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getIsotropicYieldHardening(Element *el, double shift)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getIsotropicYieldHardening(el, shift));
  return 0;
}
*/
