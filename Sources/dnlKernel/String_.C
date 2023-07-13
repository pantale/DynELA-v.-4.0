/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file String.C
  Management and manipulation of char strings

  This file contains the definition of strings for DynELA. The strings are part of a new class called String which allows most of the manipulations on the characters. All the methods of this class are documented.
  Since version v. 1.0.0, this class inherited from the \b std::string class of Standard Template Library.
  \ingroup dnlKernel
*/

#include "String_.h"
#include <strings.h>
#include <iostream>

/*
  default constructor of the String class

  Memory allocation for an empty string. This method is used to create the memory allocation for an empty string.
  \code
  String s; // allocation memoire vide (taille indefinie)
  \endcode
*/
//-----------------------------------------------------------------------------
String::String()
//-----------------------------------------------------------------------------
{
}

/*
  constructor à from type char*

  This constructor allows to allocate the memory space for a String and to initialize this string from a standard char* character string.
  \code
  String y = "a std::string";
  \endcode
*/
//-----------------------------------------------------------------------------
String::String(const char *s) // : std::string (s)
//-----------------------------------------------------------------------------
{
  assign(s);
}

//-----------------------------------------------------------------------------
String::String(const std::string &st) // : std::string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}

//-----------------------------------------------------------------------------
String::String(const String &st) // : std::string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}

/*
  Default destructor
*/
//-----------------------------------------------------------------------------
String::~String()
//-----------------------------------------------------------------------------
{
}

/*
  constructor from std::string type

  This constructor allows to allocate the memory space for a String and to initialize this string from a standard char* character string.
  \code
  String y = "a std::string";
  \endcode
*/
//-----------------------------------------------------------------------------
String &String::operator=(const char *st)
//-----------------------------------------------------------------------------
{
  assign(st);
  return (*this);
}

/*
  conversion of an integer numerical value into a String

  This method converts a numerical value of type int into a string of characters.

  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  - val numerical value to convert to a string
  Return : the character string back.
*/
//-----------------------------------------------------------------------------
String &String::convert(const int val, short leading)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%d", val);
  assign(tmpStr);
  if (length() < leading)
  {
    String leadString = std::string(leading - length(), '0');
    *this = leadString + *this;
  }
  return *this;
}

/*
  conversion of an integer numerical value into a String

  This method converts a numerical value of type int into a string of characters.

  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  - val numerical value to convert to a string
  Return : the character string back.
*/
//-----------------------------------------------------------------------------
String &String::convert(const long val, short leading)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%ld", val);
  assign(tmpStr);
  if (length() < leading)
  {
    String leadString = std::string(leading - length(), '0');
    *this = leadString + *this;
  }
  return *this;
}

/*
  conversion of a real value into a String

  This method allows to convert a real numerical value into a string of characters by specifying the type of conversion by a control string of characters.
  \code
  String toto;
  toto.convert(2.0,"%10.3E");
  cout << toto << endl;
  \endcode
  - val real value to convert to a String
  - st if we don't specify anything, we use a format like "%ld".
  Return : string of characters return
*/
//-----------------------------------------------------------------------------
String &String::convert(const double val, const char *st)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, st, val);
  assign(tmpStr);
  return *this;
}

/*
  replacement of a character in a string

  This method searches and replaces a given character with another in a character string.
  - fromItem character to be replaced in the string
  - to spare character in the chain
  Return : to a new string with the substitution
*/
//-----------------------------------------------------------------------------
String &String::replace(const char fromItem, const char to)
//-----------------------------------------------------------------------------
{
  std::string _to(1, to);
  const char *p = data();
  int l = length();

  // boucle de recherche
  for (int i = 0; i < l; i++)
    if (p[i] == fromItem)
      std::string::replace(i, 1, _to);

  // retour
  return (*this);
}

