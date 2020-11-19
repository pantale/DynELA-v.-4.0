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
  \file System.h
  \brief Definition of the System class and OS operators.

  This file defines all the OS operators in the System class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_System_h__
#define __dnlKernel_System_h__

#include <String.h>

/*!
  \brief Unix OS management

  This class contains a set of methods dedicated to the System working environment.
  These methods are used to retrieve system environment variables from a program, the current date, time and some definitions of directories and program paths, all in a System environment.
  This class also contains methods used to launch external applications.
  \ingroup dnlKernel
  \warning This class is related to the Unix system. If someone wants to port this program to a Windows environment, this may have to be changed.
*/
class System
{

public:
  System() {}  //!< Default constructor
  ~System() {} //!< Default destructor

  bool existEnvironmentValue(const String &envname);
  int execute(const String &cmd);
  long getHostId();
  String getDate(bool full = true);
  String getEnvironmentValue(const String &envname);
  String getExecutionPath();
  String getHostname();
  String getLogin();
  String getPathName();
  String getUnixTime();
};

bool argumentsLineParse(char *rr, int argc, char **argv);
char *argumentsLineGet(char *rr, int argc, char **argv);
long argumentsLinePosition(char *rr, int argc, char **argv);

#endif
