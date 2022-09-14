/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Matrices_h__
#define __dnlMaths_Matrices_h__

#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
//#include <dnlKernel.h>
//#include <Macros.h>
#include <dnlKernel.h>

#define Ind(i, j, __rows, __cols) ((i) * (__cols) + (j))

  // Interface methods excluded from SWIG
#ifndef SWIG
enum OutMatrix
{
  outMatrixTypeNormal = 0, // format Mathematica
  outMatrixTypeMupad       // format Mupad
};
#endif

//-----------------------------------------------------------------------------
// Class : Matrices
//
// Used to manage Matrices
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Matrices
{

protected:
  unsigned long _rows; // nombre de lignes de la matrice
  unsigned long _cols; // nombre de colonnes de la matrice
  char outType;        // definit le type de format d'affichage

public:
  Matrices();
  Matrices(const Matrices &mat);
  virtual ~Matrices();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  long rows() const;
  long columns() const;
  void setOutType(char outT);
  virtual long Memory() const = 0; // calcule la taille requise pour le stockage de la matrice en memoire
};

//------inline functions-------------------------------------------------------

//constructeur par defaut
/*
  Cette methode est le constructeur par defaut de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices()
//-----------------------------------------------------------------------------
{
  outType = outMatrixTypeNormal;
}

//constructeur par recopie
/*
  Cette methode est le constructeur par recopie de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices(const Matrices &mat)
//-----------------------------------------------------------------------------
{
  outType = mat.outType;
}

//destructeur
/*
  Cette methode est le destructeur de la classe Matrices.
*/
//-----------------------------------------------------------------------------
inline Matrices::~Matrices()
//-----------------------------------------------------------------------------
{
}

//nombre de lignes de la matrice
/*
  Cette methode calcule et retourne le nombre de lignes de la matrice.
  \nombre de lignes de la matrice
*/
//-----------------------------------------------------------------------------
inline long Matrices::rows() const
//-----------------------------------------------------------------------------
{
  return _rows;
}

//nombre de colonnes de la matrice
/*
  Cette methode calcule et retourne le nombre de colonnes de la matrice.
  \nombre de colonnes de la matrice
*/
//-----------------------------------------------------------------------------
inline long Matrices::columns() const
//-----------------------------------------------------------------------------
{
  return _cols;
}

//selection du type d'affichage
/*
  Cette methode permet de selectionner le type d'affichage demande. Le type de sortie est defini par les variables enumerees dans \ref OutMatrix.

*/
//-----------------------------------------------------------------------------
inline void Matrices::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
