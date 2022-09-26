/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_SymTensor2_h__
#define __dnlMaths_SymTensor2_h__

#include <string>
#include <string.h>
#include <dnlKernel.h>
//#include <Macros.h>

class Vec3D;
class Tensor2;

//-----------------------------------------------------------------------------
// Class : SymTensor2
//
// Used to manage SymTensor2
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class SymTensor2
{
  static const Tensor2Index _internalIndexes;
  friend class Vec3D;
  friend class Tensor2;

  double _data[6]; // Data storage for 6 double

private:
  bool indexOK(short, short) const;
  void buildFTF(double FTF[3][3]) const;
  void polarExtract(double eigenVectors[3][3], double eigenValues[3], SymTensor2 &U, Tensor2 &R) const;
  void polarExtractLnU(double eigenVectors[3][3], double eigenValues[3], SymTensor2 &U, Tensor2 &R) const;

public:
  SymTensor2();
  SymTensor2(double, double, double, double, double, double);
  SymTensor2(const SymTensor2 &);
  ~SymTensor2();

  // Interface methods excluded from SWIG
#ifndef SWIG
  double &operator()(short, short);
  friend std::ifstream &operator>>(std::ifstream &, SymTensor2 &);
  friend std::ofstream &operator<<(std::ofstream &, const SymTensor2 &);
  friend std::ostream &operator<<(std::ostream &, const SymTensor2 &);
  friend SymTensor2 operator*(const double &, const SymTensor2 &);
  SymTensor2 &operator=(const double &);
  SymTensor2 &operator=(const SymTensor2 &);
  void operator-=(const SymTensor2 &);
  void operator*=(const double);
  void operator/=(const double);
  void operator+=(const SymTensor2 &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool operator!=(const SymTensor2 &) const;
  bool operator==(const SymTensor2 &) const;
  double doubleProduct() const;
  double doubleProduct(const SymTensor2) const;
  double getDeterminant() const;
  double getJ2() const;
  double getMisesEquivalent() const;
  double getNorm() const;
  double getThirdTrace() const;
  double getTrace() const;
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  double operator()(short, short) const;
  SymTensor2 getCofactor() const;
  SymTensor2 getDeviator() const;
  SymTensor2 getInverse() const;
  SymTensor2 getMinor() const;
  /*   SymTensor2 getSkewSymetricPart() const;
    SymTensor2 getSymetricPart() const; */
  // SymTensor2 getTranspose() const;
  SymTensor2 operator-() const;
  SymTensor2 operator-(const SymTensor2 &) const;
  SymTensor2 operator*(const double) const;
  SymTensor2 operator/(const double) const;
  SymTensor2 operator+(const SymTensor2 &) const;
  SymTensor2 productByRTxR(const Tensor2 R) const;
  SymTensor2 productByRxRT(const Tensor2 R) const;
  SymTensor2 singleProduct() const;
  Tensor2 operator*(const SymTensor2 &) const;
  Tensor2 operator*(const Tensor2 &) const;
  Tensor2 singleProduct(const SymTensor2) const;
  Vec3D columnSum() const;
  Vec3D getColumn(short) const;
  Vec3D getRow(short) const;
  Vec3D operator*(const Vec3D &) const;
  Vec3D rowSum() const;
  Vec3D solve(const Vec3D &) const;
  void computeDeviator(SymTensor2 &deviator, double &pressure) const;
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool initialize = false) const;
  void numpyWriteZ(std::string, std::string, bool initialize = false) const;
  void polar(SymTensor2 &, Tensor2 &) const;
  void polarQL(SymTensor2 &U, Tensor2 &R) const;
  void polarJacobi(SymTensor2 &U, Tensor2 &R) const;
  void polarCuppen(SymTensor2 &U, Tensor2 &R) const;
  void polarLapack(SymTensor2 &U, Tensor2 &R) const;
  void polarLnU(SymTensor2 &, Tensor2 &) const;
  void polarQLLnU(SymTensor2 &U, Tensor2 &R) const;
  void polarJacobiLnU(SymTensor2 &U, Tensor2 &R) const;
  void polarCuppenLnU(SymTensor2 &U, Tensor2 &R) const;
  void polarLapackLnU(SymTensor2 &U, Tensor2 &R) const;
  void setToUnity();
  void setToValue(const double);
  void setToZero();
};

//------inline functions-------------------------------------------------------

/*
  tests if the couple of _internalIndexes is ok
*/
//-----------------------------------------------------------------------------
inline bool SymTensor2::indexOK(short i, short j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3))
  {
    return (true);
  }

  printf("SymTensor2::indexOK\nindice(s) [%d,%d] out of allowed range [0-2,0-2]", i, j);
  exit(-1);

  // no sense
  return (false);
}

/*
  Access to the values T[i,j] of a symmetric second order tensor

  - i short for row
  - j short for column
  Return : Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline double &SymTensor2::operator()(short i, short j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j);
#endif
  return _data[_internalIndexes.index[i][j]];
}

/*
  Access to the values T[i,j] of a symmetric second order tensor (Read only method)

  - i short for row
  - j short for column
  Return : Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::operator()(short i, short j) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j);
#endif
  return _data[_internalIndexes.index[i][j]];
}

/*
  Fill a symmetric second order tensor with a scalar value

  This method is a surdefinition of the = operator for the second order tensor class.
  \code
  SymTensor2 t1;
  t1 = setToValue(1.0); // All components of the tensor are set to 1.0
  \endcode
  - val double value to give to all components of the second order tensor
*/
//-----------------------------------------------------------------------------
inline void SymTensor2::setToValue(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] = val;
  _data[1] = val;
  _data[2] = val;
  _data[3] = val;
  _data[4] = val;
  _data[5] = val;
}

