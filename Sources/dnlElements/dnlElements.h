/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file Defines.h
  Definition of the basic mathematical operators and constants.

  This file defines all the mathematical constants, tools and basic operators.
  \ingroup dnlElements
*/

#ifndef __dnlElements_h__
#define __dnlElements_h__

// Elements class
#include <Element2D.h>
#include <Element3D.h>
#include <ElementAxi.h>
#include <ElementAxi.h>

// Data
#include <ElementData.h>

// Head include
#include <Element.h>
#include <ElementSet.h>

// Elements
#include <ElHex8N3D.h>
#include <ElQua4N2D.h>
#include <ElQua4NAx.h>
#include <ElTet10N3D.h>
#include <ElTet4N3D.h>
#include <ElTri3N2D.h>

// Fields
#include <NodalField.h>
#include <Node.h>
#include <NodeSet.h>

#endif
