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

class Vec3D;
class Tensor2;
class Tensor3;

/*
@LABEL:Tensor4::Tensor4
@SHORT:Second order tensor class.
The Tensor4 library is used to store fourth order tensors defined in the \DynELA.
The Tensor4 class is used to store and manage data relative to a fourth order tensor which can be seen as a $3\times3\times3\times3$ matrix where $T_{ijkl}$ are the components of the fourth order tensor $\T$.
Concerning the internal storage of data, the Tensor4 data is stored in a vector of $81$ components named \textsf{\_data} using the following storage scheme:
@END
*/
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
  inline void setValue(const double);
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

//  tests if the indexes are ok
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


//  Access to the values T[i,j,k,l] of a fourth order tensor
//-----------------------------------------------------------------------------
inline double &Tensor4::operator()(short i, short j, short k, short l)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j, k, l);
#endif
  return _data[dnlTensor4Ind(i, j, k, l, 3)];
}


//  Access to the values T[i,j,k,l] of a fourth order tensor
//-----------------------------------------------------------------------------
inline double Tensor4::operator()(short i, short j, short k, short l) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  indexOK(i, j, k, l);
#endif
  return _data[dnlTensor4Ind(i, j, k, l, 3)];
}


//  Fill a fourth order tensor with a scalar value
//-----------------------------------------------------------------------------
inline void Tensor4::setValue(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] = val;
}

/*
@LABEL:Tensor4::operator+=(Tensor4 B)
@SHORT:Addition of 2 fourth order tensors.
@ARG:Tensor4 & B & Second order tensor to add to the current one.
@RETURN:Tensor4 : Result of the addition operation.
This method defines the addition of 2 fourth order tensors.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\A += \B
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\B$ is the fourth order tensor value defined by parameter B.
@END
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
@LABEL:Tensor4::operator-=(Tensor4 B)
@SHORT:Difference of 2 fourth order tensors.
@ARG:Tensor4 & B & Second order tensor to add to the current one.
@RETURN:Tensor4 : Result of the difference operation.
This method defines the difference of 2 fourth order tensors.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\A -= \B
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\B$ is the fourth order tensor value defined by parameter B.
@END
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
@LABEL:Tensor4::operator*=(double l)
@SHORT:Multiplication of a fourth order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor4 : Result of the multiplication operation.
This method defines the multiplication of a fourth order tensor by a scalar value.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\lambda \A
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator*=(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] *= val;
}

/*
@LABEL:Tensor4::operator/=(double l)
@SHORT:Division of a fourth order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor4 : Result of the division operation.
This method defines the division of a fourth order tensor by a scalar value.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
inline void Tensor4::operator/=(const double val)
//-----------------------------------------------------------------------------
{
  for (short i = 0; i < 81; i++)
    _data[i] /= val;
}

#endif
