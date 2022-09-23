/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Vec3D_h__
#define __dnlMaths_Vec3D_h__

#include <string>
#include <string.h>
#include <dnlKernel.h>
//#include <Macros.h>

class Tensor2;
class SymTensor2;

//-----------------------------------------------------------------------------
// Class : Vec3D
//
// Used to manage Vec3D
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Vec3D
{
  friend class Tensor2;    // allows a direct access to private data for class Tensor2
  friend class SymTensor2; // allows a direct access to private data for class Tensor2

private:
  double _data[3]; // staticaly allocated vector storage
  bool indexOK(int) const;

public:
  Vec3D(double x = 0, double y = 0, double z = 0);
  Vec3D(const Vec3D &);
  ~Vec3D();

  // Interface methods excluded from SWIG
#ifndef SWIG
  double &operator()(int);
  friend std::ifstream &operator>>(std::ifstream &, Vec3D &);
  friend std::ofstream &operator<<(std::ofstream &, const Vec3D &);
  friend std::ostream &operator<<(std::ostream &, const Vec3D &);
  friend Vec3D operator*(const double, const Vec3D &);
  Vec3D &operator=(const double *);
  Vec3D &operator=(const Vec3D &);
  Vec3D &operator=(double);
  Vec3D &read(std::ifstream &);
  void operator-=(const Vec3D &);
  void operator*=(const double);
  void operator/=(const double);
  void operator+=(const Vec3D &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool isInsideBox(const Vec3D &mini, const Vec3D &maxi) const;
  bool operator!=(const Vec3D &) const;
  bool operator==(const Vec3D &) const;
  double distance(const Vec3D &) const;
  double dotProduct(const Vec3D &) const;
  double getNorm();
  double getSquareNorm();
  double innerProduct();
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  double operator()(int) const;
  double squareDistance(const Vec3D &) const;
  int getSize() const;
  SymTensor2 dyadicProduct() const;
  Tensor2 componentsProduct(const Tensor2 &) const;
  Tensor2 dyadicProduct(const Vec3D &) const;
  Vec3D componentsProduct(const Vec3D &) const;
  Vec3D getNormalized();
  Vec3D operator-() const;
  Vec3D operator-(const Vec3D &) const;
  Vec3D operator*(const double) const;
  Vec3D operator/(const double) const;
  Vec3D operator+(const Vec3D &) const;
  Vec3D vectorialProduct(const Vec3D &) const;
  void normalize();
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool initialize = false) const;
  void numpyWriteZ(std::string, std::string, bool initialize = false) const;
  void setNegativeValuesToZero();
  void setValue(double val = 0.0);
  void setValue(double x, double y, double z);
};

//------inline functions-------------------------------------------------------

//-----------------------------------------------------------------------------
inline bool Vec3D::indexOK(int i) const
//-----------------------------------------------------------------------------
{
  // test
  if ((i >= 0) && (i < 3))
    return (true);

  // here, we detected an error in the index
  fatalError("Vec3D::indexOK", "indice %d out of allowed range [0:3]", i);

  // stupid, because we'll never pass here because of the fatalError but it's to avoid a remark from the compiler
  return (false);
}

// Access to the values _data[i] of a 3D vector
/*
  - i indice inside of the vector
  Return : Value of the 3D vector _data[i]
*/
//-----------------------------------------------------------------------------
inline double &Vec3D::operator()(int i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif
  return _data[i];
}

// Access to the values _data[i] of 3D vector (Read only method)
/*
  - i indice inside of the vector
  Return : Value of the 3D vector _data[i]
*/
//-----------------------------------------------------------------------------
inline double Vec3D::operator()(int i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i);
#endif
  return _data[i];
}

// Fill a 3D vector with a scalar value
/*
  This method affect a value to a 3D vector class.

  Example :
  \code
  Vec3D v1;
  v1 = setValue(1.0); // All components of the vector are set to 1.0
  \endcode
  - val double value to give to all components of the 3D vector.
*/
//-----------------------------------------------------------------------------
inline void Vec3D::setValue(double val)
//-----------------------------------------------------------------------------
{
  _data[0] = _data[1] = _data[2] = val;
}

