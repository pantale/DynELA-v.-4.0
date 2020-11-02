/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file Tensor3.h
  \brief Declaration file for the third order tensor class

  This file is the declaration file for the third order tensor class. A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_Tensor3_h__
#define __dnlMaths_Tensor3_h__

#include <string>
#include <string.h>
#include <Errors.h>
#include <Macros.h>

class Vec3D;
class Tensor2;

/*!
  \brief Declaration of the third order tensor class

  A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup dnlMaths
*/
class Tensor3
{
    double v[27]; //!< Data storage for 27 double

private:
    inline bool indexOK(short i, short j, short k) const;

public:
    Tensor3();
    ~Tensor3();

    // operations sur les composantes
    inline double operator()(short i, short j, short k) const;
#ifndef SWIG
    inline double &operator()(short i, short j, short k);
#endif

    // operations d'affectation
    inline void setToValue(double val);
    void setToUnity();
#ifndef SWIG
    Tensor3 &operator=(const double &val);
    Tensor3 &operator=(const Tensor3 &tens);
#endif

    // operations de comparaison
    bool operator==(const Tensor3 &tens) const;
    bool operator!=(const Tensor3 &tens) const;

    // operations arithmetiques de base entre tenseurs
#ifndef SWIG
    inline void operator+=(const Tensor3 &tens);
    inline void operator-=(const Tensor3 &tens);
    inline void operator*=(const double val);
    inline void operator/=(const double val);
#endif
    Tensor3 operator+(const Tensor3 &tens) const;
    Tensor3 operator-(const Tensor3 &tens) const;
    Tensor3 operator*(const double &lambda) const;
    Tensor3 operator/(const double &tens) const;
#ifndef SWIG
    friend Tensor3 operator*(const double &val, const Tensor3 &tens);
    Tensor2 operator*(const Vec3D &vec) const;
#endif
    void numpyWrite(std::string filename, bool initialize = false) const;
    void numpyWriteZ(std::string filename, std::string name, bool initialize = false) const;
    void numpyRead(std::string filename);
    void numpyReadZ(std::string filename, std::string name);

    // gestion des flux entree et sortie
#ifndef SWIG
    friend std::ostream &operator<<(std::ostream &os, const Tensor3 &tens);
    friend std::ofstream &operator<<(std::ofstream &os, const Tensor3 &tens);
    friend std::ifstream &operator>>(std::ifstream &is, Tensor3 &tens);
    void write(std::ofstream &ofs) const;
    void read(std::ifstream &ifs);
    void print(std::ostream &os) const;
#endif
};

//------inline functions-------------------------------------------------------

/*!
  \brief tests if the set of indexes is ok
*/
//-----------------------------------------------------------------------------
inline bool Tensor3::indexOK(short i, short j, short k) const
//-----------------------------------------------------------------------------
{
    if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3) && (k >= 0) && (k < 3))
    {
        return (true);
    }

    printf("Tensor3::indexOK\nindice(s) [%d,%d,%d] out of allowed range [0-2,0-2,0-2]", i, j,
           k);
    exit(-1);

    // no sense
    return (false);
}

/*!
  \brief Access to the values T[i,j,k] of a third order tensor

  \param i short 1
  \param j short 2
  \param k short 3
  \return Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline double &Tensor3::operator()(short i, short j, short k)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
    indexOK(i, j, k);
#endif
    return v[dnlTensor3Ind(i, j, k, 3)];
}

/*!
  \brief Access to the values T[i,j,k] of a third order tensor (Read only method)

  \param i short 1
  \param j short 2
  \param k short 3
  \return Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline double Tensor3::operator()(short i, short j, short k) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
    indexOK(i, j, k);
#endif
    return v[dnlTensor3Ind(i, j, k, 3)];
}

/*!
  \brief Fill a third order tensor with a scalar value

  This method is a surdefinition of the = operator for the third order tensor class.
  \code
  Tensor3 t1;
  t1 = setToValue(1.0); // All components of the tensor are set to 1.0
  \endcode
  \param val double value to give to all components of the third order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor3::setToValue(double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        v[i] = val;
    }
}

/*!
  \brief Addition of 2 third order tensors

  This method defines the addition of 2 third order tensors.
  \code
  Tensor3 t1,t2;
  t2 += t1; // sum of two third order tensors
  \endcode
  \param tens Third order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator+=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la somme
    for (short i = 0; i < 27; i++)
    {
        v[i] += tens.v[i];
    }
}

/*!
  \brief Difference of 2 third order tensors

  This method defines the difference of 2 third order tensors.
  \code
  Tensor3 t1,t2;
  t2 -= t1; // difference of two third order tensors
  \endcode
  \param tens Third order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator-=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la difference
    for (short i = 0; i < 27; i++)
    {
        v[i] -= tens.v[i];
    }
}

/*!
  \brief Multiplication of a third order tensor by a scalar value

  This method defines the multiplication of a third order tensor by a scalar value
    \code
  Tensor3 t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  \param val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator*=(const double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        v[i] *= val;
    }
}

/*!
  \brief Division of a third order tensor by a scalar value

  This method defines the division of a third order tensor by a scalar value
    \code
  Tensor3 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  \param val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator/=(const double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        v[i] /= val;
    }
}

#endif
