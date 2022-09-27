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
  \file MacAddress.C
  Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
*/

#include <MacAddress.h>
#include <Exception.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <stdio.h>

/*
@LABEL:MacAddresses::MacAddresses()
@SHORT:Default constructor of the MacAddresses class.
@RETURN:MacAddresses
@END
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

/*
@LABEL:MacAddresses::getAddress(int n)
@SHORT:Return the Mac Address \#n from the list.
@RETURN:String
@ARG:int&n&The index of the MacAddresses to return.
@END
*/
//-----------------------------------------------------------------------------
String MacAddresses::getAddress(int n)
//-----------------------------------------------------------------------------
{
    String address;
    if ((n < 0) || (n >= macs.getSize()))
        return "none";
    char tmp[8];
    for (int i = 0; i < 6; i++)
    {
        sprintf(tmp, "%02x", macs(n)->mac[i]);
        if (i >= 1)
            address += ":";
        address += tmp;
    }
    address.upperCase();
    return address;
}

/*
@LABEL:MacAddresses::getInterface(int n)
@SHORT:Return the Internet interface \#n from the list.
@RETURN:String
@ARG:int&n&The index of the Internet interface to return.
@END
*/
//-----------------------------------------------------------------------------
String MacAddresses::getInterface(int n)
//-----------------------------------------------------------------------------
{
    if ((n < 0) || (n >= macs.getSize()))
        return "none";
    return macs(n)->interface;
}

/*
@LABEL:MacAddresses::getNumber()
@SHORT:Return the number of Internet interfaces of the computer.
@RETURN:int
@END
*/
//-----------------------------------------------------------------------------
int MacAddresses::getNumber()
//-----------------------------------------------------------------------------
{
    return macs.getSize();
}
