/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%{
  #include "Element.h"
  #include "Element2D.h"
  #include "Element3D.h"
  #include "ElementAxi.h"
  #include "ElementData.h"
  #include "ElementPlane.h"
  #include "ElementSet.h"
  #include "ElHex8N3D.h"
  #include "ElQua4N2D.h"
  #include "ElQua4NAx.h"
  #include "ElTet10N3D.h"
  #include "ElTet4N3D.h"
  #include "ElTri3N2D.h"
  #include "NodalField.h"
  #include "Node.h"
  #include "NodeSet.h"
%}

%include "Node.h"
%include "NodalField.h"

%include "ElementData.h"
%include "Element.h"

%include "ElementPlane.h"
%include "Element2D.h"
%include "ElementAxi.h"
%include "Element3D.h"

%include "ElHex8N3D.h"
%include "ElQua4N2D.h"
%include "ElQua4NAx.h"
%include "ElTet10N3D.h"
%include "ElTet4N3D.h"
%include "ElTri3N2D.h"

%include "NodeSet.h"
%include "ElementSet.h"
