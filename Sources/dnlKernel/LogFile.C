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

/*
  \file LogFile.C
  Definition of the LogFile class.

  This file defines the LogFile class for DynELA logs.
  \ingroup dnlKernel
*/

#include <LogFile.h>
#include <System.h>
#include <Errors.h>

/*
  Default constructor of the LogFile class.

  The constructor opens the create file and automatically adds a standard header at the top of it (see headerWrite() method).
*/
//-----------------------------------------------------------------------------
LogFile::LogFile()
//-----------------------------------------------------------------------------
{
  _verbosity = 1;
  _level = 1;
}

/*
  Default destructor of the Field class.

  The destructor closes the stream.
*/
//-----------------------------------------------------------------------------
LogFile::~LogFile()
//-----------------------------------------------------------------------------
{
  _stream.close();
}

/*
  Init the log file.
  - logFileName name of the logfile
  - verbosity requested level of verbosity
*/
//-----------------------------------------------------------------------------
void LogFile::init(String logFileName, int verbosity)
//-----------------------------------------------------------------------------
{
  if (logFileName == "")
  {
    fatalError("LogFile::LogFile", "Must specify a log filename in the constructor");
  }

  // put the name
  _fileName = logFileName;

  // open the stream
  _stream.open(_fileName.chars(), std::fstream::out);

  if (!_stream.is_open())
  {
    fatalError("LogFile::LogFile", "Cannot open _stream for file %s", _fileName.chars());
  }

  // Header write
  headerWrite();

  _verbosity = verbosity;
  _level = 1;
}

/*
  Closes the log file.
*/
//-----------------------------------------------------------------------------
void LogFile::close()
//-----------------------------------------------------------------------------
{
  // close the stream
  _stream.close();
}

/*
  Writes the header of the log file
*/
//-----------------------------------------------------------------------------
void LogFile::headerWrite()
//-----------------------------------------------------------------------------
{
  System os;

  // write the header of the file
  // _stream << "DynELA v. " << VERSION << "." << RELEASE << "." << UNDER_RELEASE << "-" << ALPHA_VERSION << std::endl;
  _stream << os.getDate() << std::endl;
  _stream << "Job created by user " << os.getEnvironmentValue("LOGNAME") << std::endl;
  //_stream << "Job executed on a " << os.getCurrHosttype() << " system\n";
  _stream << "Name of the host " << os.getHostname() << std::endl;
  _stream << "Host ID " << os.getHostId() << std::endl;
  _stream << std::endl
          << std::endl;

  separatorWrite();
}

/*
  Writes a separator to the log file
*/
//-----------------------------------------------------------------------------
void LogFile::separatorWrite(String separator)
//-----------------------------------------------------------------------------
{
  _stream << "\n//-----------------------------------------------------------------------------\n";
  _stream << "// " << separator << "\n";
  _stream << "//-----------------------------------------------------------------------------\n\n";

  // force writing information
  _stream.flush();
}

/*
  Writes a string to the log file
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const String &string)
//-----------------------------------------------------------------------------
{
  // out the string
  logFile.outputString(string);

  // continue the buffer
  return logFile;
}

/*
  Writes a long to the log file
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const long &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

/*
  Writes a short to the log file
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const short &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

/*
  Writes an int to the log file
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const int &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

/*
  Writes a double to the log file
*/
//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const double &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

/*
  Writes a string to the log file
*/
//-----------------------------------------------------------------------------
void LogFile::outputString(String str)
//-----------------------------------------------------------------------------
{
  if (_level <= _verbosity)
  {
    // write the string
    _stream << str.chars();

    // force writing information
    _stream.flush();
  }
}

/*
  Sets the output level setting for future messages

  This method allows to specify that the next messages sent will be at the level given in argument. All messages of a higher level than this one will be ignored.
  - newlevel new level.
*/
//-----------------------------------------------------------------------------
void LogFile::setLevel(int newlevel)
//-----------------------------------------------------------------------------
{
  _level = newlevel;
}

/*
  Increase the output level setting for future messages

  This method allows to specify that the next messages sent will be at the level given in argument. All messages of a higher level than this one will be ignored.
*/
//-----------------------------------------------------------------------------
void LogFile::upLevel()
//-----------------------------------------------------------------------------
{
  _level++;
}

/*
  Decreases the output level setting for future messages

  This method allows to specify that the next messages sent will be at the level given in argument. All messages of a higher level than this one will be ignored.
*/
//-----------------------------------------------------------------------------
void LogFile::downLevel()
//-----------------------------------------------------------------------------
{
  _level--;
  if (_level <= 1)
    _level = 1;
}
