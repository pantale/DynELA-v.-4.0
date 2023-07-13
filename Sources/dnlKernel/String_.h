/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlKernel_String_h__
#define __dnlKernel_String_h__

#include "String_.h"
#include <String>
//-----------------------------------------------------------------------------
// Class : String
//
// Used to manage String
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class String : public std::string
{

public:
  String();
  String(const char *);
  String(const std::string &);
  String(const String &);
  ~String();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

#ifndef SWIG
  String &operator=(const char *);
#endif

  bool contains(char c, int = 0) const;
  bool contains(const String &, int = 0) const;
  bool containsWithCase(const String &, bool) const;
  bool empty() const;
  bool ok() const;
  bool popInt(long &, String = " ");
  bool popReal(double &, String = " ");
  bool popString(String &, String = " ");
  bool scanFileLine(FILE *);
  char firstchar() const;
  char lastchar() const;
  const char *chars() const;
  double getReal() const;
  double getRealAtPos(int, String = " ");
  int containsWithCaseSub(const String &, bool) const;
  int search(char c, int = 0) const;
  int search(const String &, int = 0) const;
  long getInt() const;
  String &convert(const double, const char * = "%lf");
  String &convert(const int, short leading = 0);
  String &convert(const long, short leading = 0);
  String &replace(const char, const char);
  String &replace(const String &, const String &);
  String afterItem(char c, int = 0) const;
  String afterItem(const String &, int = 0) const;
  String afterItem(int) const;
  String afterLast(const String &) const;
  String atItem(char c, int = 0) const;
  String atItem(const String &, int = 0) const;
  String atItem(int, int) const;
  String beforeItem(char c, int = 0) const;
  String beforeItem(const String &, int = 0) const;
  String beforeItem(int) const;
  String beforeLast(const String &) const;
  String fromItem(char c, int = 0) const;
  String fromItem(const String &, int = 0) const;
  String fromItem(int) const;
  String operator()(int, int) const;
  String subString(int, int) const;
  String throughItem(char c, int = 0) const;
  String throughItem(const String &, int = 0) const;
  String throughItem(int) const;
  void addExtension(String);
  void remove(char, int = 0);
  void remove(const String &, int = 0);
  void remove(int, int);
  void fromFile(FILE *);
  void lowerCase();
  void dump(const String &) const;
  void strip();
  void toFile(FILE *);
  void upperCase();
};

#endif
