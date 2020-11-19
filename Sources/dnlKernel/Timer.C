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
  \file Timer.C
  \brief Definition of the Timer class.

  This file defines all the Timer class.
  \ingroup dnlKernel
*/

#include <Timer.h>
#include <iostream>
#include <fstream>

/*!
  \brief Default constructor of the Timer class.

  This method is the default constructor of the Timer class. All timers are initialized to _current time by default.
  The requested parameter \ref timerName is used to define the name of the timer and/or the inheritance in the timers tree.
  If \ref timerName contains ":" then father name is on the left and child name is on the right of the ":" character.
  \param timerName defines the default name of the timer
*/
//-----------------------------------------------------------------------------
Timer::Timer(const char *timerName)
//-----------------------------------------------------------------------------
{
  String name = timerName;

  if (name.contains(':'))
  {
    _timerName = name.afterItem(":");
    _fatherName = name.beforeItem(":");
  }
  else
  {
    _timerName = timerName;
    _fatherName = "";
  }
  _initial = std::chrono::high_resolution_clock::now();
  _start = _stop = _current = _initial;
}

/*!
  \brief Sets cumulate flag of the timer.
*/
//-----------------------------------------------------------------------------
void Timer::setCumulate(bool cum)
//-----------------------------------------------------------------------------
{
  _cumulate = cum;
}

/*!
  \brief Get the value of the cumulate flag of the timer.
*/
//-----------------------------------------------------------------------------
bool Timer::getCumulate()
//-----------------------------------------------------------------------------
{
  return _cumulate;
}

/*!
  \brief Store the _current time as the _start time

  This method stores the _current time as the _start time.
*/
//-----------------------------------------------------------------------------
void Timer::start()
//-----------------------------------------------------------------------------
{
  _run = true;
  _start = std::chrono::high_resolution_clock::now();
}

/*!
  \brief Store the _current time as the _stop time

  This method stores the _current time as the _stop time.
*/
//-----------------------------------------------------------------------------
void Timer::stop()
//-----------------------------------------------------------------------------
{
  _stop = std::chrono::high_resolution_clock::now();
  _calls++;
  if (_cumulate)
  {
    _totalTime += getDelay();
  }
  _run = false;
}

/*!
  \brief get the current elaspsed time

  This method computes the _current CPU, i.e. the _current time - _initial time.
  \return the delay between _current and _initial times
*/
//-----------------------------------------------------------------------------
double Timer::getCurrent()
//-----------------------------------------------------------------------------
{
  _current = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = _current - _initial;
  return elapsed_seconds.count();
}

/*!
  \brief Delay from _start to _stop times

  This method computes the delay from the _start time to the _stop time.
  \return the delay between _stop and _start times
*/
//-----------------------------------------------------------------------------
double Timer::getDelay()
//-----------------------------------------------------------------------------
{
  std::chrono::duration<double> elapsed_seconds = _stop - _start;
  return elapsed_seconds.count();
}

/*!
  \brief Cumulative _totalTime for the timer

  This method computes cumulative time. One has to define setCumulate(true).
  \return the cumulative time
*/
//-----------------------------------------------------------------------------
double Timer::getTotal()
//-----------------------------------------------------------------------------
{
  return _totalTime;
}

/*!
  \brief Number of calls to the timer
  \return the number of calls to the timer
*/
//-----------------------------------------------------------------------------
long Timer::getCalls()
//-----------------------------------------------------------------------------
{
  return _calls;
}

/*!
  \brief Timer is running ?
  \return true if timer is running
*/
//-----------------------------------------------------------------------------
bool Timer::running()
//-----------------------------------------------------------------------------
{
  return _run;
}

/*!
  \brief Name of the timer
  \return the name of the timer
*/
//-----------------------------------------------------------------------------
String Timer::getName()
//-----------------------------------------------------------------------------
{
  return _timerName;
}

/*!
  \brief Father's name of the timer
  \return the father's name of the timer
*/
//-----------------------------------------------------------------------------
String Timer::getFather()
//-----------------------------------------------------------------------------
{
  return _fatherName;
}

/*!
  \brief Gets the flag associated to a timer
*/
//-----------------------------------------------------------------------------
bool Timer::getFlag()
//-----------------------------------------------------------------------------
{
  return _flag;
}

/*!
  \brief Sets the flag associated to a timer
*/
//-----------------------------------------------------------------------------
void Timer::setFlag(bool flag)
//-----------------------------------------------------------------------------
{
  _flag = flag;
}

/*!
  \brief Gets the level associated to a timer
*/
//-----------------------------------------------------------------------------
void Timer::setLevel(short level)
//-----------------------------------------------------------------------------
{
  _level = level;
}

/*!
  \brief Sets the level associated to a timer
*/
//-----------------------------------------------------------------------------
short Timer::getLevel()
//-----------------------------------------------------------------------------
{
  return _level;
}

/*!
  \brief Access to a timer in the Timers list
  \param index index of the timer in the list of timers
  \return the requested timer
*/
//-----------------------------------------------------------------------------
Timer *Timers::timer(int index)
//-----------------------------------------------------------------------------
{
  if (index >= timers.getSize())
    return timers(timers.getSize() - 1);
  return timers(index);
}

/*!
  \brief Access to a timer in the Timers list
  \param timerName name of the timer in the list of timers
  \return the requested timer
*/
//-----------------------------------------------------------------------------
Timer *Timers::timer(const char *timerName)
//-----------------------------------------------------------------------------
{
  for (short index = 0; index < timers.getSize(); index++)
  {
    if (timers(index)->getName() == timerName)
      return timers(index);
  }

  // Doesn't exist, create it and return
  Timer *newTimer = new Timer(timerName);
  add(newTimer);
  return newTimer;
}

