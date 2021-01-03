/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_MatrixDiag_h__
#define __dnlMaths_MatrixDiag_h__

#include <Matrices.h>
#include <Vector.h>

//-----------------------------------------------------------------------------
// Class : MatrixDiag
//
// Used to manage MatrixDiag
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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

  // Interface methods excluded from SWIG
#ifndef SWIG
  double &operator()(long);
  friend MatrixDiag operator*(const double &, const MatrixDiag &);
  friend std::ifstream &operator>>(std::ifstream &, MatrixDiag &);
  friend std::ofstream &operator<<(std::ofstream &, const MatrixDiag &);
  friend std::ostream &operator<<(std::ostream &, const MatrixDiag &);
  MatrixDiag &operator=(const double &);
  MatrixDiag &operator=(const MatrixDiag &);
  void operator-=(const MatrixDiag &);
  void operator+=(const MatrixDiag &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
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
  Vector operator*(const Vector &) const;
  void divideBy(Vector &) const;
  void gatherFrom(const MatrixDiag &, long *, int);
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void productBy(Vector &) const;
  void redim(const long newSize);
  void scatterFrom(const MatrixDiag &, long *, int);
  void setToUnity();
  void setToValue(double);
  void solve(Vector &);
};

//------inline functions-------------------------------------------------------

//teste les bornes de la matrice
/*
  Cette methode teste les bornes d'une matrice
  - i long de ligne
  Return : true si l'long fourni est dans les bornes, false dans le cas contraire
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
/*
  - i long de ligne
  Return : valeur de la matrice à la ligne et colonne \c i
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
/*
  - i long de ligne
  Return : valeur de la matrice à la ligne et colonne \c i
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