// Fill a 3D vector with a scalar value
/*
  This method affect a value to a 3D vector class.

  Example :
  \code
  Vec3D v1;
  v1 = setValue(1.0,2.0,3.0); // Components of the vector are set to 1.0; 2.0, 3.0
  \endcode
  \xVal double value to give to x components of the 3D vector.
  \yVal double value to give to y components of the 3D vector.
  \zVal double value to give to z components of the 3D vector.
*/
//-----------------------------------------------------------------------------
inline void Vec3D::setValue(double xVal, double yVal, double zVal)
//-----------------------------------------------------------------------------
{
  _data[0] = xVal;
  _data[1] = yVal;
  _data[2] = zVal;
}

// Size of the 3D vector
/*
  This method returns the size of the 3D vector (of course here it's always 3).
  Return : Size of the 3D vector
*/
//-----------------------------------------------------------------------------
inline int Vec3D::getSize() const
//-----------------------------------------------------------------------------
{
  return 3;
}

// Copy the content of a 3D vector into a new one
/*
  This method is the so called = operator between two 3D vectors.

  Example :
  \code
  Vec3D v1, v2;
  v1 = v2; // copy of 3D vector
  \endcode
  - vec Second 3D vector to use for the operation
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(const Vec3D &vec)
//-----------------------------------------------------------------------------
{
  memcpy(_data, vec._data, 3 * sizeof(double));
  return *this;
}

// Fill a 3D vector with a scalar value
/*
  This method is a surdefinition of the = operator for the 3D vector class.

  Example :
  \code
  Vec3D v1;
  v1 = 1.0; // All components of the vector are set to 1.0
  \endcode
  - val double value to give to all components of the 3D vector
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(double val)
//-----------------------------------------------------------------------------
{
  // recopie des valeurs
  _data[0] = _data[1] = _data[2] = val;

  // retour du vecteur
  return *this;
}

// Fill a 3D vector with a table of values
/*
  This method is a surdefinition of the equality operator used to setValue a vector with a table of values.

  Exemple :
  \code
  Vec3D v1;
  double v2[3];
  v1 = v2; // copy the values to the vector
  \endcode
*/
//-----------------------------------------------------------------------------
inline Vec3D &Vec3D::operator=(const double *vals)
//-----------------------------------------------------------------------------
{
  memcpy(_data, vals, 3 * sizeof(double));
  return *this;
}

// Addition of 2 vectors
/*
  This method defines the addition of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} + \overrightarrow{b} \f]

  Example :
  \code
  Vec3D t1, t2, t3;
  t3 = t1 + t2; // sum of the vectors
  \endcode
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
inline Vec3D Vec3D::operator+(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + vect._data[0], _data[1] + vect._data[1], _data[2] + vect._data[2]);
}

// Difference of 2 vectors
/*
  This method defines the difference of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} - \overrightarrow{b} \f]

  Example :
  \code
  Vec3D t1, t2, t3;
  t3 = t1 - t2; // difference of the vectors
  \endcode
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
inline Vec3D Vec3D::operator-(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] - vect._data[0], _data[1] - vect._data[1], _data[2] - vect._data[2]);
}

// Opposite value of a vector
/*
  This method defines the opposite of a vector.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = - \overrightarrow{a} \f]

  Example :
  \code
  Vec3D t1, t2;
  t2 = - t1; // opposite of the vector t1
  \endcode
*/
//-----------------------------------------------------------------------------
inline Vec3D Vec3D::operator-() const
//-----------------------------------------------------------------------------
{
  return Vec3D(-_data[0], -_data[1], -_data[2]);
}