/*
  replacement of a substring in a string

  This method searches and replaces a given substring with another in a character string.
  - fromItem sub-character string to be replaced in the string
  - to sub-chain of wildcard characters in the chain.
  Return : to a new string with the substitution
*/
//-----------------------------------------------------------------------------
String &String::replace(const String &fromItem, const String &to)
//-----------------------------------------------------------------------------
{
  size_type i;
  size_type pos = length();

  // boucle de recherche
  while ((i = rfind(fromItem, pos)) != npos)
  {
    std::string::replace(i, fromItem.length(), to);
    pos = i;
  }

  // retour
  return (*this);
}

/*
  extraction of a substring of a character string

  This method extracts a character substring from a character string. The position of the sub-chain is given by the value of the start length and the length of the sub-chain. The original string is not changed.
  \code
  String a = "ma chaine de caracteres";
  String b= a.subString(4,5); // b="haine"
  \endcode
  - startpos starting position in the character string
  - length length of chain to be extracted
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String
String::subString(int startpos, int length) const
//-----------------------------------------------------------------------------
{
  String ret;
  ret.assign(*this, startpos, length);
  return ret;
}

/*
  search for a character in a string

  This method looks for a character in a character string from a given position in the string.
  \code
  String a = "ma chaine de caracteres";
  printf("%d\n",a.search('e',1); // renvoie 19 ('e' à la 19 eme position)
  printf("%d\n",a.search('e',20); // renvoie -1 (non trouve)
  \endcode
  - startpos search start position in the chain
  - c character to look for
  Return : the position of the character in the string, or value -1 if the character could not be found.
*/
//-----------------------------------------------------------------------------
int String::search(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(c, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

/*
  search for a sub-string in a string

  This method looks for a substring in a character string from a given position in the string.
  \code
  String a = "ma chaine de caracteres";
  printf("%d\n",a.search("de",20); // renvoie 10 (position dans la chaine)
  printf("%d\n",a.search("remove",20); // renvoie -1 (non trouve)
  \endcode
  - startpos search start position in the chain
  - substring substring to search to search to search
  Return : the position of the sub-chain in the chain, or value -1 if that sub-chain could not be found.
*/
//-----------------------------------------------------------------------------
int String::search(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(substring, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

/*
  deletion of part of a character string

  This method deletes a part of a character string from a given position and over a given length. The given string is modified
  \code
  String a = "ma chaine de caracteres";
  a.remove(4,5); // a="ma c de caracteres"
  \endcode
  - startpos starting position of the game to be deleted
  - length length of the part to be deleted
*/
//-----------------------------------------------------------------------------
void String::remove(int startpos, int length)
//-----------------------------------------------------------------------------
{
  std::string::replace(startpos, length, "");
}

/*
  deletion of part of a character string

  This method deletes a part of a specified character string in another string from a given starting position. The given string is modified
  \code
  String a = "ma chaine de caracteres";
  a.remove("chaine",0); // a="ma  de caracteres"
  \endcode
  - y sub-chain to be deleted
  - startpos starting position for the search in the channel
*/
//-----------------------------------------------------------------------------
void String::remove(const String &y, int startpos)
//-----------------------------------------------------------------------------
{
  std::string::replace(find(y, startpos), y.length(), "");
}

/*
  deletion of a character in a string

  This method deletes a specified character in another string from a given start position. The given string is modified
  \code
  String a = "ma chaine de caracteres";
  a.remove('c',0); // a="ma haine de caracteres"
  \endcode
  - y sub-chain to be deleted
  - startpos starting position for the search in the channel
*/
//-----------------------------------------------------------------------------
void String::remove(char c, int startpos)
//-----------------------------------------------------------------------------
{
  std::string::replace(find(c, startpos), 1, "");
}

/*
  extraction of a substring of a character string

  This method extracts a character substring from a character string. The position of the sub-chain is given by the value of the start index and the length of the sub-chain.
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem(4,5); // b="haine"
  \endcode
  - first start position in the character string
  - len of chain to be extracted
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::atItem(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

/*
  extraction of a substring of a character string

  This method extracts a character substring from a character string. The position of the sub-chain is given by the value of the start index and the length of the sub-chain.
  \code
  String a = "ma chaine de caracteres";
  String b= a(4,5); // b="haine"
  \endcode
  - first start position in the character string
  - len of chain to be extracted
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String
String::operator()(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string before a given position.
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem(4); // b="ma c"
  \endcode
  - pos end position in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::beforeItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string before a given position (including the character).
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem(4); // b="ma ch"
  \endcode
  - pos end position in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String
String::throughItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos + 1);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string after a given position.
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem(4); // b="haine de caracteres"
  \endcode
  - pos starting position in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::afterItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos + 1, length() - (pos + 1));
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string after a given position (including the character).
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem(4); // b="chaine de caracteres"
  \endcode
  - pos starting position in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::fromItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos, length() - pos);
}

/*
  extraction of a substring of a character string

  This method extracts a character substring from a character string. The position of the sub-chain is given by the first occurrence of a given string in parameters from a given position.
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem("haine",2); // b="haine"
  \endcode
  - y substring of characters to search for
  - startfirst starting position in the character string
  - len of chain to be extracted
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::atItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, y.length());
}

/*
  extraction of a character from a string of characters

  This method extracts a character from a character string. The position of the character is given by the first occurrence of this character given as a parameter from a given position.
  It's a bit silly like that !!!
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem('h',2); // b="h"
  \endcode
  - y substring of characters to search for
  - startfirst starting position in the character string
  - len of chain to be extracted
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::atItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, 1);
}
/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string before a position given by the first occurrence of a given string in the parameter of this method.
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem("chaine",2); // b="ma "
  \endcode
  - y subchain to search for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::beforeItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  return subString(0, last);
}

/*
  extraction of a substring of a character string

  This method extracts a character sub-chain in a character string before a position given by the first occurrence of a given character in the parameter of this method.
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem('c',2); // b="ma "
  \endcode
  - c character to look for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::beforeItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  return subString(0, last);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string before a position given by the first occurrence of a given string in the parameter of this method. The searched substring is included.
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem("chaine",2); // b="ma chaine"
  \endcode
  - y subchain to search for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::throughItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  if (last >= 0)
    last += y.length();
  return subString(0, last);
}

/*
  extraction of a substring of a character string

  This method extracts a character sub-chain in a character string before a position given by the first occurrence of a given character in the parameter of this method. The search character is included.
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem('c',2); // b="ma c"
  \endcode
  - c character to look for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String
String::throughItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  if (last == -1)
    return "";
  if (last >= 0)
    last += 1;
  return subString(0, last);
}

/*
  extraction of a substring of a character string

  This method extracts a character sub-chain in a character string after a position given by the first occurrence of a given string in the parameter of this method.
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem("chaine",2); // b=" de caracteres"
  \endcode
  - y subchain to search for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::afterItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  if (first >= 0)
    first += y.length();
  return subString(first, length() - first);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string after a position given by the last occurrence of a given string in the parameter of this method.
  \code
  String a = "ma chaine de caracteres de truc";
  String b= a.afterItem("de"); // b=" truc"
  \endcode
  - y subchain to search for
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String
String::afterLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return atItem(pos, length());
}

//-----------------------------------------------------------------------------
String
String::beforeLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return atItem(0, pos - 1);
}

/*
  extraction of a substring of a character string

  This method extracts a character sub-chain in a character string after a position given by the first occurrence of a given character in a parameter of this method.
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem('c',2); // b="haine de caracteres"
  \endcode
  - c character to look for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::afterItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first >= 0)
    first += 1;
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

/*
  extraction of a substring of a character string

  This method extracts a sub-character string in a character string after a position given by the first occurrence of a given string in the parameter of this method. The substring is included.
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem("chaine",1); // b="chaine de caracteres"
  \endcode
  - y subchain to search for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::fromItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

/*
  extraction of a substring of a character string

  This method extracts a character sub-chain in a character string after a position given by the first occurrence of a given character in a parameter of this method.
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem('c',2); // b="chaine de caracteres"
  \endcode
  - c character to look for
  - startpos start position of search in the character string
  Return : under extracted character string
*/
//-----------------------------------------------------------------------------
String String::fromItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//
/*
  fills in a string from a file

  This method lists a string on a file and returns a String containing that string. Characters are read from the file up to the end-of-line character '\n'. If the end of the file is reached, this method returns 0, otherwise it returns 1, so this method is well named.
  \code
  FILE* pfile;
  String s;
  pfile=fopen("fich","r");
  while (s.scanFileLine(pfile)) cout << s << "\n";
  fclose(pfile);
  \endcode
  - pfile file to read
  Return : boolean specifying if there are any characters left to read in the file.
*/
//-----------------------------------------------------------------------------
bool String::scanFileLine(FILE *pfile)
//-----------------------------------------------------------------------------
{
  int c;
  *this = "";

  while ((c = getc(pfile)) != '\n')
  {
    if (c == EOF)
      return 0;
    *this += c;
  }

  return 1;
}

//
/*
  whiteout

  This method removes the blanks at the beginning and end of a string as shown below:
  "hello" -> "hello"
*/
//-----------------------------------------------------------------------------
void String::strip()
//-----------------------------------------------------------------------------
{
  size_type start;
  size_type stop;
  if ((start = find_first_not_of(" ")) == npos)
    return;
  if ((stop = find_last_not_of(" ")) == npos)
    return;

  *this = subString(start, stop - start + 1);
}

//
/*
  tests the presence of one chain in another and returns the position

  This method looks for a string of characters in another one, taking into account or not the difference between upper and lower case letters. This method has been developed for bibView.
  - substring string to search for
  - cas boolean allowing to specify if we want sensitive search (true) or not (false)
  Return : the position of the sub-chain in the chain or -1 in case of a search failure.
*/
//-----------------------------------------------------------------------------
int String::containsWithCaseSub(const String &substring, bool cas) const
//-----------------------------------------------------------------------------
{
  int startpos = 0;
  const char *s = chars();
  const char *t = substring.chars();
  int sl = length();
  int tl = substring.length();

  if (sl > 0 && tl > 0)
  {
    if (startpos >= 0)
    {
      const char *lasts = &(s[sl - tl]);
      const char *lastt = &(t[tl]);
      const char *p = &(s[startpos]);

      while (p <= lasts)
      {
        const char *x = p++;
        const char *y = t;
        if (cas)
        {
          while (*x++ == *y++)
            if (y >= lastt)
              return --p - s;
        }
        else
        {
          // while (strncasecmp(x++, y++, 1) == 0)
            // if (y >= lastt)
              // return --p - s;
        }
      }
    }
    else
    {
      const char *firsts = &(s[tl - 1]);
      const char *lastt = &(t[tl - 1]);
      const char *p = &(s[sl + startpos + 1]);

      while (--p >= firsts)
      {
        const char *x = p;
        const char *y = lastt;
        if (cas)
        {
          while (*x-- == *y--)
            if (y < t)
              return ++x - s;
        }
        else
        {
          // while (strncasecmp(x--, y--, 1) == 0)
            // if (y < t)
              // return ++x - s;
        }
      }
    }
  }
  return -1;
}

// teste la presence d'une chaine dans une autre
/*
  tests the presence of a chain in another chain.

  This method looks for a string of characters in another one, taking into account or not the difference between upper and lower case letters. This method has been developed for bibView.
  - substring string to search for
  - case boolean allowing to specify if we want sensitive search (true) or not (false)
  Return : booleen: true if the subchain exists or false if not.
*/
//-----------------------------------------------------------------------------
bool String::containsWithCase(const String &substring, bool cas) const
//-----------------------------------------------------------------------------
{
  return (containsWithCaseSub(substring, cas) >= 0);
}

//
/*
  converts a string of characters into a char* type.

  This method converts a character string into a char* type.
  \code
  String x;
  x="chain to convert";
  printf("%s",x.chars());
  \endcode
*/
//-----------------------------------------------------------------------------
const char *String::chars() const
//-----------------------------------------------------------------------------
{
  return c_str();
}

//
/*
  tests the presence of a character in a chain

  This method tests whether a string contains a given character.
  - c character to be searched
  Return : true if the character exists, false if it doesn't...
*/
//-----------------------------------------------------------------------------
bool String::contains(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(c, startpos) != npos)
    return true;
  return false;
}

//
/*
  search for a character in a string

  This method looks for a substring of characters in an existing string and returns true, or the value false if the string does not exist.
  - startpos starting position for the search.
  - substring substring to search for
  Return : true or false depending on the search result.
*/
//-----------------------------------------------------------------------------
bool String::contains(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(substring, startpos) != npos)
    return true;
  return false;
}

//
/*
  writes a string of characters on the output stream
  This method writes a string of characters to the ostream data stream. If the string is empty, "empty" is written to the output.

  - s written string
*/
//-----------------------------------------------------------------------------
void String::dump(const String &s) const
//-----------------------------------------------------------------------------
{
  std::cout << *this << std::endl;
}

//
/*
  test if a string is empty

  This method tests if a string is empty.
  Return : true if the string is empty false if not.
*/
//-----------------------------------------------------------------------------
bool String::empty() const
//-----------------------------------------------------------------------------
{
  return ((*this) == "");
}

//
/*
  test if a string is not empty

  This method tests whether a string is not empty.
  Return : true if the string is non-empty false if not.
*/
//-----------------------------------------------------------------------------
bool String::ok() const
//-----------------------------------------------------------------------------
{
  return !(empty());
}

//
/*
  returns the first character of a String

  This method returns the first character of a std::string
  Return : first character of the G-string
*/
//-----------------------------------------------------------------------------
char String::firstchar() const
//-----------------------------------------------------------------------------
{
  return std::string::at(0);
}

//
/*
  returns the last character of a String

  This method returns the last character of a std::string
  Return : last character of the G-string
*/
//-----------------------------------------------------------------------------
char String::lastchar() const
//-----------------------------------------------------------------------------
{
  return std::string::at(length() - 1);
}

//
/*
  character conversion to lowercase

  This method converts the characters of a string into lowercase letters.
*/
//-----------------------------------------------------------------------------
void String::lowerCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((std::string::at(i) >= 'A') && (std::string::at(i) <= 'Z'))
    {
      std::string::at(i) += 'a' - 'A';
    }
  }
}

