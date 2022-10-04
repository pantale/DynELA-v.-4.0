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

class Vec3D;
class Tensor2;

/*
@LABEL:Tensor3::Tensor3
@SHORT:Third order tensor class.
The Tensor3 library is used to store third order tensors defined in the \DynELA.
The Tensor3 class is used to store and manage data relative to a third order tensor which can be seen as a $3\times3\times3$ matrix hard to represent on a plane,
where $T_{ijk}$ are the components of the third order tensor $\T$.
Concerning the internal storage of data, the Tensor3 data is stored in a vector of $27$ components named \textsf{\_data}.
@END
*/
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

//  tests if the indexes are ok
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


//  Access to the values T[i,j,k] of a third order tensor
//-----------------------------------------------------------------------------
inline double &Tensor3::operator()(short i, short j, short k)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
    indexOK(i, j, k);
#endif
    return _data[dnlTensor3Ind(i, j, k, 3)];
}


//  Access to the values T[i,j,k] of a third order tensor (Read only method)
//-----------------------------------------------------------------------------
inline double Tensor3::operator()(short i, short j, short k) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
    indexOK(i, j, k);
#endif
    return _data[dnlTensor3Ind(i, j, k, 3)];
}


 // Fill a third order tensor with a scalar value
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
@LABEL:Tensor3::operator+=(Tensor3 B)
@SHORT:Addition of 2 third order tensors.
@ARG:Tensor3 & B & Third order tensor to add to the current one.
@RETURN:Tensor3 : Result of the addition operation.
This method defines the addition of 2 third order tensors.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\A += \B
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\B$ is the third order tensor value defined by parameter B.
@END
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
@LABEL:Tensor3::operator-=(Tensor3 B)
@SHORT:Difference of 2 third order tensors.
@ARG:Tensor3 & B & Third order tensor to add to the current one.
@RETURN:Tensor3 : Result of the difference operation.
This method defines the difference of 2 third order tensors.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\A -= \B
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\B$ is the third order tensor value defined by parameter B.
@END
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
@LABEL:Tensor3::operator*=(double l)
@SHORT:Multiplication of a third order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor3 : Result of the multiplication operation.
This method defines the multiplication of a third order tensor by a scalar value.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\lambda \A
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
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
@LABEL:Tensor3::operator/=(double l)
@SHORT:Division of a third order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor3 : Result of the division operation.
This method defines the division of a third order tensor by a scalar value.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
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
