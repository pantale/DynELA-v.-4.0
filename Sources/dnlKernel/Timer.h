/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlKernel_Timer_h__
#define __dnlKernel_Timer_h__

#include <iostream>
#include <chrono>
#include <ctime>
#include <String.h>
#include <List.h>

//-----------------------------------------------------------------------------
// Class : Timer
//
// Used to manage Timer
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Timer
{
  bool _cumulate = true;                                       // Boolean flag to tell if time must be cumulated
  bool _flag = false;                                          // Boolean flag
  bool _run = false;                                           // Boolean flag to know if this timer is running
  double _totalTime = 0;                                       // Total time of the timer
  long _calls = 0;                                             // Number of calls to the timer
  short _level = 0;                                            // Level of the timer
  std::chrono::time_point<std::chrono::system_clock> _initial; // Initial time
  std::chrono::time_point<std::chrono::system_clock> _start;   // Start time
  std::chrono::time_point<std::chrono::system_clock> _current; // Current time
  std::chrono::time_point<std::chrono::system_clock> _stop;    // Stop time
  String _fatherName;                                          // Name of father timer
  String _timerName;                                           // Name of the timer

public:
  Timer(const char *timerName);
  ~Timer() {}

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool getCumulate();
  bool getFlag();
  bool running();
  double getCurrent();
  double getDelay();
  double getTotal();
  long getCalls();
  short getLevel();
  String getFather();
  String getName();
  void setCumulate(bool);
  void setFlag(bool flag);
  void setLevel(short level);
  void start();
  void stop();
};

/*
  CPU time timers

  This class is used to create and manage CPU time measures for the DynELA Finite Element code. It contains a list of Timers.
  \ingroup dnlKernel
*/
//-----------------------------------------------------------------------------
// Class : Timers
//
// Used to manage Timers
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Timers
{
  List <Timer *> timers; // List of timers

public:
  Timers(){};
  ~Timers() {}

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  double getTotalChilds(const char *father);
  int getNumber() { return timers.getSize(); } // Get the number of timers
  String conv(double value);
  Timer *timer(const char *timerName);
  Timer *timer(int);
  void add(Timer *);
  void saveReport(const char *filename = "");
  void setFlags(bool flag);
  void stop();
};

#endif
