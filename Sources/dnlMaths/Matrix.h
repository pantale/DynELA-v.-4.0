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
  \file Matrix.h
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

#ifndef __dnlMaths_Matrix_h__
#define __dnlMaths_Matrix_h__

#include <Matrices.h>
#include <Vector.h>

class MatrixDiag;
class Tensor2;

/*!
  \class Matrix Matrix.h
  \brief Classe de gestion et manipulation des matrices denses.
  \ingroup dnlMaths

  Cette classe est utilisee pour la gestion et la manipulation des matrices denses. Elle comporte toutes les methodes d'initialisation et de gestion memoire. Les valeurs stockees sont des double. La classe \c Matrix contient de plus un certain nombre de methodes de calcul sur les matrices comme decrit ci-dessous.
*/
class Matrix : public Matrices
{
  friend class Vector;

  unsigned long _dataLength;
  double *_data;

  void allocate(const long, const long);
  void desallocate();

public:
  Matrix(const long = 3, const long = 3, const double = 0.0);
  Matrix(const Matrix &);
#ifndef SWIG
  Matrix(int rows, int cols, double firstValue, double secondValue, ...);
#endif
  ~Matrix();

  bool operator!=(const Matrix &) const;
  bool operator==(const Matrix &) const;
  double operator()(long, long) const;
  Matrix operator-(const Matrix &) const;
  Matrix operator*(const double &) const;
  Matrix operator*(const Matrix &) const;
  Matrix operator*(const MatrixDiag &) const;
  Matrix operator/(const double &) const;
  Matrix operator+(const Matrix &) const;
  Vector operator*(const Vector &) const;
  Matrix operator-() const;

#ifndef SWIG
  double &operator()(long, long);
  friend Matrix operator*(const double &val, const Matrix &);
  Matrix &operator=(const double &val);
  Matrix &operator=(const Matrix &);
  Matrix &operator=(const Tensor2 &);
  void operator-=(const Matrix &);
  void operator*=(const double &);
  void operator/=(const double &);
  void operator+=(const Matrix &);
#endif

  bool indexOK(long, long) const;
  double doubleProduct() const;
  double doubleProduct(const Matrix) const;
  double getAverageTrace() const;
  double getDeterminant() const;
  double getDeterminant2x2() const;
  double getDeterminant3x3() const;
  double getTrace() const;
  double maxAbsoluteValue() const;
  double maxValue() const;
  double minAbsoluteValue() const;
  double minValue() const;
  double sumTerms() const;
  Matrix getCofactor() const;
  Matrix getInverse() const;
  void computeInverse2x2(double det, Matrix &inverse) const;
  void computeInverse3x3(double det, Matrix &inverse) const;
  Matrix getNullSpace(bool = false, double = 1e-10);
  Matrix getNullSpace2(bool = false, double = 1e-10);
  Matrix getPseudoInverse(bool = false, double = 1e-10);
  Matrix getSkewSymetricPart() const;
  Matrix getSymetricPart() const;
  Matrix getTranspose() const;
  Matrix singleProduct() const;
  Matrix singleProduct(const Matrix) const;
  Matrix singleProductNxT() const;
  Matrix singleProductTxN() const;
  Vector columnSum() const;
  Vector getColumn(long) const;
  Vector getRow(long) const;
  Vector getSolve(const Vector &x) const;
  Vector rowSum() const;
  Vector trans_mult(const Vector &) const;
  void computeSVD(Vector &, Matrix &, Matrix &);
  void computeEigenVectors2(Vector &, Matrix &);
  void computeEigenVectors2(Vector &);
  void computePseudoInverse(Matrix &, Matrix &, bool = false, double = 1e-10);
  void gatherFrom(const Matrix &, long *, int);
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void printOut();
  void productBy(Vector &) const;
  void redim(const long, const long);
  void scatterFrom(const Matrix &, long *, int);
  void setToUnity();
  void setToValue(double);
  void solve(Vector &x);
  void squareDivideBy(const MatrixDiag &);
  void squareMultiplyBy(const MatrixDiag &);

#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Matrix &);
  friend std::ofstream &operator<<(std::ofstream &, const Matrix &);
  friend std::ostream &operator<<(std::ostream &, const Matrix &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif

  long Memory() const;
  void outStdout() { std::cout << *this << std::endl; }
};

//------inline functions-------------------------------------------------------

//teste les bornes de la matrice
/*!
  Cette methode teste les bornes d'une matrice
  \param i long de ligne
  \param j long de colonne
  \return true si les indices fournis sont dans les bornes, false dans le cas contraire
*/
//-----------------------------------------------------------------------------
inline bool Matrix::indexOK(long i, long j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < _rows) && (j >= 0) && (j < _cols))
    return (true);

  printf("Matrix::indexOK\nindice(s) [%ld,%ld] out of allowed range [0-%ld,0-%ld]", i, j, _rows - 1, _cols - 1);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (false);
}

//acces aux valeurs d'une matrice
/*!
  \param i long de ligne
  \param j long de colonne
  \return valeur de la matrice à la ligne \c i et colonne \c j
*/
//-----------------------------------------------------------------------------
inline double Matrix::operator()(long i, long j) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return _data[Ind(i, j, _rows, _cols)];
}

//acces aux valeurs d'une matrice
/*!
  \param i long de ligne
  \param j long de colonne
  \return valeur de la matrice à la ligne \c i et colonne \c j
*/
//-----------------------------------------------------------------------------
inline double &Matrix::operator()(long i, long j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return _data[Ind(i, j, _rows, _cols)];
}

//somme de tous les termes d'une matrice
/*!
  Cette methode calcule la somme de tous les termes d'une matrice
  \return somme de tous les termes de la matrice
*/
//-----------------------------------------------------------------------------
inline double Matrix::sumTerms() const
//-----------------------------------------------------------------------------
{
  double val = 0;
  for (long i = 0; i < _cols * _rows; i++)
    val += _data[i];
  return val;
}

#endif
