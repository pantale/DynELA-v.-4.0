/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file Exception.C
  Definition of the Exception class.

  This file defines all the Exception class.
  \ingroup dnlKernel
*/

#include <Exception.h>

//-----------------------------------------------------------------------------
Exception::Exception() : file(""), line(0), function(""), cond(""), exc("")
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Exception::Exception(const char *f, const int l, const char *func, const char *c, const char *e)
    : file(f), line(l), function(func), cond(c), exc(e)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Exception::SetInfo(const char *f, const int l, const char *func, const char *c, const char *e)
//-----------------------------------------------------------------------------
{
  file = f;
  line = l;
  function = func;
  cond = c;
  exc = e;
}

//-----------------------------------------------------------------------------
void Exception::print()
//-----------------------------------------------------------------------------
{
  std::cerr << "The exception: " << exc << "\noccurred in line <" << line
            << "> of file <" << file
            << "> in method:" << std::endl
            << "    " << function << std::endl
            << "The violated condition was: " << std::endl
            << "    " << cond << std::endl;
}