//
/*
  character conversion in capital letters

  This method converts the characters of a string into uppercase letters.
*/
//-----------------------------------------------------------------------------
void String::upperCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((std::string::at(i) >= 'a') && (std::string::at(i) <= 'z'))
    {
      std::string::at(i) += 'A' - 'a';
    }
  }
}

//
/*
  converts a String into a real

  This method extracts from a String a real value at a given column.
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  a=s.getRealAtPos(0); // a=5.0
  b=s.getRealAtPos(1); // b=8.4
  c=s.getRealAtPos(2); // c=12.7e-6
  \endcode
  - pos specifies that you wish to extract the i-th value on the line
  - sep specifies the data separator on the line you want to use (default is a space if you don't specify anything).
  Return : actual return value converted
*/
//-----------------------------------------------------------------------------
double
String::getRealAtPos(int pos, String sep)
//-----------------------------------------------------------------------------
{
  double val;
  String tempo;
  tempo = *this;

  for (int i = 0; i < pos - 1; i++)
  {
    tempo.strip();
    tempo = tempo.afterItem(sep);
  }
  sscanf(tempo.chars(), "%lf", &val);
  return val;
}

//
/*
  extract a real value from a std::string

  This method extracts a real value from a String and reduces this string. If there are no more values to extract, this method returns false at the time of extraction of the last value of the line.
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  bool r;
  r=s.popReal(a); // a=5.0, r=true
  r=s.popReal(b); // b=8.4, r=true
  r=s.popReal(c); // c=12.7e-6, r=false
  \endcode
  - pos specifies that you wish to extract the i-th value on the line
  - sep specifies the data separator on the line you want to use (default is a space if you don't specify anything).
  Return : actual return value converted
*/
//-----------------------------------------------------------------------------
bool String::popReal(double &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {
    // la conversion du premier en un reel
    ret = getReal();

    // la recherche du suivant
    if (contains(sep))
    {
      *this = afterItem(sep);
      strip();
      return true;
    }
  }

  return false;
}

