/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Matrix_h__
#define __dnlMaths_Matrix_h__

#include <Matrices.h>
#include <Vector.h>

class MatrixDiag;
class Tensor2;

//-----------------------------------------------------------------------------
// Class : Matrix
//
// Used to manage Matrix
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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
  // Constructor excluded from SWIG
#ifndef SWIG
  Matrix(int rows, int cols, double firstValue, double secondValue, ...);
#endif
  ~Matrix();

  // Interface methods excluded from SWIG
#ifndef SWIG
  double &operator()(long, long);
  friend Matrix operator*(const double &val, const Matrix &);
  friend std::ifstream &operator>>(std::ifstream &, Matrix &);
  friend std::ofstream &operator<<(std::ofstream &, const Matrix &);
  friend std::ostream &operator<<(std::ostream &, const Matrix &);
  Matrix &operator=(const double &val);
  Matrix &operator=(const Matrix &);
  Matrix &operator=(const Tensor2 &);
  void operator-=(const Matrix &);
  void operator*=(const double &);
  void operator/=(const double &);
  void operator+=(const Matrix &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool indexOK(long, long) const;
  bool operator!=(const Matrix &) const;
  bool operator==(const Matrix &) const;
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
  double operator()(long, long) const;
  double sumTerms() const;
  long Memory() const;
  Matrix getCofactor() const;
  Matrix getInverse() const;
  Matrix getNullSpace(bool = false, double = 1e-10);
  Matrix getNullSpace2(bool = false, double = 1e-10);
  Matrix getPseudoInverse(bool = false, double = 1e-10);
  Matrix getSkewSymetricPart() const;
  Matrix getSymetricPart() const;
  Matrix getTranspose() const;
  Matrix operator-() const;
  Matrix operator-(const Matrix &) const;
  Matrix operator*(const double &) const;
  Matrix operator*(const Matrix &) const;
  Matrix operator*(const MatrixDiag &) const;
  Matrix operator/(const double &) const;
  Matrix operator+(const Matrix &) const;
  Matrix singleProduct() const;
  Matrix singleProduct(const Matrix) const;
  Matrix singleProductNxT() const;
  Matrix singleProductTxN() const;
  Vector columnSum() const;
  Vector getColumn(long) const;
  Vector getRow(long) const;
  Vector getSolve(const Vector &x) const;
  Vector operator*(const Vector &) const;
  Vector rowSum() const;
  Vector trans_mult(const Vector &) const;
  void computeEigenVectors2(Vector &, Matrix &);
  void computeEigenVectors2(Vector &);
  void computeInverse2x2(double det, Matrix &inverse) const;
  void computeInverse3x3(double det, Matrix &inverse) const;
  void computePseudoInverse(Matrix &, Matrix &, bool = false, double = 1e-10);
  void computeSVD(Vector &, Matrix &, Matrix &);
  void gatherFrom(const Matrix &, long *, int);
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void outStdout() { std::cout << *this << std::endl; }
  void printOut();
  void productBy(Vector &) const;
  void redim(const long, const long);
  void scatterFrom(const Matrix &, long *, int);
  void setToUnity();
  void setToValue(double);
  void solve(Vector &x);
  void squareDivideBy(const MatrixDiag &);
  void squareMultiplyBy(const MatrixDiag &);
};

//------inline functions-------------------------------------------------------

// teste les bornes de la matrice
/*
  Cette methode teste les bornes d'une matrice
  - i long de ligne
  - j long de colonne
  Return : true si les indices fournis sont dans les bornes, false dans le cas contraire
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

// acces aux valeurs d'une matrice
/*
  - i long de ligne
  - j long de colonne
  Return : valeur de la matrice à la ligne \c i et colonne \c j
*/
//-----------------------------------------------------------------------------
inline double Matrix::operator()(long i, long j) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j);
#endif
  return _data[Ind(i, j, _rows, _cols)];
}

// acces aux valeurs d'une matrice
/*
  - i long de ligne
  - j long de colonne
  Return : valeur de la matrice à la ligne \c i et colonne \c j
*/
//-----------------------------------------------------------------------------
inline double &Matrix::operator()(long i, long j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j);
#endif
  return _data[Ind(i, j, _rows, _cols)];
}

// somme de tous les termes d'une matrice
/*
  Cette methode calcule la somme de tous les termes d'une matrice
  Return : somme de tous les termes de la matrice
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
