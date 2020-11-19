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
  \file LogFile.h
  \brief Declaration of the LogFile class.

  This file declares the LogFile class for DynELA logs.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_LogFile_h__
#define __dnlKernel_LogFile_h__

#include <iostream>
#include <fstream>
#include <String.h>

/*!
  \brief creation and management of Log files.

  This class is used to create and manage Log files for the DynELA Finite Element code.
  \ingroup dnlKernel
*/
class LogFile
{
  String _fileName;      //!< Filename for the log file
  std::ofstream _stream; //!< Output stream for the file
  int _verbosity;        //!< Level of verbosity
  int _level;            //!< Current leve of verbosity

public:
  LogFile();
  ~LogFile();

#ifndef SWIG
  friend LogFile &operator<<(LogFile &logFile, const String &string);
  friend LogFile &operator<<(LogFile &logFile, const int &value);
  friend LogFile &operator<<(LogFile &logFile, const short &value);
  friend LogFile &operator<<(LogFile &logFile, const long &value);
  friend LogFile &operator<<(LogFile &logFile, const double &value);
#endif

  void init(String logFileName, int verbosity = 1);
  void close();
  void downLevel();
  void headerWrite();
  void outputString(String string);
  void separatorWrite(String separator = "");
  void setLevel(int newlevel);
  void setVerbosity(int verbosity) { _verbosity = verbosity; } //!< Sets the level of verbosity
  void upLevel();
};

#endif
