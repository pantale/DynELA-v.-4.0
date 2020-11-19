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

// TODOCXYFILE

/*
  Class Matrices definition
*/

#ifndef __dnlMaths_Matrices_h__
#define __dnlMaths_Matrices_h__
/*!
  \file Matrices.h
  \brief fichier .h de definition des matrices generales
  \ingroup dnlMaths

  Ce fichier set à la definition de la classe Matrice qui sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert à stocker les informations communes à tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

*/

#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <Errors.h>
#include <Macros.h>

#define Ind(i, j, __rows, __cols) ((i) * (__cols) + (j))

/*
  \enum OutMatrix
  \brief enumeration de format d'affichage de matrices.
  Cette enumeration permet de definir le format d'affichage des matrices. On a ainsi le choix entre outMatrixTypeNormal qui correspond à un affichage conforme à Mathematica et outMatrixTypeMupad qui correspond à un affichage au format Mupad.
*/
#ifndef SWIG
enum OutMatrix
{
  outMatrixTypeNormal = 0, // format Mathematica
  outMatrixTypeMupad       // format Mupad
};
#endif

/*!
  \class Matrices Matrices.h
  \brief classe de base pour les matrices
  \ingroup dnlMaths

  Cette classe sert à la definition definition generale des matrices, elle sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert à stocker les informations communes à tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

*/
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

  long rows() const;
  long columns() const;
  void setOutType(char outT);
  virtual long Memory() const = 0; // calcule la taille requise pour le stockage de la matrice en memoire
};

//------inline functions-------------------------------------------------------

//constructeur par defaut
/*!
  Cette methode est le constructeur par defaut de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices()
//-----------------------------------------------------------------------------
{
  outType = outMatrixTypeNormal;
}

//constructeur par recopie
/*!
  Cette methode est le constructeur par recopie de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices(const Matrices &mat)
//-----------------------------------------------------------------------------
{
  outType = mat.outType;
}

//destructeur
/*!
  Cette methode est le destructeur de la classe Matrices.
*/
//-----------------------------------------------------------------------------
inline Matrices::~Matrices()
//-----------------------------------------------------------------------------
{
}

//nombre de lignes de la matrice
/*!
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
/*!
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
/*!
  Cette methode permet de selectionner le type d'affichage demande. Le type de sortie est defini par les variables enumerees dans \ref OutMatrix.

*/
//-----------------------------------------------------------------------------
inline void Matrices::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