/*!
  \brief Add a timer in the Timers list
  \param newTimer name of the new timer to add to the list of timers
*/
//-----------------------------------------------------------------------------
void Timers::add(Timer *newTimer)
//-----------------------------------------------------------------------------
{
  timers << newTimer;
}

/*!
  \brief Sets all flags of all timers
*/
//-----------------------------------------------------------------------------
void Timers::setFlags(bool flag)
//-----------------------------------------------------------------------------
{
  for (short tim = 0; tim < timers.getSize(); tim++)
  {
    timers(tim)->setFlag(flag);
  }
}

/*!
  \brief Converts time in human deadable format

  This methods returns a string in human readable format of the time given as argument expressed in seconds.
  Depending of the value, this is converted into µs; ms, s, minutes, hours,...
  \param value the value to convert into human readable format
  \return the string of human readable value
*/
//-----------------------------------------------------------------------------
String Timers::conv(const double value)
//-----------------------------------------------------------------------------
{
  // Display in µs
  if (value < 1e-6)
  {
    String str1, str2;
    return str1.convert(value * 1e6) + " µs (" + str2.convert(value) + " s)";
  }

  // Display in ms
  if (value < 1e-3)
  {
    String str1, str2;
    return str1.convert(value * 1e3) + " ms (" + str2.convert(value) + " s)";
  }

  // Display in H:M:S
  if (value > 3600)
  {
    String str1, str2, str3, str4;
    int hour, min, sec;
    hour = int(value / 3600);
    min = int((value - 3600 * hour) / 60);
    sec = int(value - 3600 * hour - 60 * min);
    return str1.convert(hour, 2) + ":" + str2.convert(min, 2) + ":" + str3.convert(sec, 2) + " (" + str4.convert(value) + " s)";
  }

  // Display in M:S
  if (value > 60)
  {
    String str1, str2, str4;
    int min, sec;
    min = int(value / 60);
    sec = int(value - 60 * min);
    return str1.convert(min, 2) + ":" + str2.convert(sec, 2) + " (" + str4.convert(value) + " s)";
  }

  // Default display in sec
  String str;
  return str.convert(value) + " s";
}

/*!
  \brief Saves a report of all CPU times in a file

  This methods saves a report of all CPU times ins a txt file
  \param filename name of the file to produce
*/
//-----------------------------------------------------------------------------
void Timers::saveReport(const char *filename)
//-----------------------------------------------------------------------------
{
  double total = 0;
  std::ofstream _stream;

  String fileName = filename;
  if (fileName == "")
  {
    fileName = "CPU-TIMES.log";
  }

  // open the stream
  _stream.open(fileName.chars(), std::fstream::out);

  if (!_stream.is_open())
  {
    fatalError("Timers::saveReport", "Cannot open _stream for file %s", fileName.chars());
  }

  _stream << "# CPU Computing times report\n\n";
  _stream << "Number of tracked processes: " << timers.getSize() << "\n\n";

  // Set all flags to false
  setFlags(false);

  for (short tim = 0; tim < timers.getSize(); tim++)
  {
    Timer *t = timers(tim);
    String father = t->getFather();
    if (!t->getFlag())
    {
      _stream << "\n------------------------------------------\n";
      //  t->setFlag(true);
      if (father != "")
      {
        double fatherTime = timer(t->getFather().chars())->getTotal();
        double cumul = 0;
        _stream << "Group: " << t->getFather() << " (" << conv(fatherTime) << ")\n";
        for (short tim2 = 0; tim2 < timers.getSize(); tim2++)
        {
          Timer *t2 = timers(tim2);
          if (!t2->getFlag() && t2->getFather() == t->getFather())
          {
            t2->setFlag(true);
            _stream << "\n"
                    << t2->getName() << "\n";
            _stream << "cpu-time: " << conv(t2->getTotal()) << " - " << (int((t2->getTotal() / fatherTime) * 10000)) / 100.0 << "%\n";
            _stream << "calls   : " << t2->getCalls() << "\n";
            _stream << "cpu-time: " << conv(t2->getTotal() / t2->getCalls()) << " / call\n";
            cumul += t2->getTotal();
          }
        }
        _stream << "\nUnmeasured : " << conv(fatherTime - cumul) << " - " << (int(((fatherTime - cumul) / fatherTime) * 10000)) / 100.0 << "%\n";
      }
      else
      {
        t->setFlag(true);
        _stream << t->getName() << "\n";
        _stream << "cpu-time: " << conv(t->getTotal()) << "\n";
        _stream << "calls   : " << t->getCalls() << "\n";
        _stream << "cpu-time: " << conv(t->getTotal() / t->getCalls()) << " / call\n";
      }
    }
  }

  // close the stream
  _stream.close();
}

/*!
  \brief Stop all timers
*/
//-----------------------------------------------------------------------------
void Timers::stop()
//-----------------------------------------------------------------------------
{
  Timer *t;
  for (short tim = 0; tim < timers.getSize(); tim++)
  {
    t = timers(tim);
    if (t->running())
      timers(tim)->stop();
  }
}

/*!
  \brief Get the total cumulated time of all childs
  \param father name of the father process
  \return the cumulated time of all timers in the same level
*/
//-----------------------------------------------------------------------------
double Timers::getTotalChilds(const char *father)
//-----------------------------------------------------------------------------
{
  int i;
  Timer *t;
  double total = 0;
  bool found = false;
  for (i = 0; i < timers.getSize(); i++)
  {
    t = timers(i);
    if (t->getFather() == father)
    {
      found = true;
      total += getTotalChilds(t->getName().chars());
    }
  }
  if (!found)
  {
    for (i = 0; i < timers.getSize(); i++)
    {
      t = timers(i);
      if (t->getName() == father)
        total += t->getTotal();
    }
  }

  return total;
}
