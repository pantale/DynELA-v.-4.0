/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 4.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%varargs(30, long val = 0) DynELA::createElement;

%{
  #include "Model.h"
  #include "DynELA.h"
  #include "VtkInterface.h"
  #include "SvgInterface.h"
  #include "Drawing.h"
  #include "HistoryFile.h"
  #include "Solver.h"
  #include "Explicit.h"
  #include "Parallel.h"
%}

%include "Model.h"
%include "DynELA.h"
%include "VtkInterface.h"
%include "SvgInterface.h"
%include "Drawing.h"
%include "HistoryFile.h"
%include "Solver.h"
%include "Explicit.h"
%include "Parallel.h"
