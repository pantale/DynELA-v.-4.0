/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Tensor3_h__
#define __dnlMaths_Tensor3_h__

#include <string>
#include <string.h>
#include <dnlKernel.h>
//#include <Macros.h>

class Vec3D;
class Tensor2;

//-----------------------------------------------------------------------------
// Class : Tensor3
//
// Used to manage Tensor3
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Tensor3
{
    double _data[27]; // Data storage for 27 double

private:
    inline bool indexOK(short i, short j, short k) const;

public:
    Tensor3();
    Tensor3(const Tensor3 &);
    ~Tensor3();

    // Interface methods excluded from SWIG
#ifndef SWIG
    friend std::ifstream &operator>>(std::ifstream &is, Tensor3 &tens);
    friend std::ofstream &operator<<(std::ofstream &os, const Tensor3 &tens);
    friend std::ostream &operator<<(std::ostream &os, const Tensor3 &tens);
    friend Tensor3 operator*(const double &val, const Tensor3 &tens);
    inline double &operator()(short i, short j, short k);
    inline void operator-=(const Tensor3 &tens);
    inline void operator*=(const double val);
    inline void operator/=(const double val);
    inline void operator+=(const Tensor3 &tens);
    Tensor2 operator*(const Vec3D &vec) const;
    Tensor3 &operator=(const double &val);
    Tensor3 &operator=(const Tensor3 &tens);
    void print(std::ostream &os) const;
    void read(std::ifstream &ifs);
    void write(std::ofstream &ofs) const;
#endif

    // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

    bool operator!=(const Tensor3 &tens) const;
    bool operator==(const Tensor3 &tens) const;
    inline double operator()(short i, short j, short k) const;
    inline void setToValue(double val);
    Tensor3 operator-(const Tensor3 &tens) const;
    Tensor3 operator*(const double &lambda) const;
    Tensor3 operator/(const double &tens) const;
    Tensor3 operator+(const Tensor3 &tens) const;
    void numpyRead(std::string filename);
    void numpyReadZ(std::string filename, std::string name);
    void numpyWrite(std::string filename, bool initialize = false) const;
    void numpyWriteZ(std::string filename, std::string name, bool initialize = false) const;
    void setToUnity();
};

//------inline functions-------------------------------------------------------

/*
  tests if the set of indexes is ok
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

/*
  Access to the values T[i,j,k] of a third order tensor

  - i short 1
  - j short 2
  - k short 3
  Return : Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline double &Tensor3::operator()(short i, short j, short k)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
    indexOK(i, j, k);
#endif
    return _data[dnlTensor3Ind(i, j, k, 3)];
}

/*
  Access to the values T[i,j,k] of a third order tensor (Read only method)

  - i short 1
  - j short 2
  - k short 3
  Return : Value of the third order tensor T[i,j,k]
*/
//-----------------------------------------------------------------------------
inline double Tensor3::operator()(short i, short j, short k) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
    indexOK(i, j, k);
#endif
    return _data[dnlTensor3Ind(i, j, k, 3)];
}

/*
  Fill a third order tensor with a scalar value

  This method is a surdefinition of the = operator for the third order tensor class.
  \code
  Tensor3 t1;
  t1 = setToValue(1.0); // All components of the tensor are set to 1.0
  \endcode
  - val double value to give to all components of the third order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor3::setToValue(double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        _data[i] = val;
    }
}

/*
  Addition of 2 third order tensors

  This method defines the addition of 2 third order tensors.
  \code
  Tensor3 t1,t2;
  t2 += t1; // sum of two third order tensors
  \endcode
  - tens Third order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator+=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la somme
    for (short i = 0; i < 27; i++)
    {
        _data[i] += tens._data[i];
    }
}

/*
  Difference of 2 third order tensors

  This method defines the difference of 2 third order tensors.
  \code
  Tensor3 t1,t2;
  t2 -= t1; // difference of two third order tensors
  \endcode
  - tens Third order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator-=(const Tensor3 &tens)
//-----------------------------------------------------------------------------
{
    // calcul de la difference
    for (short i = 0; i < 27; i++)
    {
        _data[i] -= tens._data[i];
    }
}

/*
  Multiplication of a third order tensor by a scalar value

  This method defines the multiplication of a third order tensor by a scalar value
    \code
  Tensor3 t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  - val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator*=(const double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        _data[i] *= val;
    }
}

/*
  Division of a third order tensor by a scalar value

  This method defines the division of a third order tensor by a scalar value
    \code
  Tensor3 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  - val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor3::operator/=(const double val)
//-----------------------------------------------------------------------------
{
    for (short i = 0; i < 27; i++)
    {
        _data[i] /= val;
    }
}

#endif
