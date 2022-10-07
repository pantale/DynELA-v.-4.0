/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_MatrixDiag_h__
#define __dnlMaths_MatrixDiag_h__

#include <Matrices.h>
#include <Vector.h>

/*
@LABEL:MatrixDiag::MatrixDiag
@SHORT:MatrixDiag class.
This class is used to store information for diagonal matrices.
A MatrixDiag class is a two dimensional square object with size $n\times n$ with the following form:
\begin{equation*}
\M=\left[\begin{array}{cccc}
  M_{11} & 0 & \hdots & 0\\
  0 & M_{22} & \hdots & 0\\
  \vdots & \vdots & \hdots & \vdots\\
  0 & 0 & \hdots & M_{nn}
  \end{array}\right]
\end{equation*}
The storage scheme is the same as a vector, since only the diagonal of the matrix is effectively stored into memory.
But the nature of the operators conform to a matrix.
@END
*/
class MatrixDiag : public Matrices
{
  friend class Matrix;
  friend class Vector;

  double *_data;

  void allocate(const long);
  void deallocate();

public:
  MatrixDiag(const long = 3, double = 0.0);
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
  double maxAbs();
  double maxVal();
  double minAbs();
  double minVal();
  double trace();
  long Memory() const;
  MatrixDiag dot(const MatrixDiag &) const;
  MatrixDiag inverse() const;
  MatrixDiag transpose();
  Vector getSolve(Vector &);
  Vector operator*(const Vector &) const;
  void divideBy(Vector &) const;
  void dot(Vector &) const;
  void gatherFrom(const MatrixDiag &, long *, int);
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void redim(const long);
  void scatterFrom(const MatrixDiag &, long *, int);
  void setToUnity();
  void setValue(double);
  void solve(Vector &);
};

//------inline functions-------------------------------------------------------

//  tests if the index is ok
//-----------------------------------------------------------------------------
inline bool MatrixDiag::indexOK(long i) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < _rows))
    return (true);

  printf("MatrixDiag::indexOK\nindice(s) [%ld] out of allowed range [0-%ld]", i, _rows - 1);
  exit(-1);
  // fatalError ("MatrixDiag::indexOK","long(s) [%ld] out of allowed range [0-%ld]", i, _rows - 1);

  // fonction inutile, mais necessaire pour le compilateur
  return (false);
}

// Access to the values _data[i,j] of matrix
//-----------------------------------------------------------------------------
inline double &MatrixDiag::operator()(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif

  return _data[i];
}

// Access to the values _data[i,j] of matrix (Read only method)
//-----------------------------------------------------------------------------
inline double MatrixDiag::operator()(long i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif

  return _data[i];
}

#endif
