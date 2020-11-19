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

/*!
  \file MatrixDiag.h
  \brief Declaration file for the matrix class

  This file is the declaration file for the matrix class. A matrix class is a matrix with the following form:
  \f[ \overrightarrow{_data}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  ...\\
  v_{n}
  \end{array}\right] \f]

  \ingroup dnlMaths
*/

#ifndef __dnlMaths_MatrixDiag_h__
#define __dnlMaths_MatrixDiag_h__

#include <Matrices.h>
#include <Vector.h>

/*!
  \class MatrixDiag MatrixDiag.h
  \brief Classe de gestion et manipulation des matrices disgonales.
  \ingroup dnlMaths

  Cette classe est utilisee pour la gestion et la manipulation des matrices disgonales. Elle comporte toutes les methodes d'initialisation et de gestion memoire. Les valeurs stockees sont des double. La classe \c MatrixDiag contient de plus un certain nombre de methodes de calcul sur les matrices comme decrit ci-dessous.
*/
class MatrixDiag : public Matrices
{
  friend class Matrix;
  friend class Vector;

  double *_data;

  void allocate(const long);
  void desallocate();

public:
  MatrixDiag(const long numberOfTerms = 3, double value = 0.0);
  MatrixDiag(const MatrixDiag &);
  ~MatrixDiag();

  bool operator!=(const MatrixDiag &) const;
  bool operator==(const MatrixDiag &) const;
  double operator()(long) const;
  MatrixDiag operator-() const;
  MatrixDiag operator-(const MatrixDiag &) const;
  MatrixDiag operator*(const double &) const;
  MatrixDiag operator*(const MatrixDiag &) const;
  MatrixDiag operator/(const double &) const;
  MatrixDiag operator+(const MatrixDiag &) const;

#ifndef SWIG
  double &operator()(long);
  friend MatrixDiag operator*(const double &, const MatrixDiag &);
  MatrixDiag &operator=(const double &);
  MatrixDiag &operator=(const MatrixDiag &);
  void operator-=(const MatrixDiag &);
  void operator+=(const MatrixDiag &);
#endif

  bool indexOK(long) const;
  double getTrace();
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  long Memory() const;
  MatrixDiag getInverse() const;
  MatrixDiag getTranspose();
  MatrixDiag singleProduct(const MatrixDiag &) const;
  Vector getSolve(Vector &);
  void solve(Vector &);
  Vector operator*(const Vector &) const;
  void divideBy(Vector &) const;
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void productBy(Vector &) const;
  void redim(const long newSize);
  void setToUnity();
  void setToValue(double);
  void gatherFrom(const MatrixDiag &, long *, int);
  void scatterFrom(const MatrixDiag &, long *, int);

#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, MatrixDiag &);
  friend std::ofstream &operator<<(std::ofstream &, const MatrixDiag &);
  friend std::ostream &operator<<(std::ostream &, const MatrixDiag &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif
};

//------inline functions-------------------------------------------------------

//teste les bornes de la matrice
/*!
  Cette methode teste les bornes d'une matrice
  \param i long de ligne
  \return true si l'long fourni est dans les bornes, false dans le cas contraire
*/
//-----------------------------------------------------------------------------
inline bool MatrixDiag::indexOK(long i) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < _rows))
    return (true);

  printf("MatrixDiag::indexOK\nindice(s) [%ld] out of allowed range [0-%ld]", i, _rows - 1);
  exit(-1);
  //fatalError ("MatrixDiag::indexOK","long(s) [%ld] out of allowed range [0-%ld]", i, _rows - 1);

  // fonction inutile, mais necessaire pour le compilateur
  return (false);
}

//acces aux valeurs d'une matrice
/*!
  \param i long de ligne
  \return valeur de la matrice à la ligne et colonne \c i
*/
//-----------------------------------------------------------------------------
inline double &MatrixDiag::operator()(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif

  return _data[i];
}

//acces aux valeurs d'une matrice
/*!
  \param i long de ligne
  \return valeur de la matrice à la ligne et colonne \c i
*/
//-----------------------------------------------------------------------------
inline double
MatrixDiag::operator()(long i)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif

  return _data[i];
}

#endif
