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
  \file PolynomialFunction.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#include <Macros.h>
#include <PolynomialFunction.h>

//-----------------------------------------------------------------------------
PolynomialFunction::PolynomialFunction(char *newName) : Function(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
PolynomialFunction::~PolynomialFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double PolynomialFunction::getValue(double x)
//-----------------------------------------------------------------------------
{
  long i;
  double val;
  val = 0;
  for (i = 0; i < fragment.getSize(); i++)
  {
    val += fragment(i).value * pow(x, fragment(i).factor);
  }
  return val;
}

//-----------------------------------------------------------------------------
void PolynomialFunction::setFunction(long fac, double val)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < fragment.getSize(); i++)
  {
    if (fragment(i).factor == fac)
    {
      fragment(i).value = val;
    }
  }

  PolynomialFragment pol;
  pol.factor = fac;
  pol.value = val;
  fragment << pol;
}

//-----------------------------------------------------------------------------
double PolynomialFunction::get(long fac)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < fragment.getSize(); i++)
  {
    if (fragment(i).factor == fac)
    {
      return fragment(i).value;
    }
  }
  return 0.0;
}

//-----------------------------------------------------------------------------
double PolynomialFunction::getSlope(double x)
//-----------------------------------------------------------------------------
{
  long i;
  double val;
  val = 0;
  for (i = 0; i < fragment.getSize(); i++)
  {
    if (fragment(i).factor != 0)
      val += fragment(i).value * fragment(i).factor * pow(x, fragment(i).factor - 1);
  }
  return val;
}

//-----------------------------------------------------------------------------
long PolynomialFunction::getNumberOfFragments()
//-----------------------------------------------------------------------------
{
  return fragment.getSize();
}

//-----------------------------------------------------------------------------
double PolynomialFunction::getFactor(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).factor;
}

//-----------------------------------------------------------------------------
double PolynomialFunction::getCoefficient(long i)
//-----------------------------------------------------------------------------
{
  return fragment(i).value;
}

//-----------------------------------------------------------------------------
void PolynomialFunction::print()
//-----------------------------------------------------------------------------
{
  long i;
  printf("Polynomial Fonction : %s\n", name.chars());
  printf("f(x) = ");
  for (i = 0; i < fragment.getSize(); i++)
  {
    if (fragment(i).value < 0)
      printf(" - ");
    else if (i != 0)
      printf(" + ");
    printf("%lf*x^(%d)", dnlAbs(fragment(i).value), fragment(i).factor);
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
void PolynomialFunction::flush()
//-----------------------------------------------------------------------------
{
  fragment.flush();
}

//-----------------------------------------------------------------------------
void PolynomialFunction::toGnuplot(String filename, double xmin, double xmax, double steps)
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
String PolynomialFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "PolynomialFunction %s;\n", name.chars());
  str += sstr;

  for (long i = 0; i < getNumberOfFragments(); i++)
  {
    sprintf(sstr, "%s.set(%12.7E, %12.7E);\n", name.chars(), getFactor(i), getCoefficient(i));
    str += sstr;
  }
  return str;
}
