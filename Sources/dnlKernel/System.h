/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlKernel_System_h__
#define __dnlKernel_System_h__

#include <String.h>

//-----------------------------------------------------------------------------
// Class : System
//
// Used to manage System
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class System
{

public:
  System() {}  // Default constructor
  ~System() {} // Default destructor

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

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
