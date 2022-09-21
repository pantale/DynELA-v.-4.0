/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Tensor4_h__
#define __dnlMaths_Tensor4_h__

#include <string>
#include <string.h>
#include <dnlKernel.h>
//#include <Macros.h>

class Vec3D;
class Tensor2;
class Tensor3;

//-----------------------------------------------------------------------------
// Class : Tensor4
//
// Used to manage Tensor4
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Tensor4
{
  double _data[81]; // Data storage for 81 double

public:
  // constructeurs
  Tensor4();
  ~Tensor4();

  // Interface methods excluded from SWIG
#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Tensor4 &);
  friend std::ofstream &operator<<(std::ofstream &, const Tensor4 &);
  friend std::ostream &operator<<(std::ostream &, const Tensor4 &);
  friend Tensor4 operator*(const double &, const Tensor4 &);
  inline double &operator()(short, short, short, short);
  inline void operator-=(const Tensor4 &);
  inline void operator*=(const double);
  inline void operator/=(const double);
  inline void operator+=(const Tensor4 &);
  Tensor4 &operator=(const double &);
  Tensor4 &operator=(const Tensor4 &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool operator!=(const Tensor4 &) const;
  bool operator==(const Tensor4 &) const;
  inline bool indexOK(short, short, short, short) const;
  inline double operator()(short, short, short, short) const;
  inline void setToValue(const double);
  Tensor2 operator*(const Tensor2 &t) const;
  Tensor3 operator*(const Vec3D &vec) const;
  Tensor4 operator-(const Tensor4 &) const;
  Tensor4 operator*(const double &) const;
  Tensor4 operator/(const double &) const;
  Tensor4 operator+(const Tensor4 &) const;
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void setToUnity();
};

//------inline functions-------------------------------------------------------

/*
  tests if the couple of indexes is ok
*/
//-----------------------------------------------------------------------------
inline bool Tensor4::indexOK(short i, short j, short k, short l) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3) && (k >= 0) && (k < 3) && (l >= 0) && (l < 3))
    return (true);

  printf("Tensor4::indexOK\nindice(s) [%d,%d,%d,%d] out of allowed range [0-2,0-2,0-2,0-2]",
         i, j, k, l);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (false);
}

/*
  Access to the values T[i,j,k,l] of a fourth order tensor

  Return : Value of the fourth order tensor T[i,j,k,l]
*/
//-----------------------------------------------------------------------------
inline double &Tensor4::operator()(short i, short j, short k, short l)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j, k, l);
#endif
  return _data[dnlTensor4Ind(i, j, k, l, 3)];
}

/*
  Access to the values T[i,j,k,l] of a fourth order tensor

  Return : Value of the fourth order tensor T[i,j,k,l]
*/
//-----------------------------------------------------------------------------
inline double Tensor4::operator()(short i, short j, short k, short l) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j, k, l);
#endif
  return _data[dnlTensor4Ind(i, j, k, l, 3)];
}

/*
  Fill a fourth order tensor with a scalar value

  This method is a surdefinition of the = operator for the fourth order tensor class.
  \code
  Tensor4 t1;
  t1 = setToValue(1.0); // All components of the tensor are set to 1.0
  \endcode
  - val double value to give to all components of the fourth order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor4::setToValue(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] = val;
}

/*
  Addition of 2 fourth order tensors

  This method defines the addition of 2 fourth order tensors.
  \code
  Tensor4 t1,t2;
  t2 += t1; // sum of two fourth order tensors
  \endcode
  - tensor fourth order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator+=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  for (short i = 0; i < 81; i++)
    _data[i] += tens._data[i];
}

/*
  Difference of 2 fourth order tensors

  This method defines the difference of 2 fourth order tensors.
  \code
  Tensor4 t1,t2;
  t2 -= t1; // difference of two fourth order tensors
  \endcode
  - tensor fourth order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator-=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  for (short i = 0; i < 81; i++)
    _data[i] -= tens._data[i];
}

/*
  Multiplication of a fourth order tensor by a scalar value

  This method defines the multiplication of a fourth order tensor by a scalar value
  \code
  Tensor4 t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  - val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator*=(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] *= val;
}

/*
  Division of a fourth order tensor by a scalar value

  This method defines the division of a fourth order tensor by a scalar value
  \code
  Tensor4 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  - val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator/=(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] /= val;
}

#endif
