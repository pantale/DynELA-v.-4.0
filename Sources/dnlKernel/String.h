/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file String.h
  \brief Management and manipulation of char strings

  This file contains the definition of strings for DynELA. The strings are part of a new class called String which allows most of the manipulations on the characters. All the methods of this class are documented.
  Since version v. 1.0.0, this class inherited from the \b std::string class of Standard Template Library.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_String_h__
#define __dnlKernel_String_h__

#include <string>

/*!
  \brief Management and manipulation of char strings

  This class is used to implement strings on DynELA. It is inspired by the examples given by B. Stroutrup in his book on C++. Additional functions have been added to what is given in this book.
  Since version v. 1.0.0, this class inherited from the \b std::string class of Standard Template Library.
  \ingroup dnlKernel
*/
class String : public std::string
{

public:
  String();
  String(const char *);
  String(const std::string &);
  String(const String &);
  ~String();

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
