/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <System.h>
#include <Errors.h>
#include <Exception.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>

/*
@LABEL:System::execute(String s)
@SHORT:Execution of a system command
@ARG:String & s & System command to launch.
@RETURN:int : Status of the system command. A value of 0 is returned if everything works well, another value is returned if a problem was encountered during execution.
This method executes an external command. Launching a new process through the system() command.
The new program is a new process totally independent from the current application.
The only link is that the calling program waits for the end of the execution of the new process to continue the operations (except if we use the magic parameter \& in the command line...).
Do you know System?). As this command is simple, the working environment for the execution of the new process is /bin/sh. An error message is generated if a problem is encountered during execution.
For more complex situations, it will be necessary to use the "classic" fork() and exec() program launch methods. But this inevitably leads to threads problems and that's another story actually ;-0
@END
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

/*
@LABEL:System::env(String s)
@SHORT:Retrieve the value associated with an environment variable
@ARG:String & s & Environment variable.
@RETURN:String : Value associated with the environment variable in the form of String
This method retrieves the value associated with a System environment variable. If this variable is not defined, this method returns the following string "cannot get environment value". The returned value is of type String.
@END
*/
//-----------------------------------------------------------------------------
String System::env(const String &envname)
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

/*
@LABEL:System::existEnv(String s)
@SHORT:Tests for the presence of a defined environment variable
@ARG:String & s & Environment variable.
@RETURN:bool : true if the environment variable is set on the system, false if not.
This method tests the definition of an environment variable. It returns a boolean value that indicates the state of definition of this environment variable.
@END
*/
//-----------------------------------------------------------------------------
bool System::existEnv(const String &envname)
//-----------------------------------------------------------------------------
{
  char *c = std::getenv(envname.chars());

  // test if defined
  if (c != NULL)
    return true;

  // not defined
  return false;
}

/*
@LABEL:System::login()
@SHORT:Returns the user's login.
@RETURN:String : The user's login or "unknown user" if this information cannot be given.
This method returns the login of the system user in the form of a string.
@END
*/
//-----------------------------------------------------------------------------
String System::login()
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

/*
@LABEL:System::hostname()
@SHORT:Returns the machine name
@RETURN:String : Machine name or "unknown host" if this information cannot be given.
This method returns the name of the machine on which the application is running.
@END
*/
//-----------------------------------------------------------------------------
String System::hostname()
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

/*
@LABEL:System::getDate(bool b)
@ARGS:bool & b & Nature of the result returned
@SHORT:Returns the current date and time
@RETURN:String : Current date and time or "unknown date" if this information cannot be given.
This method returns the current date and time at the system level.
  - full this boolean value defines the nature of the result returned. If the value is true then the format is the full format of the form (Fri Jan 25 15:08:24 2002) if the value is false then the format returned is the short format of the form (Jan 25, 2002). The default value if nothing is accurate is true.
@END
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

/*
@LABEL:System::getTime()
@SHORT:Returns the current time
@RETURN:String : Vlue of the current time as a String (format: 14:23:26)
This method returns the current time to the system level.
@END
*/
//-----------------------------------------------------------------------------
String System::getTime()
//-----------------------------------------------------------------------------
{
  String s = getDate(true);

  // Time format is 14:23:26
  s.remove(0, 11);
  s = s.beforeItem(8);

  return (s);
}

/*
@LABEL:System::pathname()
@SHORT:Returns the name of the current directory
@RETURN:String : Current directory or "unknown pathname" if this information cannot be given.
This method returns the name of the current directory from which the execution was started.
@END
*/
//-----------------------------------------------------------------------------
String System::pathname()
//-----------------------------------------------------------------------------
{
  char pathname[2500];
  if (getcwd(pathname, 2500) == NULL)
  {
    fatalError("System::pathname()",
               "pathname()\nsomething went wrong when calling the system function getcwd\n");
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

/*
@LABEL:System::execPath()
@SHORT:Returns the execution path of the current application path
@RETURN:String : Current path or "unknown pathname" if this information cannot be given.
This method returns the name of the current application path from which the execution was started.
@END
*/
//-----------------------------------------------------------------------------
String System::execPath()
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

/*
@LABEL:System::hostID()
@SHORT:Returns the host back from the machine
@RETURN:String : hostId of the machine.
This method returns the host of the machine on which the program is executed. The host is an integer value usually given in hexadecimal form 0xFFFFFFFFFFFFFF on 32 bits. This number is unique per machine.
@END
*/
//-----------------------------------------------------------------------------
long System::hostID()
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
