/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <fstream>
#include <Tensor2.h>
#include <SymTensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>
#include <Eigen3x3.h>
#include <lapacke.h>

const Tensor2Index Tensor2::_internalIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8};

/*
@LABEL:Tensor2::Tensor2()
@SHORT:Default constructor of the Tensor2 class.
@RETURN:Tensor2 : The new Tensor2 object created by the constructor.
This is the default constructor of the Tensor2 class, where all components are initialized to zero by default.
\begin{equation*}
\T=\left[\begin{array}{ccc}
0 & 0 & 0\\
0 & 0 & 0\\
0 & 0 & 0
\end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2::Tensor2()
//-----------------------------------------------------------------------------
{
  setValue(0);
}

// Copy constructor
//-----------------------------------------------------------------------------
Tensor2::Tensor2(const Tensor2 &T)
//-----------------------------------------------------------------------------
{
  memcpy(_data, T._data, 9 * sizeof(double));
}

/*
@LABEL:Tensor2::Tensor2(double t11, double t12, ...)
@SHORT:Constructor of the Tensor2 class.
@RETURN:Tensor2 : The new Tensor2 object created by the constructor.
@ARG:double & t11 & Component $T_{11}$ of the second order tensor.
@ARG:double & t12 & Component $T_{12}$ of the second order tensor.
@ARG:double & t13 & Component $T_{13}$ of the second order tensor.
@ARG:double & t21 & Component $T_{21}$ of the second order tensor.
@ARG:double & t22 & Component $T_{22}$ of the second order tensor.
@ARG:double & t23 & Component $T_{23}$ of the second order tensor.
@ARG:double & t31 & Component $T_{31}$ of the second order tensor.
@ARG:double & t32 & Component $T_{32}$ of the second order tensor.
@ARG:double & t33 & Component $T_{33}$ of the second order tensor.
Constructor of a second order tensor with explicit initialization of the $9$ components of the tensor.
\begin{equation*}
\T=\left[\begin{array}{ccc}
  T_{11} & T_{12} & T_{13}\\
  T_{21} & T_{22} & T_{23}\\
  T_{31} & T_{32} & T_{33}
  \end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2::Tensor2(double t11, double t12, double t13, double t21, double t22, double t23, double t31, double t32, double t33)
//-----------------------------------------------------------------------------
{
  _data[0] = t11;
  _data[1] = t12;
  _data[2] = t13;
  _data[3] = t21;
  _data[4] = t22;
  _data[5] = t23;
  _data[6] = t31;
  _data[7] = t32;
  _data[8] = t33;
}

// Destructor
//-----------------------------------------------------------------------------
Tensor2::~Tensor2()
//-----------------------------------------------------------------------------
{
}

// Send the content of a second order tensor to the output flux for display
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &outputStream, const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.print(outputStream);
  return outputStream;
}

// Print the content of a second order tensor to the output flux for display
//-----------------------------------------------------------------------------
void Tensor2::print(std::ostream &outputStream) const
//-----------------------------------------------------------------------------
{
  short i, j;
  outputStream << "tensor 3x3 ={{";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (j != 0)
      {
        outputStream << ",";
      }
      outputStream << _data[_internalIndexes.index[i][j]];
    }
    if (i != 2)
    {
      outputStream << "},{";
    }
  }
  outputStream << "}}";
}

/*
@LABEL:Tensor2::setToUnity()
@SHORT:Unity second order tensor.
@SMOD
This method transforms the current tensor to a unity tensor.
\begin{equation*}
\T=\left[\begin{array}{ccc}
1&0&0\\
0&1&0\\
0&0&1
\end{array}\right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::setToUnity()
//-----------------------------------------------------------------------------
{
  _data[0] = 1;
  _data[1] = 0;
  _data[2] = 0;
  _data[3] = 0;
  _data[4] = 1;
  _data[5] = 0;
  _data[6] = 0;
  _data[7] = 0;
  _data[8] = 1;
}

/*
@LABEL:Tensor2::setToZero()
@SHORT:Zero second order tensor.
@SMOD
This method transforms the current second order tensor to a zero tensor.
\begin{equation*}
\T=\left[\begin{array}{ccc}
0 & 0 & 0\\
0 & 0 & 0\\
0 & 0 & 0
\end{array}\right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::setToZero()
//-----------------------------------------------------------------------------
{
  setValue(0);
}

/*
@LABEL:Tensor2::operator=(double v)
@SHORT:Fill a second order tensor with a scalar value.
@RETURN:Tensor2
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the second order tensor class.
\begin{equation*}
\T=\left[\begin{array}{ccc}
v & v & v\\
v & v & v\\
v & v & v
\end{array}\right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself and $v$ is the scalar value defined by parameter v.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setValue(val);
  return *this;
}

// Copy the content of a second order tensor into a new one
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const Tensor2 &T)
//-----------------------------------------------------------------------------
{
  memcpy(_data, T._data, 9 * sizeof(double));
  return *this;
}

/*
@LABEL:Tensor2::operator=(SymTensor2 T)
@SHORT:Copy the content of a SymTensor2 into a Tensor2.
@RETURN:Tensor2
@ARG:SymTensor2 & T & Symmetric second order tensor to copy.
The result of this operation is a second order tensor as a copy of a symmetric second order tensor where $\T$ is a symmetric second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const SymTensor2 &T)
//-----------------------------------------------------------------------------
{
  _data[0] = T._data[0];
  _data[1] = T._data[1];
  _data[2] = T._data[2];
  _data[3] = T._data[1];
  _data[4] = T._data[3];
  _data[5] = T._data[4];
  _data[6] = T._data[2];
  _data[7] = T._data[4];
  _data[8] = T._data[5];
  return *this;
}

/*
@LABEL:Tensor2::operator+(Tensor2 B)
@SHORT:Addition of 2 second order tensors.
@ARG:Tensor2 & B & Second order tensor to add to the current one.
@RETURN:Tensor2 : Result of the addition operation.
This method defines the addition of 2 second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A + \B
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is the second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator+(const Tensor2 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] + T._data[0];
  result._data[1] = _data[1] + T._data[1];
  result._data[2] = _data[2] + T._data[2];
  result._data[3] = _data[3] + T._data[3];
  result._data[4] = _data[4] + T._data[4];
  result._data[5] = _data[5] + T._data[5];
  result._data[6] = _data[6] + T._data[6];
  result._data[7] = _data[7] + T._data[7];
  result._data[8] = _data[8] + T._data[8];

  // renvoi de l'objet
  return result;
}

/*
@LABEL:Tensor2::operator-(Tensor2 B)
@SHORT:Difference of 2 second order tensors.
@ARG:Tensor2 & B & Second order tensor to subtract to the current one.
@RETURN:Tensor2 : Result of the difference operation.
This method defines the subtraction of 2 second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A - \B
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is the second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator-(const Tensor2 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] - T._data[0];
  result._data[1] = _data[1] - T._data[1];
  result._data[2] = _data[2] - T._data[2];
  result._data[3] = _data[3] - T._data[3];
  result._data[4] = _data[4] - T._data[4];
  result._data[5] = _data[5] - T._data[5];
  result._data[6] = _data[6] - T._data[6];
  result._data[7] = _data[7] - T._data[7];
  result._data[8] = _data[8] - T._data[8];

  // renvoi de l'objet
  return result;
}

/*
@LABEL:Tensor2::operator-()
@SHORT:Opposite of a second order tensor.
@RETURN:Tensor2 : The opposite second order tensor.
This method defines the opposite of a second order tensor.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = - \A
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator-() const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = -_data[0];
  result._data[1] = -_data[1];
  result._data[2] = -_data[2];
  result._data[3] = -_data[3];
  result._data[4] = -_data[4];
  result._data[5] = -_data[5];
  result._data[6] = -_data[6];
  result._data[7] = -_data[7];
  result._data[8] = -_data[8];

  return result;
}

/*
@LABEL:Tensor2::operator*(double l)
@SHORT:Multiplication of a second order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor2 : Result of the multiplication operation.
This method defines the multiplication of a second order tensor by a scalar value.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const double l) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = l * _data[0];
  result._data[1] = l * _data[1];
  result._data[2] = l * _data[2];
  result._data[3] = l * _data[3];
  result._data[4] = l * _data[4];
  result._data[5] = l * _data[5];
  result._data[6] = l * _data[6];
  result._data[7] = l * _data[7];
  result._data[8] = l * _data[8];

  return result;
}

/*
@LABEL:Tensor2::operator/(double l)
@SHORT:Division of a second order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor2 : Result of the division operation.
This method defines the division of a second order tensor by a scalar value.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator/(const double l) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

#ifdef VERIF_maths
  if (l == 0)
  {
    fatalError("Tensor2:: operator /", "divide by zero");
  }
#endif

  result._data[0] = _data[0] / l;
  result._data[1] = _data[1] / l;
  result._data[2] = _data[2] / l;
  result._data[3] = _data[3] / l;
  result._data[4] = _data[4] / l;
  result._data[5] = _data[5] / l;
  result._data[6] = _data[6] / l;
  result._data[7] = _data[7] / l;
  result._data[8] = _data[8] / l;

  return result;
}

/*
@LABEL:operator*(double l, Tensor2 A)
@SHORT:Multiplication of a second order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@ARG:Tensor2 & A & Second order tensor to use for the operation.
@RETURN:Tensor2 : Result of the multiplication operation.
This method defines the multiplication of a second order tensor by a scalar value.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a second order tensor and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 operator*(const double &l, const Tensor2 &T)
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = l * T._data[0];
  result._data[1] = l * T._data[1];
  result._data[2] = l * T._data[2];
  result._data[3] = l * T._data[3];
  result._data[4] = l * T._data[4];
  result._data[5] = l * T._data[5];
  result._data[6] = l * T._data[6];
  result._data[7] = l * T._data[7];
  result._data[8] = l * T._data[8];

  return result;
}

/*
@LABEL:Tensor2::dot()
@SHORT:Single contracted product of a second order tensor by itself.
@RETURN:Tensor2 : Result of the multiplication operation.
This method defines a single contracted product of a second order tensor by itself.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \A
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::dot() const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * _data[0] + _data[1] * _data[3] + _data[2] * _data[6];
  result._data[1] = _data[0] * _data[1] + _data[1] * _data[4] + _data[2] * _data[7];
  result._data[2] = _data[0] * _data[2] + _data[1] * _data[5] + _data[2] * _data[8];
  result._data[3] = _data[3] * _data[0] + _data[4] * _data[3] + _data[5] * _data[6];
  result._data[4] = _data[3] * _data[1] + _data[4] * _data[4] + _data[5] * _data[7];
  result._data[5] = _data[3] * _data[2] + _data[4] * _data[5] + _data[5] * _data[8];
  result._data[6] = _data[6] * _data[0] + _data[7] * _data[3] + _data[8] * _data[6];
  result._data[7] = _data[6] * _data[1] + _data[7] * _data[4] + _data[8] * _data[7];
  result._data[8] = _data[6] * _data[2] + _data[7] * _data[5] + _data[8] * _data[8];

  return result;
}

/*
@LABEL:Tensor2::dot(Tensor2 B)
@SHORT:Single contracted product of two second order tensors.
@RETURN:Tensor2 : Result of the multiplication operation.
@ARG:Tensor2 & B & Second tensor for the multiplication operation.
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is the second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::dot(const Tensor2 T) const
//-----------------------------------------------------------------------------
{
  //  return (*this) * T;
  Tensor2 result;

  result._data[0] = _data[0] * T._data[0] + _data[1] * T._data[3] + _data[2] * T._data[6];
  result._data[1] = _data[0] * T._data[1] + _data[1] * T._data[4] + _data[2] * T._data[7];
  result._data[2] = _data[0] * T._data[2] + _data[1] * T._data[5] + _data[2] * T._data[8];
  result._data[3] = _data[3] * T._data[0] + _data[4] * T._data[3] + _data[5] * T._data[6];
  result._data[4] = _data[3] * T._data[1] + _data[4] * T._data[4] + _data[5] * T._data[7];
  result._data[5] = _data[3] * T._data[2] + _data[4] * T._data[5] + _data[5] * T._data[8];
  result._data[6] = _data[6] * T._data[0] + _data[7] * T._data[3] + _data[8] * T._data[6];
  result._data[7] = _data[6] * T._data[1] + _data[7] * T._data[4] + _data[8] * T._data[7];
  result._data[8] = _data[6] * T._data[2] + _data[7] * T._data[5] + _data[8] * T._data[8];

  return result;
}

/*
@LABEL:Tensor2::dotTxN()
@SHORT:Single contracted product of a second order tensor by its transpose.
@RETURN:SymTensor2 : Result of the multiplication operation.
This method defines a single contracted product of a second order tensor by its transpose.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A^T\cdot \A
\end{equation*}
where $\A$ is a second order tensor defined by the object itself. Result is a symmetric second order tensor.
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Tensor2::dotTxN() const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = _data[0] * _data[0] + _data[3] * _data[3] + _data[6] * _data[6];
  result._data[1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  result._data[2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  result._data[3] = _data[1] * _data[1] + _data[4] * _data[4] + _data[7] * _data[7];
  result._data[4] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  result._data[5] = _data[2] * _data[2] + _data[5] * _data[5] + _data[8] * _data[8];

  return result;
}

/*
@LABEL:Tensor2::dotNxT()
@SHORT:Single contracted product of a second order tensor by its transpose.
@RETURN:SymTensor2 : Result of the multiplication operation.
This method defines a single contracted product of a second order tensor by its transpose.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \A^T
\end{equation*}
where $\A$ is a second order tensor defined by the object itself. Result is a symmetric second order tensor.
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Tensor2::dotNxT() const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = _data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2];
  result._data[1] = _data[0] * _data[3] + _data[1] * _data[4] + _data[2] * _data[5];
  result._data[2] = _data[0] * _data[6] + _data[1] * _data[7] + _data[2] * _data[8];
  result._data[3] = _data[3] * _data[3] + _data[4] * _data[4] + _data[5] * _data[5];
  result._data[4] = _data[3] * _data[6] + _data[4] * _data[7] + _data[5] * _data[8];
  result._data[5] = _data[6] * _data[6] + _data[7] * _data[7] + _data[8] * _data[8];

  return result;
}

/*
@LABEL:Tensor2::operator*(Tensor2 B)
@SHORT:Single contracted product of two second order tensors.
@RETURN:Tensor2 : Result of the multiplication operation.
@ARG:Tensor2 & B & Second tensor for the multiplication operation.
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is the second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const Tensor2 &T) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * T._data[0] + _data[1] * T._data[3] + _data[2] * T._data[6];
  result._data[1] = _data[0] * T._data[1] + _data[1] * T._data[4] + _data[2] * T._data[7];
  result._data[2] = _data[0] * T._data[2] + _data[1] * T._data[5] + _data[2] * T._data[8];
  result._data[3] = _data[3] * T._data[0] + _data[4] * T._data[3] + _data[5] * T._data[6];
  result._data[4] = _data[3] * T._data[1] + _data[4] * T._data[4] + _data[5] * T._data[7];
  result._data[5] = _data[3] * T._data[2] + _data[4] * T._data[5] + _data[5] * T._data[8];
  result._data[6] = _data[6] * T._data[0] + _data[7] * T._data[3] + _data[8] * T._data[6];
  result._data[7] = _data[6] * T._data[1] + _data[7] * T._data[4] + _data[8] * T._data[7];
  result._data[8] = _data[6] * T._data[2] + _data[7] * T._data[5] + _data[8] * T._data[8];

  return result;
}

/*
@LABEL:Tensor2::operator*(SymTensor2 B)
@SHORT:Single contracted product of a second order tensor and a symmetric second order tensor.
@RETURN:Tensor2 : Result of the multiplication operation.
@ARG:Tensor2 & B & Second tensor for the multiplication operation.
This method defines a single contracted product of a second order tensor and a symmetric second order tensor.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is a symmetric second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const SymTensor2 &T) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * T._data[0] + _data[1] * T._data[1] + _data[2] * T._data[2];
  result._data[1] = _data[0] * T._data[1] + _data[1] * T._data[3] + _data[2] * T._data[4];
  result._data[2] = _data[0] * T._data[2] + _data[1] * T._data[4] + _data[2] * T._data[5];
  result._data[3] = _data[3] * T._data[0] + _data[4] * T._data[1] + _data[5] * T._data[2];
  result._data[4] = _data[3] * T._data[1] + _data[4] * T._data[3] + _data[5] * T._data[4];
  result._data[5] = _data[3] * T._data[2] + _data[4] * T._data[4] + _data[5] * T._data[5];
  result._data[6] = _data[6] * T._data[0] + _data[7] * T._data[1] + _data[8] * T._data[2];
  result._data[7] = _data[6] * T._data[1] + _data[7] * T._data[3] + _data[8] * T._data[4];
  result._data[8] = _data[6] * T._data[2] + _data[7] * T._data[4] + _data[8] * T._data[5];

  return result;
}

/*
@LABEL:Tensor2::operator*(Vec3D V)
@SHORT:Multiplication of a second order tensor by a vector.
@RETURN:Vec3D : Result of the multiplication operation.
@ARG:Vec3D & V & Vec3D to use for the multiplication operation.
This method defines the product of a second order tensor by a vector.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\overrightarrow{x}$ is a Vec3D defined by parameter V.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::operator*(const Vec3D &V) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  result._data[0] = _data[0] * V._data[0] + _data[1] * V._data[1] + _data[2] * V._data[2];
  result._data[1] = _data[3] * V._data[0] + _data[4] * V._data[1] + _data[5] * V._data[2];
  result._data[2] = _data[6] * V._data[0] + _data[7] * V._data[1] + _data[8] * V._data[2];

  return result;
}

/*
@LABEL:Tensor2::doubleDot()
@SHORT:Double contracted product of a second order tensor by itself.
@RETURN:double : Result of the multiplication operation.
This method defines a double contracted product of a second order tensor by itself.
The result of this operation is a scalar $s$ defined by:
\begin{equation*}
s = \A : \A = \sum_{i=1}^{3} \sum_{j=1}^{3} A_{ij}\times A_{ij}
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::doubleDot() const
//-----------------------------------------------------------------------------
{
  return (_data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2] +
          _data[3] * _data[3] + _data[4] * _data[4] + _data[5] * _data[5] +
          _data[6] * _data[6] + _data[7] * _data[7] + _data[8] * _data[8]);
}

/*
@LABEL:Tensor2::doubleDot(Tensor2 B)
@SHORT:Double contracted product of 2 second order tensors.
@RETURN:double : Result of the multiplication operation.
@ARG:Tensor2 & B & Second tensor for the multiplication operation.
This method defines a double contracted product of two second order tensors.
The result of this operation is a scalar $s$ defined by:
\begin{equation*}
s = \A : \B = \sum_{i=1}^{3} \sum_{j=1}^{3} A_{ij}\times B_{ij}
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\B$ is a second order tensor defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::doubleDot(const Tensor2 T) const
//-----------------------------------------------------------------------------
{
  return (_data[0] * T._data[0] + _data[1] * T._data[1] + _data[2] * T._data[2] +
          _data[3] * T._data[3] + _data[4] * T._data[4] + _data[5] * T._data[5] +
          _data[6] * T._data[6] + _data[7] * T._data[7] + _data[8] * T._data[8]);
}

/*
@LABEL:Tensor2::deviator()
@SHORT:Deviatoric part of a second order tensor.
@RETURN:Tensor2 : The deviatoric part of the second order tensor.
This method defines the deviatoric part of a second order tensor.
The result of this operation is a second order tensor defined by the following equation:
\begin{equation*}
\A^d=\A-\frac{1}{3}\tr[\A].\Id
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\Id$ is the unit tensor.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::deviator() const
//-----------------------------------------------------------------------------
{
  Tensor2 result(*this);
  double pressure = thirdTrace();

  result._data[0] -= pressure;
  result._data[4] -= pressure;
  result._data[8] -= pressure;

  return result;
}

/*
@LABEL:Tensor2::transpose()
@SHORT:Transpose of a second order tensor.
@RETURN:Tensor2 : The transpose of the second order tensor.
This method defines the transpose of a second order tensor.
The result of this operation is a second order tensor defined by the following equation:
\begin{equation*}
\B=\A^T =\left[\begin{array}{ccc}
  A_{11} & A_{21} & A_{31}\\
  A_{12} & A_{22} & A_{32}\\
  A_{13} & A_{23} & A_{33}
  \end{array}\right]
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::transpose() const
//-----------------------------------------------------------------------------
{
  return Tensor2(_data[0], _data[3], _data[6], _data[1], _data[4], _data[7], _data[2], _data[5], _data[8]);
}

/*
@LABEL:Tensor2::rowSum()
@SHORT:Sum of the rows of a second order tensor.
@RETURN:Vec3D : The sums of the rows of the second order tensor.
This method returns a vector by computing the sum of the components on all rows of a second order tensor.
The result of this operation is a vector $\overrightarrow{v}$ defined by:
\begin{equation*}
v_{i}=\sum_{j=1}^{3} T_{ji}
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::rowSum() const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + _data[1] + _data[2],
               _data[3] + _data[4] + _data[5],
               _data[6] + _data[7] + _data[8]);
}

/*
@LABEL:Tensor2::colSum()
@SHORT:Sum of the columns of a second order tensor.
@RETURN:Vec3D : The sums of the columns of the second order tensor.
This method returns a vector by computing the sum of the components on all columns of a second order tensor.
The result of this operation is a vector $\overrightarrow{v}$ defined by:
\begin{equation*}
v_{i}=\sum_{j=1}^{3}T_{ij}
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::colSum() const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + _data[3] + _data[6],
               _data[1] + _data[4] + _data[7],
               _data[2] + _data[5] + _data[8]);
}

/*
@LABEL:Tensor2::symmetric()
@SHORT:Symmetric part of a second order tensor.
@RETURN:Tensor2 : Symmetric part of the second order tensor.
This method returns the symmetric part of a second order tensor.
The result of this operation is a second order tensor defined by:
\begin{equation*}
\B = \left[\begin{array}{ccc}
 A_{11} & \frac{A_{12} + A_{21}}{2} & \frac{A_{13} + A_{31}}{2}\\
 \frac{A_{12} + A_{21}}{2} & A_{22} & \frac {A_{23} + A_{32}}{2}\\
 \frac{A_{13} + A_{31}}{2} & \frac {A_{23} + A_{32}}{2} & A_{33}\end{array}
\right]
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::symmetric() const
//-----------------------------------------------------------------------------
{
  return Tensor2(_data[0],
                 (_data[1] + _data[3]) / 2,
                 (_data[2] + _data[6]) / 2,
                 (_data[1] + _data[3]) / 2,
                 _data[4],
                 (_data[5] + _data[7]) / 2,
                 (_data[2] + _data[6]) / 2,
                 (_data[5] + _data[7]) / 2,
                 _data[8]);
}

/*
@LABEL:Tensor2::skewSymmetric()
@SHORT:Skew-symmetric part of a second order tensor.
@RETURN:Tensor2 : Symmetric part of the second order tensor.
This method returns the skew-symmetric part of a second order tensor.
The result of this operation is a second order tensor defined by:
\begin{equation*}
\B = \left[\begin{array}{ccc}
 A_{11} & \frac{A_{12} - A_{21}}{2} & \frac{A_{13} - A_{31}}{2}\\
 -\frac{A_{12} -  A_{21}}{2} & A_{22} & \frac {A_{23} - A_{32}}{2}\\
 -\frac{A_{13} - A_{31}}{2} & -\frac {A_{23} - A_{32}}{2} & A_{33}\end{array}
\right]
\end{equation*}
where $\A$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::skewSymmetric() const
//-----------------------------------------------------------------------------
{
  return Tensor2(0,
                 (_data[1] - _data[3]) / 2,
                 (_data[2] - _data[6]) / 2,
                 (_data[3] - _data[1]) / 2,
                 0,
                 (_data[5] - _data[7]) / 2,
                 (_data[6] - _data[2]) / 2,
                 (_data[7] - _data[5]) / 2,
                 0);
}

/*
@LABEL:Tensor2::row(short r)
@SHORT:Extraction of a row from a second order tensor.
@RETURN:Vec3D : The extracted row.
@ARG:short & r & Row to extract
This method returns a vector as part of a second order tensor.
The result of this operation with the argument r is a vector defined by:
\begin{equation*}
v_{i} = T_{ri}
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::row(short row) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  for (short i = 0; i < 3; i++)
  {
    result(i) = _data[_internalIndexes.index[row][i]];
  }

  return result;
}

/*
@LABEL:Tensor2::col(short c)
@SHORT:Extraction of a column from a second order tensor.
@RETURN:Vec3D : The extracted col.
@ARG:short & c & Column to extract
This method returns a vector as part of a second order tensor.
The result of this operation with the argument c is a vector defined by:
\begin{equation*}
v_{i} = T_{ic}
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::col(short col) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  for (short i = 0; i < 3; i++)
  {
    result(i) = _data[_internalIndexes.index[i][col]];
  }

  return result;
}

//  Test the equality of two second order tensors
//-----------------------------------------------------------------------------
bool Tensor2::operator==(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  short i;

  for (i = 0; i < 9; i++)
    if (_data[i] != tensor._data[i])
    {
      return false;
    }
  return true;
}

//  Test the inequality of two second order tensors
//-----------------------------------------------------------------------------
bool Tensor2::operator!=(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  return !(*this == tensor);
}

//  Writes a second order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
void Tensor2::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)_data, 9 * sizeof(double));
}

//  Reads a second order tensor in a binary flux from storage
//-----------------------------------------------------------------------------
void Tensor2::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)_data, 9 * sizeof(double));
}

//  Writes a second order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.write(os);
  return os;
}

//  Reads a second order tensor from a binary flux for storage
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.read(is);
  return is;
}

/*
@LABEL:Tensor2::maxVal()
@SHORT:Maximum component in a second order tensor.
@RETURN:double : The maximum component of the second order tensor.
This method returns the maximum component in a second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::maxVal()
//-----------------------------------------------------------------------------
{
  double max = _data[0];
  for (short i = 1; i < 9; i++)
  {
    if (_data[i] > max)
      max = _data[i];
  }
  return max;
}

/*
@LABEL:Tensor2::minVal()
@SHORT:Minimum component in a second order tensor.
@RETURN:double : The minimum component of the second order tensor.
This method returns the minimum component in a second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::minVal()
//-----------------------------------------------------------------------------
{
  double min = _data[0];
  for (short i = 1; i < 9; i++)
  {
    if (_data[i] < min)
      min = _data[i];
  }
  return min;
}

/*
@LABEL:Tensor2::maxAbs()
@SHORT:Maximum absolute component in a second order tensor.
@RETURN:double : The maximum component of the second order tensor.
This method returns the maximum absolute component in a second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::maxAbs()
//-----------------------------------------------------------------------------
{
  double max = dnlAbs(_data[0]);
  for (short i = 1; i < 9; i++)
  {
    if (dnlAbs(_data[i]) > max)
      max = dnlAbs(_data[i]);
  }
  return max;
}

/*
@LABEL:Tensor2::minAbs()
@SHORT:Minimum absolute component in a second order tensor.
@RETURN:double : The minimum component of the second order tensor.
This method returns the minimum absolute component in a second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::minAbs()
//-----------------------------------------------------------------------------
{
  double min = dnlAbs(_data[0]);
  for (short i = 1; i < 9; i++)
  {
    if (dnlAbs(_data[i]) < min)
      min = dnlAbs(_data[i]);
  }
  return min;
}

/*
@LABEL:Tensor2::inverse()
@SHORT:Inverse of a second order tensor.
@RETURN:Tensor2 : The inverse of the second order tensor.
This method returns the inverse of a second order tensor.
The result of this operation is a second order tensor defined by:
\begin{equation*}
d = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33}
\end{equation*}
\begin{equation*}
T^{-1} = \frac {1}{d} \left[\begin{array}{ccc}
  T_{22}T_{33}-T_{23}T_{32}&T_{13}T_{32}-T_{12}T_{33}&T_{12}T_{23}-T_{13}T_{22}\\
  T_{23}T_{31}-T_{21}T_{33}&T_{11}T_{33}-T_{13}T_{31}&T_{13}T_{21}-T_{11}T_{23}\\
  T_{21}T_{32}-T_{22}T_{31}&T_{12}T_{31}-T_{11}T_{32}&T_{11}T_{22}-T_{12}T_{21}
  \end{array}
  \right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::inverse() const
//-----------------------------------------------------------------------------
{
  double t1 = _data[4] * _data[8];
  double t2 = _data[2] * _data[7];
  double t3 = _data[1] * _data[5];
  double t4 = _data[5] * _data[7];
  double t5 = _data[1] * _data[8];
  double t6 = _data[2] * _data[4];

  double unSurDeter = 1 / (_data[0] * t1 +
                           _data[3] * t2 +
                           _data[6] * t3 -
                           _data[0] * t4 -
                           _data[3] * t5 -
                           _data[6] * t6);
  return Tensor2((t1 - t4) * unSurDeter,
                 (t2 - t5) * unSurDeter,
                 (t3 - t6) * unSurDeter,
                 (_data[5] * _data[6] - _data[3] * _data[8]) * unSurDeter,
                 (_data[0] * _data[8] - _data[2] * _data[6]) * unSurDeter,
                 (_data[2] * _data[3] - _data[0] * _data[5]) * unSurDeter,
                 (_data[3] * _data[7] - _data[4] * _data[6]) * unSurDeter,
                 (_data[1] * _data[6] - _data[0] * _data[7]) * unSurDeter,
                 (_data[0] * _data[4] - _data[1] * _data[3]) * unSurDeter);
}

/*
@LABEL:Tensor2::minor()
@SHORT:Minor of a second order tensor.
@RETURN:Tensor2 : The minor of the second order tensor.
This method returns the minor of a second order tensor.
\begin{equation*}
T^{minor} = \left[\begin{array}{ccc}
T_{22}T_{33}-T_{32}T_{23} & T_{33}T_{21}-T_{23}T_{31} & T_{21}T_{32}-T_{31}T_{22}\\
T_{12}T_{33}-T_{13}T_{32} & T_{33}T_{11}-T_{13}T_{31} & T_{11}T_{32}-T_{31}T_{12}\\
T_{12}T_{23}-T_{22}T_{13} & T_{23}T_{11}-T_{13}T_{21} & T_{11}T_{22}-T_{21}T_{12}
\end{array}
\right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::minor() const
//-----------------------------------------------------------------------------
{
  return Tensor2(_data[4] * _data[8] - _data[7] * _data[5],
                 _data[8] * _data[3] - _data[5] * _data[6],
                 _data[3] * _data[7] - _data[6] * _data[4],
                 _data[1] * _data[8] - _data[2] * _data[7],
                 _data[8] * _data[0] - _data[2] * _data[6],
                 _data[0] * _data[7] - _data[6] * _data[1],
                 _data[1] * _data[5] - _data[4] * _data[2],
                 _data[5] * _data[0] - _data[2] * _data[3],
                 _data[0] * _data[4] - _data[3] * _data[1]);
}

/*
@LABEL:Tensor2::cofactors()
@SHORT:Cofactors of a second order tensor.
@RETURN:Tensor2 : The cofactor of the second order tensor.
This method returns the cofactor of a second order tensor.
\begin{equation*}
T^{cof} = \left[\begin{array}{ccc}
T_{22}T_{33}-T_{32}T_{23} & T_{23}T_{31}-T_{33}T_{21} & T_{21}T_{32}-T_{31}T_{22}\\
T_{13}T_{32}-T_{12}T_{33} & T_{33}T_{11}-T_{13}T_{31} & T_{31}T_{12}-T_{11}T_{32}\\
T_{12}T_{23}-T_{22}T_{13} & T_{13}T_{21}-T_{23}T_{11} & T_{11}T_{22}-T_{21}T_{12}
\end{array}
\right]
\end{equation*}
where $\T$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::cofactors() const
//-----------------------------------------------------------------------------
{
  return Tensor2(_data[4] * _data[8] - _data[7] * _data[5],
                 _data[5] * _data[6] - _data[8] * _data[3],
                 _data[3] * _data[7] - _data[6] * _data[4],
                 _data[2] * _data[7] - _data[1] * _data[8],
                 _data[8] * _data[0] - _data[2] * _data[6],
                 _data[6] * _data[1] - _data[0] * _data[7],
                 _data[1] * _data[5] - _data[4] * _data[2],
                 _data[2] * _data[3] - _data[5] * _data[0],
                 _data[0] * _data[4] - _data[3] * _data[1]);
}

/*
@LABEL:Tensor2::solve(Vec3D x)
@SHORT:Solves a small linear system $\A\cdot \overrightarrow{x} = \overrightarrow{b}$.
@RETURN:Vec3D : The solution of the linear system.
This method returns the solution of a small linear system with the following form:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a second order tensor defined by the object itself and $\overrightarrow{x}$ is a vector defined by parameter x.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::solve(const Vec3D &b) const
//-----------------------------------------------------------------------------
{
  return Vec3D(inverse() * b);
}

//-----------------------------------------------------------------------------
void Tensor2::polarExtract(double eigenVectors[3][3], double eigenValues[3], SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double sq[3];

  // eigenVectors 1
  double U0[6];
  U0[0] = dnlSquare(eigenVectors[0][0]);
  U0[1] = eigenVectors[0][0] * eigenVectors[1][0];
  U0[2] = eigenVectors[0][0] * eigenVectors[2][0];
  U0[3] = dnlSquare(eigenVectors[1][0]);
  U0[4] = eigenVectors[1][0] * eigenVectors[2][0];
  U0[5] = dnlSquare(eigenVectors[2][0]);
  // eigenVectors 2
  double U1[6];
  U1[0] = dnlSquare(eigenVectors[0][1]);
  U1[1] = eigenVectors[0][1] * eigenVectors[1][1];
  U1[2] = eigenVectors[0][1] * eigenVectors[2][1];
  U1[3] = dnlSquare(eigenVectors[1][1]);
  U1[4] = eigenVectors[1][1] * eigenVectors[2][1];
  U1[5] = dnlSquare(eigenVectors[2][1]);
  // eigenVectors 3
  double U2[6];
  U2[0] = dnlSquare(eigenVectors[0][2]);
  U2[1] = eigenVectors[0][2] * eigenVectors[1][2];
  U2[2] = eigenVectors[0][2] * eigenVectors[2][2];
  U2[3] = dnlSquare(eigenVectors[1][2]);
  U2[4] = eigenVectors[1][2] * eigenVectors[2][2];
  U2[5] = dnlSquare(eigenVectors[2][2]);

  sq[0] = sqrt(eigenValues[0]);
  sq[1] = sqrt(eigenValues[1]);
  sq[2] = sqrt(eigenValues[2]);
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];

  double Um1[6];
  double t1 = U._data[3] * U._data[5];
  double t2 = U._data[2] * U._data[4];
  double t4 = U._data[4] * U._data[4];
  double t5 = U._data[1] * U._data[5];
  double t6 = U._data[2] * U._data[3];

  double deter = U._data[0] * t1 + 2.0 * U._data[1] * t2 - U._data[0] * t4 - U._data[1] * t5 - U._data[2] * t6;

  Um1[0] = (t1 - t4) / deter;
  Um1[1] = (t2 - t5) / deter;
  Um1[2] = (U._data[1] * U._data[4] - t6) / deter;
  Um1[3] = (U._data[0] * U._data[5] - U._data[2] * U._data[2]) / deter;
  Um1[4] = (U._data[2] * U._data[1] - U._data[0] * U._data[4]) / deter;
  Um1[5] = (U._data[0] * U._data[3] - U._data[1] * U._data[1]) / deter;

  R._data[0] = _data[0] * Um1[0] + _data[1] * Um1[1] + _data[2] * Um1[2];
  R._data[1] = _data[0] * Um1[1] + _data[1] * Um1[3] + _data[2] * Um1[4];
  R._data[2] = _data[0] * Um1[2] + _data[1] * Um1[4] + _data[2] * Um1[5];
  R._data[3] = _data[3] * Um1[0] + _data[4] * Um1[1] + _data[5] * Um1[2];
  R._data[4] = _data[3] * Um1[1] + _data[4] * Um1[3] + _data[5] * Um1[4];
  R._data[5] = _data[3] * Um1[2] + _data[4] * Um1[4] + _data[5] * Um1[5];
  R._data[6] = _data[6] * Um1[0] + _data[7] * Um1[1] + _data[8] * Um1[2];
  R._data[7] = _data[6] * Um1[1] + _data[7] * Um1[3] + _data[8] * Um1[4];
  R._data[8] = _data[6] * Um1[2] + _data[7] * Um1[4] + _data[8] * Um1[5];
}

//-----------------------------------------------------------------------------
void Tensor2::polarExtractLnU(double eigenVectors[3][3], double eigenValues[3], SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double sq[3];

  // eigenVectors 1
  double U0[6];
  U0[0] = dnlSquare(eigenVectors[0][0]);
  U0[1] = eigenVectors[0][0] * eigenVectors[1][0];
  U0[2] = eigenVectors[0][0] * eigenVectors[2][0];
  U0[3] = dnlSquare(eigenVectors[1][0]);
  U0[4] = eigenVectors[1][0] * eigenVectors[2][0];
  U0[5] = dnlSquare(eigenVectors[2][0]);
  // eigenVectors 2
  double U1[6];
  U1[0] = dnlSquare(eigenVectors[0][1]);
  U1[1] = eigenVectors[0][1] * eigenVectors[1][1];
  U1[2] = eigenVectors[0][1] * eigenVectors[2][1];
  U1[3] = dnlSquare(eigenVectors[1][1]);
  U1[4] = eigenVectors[1][1] * eigenVectors[2][1];
  U1[5] = dnlSquare(eigenVectors[2][1]);
  // eigenVectors 3
  double U2[6];
  U2[0] = dnlSquare(eigenVectors[0][2]);
  U2[1] = eigenVectors[0][2] * eigenVectors[1][2];
  U2[2] = eigenVectors[0][2] * eigenVectors[2][2];
  U2[3] = dnlSquare(eigenVectors[1][2]);
  U2[4] = eigenVectors[1][2] * eigenVectors[2][2];
  U2[5] = dnlSquare(eigenVectors[2][2]);

  sq[0] = sqrt(eigenValues[0]);
  sq[1] = sqrt(eigenValues[1]);
  sq[2] = sqrt(eigenValues[2]);
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];

  double Um1[6];
  double t1 = U._data[3] * U._data[5];
  double t2 = U._data[2] * U._data[4];
  double t4 = U._data[4] * U._data[4];
  double t5 = U._data[1] * U._data[5];
  double t6 = U._data[2] * U._data[3];

  double deter = U._data[0] * t1 + 2.0 * U._data[1] * t2 - U._data[0] * t4 - U._data[1] * t5 - U._data[2] * t6;

  Um1[0] = (t1 - t4) / deter;
  Um1[1] = (t2 - t5) / deter;
  Um1[2] = (U._data[1] * U._data[4] - t6) / deter;
  Um1[3] = (U._data[0] * U._data[5] - U._data[2] * U._data[2]) / deter;
  Um1[4] = (U._data[2] * U._data[1] - U._data[0] * U._data[4]) / deter;
  Um1[5] = (U._data[0] * U._data[3] - U._data[1] * U._data[1]) / deter;

  R._data[0] = _data[0] * Um1[0] + _data[1] * Um1[1] + _data[2] * Um1[2];
  R._data[1] = _data[0] * Um1[1] + _data[1] * Um1[3] + _data[2] * Um1[4];
  R._data[2] = _data[0] * Um1[2] + _data[1] * Um1[4] + _data[2] * Um1[5];
  R._data[3] = _data[3] * Um1[0] + _data[4] * Um1[1] + _data[5] * Um1[2];
  R._data[4] = _data[3] * Um1[1] + _data[4] * Um1[3] + _data[5] * Um1[4];
  R._data[5] = _data[3] * Um1[2] + _data[4] * Um1[4] + _data[5] * Um1[5];
  R._data[6] = _data[6] * Um1[0] + _data[7] * Um1[1] + _data[8] * Um1[2];
  R._data[7] = _data[6] * Um1[1] + _data[7] * Um1[3] + _data[8] * Um1[4];
  R._data[8] = _data[6] * Um1[2] + _data[7] * Um1[4] + _data[8] * Um1[5];

  sq[0] = log(eigenValues[0]) / 2;
  sq[1] = log(eigenValues[1]) / 2;
  sq[2] = log(eigenValues[2]) / 2;
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];
}

//-----------------------------------------------------------------------------
void Tensor2::buildFTF(double FTF[3][3]) const
//-----------------------------------------------------------------------------
{
  FTF[0][0] = dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[6]);
  FTF[0][1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  FTF[0][2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  FTF[1][0] = FTF[0][1];
  FTF[1][1] = dnlSquare(_data[1]) + dnlSquare(_data[4]) + dnlSquare(_data[7]);
  FTF[1][2] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  FTF[2][0] = FTF[0][2];
  FTF[2][1] = FTF[1][2];
  FTF[2][2] = dnlSquare(_data[2]) + dnlSquare(_data[5]) + dnlSquare(_data[8]);
}

/*
@LABEL:Tensor2::polarQL(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the QL with implicit shifts algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\U$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\U$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarQL(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevq3(FTF, eigenVectors, eigenValues); // QL with implicit shifts

  // Extract the tensors for U and R
  polarExtract(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarQLLnU(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the QL with implicit shifts algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\log[\U]$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\log[\U]$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
\begin{equation*}
\log [\U] =\sum _{i=1}^{3}\log[\lambda_{i}](\overrightarrow{u}_{i}\otimes \overrightarrow{u}_{i})
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarQLLnU(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevq3(FTF, eigenVectors, eigenValues); // QL with implicit shifts

  // Extract the tensors for U and R
  polarExtractLnU(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarCuppen(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Cuppen’s Divide and Conquer algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\U$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\U$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarCuppen(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevd3(FTF, eigenVectors, eigenValues); // Cuppen

  // Extract the tensors for U and R
  polarExtract(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarCuppenLnU(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Cuppen’s Divide and Conquer algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\log[\U]$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\log[\U]$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
\begin{equation*}
\log [\U] =\sum _{i=1}^{3}\log[\lambda_{i}](\overrightarrow{u}_{i}\otimes \overrightarrow{u}_{i})
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarCuppenLnU(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevd3(FTF, eigenVectors, eigenValues); // Cuppen

  // Extract the tensors for U and R
  polarExtractLnU(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarJacobi(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Jacobi algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\U$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\U$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarJacobi(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevj3(FTF, eigenVectors, eigenValues); // Jacobi

  // Extract the tensors for U and R
  polarExtract(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarJacobiLnU(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Jacobi algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\log[\U]$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\log[\U]$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
\begin{equation*}
\log [\U] =\sum _{i=1}^{3}\log[\lambda_{i}](\overrightarrow{u}_{i}\otimes \overrightarrow{u}_{i})
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarJacobiLnU(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[3][3];
  double eigenVectors[3][3];
  double eigenValues[3];

  // Build the F(T).F symmetric matrix
  buildFTF(FTF);

  // Compute the eigenvalues and eigenvectors
  dsyevj3(FTF, eigenVectors, eigenValues); // Jacobi

  // Extract the tensors for U and R
  polarExtractLnU(eigenVectors, eigenValues, U, R);
}

/*
@LABEL:Tensor2::polarLapack(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Jacobi algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\U$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\U$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
It uses the \textsf{dgeev} function of the Lapack library which is far from efficient for a trivial 3x3 matrix. So this method is very slow.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarLapack(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[9];

  FTF[0] = dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[6]);
  FTF[1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  FTF[2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  FTF[3] = FTF[1];
  FTF[4] = dnlSquare(_data[1]) + dnlSquare(_data[4]) + dnlSquare(_data[7]);
  FTF[5] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  FTF[6] = FTF[2];
  FTF[7] = FTF[5];
  FTF[8] = dnlSquare(_data[2]) + dnlSquare(_data[5]) + dnlSquare(_data[8]);

  double eigenVectors[9];
  double tmpMatrix[9];
  double eigenValues[3];
  double tmpVector[3];
  double sq[3];

  int info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', 3, FTF, 3, eigenValues, tmpVector, tmpMatrix, 3, eigenVectors, 3);

  // eigenVectors 1
  double U0[6];
  U0[0] = dnlSquare(eigenVectors[0]);
  U0[1] = eigenVectors[0] * eigenVectors[3];
  U0[2] = eigenVectors[0] * eigenVectors[6];
  U0[3] = dnlSquare(eigenVectors[3]);
  U0[4] = eigenVectors[3] * eigenVectors[6];
  U0[5] = dnlSquare(eigenVectors[6]);
  // eigenVectors 2
  double U1[6];
  U1[0] = dnlSquare(eigenVectors[1]);
  U1[1] = eigenVectors[1] * eigenVectors[4];
  U1[2] = eigenVectors[1] * eigenVectors[7];
  U1[3] = dnlSquare(eigenVectors[4]);
  U1[4] = eigenVectors[4] * eigenVectors[7];
  U1[5] = dnlSquare(eigenVectors[7]);
  // eigenVectors 3
  double U2[6];
  U2[0] = dnlSquare(eigenVectors[2]);
  U2[1] = eigenVectors[2] * eigenVectors[5];
  U2[2] = eigenVectors[2] * eigenVectors[8];
  U2[3] = dnlSquare(eigenVectors[5]);
  U2[4] = eigenVectors[5] * eigenVectors[8];
  U2[5] = dnlSquare(eigenVectors[8]);

  sq[0] = sqrt(eigenValues[0]);
  sq[1] = sqrt(eigenValues[1]);
  sq[2] = sqrt(eigenValues[2]);
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];

  double Um1[6];
  double t1 = U._data[3] * U._data[5];
  double t2 = U._data[2] * U._data[4];
  double t4 = U._data[4] * U._data[4];
  double t5 = U._data[1] * U._data[5];
  double t6 = U._data[2] * U._data[3];

  double deter = U._data[0] * t1 + 2.0 * U._data[1] * t2 - U._data[0] * t4 - U._data[1] * t5 - U._data[2] * t6;
  Um1[0] = (t1 - t4) / deter;
  Um1[1] = (t2 - t5) / deter;
  Um1[2] = (U._data[1] * U._data[4] - t6) / deter;
  Um1[3] = (U._data[0] * U._data[5] - U._data[2] * U._data[2]) / deter;
  Um1[4] = (U._data[2] * U._data[1] - U._data[0] * U._data[4]) / deter;
  Um1[5] = (U._data[0] * U._data[3] - U._data[1] * U._data[1]) / deter;

  R._data[0] = _data[0] * Um1[0] + _data[1] * Um1[1] + _data[2] * Um1[2];
  R._data[1] = _data[0] * Um1[1] + _data[1] * Um1[3] + _data[2] * Um1[4];
  R._data[2] = _data[0] * Um1[2] + _data[1] * Um1[4] + _data[2] * Um1[5];
  R._data[3] = _data[3] * Um1[0] + _data[4] * Um1[1] + _data[5] * Um1[2];
  R._data[4] = _data[3] * Um1[1] + _data[4] * Um1[3] + _data[5] * Um1[4];
  R._data[5] = _data[3] * Um1[2] + _data[4] * Um1[4] + _data[5] * Um1[5];
  R._data[6] = _data[6] * Um1[0] + _data[7] * Um1[1] + _data[8] * Um1[2];
  R._data[7] = _data[6] * Um1[1] + _data[7] * Um1[3] + _data[8] * Um1[4];
  R._data[8] = _data[6] * Um1[2] + _data[7] * Um1[4] + _data[8] * Um1[5];
}

/*
@LABEL:Tensor2::polarLapackLnU(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the Jacobi algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\log[\U]$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\log[\U]$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
\begin{equation*}
\log [\U] =\sum _{i=1}^{3}\log[\lambda_{i}](\overrightarrow{u}_{i}\otimes \overrightarrow{u}_{i})
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
It uses the \textsf{dgeev} function of the Lapack library which is far from efficient for a trivial 3x3 matrix. So this method is very slow.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarLapackLnU(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  double FTF[9];

  FTF[0] = dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[6]);
  FTF[1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  FTF[2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  FTF[3] = FTF[1];
  FTF[4] = dnlSquare(_data[1]) + dnlSquare(_data[4]) + dnlSquare(_data[7]);
  FTF[5] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  FTF[6] = FTF[2];
  FTF[7] = FTF[5];
  FTF[8] = dnlSquare(_data[2]) + dnlSquare(_data[5]) + dnlSquare(_data[8]);

  double eigenVectors[9];
  double tmpMatrix[9];
  double eigenValues[3];
  double tmpVector[3];
  double sq[3];

  int info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', 3, FTF, 3, eigenValues, tmpVector, tmpMatrix, 3, eigenVectors, 3);

  // eigenVectors 1
  double U0[6];
  U0[0] = dnlSquare(eigenVectors[0]);
  U0[1] = eigenVectors[0] * eigenVectors[3];
  U0[2] = eigenVectors[0] * eigenVectors[6];
  U0[3] = dnlSquare(eigenVectors[3]);
  U0[4] = eigenVectors[3] * eigenVectors[6];
  U0[5] = dnlSquare(eigenVectors[6]);
  // eigenVectors 2
  double U1[6];
  U1[0] = dnlSquare(eigenVectors[1]);
  U1[1] = eigenVectors[1] * eigenVectors[4];
  U1[2] = eigenVectors[1] * eigenVectors[7];
  U1[3] = dnlSquare(eigenVectors[4]);
  U1[4] = eigenVectors[4] * eigenVectors[7];
  U1[5] = dnlSquare(eigenVectors[7]);
  // eigenVectors 3
  double U2[6];
  U2[0] = dnlSquare(eigenVectors[2]);
  U2[1] = eigenVectors[2] * eigenVectors[5];
  U2[2] = eigenVectors[2] * eigenVectors[8];
  U2[3] = dnlSquare(eigenVectors[5]);
  U2[4] = eigenVectors[5] * eigenVectors[8];
  U2[5] = dnlSquare(eigenVectors[8]);

  sq[0] = sqrt(eigenValues[0]);
  sq[1] = sqrt(eigenValues[1]);
  sq[2] = sqrt(eigenValues[2]);
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];

  double Um1[6];
  double t1 = U._data[3] * U._data[5];
  double t2 = U._data[2] * U._data[4];
  double t4 = U._data[4] * U._data[4];
  double t5 = U._data[1] * U._data[5];
  double t6 = U._data[2] * U._data[3];

  double deter = U._data[0] * t1 + 2.0 * U._data[1] * t2 - U._data[0] * t4 - U._data[1] * t5 - U._data[2] * t6;
  Um1[0] = (t1 - t4) / deter;
  Um1[1] = (t2 - t5) / deter;
  Um1[2] = (U._data[1] * U._data[4] - t6) / deter;
  Um1[3] = (U._data[0] * U._data[5] - U._data[2] * U._data[2]) / deter;
  Um1[4] = (U._data[2] * U._data[1] - U._data[0] * U._data[4]) / deter;
  Um1[5] = (U._data[0] * U._data[3] - U._data[1] * U._data[1]) / deter;

  R._data[0] = _data[0] * Um1[0] + _data[1] * Um1[1] + _data[2] * Um1[2];
  R._data[1] = _data[0] * Um1[1] + _data[1] * Um1[3] + _data[2] * Um1[4];
  R._data[2] = _data[0] * Um1[2] + _data[1] * Um1[4] + _data[2] * Um1[5];
  R._data[3] = _data[3] * Um1[0] + _data[4] * Um1[1] + _data[5] * Um1[2];
  R._data[4] = _data[3] * Um1[1] + _data[4] * Um1[3] + _data[5] * Um1[4];
  R._data[5] = _data[3] * Um1[2] + _data[4] * Um1[4] + _data[5] * Um1[5];
  R._data[6] = _data[6] * Um1[0] + _data[7] * Um1[1] + _data[8] * Um1[2];
  R._data[7] = _data[6] * Um1[1] + _data[7] * Um1[3] + _data[8] * Um1[4];
  R._data[8] = _data[6] * Um1[2] + _data[7] * Um1[4] + _data[8] * Um1[5];

  sq[0] = log(eigenValues[0]) / 2;
  sq[1] = log(eigenValues[1]) / 2;
  sq[2] = log(eigenValues[2]) / 2;
  U._data[0] = sq[0] * U0[0] + sq[1] * U1[0] + sq[2] * U2[0];
  U._data[1] = sq[0] * U0[1] + sq[1] * U1[1] + sq[2] * U2[1];
  U._data[2] = sq[0] * U0[2] + sq[1] * U1[2] + sq[2] * U2[2];
  U._data[3] = sq[0] * U0[3] + sq[1] * U1[3] + sq[2] * U2[3];
  U._data[4] = sq[0] * U0[4] + sq[1] * U1[4] + sq[2] * U2[4];
  U._data[5] = sq[0] * U0[5] + sq[1] * U1[5] + sq[2] * U2[5];
}

/*
@LABEL:Tensor2::polarLnU(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the old \DynELA algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\U$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\log[\U]$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
\begin{equation*}
\log [\U] =\sum _{i=1}^{3}\log[\lambda_{i}](\overrightarrow{u}_{i}\otimes \overrightarrow{u}_{i})
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polarLnU(SymTensor2 &LnU, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  short p, q;
  double alpha, theta, Ctheta, Stheta;
  double _tmp1, _tmp2, _tmp3, _tmp4, _tmp5, _tmp6, _tmp7, _tmp8;
  double Rot11, Rot12, Rot13, Rot21, Rot22, Rot23, Rot31, Rot32, Rot33;
  SymTensor2 FF;
  short it = 1;

  // calcul de FF=F(T).F
  FF._data[0] = dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[6]);
  FF._data[1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  FF._data[2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  FF._data[3] = dnlSquare(_data[1]) + dnlSquare(_data[4]) + dnlSquare(_data[7]);
  FF._data[4] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  FF._data[5] = dnlSquare(_data[2]) + dnlSquare(_data[5]) + dnlSquare(_data[8]);

  // initialisation de Rot
  Rot11 = 1;
  Rot22 = 1;
  Rot33 = 1;
  Rot12 = 0;
  Rot13 = 0;
  Rot21 = 0;
  Rot23 = 0;
  Rot31 = 0;
  Rot32 = 0;

  while (it < 30)
  {
    // search for the max term between 01, 02 and 12
    q = (dnlAbs(FF._data[1]) > dnlAbs(FF._data[2]) ? 1 : 2);
    p = (dnlAbs(FF._data[q]) > dnlAbs(FF._data[4]) ? 0 : 1);
    if (p == 1)
      q = 2;

    // test de convergence
    if (dnlAbs(FF._data[SymTensor2::_internalIndexes.index[p][q]]) < precisionPolarDecompose)
    {
      _tmp1 = log(FF._data[0]) / 2;
      _tmp2 = log(FF._data[3]) / 2;
      _tmp3 = log(FF._data[5]) / 2;
      LnU._data[0] = dnlSquare(Rot11) * _tmp1 + dnlSquare(Rot12) * _tmp2 + dnlSquare(Rot13) * _tmp3;
      LnU._data[3] = dnlSquare(Rot21) * _tmp1 + dnlSquare(Rot22) * _tmp2 + dnlSquare(Rot23) * _tmp3;
      LnU._data[5] = dnlSquare(Rot31) * _tmp1 + dnlSquare(Rot32) * _tmp2 + dnlSquare(Rot33) * _tmp3;
      LnU._data[1] = (Rot11 * Rot21 * _tmp1 + Rot12 * Rot22 * _tmp2 + Rot13 * Rot23 * _tmp3);
      LnU._data[2] = (Rot11 * Rot31 * _tmp1 + Rot12 * Rot32 * _tmp2 + Rot13 * Rot33 * _tmp3);
      LnU._data[4] = (Rot21 * Rot31 * _tmp1 + Rot22 * Rot32 * _tmp2 + Rot23 * Rot33 * _tmp3);

      _tmp1 = 1 / sqrt(FF._data[0]);
      _tmp2 = 1 / sqrt(FF._data[3]);
      _tmp3 = 1 / sqrt(FF._data[5]);
      _tmp4 = (Rot13 * _data[0] + Rot23 * _data[1] + Rot33 * _data[2]) * _tmp3;
      _tmp5 = (Rot12 * _data[0] + Rot22 * _data[1] + Rot32 * _data[2]) * _tmp2;
      _tmp6 = (Rot11 * _data[0] + Rot21 * _data[1] + Rot31 * _data[2]) * _tmp1;
      R._data[0] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[1] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[2] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[3] + Rot23 * _data[4] + Rot33 * _data[5]) * _tmp3;
      _tmp5 = (Rot12 * _data[3] + Rot22 * _data[4] + Rot32 * _data[5]) * _tmp2;
      _tmp6 = (Rot11 * _data[3] + Rot21 * _data[4] + Rot31 * _data[5]) * _tmp1;
      R._data[3] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[4] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[5] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[6] + Rot23 * _data[7] + Rot33 * _data[8]) * _tmp3;
      _tmp5 = (Rot12 * _data[6] + Rot22 * _data[7] + Rot32 * _data[8]) * _tmp2;
      _tmp6 = (Rot11 * _data[6] + Rot21 * _data[7] + Rot31 * _data[8]) * _tmp1;
      R._data[6] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[7] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[8] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      return;
    }

    // calcul de l'angle
    alpha = (FF._data[SymTensor2::_internalIndexes.index[q][q]] -
             FF._data[SymTensor2::_internalIndexes.index[p][p]]) /
            (2 * FF._data[SymTensor2::_internalIndexes.index[p][q]]);
    theta = (alpha != 0 ? 1 / (alpha + sqrt(alpha * alpha + 1) * (alpha > 0 ? 1 : -1)) : 1);

    // calcul des cosinus
    Ctheta = 1 / (sqrt(1 + theta * theta));
    Stheta = theta * Ctheta;

    // calcul du nouveau tenseur
    R.setToUnity();
    R(p, q) = Stheta;
    R(q, p) = -Stheta;
    R(p, p) = R(q, q) = Ctheta;

    // produit R(T).FF.R avec FF symétrique
    _tmp1 = FF._data[0] * R._data[0] + FF._data[1] * R._data[3] + FF._data[2] * R._data[6];
    _tmp2 = FF._data[1] * R._data[0] + FF._data[3] * R._data[3] + FF._data[4] * R._data[6];
    _tmp3 = FF._data[2] * R._data[0] + FF._data[4] * R._data[3] + FF._data[5] * R._data[6];
    _tmp4 = FF._data[0] * R._data[1] + FF._data[1] * R._data[4] + FF._data[2] * R._data[7];
    _tmp5 = FF._data[1] * R._data[1] + FF._data[3] * R._data[4] + FF._data[4] * R._data[7];
    _tmp6 = FF._data[2] * R._data[1] + FF._data[4] * R._data[4] + FF._data[5] * R._data[7];
    FF._data[5] = R._data[2] * (FF._data[0] * R._data[2] + FF._data[1] * R._data[5] + FF._data[2] * R._data[8]) +
                  R._data[5] * (FF._data[1] * R._data[2] + FF._data[3] * R._data[5] + FF._data[4] * R._data[8]) +
                  R._data[8] * (FF._data[2] * R._data[2] + FF._data[4] * R._data[5] + FF._data[5] * R._data[8]);
    FF._data[0] = R._data[0] * _tmp1 + R._data[3] * _tmp2 + R._data[6] * _tmp3;
    FF._data[1] = R._data[1] * _tmp1 + R._data[4] * _tmp2 + R._data[7] * _tmp3;
    FF._data[2] = R._data[2] * _tmp1 + R._data[5] * _tmp2 + R._data[8] * _tmp3;
    FF._data[3] = R._data[1] * _tmp4 + R._data[4] * _tmp5 + R._data[7] * _tmp6;
    FF._data[4] = R._data[2] * _tmp4 + R._data[5] * _tmp5 + R._data[8] * _tmp6;

    // cumulate of R
    _tmp1 = R._data[0] * Rot11 + R._data[3] * Rot12 + R._data[6] * Rot13;
    _tmp2 = R._data[1] * Rot11 + R._data[4] * Rot12 + R._data[7] * Rot13;
    Rot13 = R._data[2] * Rot11 + R._data[5] * Rot12 + R._data[8] * Rot13;
    _tmp4 = R._data[0] * Rot21 + R._data[3] * Rot22 + R._data[6] * Rot23;
    _tmp5 = R._data[1] * Rot21 + R._data[4] * Rot22 + R._data[7] * Rot23;
    Rot23 = R._data[2] * Rot21 + R._data[5] * Rot22 + R._data[8] * Rot23;
    _tmp7 = R._data[0] * Rot31 + R._data[3] * Rot32 + R._data[6] * Rot33;
    _tmp8 = R._data[1] * Rot31 + R._data[4] * Rot32 + R._data[7] * Rot33;
    Rot33 = R._data[2] * Rot31 + R._data[5] * Rot32 + R._data[8] * Rot33;
    Rot11 = _tmp1;
    Rot12 = _tmp2;
    Rot21 = _tmp4;
    Rot22 = _tmp5;
    Rot31 = _tmp7;
    Rot32 = _tmp8;
    it++;
  }

  std::cout << (*this) << std::endl;
  fatalError("Tensor2::polarLnU", "No convergence");
}

/*
@LABEL:Tensor2::polar(SymTensor2 U, Tensor2 R)
@SHORT:Polar decomposition of a second order tensor using the old \DynELA algorithm.
@RETURN:SymTensor2 and Tensor2
@ARG:SymTensor2 & U & Symmetric tensor $\log[\U]$
@ARG:Tensor2 & R & Rotation tensor $\R$
This method computes the polar decomposition of a second order tensor $\F$ and returns the symmetric tensor $\R$ and the tensor $\U$ so that:
\begin{equation*}
\F = \R \cdot \U
\end{equation*}
where $\F$ is a second order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::polar(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  short p, q;
  double alpha, theta, Ctheta, Stheta;
  double _tmp1, _tmp2, _tmp3, _tmp4, _tmp5, _tmp6, _tmp7, _tmp8;
  double Rot11, Rot12, Rot13, Rot21, Rot22, Rot23, Rot31, Rot32, Rot33;
  SymTensor2 FF;
  short it = 1;

  // calcul de FF=F(T).F
  FF._data[0] = dnlSquare(_data[0]) + dnlSquare(_data[3]) + dnlSquare(_data[6]);
  FF._data[1] = _data[0] * _data[1] + _data[3] * _data[4] + _data[6] * _data[7];
  FF._data[2] = _data[0] * _data[2] + _data[3] * _data[5] + _data[6] * _data[8];
  FF._data[3] = dnlSquare(_data[1]) + dnlSquare(_data[4]) + dnlSquare(_data[7]);
  FF._data[4] = _data[1] * _data[2] + _data[4] * _data[5] + _data[7] * _data[8];
  FF._data[5] = dnlSquare(_data[2]) + dnlSquare(_data[5]) + dnlSquare(_data[8]);

  // initialisation de Rot
  Rot11 = 1;
  Rot22 = 1;
  Rot33 = 1;
  Rot12 = 0;
  Rot13 = 0;
  Rot21 = 0;
  Rot23 = 0;
  Rot31 = 0;
  Rot32 = 0;

  while (it < 30)
  {
    // search for the max term between 12, 13 and 23
    q = (dnlAbs(FF._data[1]) > dnlAbs(FF._data[2]) ? 1 : 2);
    p = (dnlAbs(FF._data[q]) > dnlAbs(FF._data[4]) ? 0 : 1);
    if (p == 1)
      q = 2;

    // test de convergence
    if (dnlAbs(FF._data[SymTensor2::_internalIndexes.index[p][q]]) < precisionPolarDecompose)
    {
      _tmp1 = sqrt(FF._data[0]);
      _tmp2 = sqrt(FF._data[3]);
      _tmp3 = sqrt(FF._data[5]);
      U._data[0] = (dnlSquare(Rot11) * _tmp1 + dnlSquare(Rot12) * _tmp2 + dnlSquare(Rot13) * _tmp3);
      U._data[3] = (dnlSquare(Rot21) * _tmp1 + dnlSquare(Rot22) * _tmp2 + dnlSquare(Rot23) * _tmp3);
      U._data[5] = (dnlSquare(Rot31) * _tmp1 + dnlSquare(Rot32) * _tmp2 + dnlSquare(Rot33) * _tmp3);
      U._data[1] = (Rot11 * Rot21 * _tmp1 + Rot12 * Rot22 * _tmp2 + Rot13 * Rot23 * _tmp3);
      U._data[2] = (Rot11 * Rot31 * _tmp1 + Rot12 * Rot32 * _tmp2 + Rot13 * Rot33 * _tmp3);
      U._data[4] = (Rot21 * Rot31 * _tmp1 + Rot22 * Rot32 * _tmp2 + Rot23 * Rot33 * _tmp3);

      _tmp1 = 1 / sqrt(FF._data[0]);
      _tmp2 = 1 / sqrt(FF._data[3]);
      _tmp3 = 1 / sqrt(FF._data[5]);
      _tmp4 = (Rot13 * _data[0] + Rot23 * _data[1] + Rot33 * _data[2]) * _tmp3;
      _tmp5 = (Rot12 * _data[0] + Rot22 * _data[1] + Rot32 * _data[2]) * _tmp2;
      _tmp6 = (Rot11 * _data[0] + Rot21 * _data[1] + Rot31 * _data[2]) * _tmp1;
      R._data[0] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[1] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[2] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[3] + Rot23 * _data[4] + Rot33 * _data[5]) * _tmp3;
      _tmp5 = (Rot12 * _data[3] + Rot22 * _data[4] + Rot32 * _data[5]) * _tmp2;
      _tmp6 = (Rot11 * _data[3] + Rot21 * _data[4] + Rot31 * _data[5]) * _tmp1;
      R._data[3] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[4] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[5] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[6] + Rot23 * _data[7] + Rot33 * _data[8]) * _tmp3;
      _tmp5 = (Rot12 * _data[6] + Rot22 * _data[7] + Rot32 * _data[8]) * _tmp2;
      _tmp6 = (Rot11 * _data[6] + Rot21 * _data[7] + Rot31 * _data[8]) * _tmp1;
      R._data[6] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[7] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[8] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      return;
    }

    // calcul de l'angle
    alpha = (FF._data[SymTensor2::_internalIndexes.index[q][q]] - FF._data[SymTensor2::_internalIndexes.index[p][p]]) /
            (2 * FF._data[SymTensor2::_internalIndexes.index[p][q]]);
    theta = (alpha != 0 ? 1 / (alpha + sqrt(alpha * alpha + 1) * (alpha > 0 ? 1 : -1)) : 1);

    // calcul des cosinus
    Ctheta = 1 / (sqrt(1 + theta * theta));
    Stheta = theta * Ctheta;

    // calcul du nouveau tenseur
    R.setToUnity();
    R(p, q) = Stheta;
    R(q, p) = -Stheta;
    R(p, p) = R(q, q) = Ctheta;

    // produit R(T).FF.R avec FF symétrique
    _tmp1 = FF._data[0] * R._data[0] + FF._data[1] * R._data[3] + FF._data[2] * R._data[6];
    _tmp2 = FF._data[1] * R._data[0] + FF._data[3] * R._data[3] + FF._data[4] * R._data[6];
    _tmp3 = FF._data[2] * R._data[0] + FF._data[4] * R._data[3] + FF._data[5] * R._data[6];
    _tmp4 = FF._data[0] * R._data[1] + FF._data[1] * R._data[4] + FF._data[2] * R._data[7];
    _tmp5 = FF._data[1] * R._data[1] + FF._data[3] * R._data[4] + FF._data[4] * R._data[7];
    _tmp6 = FF._data[2] * R._data[1] + FF._data[4] * R._data[4] + FF._data[5] * R._data[7];
    FF._data[5] = R._data[2] * (FF._data[0] * R._data[2] + FF._data[1] * R._data[5] + FF._data[2] * R._data[8]) +
                  R._data[5] * (FF._data[1] * R._data[2] + FF._data[3] * R._data[5] + FF._data[4] * R._data[8]) +
                  R._data[8] * (FF._data[2] * R._data[2] + FF._data[4] * R._data[5] + FF._data[5] * R._data[8]);
    FF._data[0] = R._data[0] * _tmp1 + R._data[3] * _tmp2 + R._data[6] * _tmp3;
    FF._data[1] = R._data[1] * _tmp1 + R._data[4] * _tmp2 + R._data[7] * _tmp3;
    FF._data[2] = R._data[2] * _tmp1 + R._data[5] * _tmp2 + R._data[8] * _tmp3;
    FF._data[3] = R._data[1] * _tmp4 + R._data[4] * _tmp5 + R._data[7] * _tmp6;
    FF._data[4] = R._data[2] * _tmp4 + R._data[5] * _tmp5 + R._data[8] * _tmp6;

    // cumulate of R
    _tmp1 = R._data[0] * Rot11 + R._data[3] * Rot12 + R._data[6] * Rot13;
    _tmp2 = R._data[1] * Rot11 + R._data[4] * Rot12 + R._data[7] * Rot13;
    Rot13 = R._data[2] * Rot11 + R._data[5] * Rot12 + R._data[8] * Rot13;
    _tmp4 = R._data[0] * Rot21 + R._data[3] * Rot22 + R._data[6] * Rot23;
    _tmp5 = R._data[1] * Rot21 + R._data[4] * Rot22 + R._data[7] * Rot23;
    Rot23 = R._data[2] * Rot21 + R._data[5] * Rot22 + R._data[8] * Rot23;
    _tmp7 = R._data[0] * Rot31 + R._data[3] * Rot32 + R._data[6] * Rot33;
    _tmp8 = R._data[1] * Rot31 + R._data[4] * Rot32 + R._data[7] * Rot33;
    Rot33 = R._data[2] * Rot31 + R._data[5] * Rot32 + R._data[8] * Rot33;
    Rot11 = _tmp1;
    Rot12 = _tmp2;
    Rot21 = _tmp4;
    Rot22 = _tmp5;
    Rot31 = _tmp7;
    Rot32 = _tmp8;
    it++;
  }

  std::cout << (*this) << std::endl;
  fatalError("Tensor2::polar", "No convergence");
}

//  Saves the content of a Tensor2 into a NumPy file
//-----------------------------------------------------------------------------
void Tensor2::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {3, 3}, mode);
}

//  Saves the content of a Tensor2 into a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor2::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {3, 3}, mode);
}

//  Read the content of a Tensor2 from a NumPy file
//-----------------------------------------------------------------------------
void Tensor2::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);

  if (arr.num_vals != 9)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//  Read the content of a Tensor2 from a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor2::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);

  if (arr.num_vals != 9)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