// Addition of 2 3D vectors
/*
  This method defines the addition of 2 3D vectors.

  Example :
  \code
  Vec3D t1,t2;
  t2 += t1; // sum of two 3D vectors
  \endcode
  - vect Second vector to add
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator+=(const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  _data[0] += vect._data[0];
  _data[1] += vect._data[1];
  _data[2] += vect._data[2];
}

// Difference of 2 3D vectors
/*
  This method defines the difference of 2 3D vectors.

  Example :
  \code
  Vec3D t1,t2;
  t2 -= t1; // sum of two 3D vectors
  \endcode
  - vect Second vector to add
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator-=(const Vec3D &vect)
//-----------------------------------------------------------------------------
{
  _data[0] -= vect._data[0];
  _data[1] -= vect._data[1];
  _data[2] -= vect._data[2];
}

// Multiplication of a 3D vector by a scalar value
/*
  This method defines the multiplication of a 3D vector by a scalar value

  Example :
  \code
  Vec3D t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator*=(const double lambda)
//-----------------------------------------------------------------------------
{
  _data[0] *= lambda;
  _data[1] *= lambda;
  _data[2] *= lambda;
}

// Division of a 3D vector by a scalar value
/*
  This method defines the division of a 3D vector by a scalar value

  Example :
  \code
  Tensor2 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  - lambda Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Vec3D::operator/=(const double lambda)
//-----------------------------------------------------------------------------
{
  if (lambda == 0.0)
  {
    fatalError("Vec3D:: operator /=", "divide by zero");
  }
  _data[0] /= lambda;
  _data[1] /= lambda;
  _data[2] /= lambda;
}

// Returns the getJ2 norm of a 3D vector
/*
  This method returns getJ2 norm of a 3D vector defined by:
  \f[ \left\Vert \overrightarrow{_data} \right\Vert  = \sqrt {v_{1}^2 + v_{2}^2 + v_{3}^2} \f]
  Return : getJ2 norm of a 3D vector
*/
//-----------------------------------------------------------------------------
inline double Vec3D::getNorm()
//-----------------------------------------------------------------------------
{
  return sqrt(dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]));
}

//-----------------------------------------------------------------------------
inline double Vec3D::getSquareNorm()
//-----------------------------------------------------------------------------
{
  return dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]);
}

// Returns the inner product of a 3D vector by itself
/*
  This method returns inner product of a 3D vector by itself defined by:
  \f[ \left\Vert \overrightarrow{_data} \right\Vert  = v_{1}^2 + v_{2}^2 + v_{3}^2 \f]
  Return : inner product of a 3D vector by itself
*/
//-----------------------------------------------------------------------------
inline double Vec3D::innerProduct()
//-----------------------------------------------------------------------------
{
  return (dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]));
}

// Normalization of a 3D vector
/*
  This method modifies the given vector and makes its norm equal to 1.0
*/
//-----------------------------------------------------------------------------
inline void Vec3D::normalize()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double norm = getNorm();

  // recalcul des composantes
  _data[0] /= norm;
  _data[1] /= norm;
  _data[2] /= norm;
}

// Nomalized 3D vector
/*
  This method returns an colinear vector with a unary norm.
  Return : colinear vector with a norm equal to 1.
*/
//-----------------------------------------------------------------------------
inline Vec3D Vec3D::getNormalized()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double norm = getNorm();

  return Vec3D(_data[0] / norm, _data[1] / norm, _data[2] / norm);
}

// Sets all negative components to zero
/*
  This method sets all negative components of a vector equal zero
*/
//-----------------------------------------------------------------------------
inline void Vec3D::setNegativeValuesToZero()
//-----------------------------------------------------------------------------
{
  if (_data[0] < 0.)
    _data[0] = 0.;
  if (_data[1] < 0.)
    _data[1] = 0.;
  if (_data[2] < 0.)
    _data[2] = 0.;
}

// Distance between two points
/*
  This method computes the distance between two points using an Euclidian getNorm.
  - vect Second vector to use
  \f[ d = \left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert \f]
  Return : Distance between both points
*/
//-----------------------------------------------------------------------------
inline double Vec3D::distance(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  Vec3D x = vect - *this;
  return x.getNorm();
}

// Square value of the distance between two points
/*
  This method computes the distance between two points using an Euclidian getNorm and returns the square value of this distance.
  - vect Second vector to use
  \f[ d = {\left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert}^2 \f]
  Return : Square value of the distance between two points
*/
//-----------------------------------------------------------------------------
inline double Vec3D::squareDistance(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
  Vec3D x = vect - *this;
  return x.innerProduct();
}

#endif
