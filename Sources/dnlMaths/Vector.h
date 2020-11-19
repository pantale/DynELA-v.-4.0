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
  \file Vector.h
  \brief Declaration file for the vector class

  This file is the declaration file for the vector class. A vector class is a vector with the following form:
  \f[ \overrightarrow{_data}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  ...\\
  v_{n}
  \end{array}\right] \f]

  \ingroup dnlMaths
*/

#ifndef __dnlMaths_Vector_h__
#define __dnlMaths_Vector_h__

#include <string>
#include <string.h>
#include <Errors.h>
#include <Macros.h>

/*
  \enum OutVector
  \brief enumeration de format d'affichage de vecteurs.
  Cette enumeration permet de definir le format d'affichage des vecteurs. On a ainsi le choix entre outVectorTypeNormal qui correspond à un affichage conforme à Mathematica et outVectorTypeMupad qui correspond à un affichage au format Mupad.
*/
#ifndef SWIG
enum OutVector
{
  outVectorTypeNormal = 0, // format Mathematica
  outVectorTypeMupad       // format Mupad
};
class Matrix;
#endif

/*!
  \class Vector
  \brief Declaration of the vector class

  A vector class is a vector with the following form:
  \f[ \overrightarrow{V}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  ...\\
  v_{n}
  \end{array}\right] \f]
  This type of data structure is useful for storing three-dimensional coordinates (for example nodal coordinates, vectors forces,...).

  \ingroup dnlMaths
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
  Vector(long = 3, double = 0.0);
  Vector(const Vector &);
#ifndef SWIG
  Vector(int vectorLength, double firstValue, double secondValue, ...);
#endif
  ~Vector();

  bool operator!=(const Vector &) const;
  bool operator==(const Vector &) const;
  double operator()(long) const;
  Vector operator-() const;
  Vector operator-(const Vector &) const;
  Vector operator*(const double) const;
  Vector operator/(const double) const;
  Vector operator+(const Vector &) const;

#ifndef SWIG
  double &operator()(long);
  friend Vector operator*(const double, const Vector &);
  Vector &operator=(const double *);
  Vector &operator=(const Vector &);
  Vector &operator=(double);
  void operator-=(const Vector &);
  void operator*=(const double);
  void operator/=(const double);
  void operator+=(const Vector &);
#endif

  bool indexOK(long) const;
  double distance(const Vector &) const;
  double dotProduct(const Vector &) const;
  double getNorm();
  double innerProduct();
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  double squareDistance(const Vector &) const;
  long getSize() const;
  Matrix dyadicProduct();
  Matrix dyadicProduct(const Vector &);
  Vector getNormalized();
  Vector vectorialProduct(const Vector &) const;
  void gatherFrom(const Vector &, long *, int);
  void normalize();
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void printOut();
  void resizeVector(const long);
  void redim(const long);
  void scatterFrom(const Vector &, long *, int);
  void setOutType(char);
  void setToValue(double);
  void swapWith(Vector &);

#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Vector &);
  friend std::ofstream &operator<<(std::ofstream &, const Vector &);
  friend std::ostream &operator<<(std::ostream &, const Vector &);
  Vector &read(std::ifstream &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif
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
/*!
  \param i indice inside of the vector
  \return Value of the 3D vector _data[i]
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
/*!
  \param i indice inside of the vector
  \return Value of the 3D vector _data[i]
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
/*!
  This method returns the size of the vector.
  \return Size of the vector
*/
//-----------------------------------------------------------------------------
inline long Vector::getSize() const
//-----------------------------------------------------------------------------
{
  return _dataLength;
}

//Selection of the output display type
/*!
  This method allows you to select the type of display requested. The output type is defined by the variables listed in \ref OutVector.
  \param outT Type of the output
*/
//-----------------------------------------------------------------------------
inline void Vector::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