//
/*
  extracts an integer value from a std::string

  This method extracts an integer value from a String and reduces this string. If there are no more values to extract, this method returns false at the time of extraction of the last value of the line.
  \code
  String s="5 8 12";
  int a,b,c;
  bool r;
  r=s.popInt(a); // a=5, r=true
  r=s.popInt(b); // b=8, r=true
  r=s.popInt(c); // c=12, r=false
  \endcode
  - pos specifies that you wish to extract the i-th value on the line
  - sep specifies the data separator on the line you want to use (default is a space if you don't specify anything).
  Return : actual return value converted
*/
//-----------------------------------------------------------------------------
bool String::popInt(long &val, String sep)
//-----------------------------------------------------------------------------
{
  double rval = 0;
  bool ret;
  ret = popReal(rval, sep);
  val = (long)rval;
  return ret;
}

//
/*
  extract a word from a string of characters

  This method extracts a word from a string of characters. This method returns the value true if there are mosts left in the string, false otherwise.
  - ret word retrieved from the string
  - sep specifies the data separator on the line you want to use (default is a space if you don't specify anything).
  Return : true if the string contains other words, false if not...
*/
//-----------------------------------------------------------------------------
bool String::popString(String &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {

    // la recherche du suivant
    if (contains(sep))
    {
      ret = beforeItem(sep);
      *this = afterItem(sep);
      strip();
      return true;
    }
    ret = *this;
    return true;
  }
  return false;
}

