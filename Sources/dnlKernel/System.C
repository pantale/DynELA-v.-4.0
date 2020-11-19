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

/*!
  \file System.C
  \brief System working environment

  This class contains a set of methods dedicated to the System working environment.
  \ingroup dnlKernel
*/

#include <System.h>
#include <Errors.h>
#include <Exception.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>

/*!
  \brief Execution of a system command

  This method executes an external command. Launching a new process through the \b system() command.
  The new program is a new process totally independent from the current application.
  The only link is that the calling program waits for the end of the execution of the new process to continue the operations (except if we use the magic parameter & in the command line...).
   Do you know System?). As this command is simple, the working environment for the execution of the new process is /bin/sh. An error message is generated if a problem is encountered during execution.
  For more complex situations, it will be necessary to use the "classic" fork() and exec() program launch methods. But this inevitably leads to threads problems and that's another story actually ;-0

  \param commandToExecute string of type \b String defining the system command to run
  \return value indicating whether the command execution ran smoothly. A value of 0 is returned if everything works well, another value is returned if a problem was encountered during execution.
*/
//-----------------------------------------------------------------------------
int System::execute(const String &commandToExecute)
//-----------------------------------------------------------------------------
{
  int status = system(commandToExecute.chars());

  if (status != 0)
  {
    if (status == 127)
    {
      std::cerr << "/bin/sh could not be executed\n";
    }
    std::cerr << "System::execute()\n";
    std::cerr << "Last System command execution:\n";
    std::cerr << commandToExecute.chars() << "\nfailed (return value=" << status << ")\n";
  }

  return status;
}

/*!
  \brief retrieve the value associated with an environment variable

  This method retrieves the value associated with a System environment variable. If this variable is not defined, this method returns the following string "cannot get environment value". The returned value is of type String.
  \param envname environment variable name
  \return value associated with the environment variable in the form of String
*/
//-----------------------------------------------------------------------------
String System::getEnvironmentValue(const String &envname)
//-----------------------------------------------------------------------------
{
  String s;
  char *c = std::getenv(envname.chars());

  // if variable is defined
  if (c != NULL)
  {
    s = c;
    s.strip();
  }

  // else if not defined
  else
    s = "cannot get " + envname + " value";

  // return the string
  return (s);
}

/*!
  \brief tests for the presence of a defined environment variable

  This method tests the definition of an environment variable. It returns a boolean value that indicates the state of definition of this environment variable.
  \param envname name of the environment variable
  \return  \c true if the environment variable is set on the system, \c false if not.
*/
//-----------------------------------------------------------------------------
bool System::existEnvironmentValue(const String &envname)
//-----------------------------------------------------------------------------
{
  char *c = std::getenv(envname.chars());

  // test if defined
  if (c != NULL)
    return true;

  // not defined
  return false;
}

/*!
  \brief returns the user's login

  This method returns the login of the system user in the form of a string.
  \return the user's login or "unknown user" if this information cannot be given.
*/
//-----------------------------------------------------------------------------
String System::getLogin()
//-----------------------------------------------------------------------------
{
  struct passwd *pwd;
  try
  {
    pwd = getpwuid(getuid());
    IfException(pwd == NULL, StandardExceptions::NullPointer);
  }
  catch (Exception &e)
  {
    e.print();
    return "";
  }

  return (pwd->pw_name);
}

/*!
  \brief returns the machine name

  This method returns the name of the machine on which the application is running.
  \return machine name or "unknown host" if this information cannot be given.
*/
//-----------------------------------------------------------------------------
String System::getHostname()
//-----------------------------------------------------------------------------
{
  String s;
  char hostname[80];

  try
  {
    int ret = gethostname(hostname, 80);
    IfException(ret != 0, StandardExceptions::NullPointer);
    s = hostname;
    s.strip();
  }
  catch (Exception &e)
  {
    s = "";
  }

  return (s);
}

/*!
  \brief returns the current date and time

  This method returns the current date and time at the system level.
  \param full this boolean value defines the nature of the result returned. If the value is true then the format is the full format of the form (Fri Jan 25 15:08:24 2002) if the value is false then the format returned is the short format of the form (Jan 25, 2002). The default value if nothing is accurate is true.
  \return current date and time or "unknown date" if this information cannot be given.
*/
//-----------------------------------------------------------------------------
String System::getDate(bool full)
//-----------------------------------------------------------------------------
{
  char *thedate;

  time_t theclock;

  theclock = time(0);
  thedate = (char *)ctime(&theclock);

  String s;
  if (thedate != NULL)
  {
    // Full format: Fri Aug 20 14:23:26 1993
    s = thedate;
    s.strip();

    // Short format: Aug 20, 1993
    if (!full)
    {
      s.remove(0, 4); // Kill day + space, e.g. Fri
      int i = 5;
      while (s[i] != ' ')
        ++i;
      s.remove(i + 1, 8); // Kill time and year and spaces, e.g. 13:00:00 1993
      s[6] = ',';
    }
  }

  else
    s = "unknown date";

  return (s);
}

/*!
  \brief returns the current time

  This method returns the current time to the system level.
  \return value of the current time as a String (format: 14:23:26)
*/
//-----------------------------------------------------------------------------
String System::getUnixTime()
//-----------------------------------------------------------------------------
{
  String s = getDate(true);

  // Time format is 14:23:26
  s.remove(0, 11);
  s = s.beforeItem(8);

  return (s);
}

//
/*!
  \brief returns the name of the current directory

  This method returns the name of the current directory from which the execution was started.
  \return current directory or "unknown pathname" if this information cannot be given.
*/
//-----------------------------------------------------------------------------
String System::getPathName()
//-----------------------------------------------------------------------------
{
  char pathname[2500];
  if (getcwd(pathname, 2500) == NULL)
  {
    fatalError("System::getPathName()",
               "getPathName()\nsomething went wrong when calling the system function getcwd\n");
  }

  String s;
  if (strlen(pathname) > 0)
  {
    s = pathname;
    s.strip();
  }

  else
    s = "unknown pathname";

  return (s);
}

//-----------------------------------------------------------------------------
String System::getExecutionPath()
//-----------------------------------------------------------------------------
{
  int bufsize = 1024;
  char buf[bufsize];
  ssize_t count = readlink("/proc/self/exe", buf, bufsize);
  const char *path;
  if (count != -1)
  {
    path = dirname(buf);
  }
  return path;
}

/*!
  \brief sends the host back from the machine

  This method returns the host of the machine on which the program is executed. The host is an integer value usually given in hexadecimal form 0xFFFFFFFFFFFFFF on 32 bits. This number is unique per machine.
  \return hostId of the machine.
*/
//-----------------------------------------------------------------------------
long System::getHostId()
//-----------------------------------------------------------------------------
{
  return gethostid();
}

//-----------------------------------------------------------------------------
bool argumentsLineParse(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {
    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // retour
      return true;
    }
  }

  // retour
  return false;
}

//-----------------------------------------------------------------------------
long argumentsLinePosition(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {
    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // retour
      return i;
    }
  }

  // retour
  return 0;
}

//-----------------------------------------------------------------------------
char *argumentsLineGet(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {
    // test d'argument
    if (strcmp(rr, argv[i] + 1) == 0)
    {
      // retour
      return argv[i + 1];
    }
  }

  // retour
  return NULL;
}
