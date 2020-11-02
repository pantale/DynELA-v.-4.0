/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*!
  \file MacAddress.C
  \brief Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
*/

#include <MacAddress.h>
#include <Exception.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <stdio.h>

/*!
  \brief default constructor of the MacAddresses class
  */
//-----------------------------------------------------------------------------
MacAddresses::MacAddresses()
//-----------------------------------------------------------------------------
{
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs *ifa = NULL;
    int i = 0;

    try
    {
        int ret = getifaddrs(&ifaddr);
        IfException(ret == -1, StandardExceptions::CheckFailed);
    }
    catch (Exception &e)
    {
        e.print();
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if ((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET))
        {
            struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;
            MacAddress *mac = new MacAddress();
            mac->interface = ifa->ifa_name;
            for (i = 0; i < s->sll_halen; i++)
                mac->mac[i] = s->sll_addr[i];
            macs << mac;
        }
    }
    freeifaddrs(ifaddr);
}

/*!
  \brief Returns a string containing the ith MacAddress of the computer

  \param num ith mac adress of the computer
  \returns the mac adress of the computer
  */
//-----------------------------------------------------------------------------
String MacAddresses::getAddress(int num)
//-----------------------------------------------------------------------------
{
    String address;
    if ((num < 0) || (num >= macs.getSize()))
        return "none";
    char tmp[8];
    for (int i = 0; i < 6; i++)
    {
        sprintf(tmp, "%02x", macs(num)->mac[i]);
        if (i >= 1)
            address += ":";
        address += tmp;
    }
    address.upperCase();
    return address;
}

/*!
  \brief Returns a string containing the ith interface of the computer

  \param num ith interface of the computer
  \returns the interface of the computer
  */
//-----------------------------------------------------------------------------
String MacAddresses::getInterface(int num)
//-----------------------------------------------------------------------------
{
    if ((num < 0) || (num >= macs.getSize()))
        return "none";
    return macs(num)->interface;
}

/*!
  \brief Returns the number of MacAddresses

  \returns the number of MacAddresses
  */
//-----------------------------------------------------------------------------
int MacAddresses::getNumber()
//-----------------------------------------------------------------------------
{
    return macs.getSize();
}
