/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
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
#include <Errors.h>
#include <RampFunction.h>

//-----------------------------------------------------------------------------
RampFunction::RampFunction(char *newName) : Function(newName)
//-----------------------------------------------------------------------------
{
  _type = Unknown;
}

//-----------------------------------------------------------------------------
RampFunction::~RampFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void RampFunction::setFunction(short newType, double newMin, double newMax)
//-----------------------------------------------------------------------------
{
  _min = newMin;
  _max = newMax;
  double mm = _max - _min;

  _type = newType;

  if (_type == Linear)
  {
    // equation y = a0 x + a1
    _arg[0] = -2.0 / mm;
    _arg[1] = 2.0 / mm;
    return;
  }
  if (_type == Sinus)
  {
    // equation y = a0*(1-sin(a1 x));
    _arg[1] = PI / 2;
    _arg[0] = 1.0 / (1.0 - 2.0 / PI) / mm;
    return;
  }
  if (_type == Constant)
  {
    // equation y = a0;
    _arg[0] = 1.0 / mm;
    return;
  }
  if (_type == Erf)
  {
    // equation y = exp(-(a0*x)^2) / a1;
    _arg[0] = 5;
    _arg[1] = sqrt(PI) * mm / 10;
    return;
  }
  if (_type == Soft)
  {
    // equation y = cos(PI * x)+1;
    _arg[0] = 1 / mm;
    return;
  }

  fatalError("RampFunction::set", "%d _type is unknown for ramp functions", newType);
}

//-----------------------------------------------------------------------------
double RampFunction::getValue(double x)
//-----------------------------------------------------------------------------
{
  double value = 0;

  if (_type == Unknown)
    return 0;

  if ((x < _min) || (x > _max))
    return 0;

  // map function in range [0-1]
  double point = (x - _min) / (_max - _min);

  switch (_type)
  {
  case Linear:
  {
    return _arg[0] * point + _arg[1];
  }
  break;
  case Sinus:
  {
    return _arg[0] * (1.0 - sin(_arg[1] * point));
  }
  break;
  case Constant:
  {
    return _arg[0];
  }
  break;
  case Erf:
  {
    return exp(-dnlSquare(_arg[0] * point)) / _arg[1];
  }
  break;
  case Soft:
  {
    return _arg[0] * (PI * sin(PI * point) / 2);
  }
  break;
  default:
    internalFatalError("RampFunction::getValue", "Unknown function %d\n", _type);
  }

  return value;
}

//-----------------------------------------------------------------------------
double RampFunction::getSlope(double x)
//-----------------------------------------------------------------------------
{
  double value = 0;

  if (_type == Unknown)
    return 0;

  if ((x < _min) || (x > _max))
    return 0;

  // map function in range [0-1]
  double point = (x - _min) / (_max - _min);

  switch (_type)
  {
  case Linear:
    return _arg[0];
    break;
  case Sinus:
    return -_arg[0] * _arg[1] * cos(_arg[0] * point);
    break;
  case Erf:
    return -(2 * point * dnlSquare(_arg[0]) * exp(-dnlSquare(point * _arg[0]))) / _arg[1];
    break;
  case Constant:
    return 0;
    break;
  case Soft:
  {
    return _arg[0] * dnlSquare(PI) * cos(PI * point) / 2;
  }
  break;
  default:
    internalFatalError("RampFunction::getValue", "Unknown function %d\n", _type);
  }

  return value;
}

//-----------------------------------------------------------------------------
void RampFunction::print()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void RampFunction::toGnuplot(String filename, long steps)
//-----------------------------------------------------------------------------
{
  FILE *pfile;

  pfile = fopen(filename.chars(), "w");
  if (pfile == NULL)
    fatalError("open Gnuplot file", "failed to open file for write");
  fprintf(pfile, "#DynELA_plot :%s\n", name.chars());
  fprintf(pfile, "#plotted :%s\n", name.chars());

  long st = 0;
  double x, dx;
  double xmin, xmax;
  xmin = _min - (_max - _min) / 2;
  xmax = _max + (_max - _min) / 2;
  x = xmin;
  dx = (xmax - xmin) / steps;
  for (st = 0; st <= steps; st++)
  {
    fprintf(pfile, "%10.6E %10.6E\n", x, getValue(x));
    x += dx;
  }
  fclose(pfile);
}

//-----------------------------------------------------------------------------
short RampFunction::getType(long i)
//-----------------------------------------------------------------------------
{
  return _type;
}

//-----------------------------------------------------------------------------
String RampFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
  String str = "";
  char sstr[1000];

  sprintf(sstr, "RampFunction %s;\n", name.chars());
  str += sstr;

  switch (_type)
  {
  case Linear:
    sprintf(sstr, "%s.set(\"Linear\", %12.7E, %12.7E);\n", name.chars(), _min, _max);
    break;
  case Sinus:
    sprintf(sstr, "%s.set(\"Sinus\", %12.7E, %12.7E);\n", name.chars(), _min, _max);
    break;
  case Constant:
    sprintf(sstr, "%s.set(\"Constant\", %12.7E, %12.7E);\n", name.chars(), _min, _max);
    break;
  case Erf:
    sprintf(sstr, "%s.set(\"Erf\", %12.7E, %12.7E);\n", name.chars(), _min, _max);
    break;
  case Soft:
    sprintf(sstr, "%s.set(\"Soft\", %12.7E, %12.7E);\n", name.chars(), _min, _max);
    break;
  default:
    fatalError("RampFunction::outDyn", " case not implemented\n");
  }
  str += sstr;

  return str;
}
