/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file Exception.h
  \brief Definition of the Exception class.

  This file defines all the Exception class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_Exception_h__
#define __dnlKernel_Exception_h__

#include <iostream>

/*!
  \brief Treatment of C++ exceptions
  \ingroup dnlKernel
*/
class Exception
{

protected:
  const char *file;
  unsigned int line;
  const char *function;
  const char *cond;
  const char *exc;

public:
  Exception();
  Exception(const char *f, const int l, const char *func, const char *c, const char *e);
  virtual ~Exception() {}

  void SetInfo(const char *f, const int l, const char *func, const char *c, const char *e);
  virtual void print();
};

#define DeclException(exc, mess)                                                                                     \
  class exc : public Exception                                                                                       \
  {                                                                                                                  \
  public:                                                                                                            \
    exc(const char *f, const int l, const char *func, const char *c, const char *e) : Exception(f, l, func, c, e){}; \
    virtual void print()                                                                                             \
    {                                                                                                                \
      std::cerr << "--- " << mess << " ---\n";                                                                       \
      Exception::print();                                                                                            \
    }                                                                                                                \
  };

#define CreateException(exc, message) StandardExceptions::exc(__FILE__, __LINE__, __PRETTY_FUNCTION__, #message, #exc)

namespace StandardExceptions
{
  DeclException(OutOfMemory, "Out of memory");
  DeclException(NullPointer, "Pointer is NULL");
  DeclException(NotImplemented, "This feature is not implemented yet");
  DeclException(CheckFailed, "The requested condition has failed");
  DeclException(BadValue, "The requested value is bad");
  DeclException(Bug, "You seems to have found a BUG in DynELA");
  DeclException(FileNotFound, "The requested file was not found");
  DeclException(FileNotOpen, "The requested file cannot be oppened");
}; // namespace StandardExceptions

#define ThrowException(exc, message) throw exc(__FILE__, __LINE__, __PRETTY_FUNCTION__, #message, #exc)
#define IfException(test, exc) \
  if (test)                    \
  throw exc(__FILE__, __LINE__, __PRETTY_FUNCTION__, #test, #exc)

#ifdef DEBUG
#define Assert(condition)                                            \
  {                                                                  \
    if (!(condition))                                                \
      cerr << "Assertion failed in execution\nin line <" << __LINE__ \
           << "> of file <" << __FILE__                              \
           << "> in method\n"                                        \
           << "    " << __PRETTY_FUNCTION__ << "\n"                  \
           << "The violated condition was: \n"                       \
           << "    " << #condition << "\n";                          \
    abort();                                                         \
  }
#else
#define Assert(condition) \
  {                       \
  }
#endif

#endif