// conversion of a String into an integer
//-----------------------------------------------------------------------------
long String::getInt() const
//-----------------------------------------------------------------------------
{
  return atoi(c_str());
}

// conversion of a String into a reel
//-----------------------------------------------------------------------------
double String::getReal() const
//-----------------------------------------------------------------------------
{
  return atof(c_str());
}

// addition of an extension if it is not present
//-----------------------------------------------------------------------------
void String::addExtension(String ext)
//-----------------------------------------------------------------------------
{
  bool present = false;

  // test if len is sufficient
  if (length() > ext.length())
  {
    std::cout << (*this)(length() - ext.length(), length()) << std::endl;
    if ((*this)(length() - ext.length(), length()) == ext)
    {
      present = true;
    }
  }

  // is not present, so add it !!
  if (!present)
    *this = *this + ext;
}

//-----------------------------------------------------------------------------
void String::fromFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // Read the length of the string as long
  long stringLen;
  long ret;
  ret = fread((void *)(&stringLen), sizeof(long), 1, pfile);

  // Read the string itself
  char stt[stringLen + 5];
  ret = fread(stt, sizeof(char), stringLen, pfile);

  // Add the CR to the string
  stt[stringLen] = '\0';

  (*this) = stt;
}

//-----------------------------------------------------------------------------
void String::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // Write the length of the string as long
  long stringLen = length();
  fwrite((void *)(&stringLen), sizeof(long), 1, pfile);

  // Write the string itself
  const char *p = data();
  fwrite((void *)(p), sizeof(char), stringLen, pfile);
}
