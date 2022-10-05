/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Vector_h__
#define __dnlMaths_Vector_h__

#include <string>
#include <string.h>
#include <dnlKernel.h>

// Interface methods excluded from SWIG
#ifndef SWIG
enum OutVector
{
  outVectorTypeNormal = 0, // format Mathematica
  outVectorTypeMupad       // format Mupad
};
class Matrix;
#endif

/*
@LABEL:Vector::Vector
@SHORT:Vector class.
This class is used to store information for Vectors.
This file is the declaration file for the Vector class. A Vector class is a vector with the following form:
\begin{equation*}
\overrightarrow{x}=\left[\begin{array}{c}
  x_{1}\\
  x_{2}\\
  \vdots\\
  x_{n}
  \end{array}\right]
\end{equation*}
@END
*/
class Vector
{
  friend class Matrix;
  friend class MatrixSparse;
  friend class MatrixDiag;

  unsigned long _dataLength;
  double *_data;
  char outType = outVectorTypeNormal;
  void allocate(const long);
  void desallocate();

public:
  Vector(long = 3, double = 0);
  Vector(const Vector &);
  ~Vector();

  // Interface methods excluded from SWIG
#ifndef SWIG
  double &operator()(long);
  friend std::ifstream &operator>>(std::ifstream &, Vector &);
  friend std::ofstream &operator<<(std::ofstream &, const Vector &);
  friend std::ostream &operator<<(std::ostream &, const Vector &);
  friend Vector operator*(const double, const Vector &);
  Vector &operator=(const double *);
  Vector &operator=(const Vector &);
  Vector &operator=(double);
  Vector &read(std::ifstream &);
  Vector(int, double, double, ...);
  void operator-=(const Vector &);
  void operator*=(const double);
  void operator/=(const double);
  void operator+=(const Vector &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool indexOK(long) const;
  bool operator!=(const Vector &) const;
  bool operator==(const Vector &) const;
  double distance(const Vector &) const;
  double dot();
  double dot(const Vector &) const;
  double getMean() const;
  double getSum() const;
  double maxAbs();
  double maxVal();
  double minAbs();
  double minVal();
  double norm();
  double operator()(long) const;
  double squareDistance(const Vector &) const;
  long size() const;
  Matrix dyadic();
  Matrix dyadic(const Vector &);
  Vector ewAddReal(const double);
  Vector ewExp();
  Vector ewInverse();
  Vector ewProduct(const Vector &);
  Vector ewSquare();
  Vector getNormalized();
  Vector operator-() const;
  Vector operator-(const Vector &) const;
  Vector operator*(const double) const;
  Vector operator/(const double) const;
  Vector operator+(const Vector &) const;
  Vector vectorProduct(const Vector &) const;
  void gatherFrom(const Vector &, long *, int);
  void normalize();
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void printOut();
  void redim(const long);
  void resize(const long);
  void scatterFrom(const Vector &, long *, int);
  void setOutType(char);
  void setValue(double);
  void swap(Vector &);
};

//------inline functions-------------------------------------------------------

//  tests if the index is ok
//-----------------------------------------------------------------------------
inline bool Vector::indexOK(long i) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < _dataLength))
    return (true);

  // ici, on a detecte une erreur dans les indices
  fatalError("Vector::indexOK", "indice %d out of allowed range [0-%ld]", i, _dataLength - 1);

  // idiot, car on ne passera jamais ici a cause du fatalError
  // mais c'est pour eviter une remarque du compilateur
  return (false);
}

// Access to the values _data[i] of 3D vector
//-----------------------------------------------------------------------------
inline double &Vector::operator()(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif
  return _data[i];
}

// Access to the values _data[i] of 3D vector (Read only method)
//-----------------------------------------------------------------------------
inline double Vector::operator()(long i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif
  return _data[i];
}

/*
@LABEL:Vector::size()
@SHORT:Returns the length of the vector.
@RETURN:long : Length of the vector.
This method returns the length of the vector, \ie the number of components of the vector.
@END
*/
//-----------------------------------------------------------------------------
inline long Vector::size() const
//-----------------------------------------------------------------------------
{
  return _dataLength;
}

// Selection of the output display type
//-----------------------------------------------------------------------------
inline void Vector::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

//-----------------------------------------------------------------------------
inline double Vector::getSum() const
//-----------------------------------------------------------------------------
{
  double val = 0;
  for (long i = 0; i < _dataLength; i++)
    val += _data[i];
  return val;
}

//-----------------------------------------------------------------------------
inline double Vector::getMean() const
//-----------------------------------------------------------------------------
{
  double val = 0;
  for (long i = 0; i < _dataLength; i++)
    val += _data[i];
  return val / _dataLength;
}

#endif