/*
  Addition of 2 second order tensors
  This method defines the addition of 2 second order tensors.

  \code
  SymTensor2 t1,t2;
  t2 += t1; // sum of two second order tensors
  \endcode
  - tensor Second order tensor to add
*/
//-----------------------------------------------------------------------------
inline void SymTensor2::operator+=(const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  _data[0] += tensor._data[0];
  _data[1] += tensor._data[1];
  _data[2] += tensor._data[2];
  _data[3] += tensor._data[3];
  _data[4] += tensor._data[4];
  _data[5] += tensor._data[5];
}

/*
  Difference of 2 second order tensors

  This method defines the difference of 2 second order tensors.
  \code
  SymTensor2 t1,t2;
  t2 -= t1; // difference of two second order tensors
  \endcode
  - tensor Second order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void SymTensor2::operator-=(const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  _data[0] -= tensor._data[0];
  _data[1] -= tensor._data[1];
  _data[2] -= tensor._data[2];
  _data[3] -= tensor._data[3];
  _data[4] -= tensor._data[4];
  _data[5] -= tensor._data[5];
}

/*
  Multiplication of a symmetric second order tensor by a scalar value

  This method defines the multiplication of a symmetric second order tensor by a scalar value
  \code
  SymTensor2 t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  - val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void SymTensor2::operator*=(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] *= val;
  _data[1] *= val;
  _data[2] *= val;
  _data[3] *= val;
  _data[4] *= val;
  _data[5] *= val;
}

/*
  Division of a symmetric second order tensor by a scalar value

  This method defines the division of a symmetric second order tensor by a scalar value

  \code
  SymTensor2 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  - val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void SymTensor2::operator/=(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] /= val;
  _data[1] /= val;
  _data[2] /= val;
  _data[3] /= val;
  _data[4] /= val;
  _data[5] /= val;
}

/*
@LABEL:SymTensor2::getTrace()
@SHORT:Returns the trace of a symmetric second order tensor.
@RETURN:double
  This method returns the trace of a symmetric second order tensor, i.e. the sum of all the terms of the diagonal:
\begin{equation*}
v = tr[\T] = T_{11}+T_{22}+T_{33}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getTrace() const
//-----------------------------------------------------------------------------
{
  return (_data[0] + _data[3] + _data[5]);
}

/*
@LABEL:SymTensor2::getThirdTrace()
@SHORT:Returns the average value of the trace of a symmetric second order tensor.
@RETURN:double
This method returns average value of the trace of a symmetric second order tensor, i.e. the sum of all the terms of the diagonal divided by 3:
\begin{equation*}
v = \frac{1}{3} tr[\T] =  \frac{1}{3} \left( T_{11}+T_{22}+T_{33} \right)
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getThirdTrace() const
//-----------------------------------------------------------------------------
{
  return (_data[0] + _data[3] + _data[5]) / 3.0;
}

/*
@LABEL:SymTensor2::getMisesEquivalent()
@SHORT:Returns the von Mises stress of a symmetric second order tensor.
@RETURN:double
This method returns the von Mises stress of a symmetric second order tensor defined by:
\begin{equation*}
\overline{\sigma} = \frac {1}{\sqrt{2}}\sqrt{(s_{11}-s_{22})^2+(s_{22}-s_{33})^2+(s_{33}-s_{11})^2+6(s_{12}^2+s_{23}^2+s_{31}^2)}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getMisesEquivalent() const
//-----------------------------------------------------------------------------
{
  return (dnlVonMises(_data[0], _data[3], _data[5], _data[1], _data[2], _data[4]));
}

/*
@LABEL:SymTensor2::getNorm()
@SHORT:Norm of a symmetric second order tensor.
@RETURN:double
This method returns the norm of a symmetric second order tensor defined by:
\begin{equation*}
\left\Vert s \right\Vert  = \sqrt {s_{ij}:s_{ij}}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getNorm() const
//-----------------------------------------------------------------------------
{
  return sqrt(dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[5]) +
              2.0 * (dnlSquare(_data[1]) + dnlSquare(_data[2]) + dnlSquare(_data[4])));
}

/*
@LABEL:SymTensor2::getJ2()
@SHORT:J2 norm of a symmetric second order tensor.
@RETURN:double
This method returns the J2 norm of a symmetric second order tensor defined by:
\begin{equation*}
\sqrt {\frac{3}{2}} \left\Vert s \right\Vert  = \sqrt {\frac{3}{2} s_{ij}:s_{ij}}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getJ2() const
//-----------------------------------------------------------------------------
{
  return dnlSqrt32 * sqrt(dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[5]) +
                          2.0 * (dnlSquare(_data[1]) + dnlSquare(_data[2]) + dnlSquare(_data[4])));
}

/*
@LABEL:SymTensor2::getDeterminant()
@SHORT:Determinant of a symmetric second order tensor.
@RETURN:double
This method returns the determinant of a symmetric second second order tensor.
The result of this operation is a scalar value defined by:
\begin{equation*}
D = T_{11} T_{22} T_{33} + 2 T_{12} T_{23} T_{13} - T_{22} T_{13}^2 - T_{11} T_{23}^2 - T_{33} T_{12}^2
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
inline double SymTensor2::getDeterminant() const
//-----------------------------------------------------------------------------
{
  return (_data[0] * _data[3] * _data[5] +
          2.0 * _data[1] * _data[4] * _data[2] -
          _data[2] * _data[3] * _data[2] -
          _data[0] * _data[4] * _data[4] -
          _data[1] * _data[1] * _data[5]);
}

#endif
