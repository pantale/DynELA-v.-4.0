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

/*
@LABEL:System::System
@SHORT:System class.
This class is used to store system information for \DynELA.
@END
*/
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

  bool existEnv(const String &);
  int execute(const String &);
  long hostID();
  String env(const String &);
  String execPath();
  String getDate(bool = true);
  String pathname();
  String getTime();
  String hostname();
  String login();
};

bool argumentsLineParse(char *, int, char **);
char *argumentsLineGet(char *, int, char **);
long argumentsLinePosition(char *, int, char **);

#endif
