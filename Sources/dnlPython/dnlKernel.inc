/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%{
  #include "LogFile.h"
  #include "MacAddress.h"
  #include "Settings.h"
  #include "String.h"
  #include "System.h"
  #include "Timer.h"
  #include "Field.h"
%}
%include "String.h"
%extend String
{
  const char *__str__()
  {
    return self->chars();
  }
}

%include "System.h"

%include "MacAddress.h"

%include "Timer.h"

%include "Settings.h"
%extend Settings
{
  void setValue(std::string k, std::string st)
  {
    self->setValue(k,st);
  }
  const char * getValue(std::string k)
  {
    static char temp[512];
    std::string st;
    self->getValue(k,st);
    sprintf(temp,"%s",st.c_str());
    return temp;
  }
}

%include "LogFile.h"

%include "Field.h"
