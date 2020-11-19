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
  \file MacAddress.h
  \brief Declaration of the Mac Address class.

  This file declares the Mac Address class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_MacAddress_h__
#define __dnlKernel_MacAddress_h__

#include <String.h>
#include <List.h>

/*!
  \brief Storage of mac address

  This class contains a set of methods dedicated to the MacAddress working environment.
  \ingroup dnlKernel
*/
class MacAddress
{
  unsigned char mac[6]; //!< Mac address stored as 6 chars
  String interface;     //!< Name of the interface

  friend class MacAddresses;

public:
  MacAddress() {}  //!< default constructor of the MacAddress class
  ~MacAddress() {} //!< default destructor of the MacAddress class
};

/*!
  \brief Managment of mac addresses of computer

  This class contains a set of methods dedicated to the MacAddresses working environment. The MacAddresses class is used to manage the different Mac Address of a computer.
  \ingroup dnlKernel
*/
class MacAddresses
{
  List<MacAddress *> macs; //!< List of mac adresses

public:
  MacAddresses();
  ~MacAddresses() {} //!< default destructor of the MacAddresses class
  int getNumber();
  String getAddress(int);
  String getInterface(int);
};

#endif
