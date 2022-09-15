/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlKernel_MacAddress_h__
#define __dnlKernel_MacAddress_h__

#include <String.h>
#include <List.h>

/*
  Storage of mac address

  This class contains a set of methods dedicated to the MacAddress working environment.
  \ingroup dnlKernel
*/
//-----------------------------------------------------------------------------
// Class : MacAddress
//
// Used to manage MacAddress
//
// This class is excluded from SWIG
//-----------------------------------------------------------------------------
#if !defined(SWIG)
class MacAddress
{
  unsigned char mac[6]; // Mac address stored as 6 chars
  String interface;     // Name of the interface

  friend class MacAddresses;

public:
  MacAddress() {}  // default constructor of the MacAddress class
  ~MacAddress() {} // default destructor of the MacAddress class
};
#endif

/*
  Managment of mac addresses of computer

  This class contains a set of methods dedicated to the MacAddresses working environment. The MacAddresses class is used to manage the different Mac Address of a computer.
  \ingroup dnlKernel
*/
//-----------------------------------------------------------------------------
// Class : MacAddresses
//
// Used to manage MacAddresses
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class MacAddresses
{
  List <MacAddress *> macs; // List of mac adresses

public:
  MacAddresses();
  ~MacAddresses() {} // default destructor of the MacAddresses class

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  int getNumber();
  String getAddress(int);
  String getInterface(int);
};

#endif
