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
#include <Errors.h>
#include <Macros.h>

  // Interface methods excluded from SWIG
#ifndef SWIG
enum OutVector
{
  outVectorTypeNormal = 0, // format Mathematica
  outVectorTypeMupad       // format Mupad
};
class Matrix;
#endif

//-----------------------------------------------------------------------------
// Class : Vector
//
// Used to manage Vector
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
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
  Vector(long = 3, double = 0.0);
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
  Vector(int vectorLength, double firstValue, double secondValue, ...);
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
  double dotProduct(const Vector &) const;
  double getNorm();
  double innerProduct();
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  double operator()(long) const;
  double squareDistance(const Vector &) const;
  long getSize() const;
  Matrix dyadicProduct();
  Matrix dyadicProduct(const Vector &);
  Vector getNormalized();
  Vector operator-() const;
  Vector operator-(const Vector &) const;
  Vector operator*(const double) const;
  Vector operator/(const double) const;
  Vector operator+(const Vector &) const;
  Vector vectorialProduct(const Vector &) const;
  void gatherFrom(const Vector &, long *, int);
  void normalize();
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void printOut();
  void redim(const long);
  void resizeVector(const long);
  void scatterFrom(const Vector &, long *, int);
  void setOutType(char);
  void setToValue(double);
  void swapWith(Vector &);
};

//------inline functions-------------------------------------------------------

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

//Access to the values _data[i] of 3D vector
/*
  - i indice inside of the vector
  Return : Value of the 3D vector _data[i]
*/
//-----------------------------------------------------------------------------
inline double &Vector::operator()(long i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return _data[i];
}

//Access to the values _data[i] of 3D vector (Read only method)
/*
  - i indice inside of the vector
  Return : Value of the 3D vector _data[i]
*/
//-----------------------------------------------------------------------------
inline double Vector::operator()(long i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i);
#endif
  return _data[i];
}

//Size of the vector
/*
  This method returns the size of the vector.
  Return : Size of the vector
*/
//-----------------------------------------------------------------------------
inline long Vector::getSize() const
//-----------------------------------------------------------------------------
{
  return _dataLength;
}

//Selection of the output display type
/*
  This method allows you to select the type of display requested. The output type is defined by the variables listed in \ref OutVector.
  - outT Type of the output
*/
//-----------------------------------------------------------------------------
inline void Vector::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
