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
  \file PolynomialFunction.h
  Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#include <Macros.h>
#include <SinusFunction.h>

// constructeur par defaut de la classe SinusFunction
/*
 */
//-----------------------------------------------------------------------------
SinusFunction::SinusFunction(char *newName) : Function(newName)
//-----------------------------------------------------------------------------
{
  constant = 0.0;
}

//-----------------------------------------------------------------------------
SinusFunction::~SinusFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double SinusFunction::getValue(double x)
//-----------------------------------------------------------------------------
{
  double value = constant;

  for (long i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).type == Sin)
      value += fragment(i).factor * sin(fragment(i).pulse * x + fragment(i).constant);
    if (fragment(i).type == Cos)
      value += fragment(i).factor * cos(fragment(i).pulse * x + fragment(i).constant);
  }

  return value;
}

//-----------------------------------------------------------------------------
void SinusFunction::setSin(double coef, double pulse, double constant)
//-----------------------------------------------------------------------------
{
  SinusFragment fr;
  fr.type = Sin;
  fr.factor = coef;
  fr.pulse = pulse;
  fr.constant = constant;
  fragment << fr;
}

//-----------------------------------------------------------------------------
void SinusFunction::setCos(double coef, double pulse, double constant)
//-----------------------------------------------------------------------------
{
  SinusFragment fr;
  fr.type = Cos;
  fr.factor = coef;
  fr.pulse = pulse;
  fr.constant = constant;
  fragment << fr;
}

//-----------------------------------------------------------------------------
void SinusFunction::setConstant(double _constant)
//-----------------------------------------------------------------------------
{
  constant = _constant;
}

//-----------------------------------------------------------------------------
double SinusFunction::getSlope(double x)
//-----------------------------------------------------------------------------
{
  double value = 0;

  for (long i = 0; i < fragment.size(); i++)
  {
    if (fragment(i).type == Sin)
      value += fragment(i).pulse * fragment(i).factor * cos(fragment(i).pulse * x + fragment(i).constant);
    if (fragment(i).type == Cos)
      value -= fragment(i).pulse * fragment(i).factor * sin(fragment(i).pulse * x + fragment(i).constant);
  }

  return value;
}

//-----------------------------------------------------------------------------
void SinusFunction::print()
//-----------------------------------------------------------------------------
{
  printf("Sinus Fonction : %s\nf(x) = ", name.chars());
  if (constant != 0)
    printf("%lf ", constant);
  for (long i = 0; i < fragment.size(); i++)
  {
    if ((fragment(i).factor >= 0) && ((i != 0) || (constant != 0)))
      printf("+ ");
    if (fragment(i).factor < 0)
      printf("- ");
    if (fragment(i).type == Sin)
    {
      printf("%lf sin (%lf x", dnlAbs(fragment(i).factor), fragment(i).pulse);
      if (fragment(i).constant != 0)
        printf(" %c %lf", (fragment(i).constant >= 0 ? '+' : '-'), dnlAbs(fragment(i).constant));
      printf(") ");
    }
    if (fragment(i).type == Cos)
    {
      printf("%lf cos (%lf x", dnlAbs(fragment(i).factor), fragment(i).pulse);
      if (fragment(i).constant != 0)
        printf(" %c %lf", (fragment(i).constant >= 0 ? '+' : '-'), dnlAbs(fragment(i).constant));
      printf(") ");
    }
  }

  printf("\n");
}

//-----------------------------------------------------------------------------
void SinusFunction::toGnuplot(String filename, double xmin, double xmax, double steps)
//-----------------------------------------------------------------------------
{
  FILE *pfile;

  pfile = fopen(filename.chars(), "w");
  if (pfile == NULL)
    fatalError("open Gnuplot file", "failed to open file for write");
  fprintf(pfile, "#DynELA_plot :%s\n", name.chars());
  fprintf(pfile, "#plotted :%s\n", name.chars());

  for (double x = xmin; x <= xmax; x += steps)
    fprintf(pfile, "%10.6E %10.6E\n", x, getValue(x));
  fclose(pfile);
}

//-----------------------------------------------------------------------------
short SinusFunction::getType(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).type;
}

//-----------------------------------------------------------------------------
double SinusFunction::getPulse(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).pulse;
}

//-----------------------------------------------------------------------------
double SinusFunction::getCoefficient(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).factor;
}

//-----------------------------------------------------------------------------
double SinusFunction::getConstant(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).constant;
}

//-----------------------------------------------------------------------------
double SinusFunction::getConstant()
//-----------------------------------------------------------------------------
{
  return constant;
}

//-----------------------------------------------------------------------------
long SinusFunction::getNumberOfFragments()
//-----------------------------------------------------------------------------
{
  return fragment.size();
}

//-----------------------------------------------------------------------------
String SinusFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "SinusFunction %s;\n", name.chars());
  str += sstr;

  if (constant != 0.0)
    sprintf(sstr, "%s.setConstant(%12.7E);\n", name.chars(), constant);
  str += sstr;

  for (long i = 0; i < getNumberOfFragments(); i++)
  {
    if (getType(i) == SinusFunction::Cos)
      sprintf(sstr, "%s.setCos(%12.7E, %12.7E, %12.7E);\n", name.chars(), getCoefficient(i), getPulse(i), getConstant(i));
    if (getType(i) == SinusFunction::Sin)
      sprintf(sstr, "%s.setSin(%12.7E, %12.7E, %12.7E);\n", name.chars(), getCoefficient(i), getPulse(i), getConstant(i));
    str += sstr;
  }

  return str;
}
