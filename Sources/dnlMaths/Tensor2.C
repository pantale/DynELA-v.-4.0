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
@RETURN:Tensor2
All components are initialized to zero by default.
\begin{equation*}
\T=\left[\begin{array}{ccc}
0&0&0\\
0&0&0\\
0&0&0
\end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2::Tensor2()
//-----------------------------------------------------------------------------
{
  setToValue(0);
}

/*
@LABEL:Tensor2::Tensor2(Tensor2)
@SHORT:Copy constructor of the Tensor2 class.
@RETURN:Tensor2
@END
*/
//-----------------------------------------------------------------------------
Tensor2::Tensor2(const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  memcpy(_data, tensor._data, 9 * sizeof(double));
}

/*
@LABEL:Tensor2::Tensor2(double,...)
@SHORT:Constructor of the Tensor2 class.
@RETURN:Tensor2
Constructor of a second order tensor with initialization of the 9 values.
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

/*
@LABEL:Tensor2::~Tensor2()
@SHORT:Destructor of the Tensor2 class.
@END
*/
//-----------------------------------------------------------------------------
Tensor2::~Tensor2()
//-----------------------------------------------------------------------------
{
}

/*
  Send the content of a second order tensor to the output flux for display

  \code
  Tensor2 _data;
  std::cout << _data << endl;
  \endcode
  - outputStream Output flux
  - tensor tensor
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &outputStream, const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.print(outputStream);
  return outputStream;
}

/*
  Print the content of a second order tensor to the output flux for display

  - outputStream Output flux
*/
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
@SHORT:Unity tensor.
@WARNING:This method modifies its own argument
This method transforms the current tensor to a unity tensor.
\begin{equation*}
\T=\left[\begin{array}{ccc}
1&0&0\\
0&1&0\\
0&0&1
\end{array}\right]
\end{equation*}
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
@SHORT:Sets all components of the tensor to zero.
@WARNING:This method modifies its own argument
\begin{equation*}
\T=\left[\begin{array}{ccc}
0&0&0\\
0&0&0\\
0&0&0
\end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
void Tensor2::setToZero()
//-----------------------------------------------------------------------------
{
  setToValue(0);
}

/*
@LABEL:Tensor2::operator=(double)
@SHORT:Fill a second order tensor with a scalar value.
@RETURN:Tensor2
This method is a surdefinition of the = operator for the second order tensor class.
\begin{equation*}
\T=\left[\begin{array}{ccc}
m&m&m\\
m&m&m\\
m&m&m
\end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

/*
  Copy the content of a second order tensor into a new one

  This method is the so called = operator between two second order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.
  \code
  Tensor2 tensor1, tensor2;
  tensor1 = tensor2; // copy of the tensor
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  memcpy(_data, tensor._data, 9 * sizeof(double));
  return *this;
}

/*
  Copy a SymTensor2 into a Tensor2
*/
//-----------------------------------------------------------------------------
Tensor2 &Tensor2::operator=(const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  _data[0] = tensor._data[0];
  _data[1] = tensor._data[1];
  _data[2] = tensor._data[2];
  _data[3] = tensor._data[1];
  _data[4] = tensor._data[3];
  _data[5] = tensor._data[4];
  _data[6] = tensor._data[2];
  _data[7] = tensor._data[4];
  _data[8] = tensor._data[5];
  return *this;
}

/*
  Addition of 2 second order tensors

  This method defines the addition of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A + B \f]
  \code
  Tensor2 tensor1,tensor2,t3;
  t3 = tensor1 + tensor2; // sum of 2 second order tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator+(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] + tensor._data[0];
  result._data[1] = _data[1] + tensor._data[1];
  result._data[2] = _data[2] + tensor._data[2];
  result._data[3] = _data[3] + tensor._data[3];
  result._data[4] = _data[4] + tensor._data[4];
  result._data[5] = _data[5] + tensor._data[5];
  result._data[6] = _data[6] + tensor._data[6];
  result._data[7] = _data[7] + tensor._data[7];
  result._data[8] = _data[8] + tensor._data[8];

  // renvoi de l'objet
  return result;
}

/*
  Difference of 2 second order tensors

  This method defines the difference of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A - B \f]
  \code
  Tensor2 tensor1,tensor2,t3;
  t3 = tensor1 - tensor2; // difference of 2 second order tensors
  \endcode
  - tensor1 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator-(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] - tensor._data[0];
  result._data[1] = _data[1] - tensor._data[1];
  result._data[2] = _data[2] - tensor._data[2];
  result._data[3] = _data[3] - tensor._data[3];
  result._data[4] = _data[4] - tensor._data[4];
  result._data[5] = _data[5] - tensor._data[5];
  result._data[6] = _data[6] - tensor._data[6];
  result._data[7] = _data[7] - tensor._data[7];
  result._data[8] = _data[8] - tensor._data[8];

  // renvoi de l'objet
  return result;
}

/*
  Return the opposite tensor
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
  Multiplication of a second order tensor by a scalar value

  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . \lambda \f]
  \code
  Tensor2 tensor1,result;
  double l;
  result = tensor1 * l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const double lambda) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = lambda * _data[0];
  result._data[1] = lambda * _data[1];
  result._data[2] = lambda * _data[2];
  result._data[3] = lambda * _data[3];
  result._data[4] = lambda * _data[4];
  result._data[5] = lambda * _data[5];
  result._data[6] = lambda * _data[6];
  result._data[7] = lambda * _data[7];
  result._data[8] = lambda * _data[8];

  return result;
}

/*
  Division of a second order tensor by a scalar value

  This method defines the division of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  \code
  Tensor2 tensor1,result;
  double l;
  result = tensor1 / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator/(const double lambda) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

#ifdef VERIF_maths
  if (lambda == 0)
  {
    fatalError("Tensor2:: operator /", "divide by zero");
  }
#endif

  result._data[0] = _data[0] / lambda;
  result._data[1] = _data[1] / lambda;
  result._data[2] = _data[2] / lambda;
  result._data[3] = _data[3] / lambda;
  result._data[4] = _data[4] / lambda;
  result._data[5] = _data[5] / lambda;
  result._data[6] = _data[6] / lambda;
  result._data[7] = _data[7] / lambda;
  result._data[8] = _data[8] / lambda;

  return result;
}

/*
  Multiplication of a second order tensor by a scalar value

  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \lambda . A \f]
  \code
  Tensor2 tensor, result;
  double l;
  result = l * tensor; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 operator*(const double &lambda, const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = lambda * tensor._data[0];
  result._data[1] = lambda * tensor._data[1];
  result._data[2] = lambda * tensor._data[2];
  result._data[3] = lambda * tensor._data[3];
  result._data[4] = lambda * tensor._data[4];
  result._data[5] = lambda * tensor._data[5];
  result._data[6] = lambda * tensor._data[6];
  result._data[7] = lambda * tensor._data[7];
  result._data[8] = lambda * tensor._data[8];

  return result;
}

/*
@LABEL:Tensor2::singleProduct()
@SHORT:Contracted product of a second order tensor by itself.
@RETURN:Tensor2
This method defines a single contracted product of of a second order tensor by itself.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \A
\end{equation*}
where $\A$ is a two second order tensor.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::singleProduct() const
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
@LABEL:Tensor2::singleProductTxN()
@SHORT:Contracted product of a second order tensor by its transpose.
@RETURN:SymTensor2
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A^T\cdot \A
\end{equation*}
where $\A$ is a second order tensor. Result is a symmetric second order tensor.
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Tensor2::singleProductTxN() const
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
@LABEL:Tensor2::singleProductNxT()
@SHORT:Contracted product of a second order tensor by its transpose.
@RETURN:SymTensor2
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \A^T
\end{equation*}
where $\A$ is a second order tensor. Result is a symmetric second order tensor.
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Tensor2::singleProductNxT() const
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
@LABEL:Tensor2::operator*(Tensor2)
@SHORT:Multiplication of 2 second order tensors.
@RETURN:Tensor2
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ and $\B$ are two second order tensors.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[3] + _data[2] * tensor._data[6];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[4] + _data[2] * tensor._data[7];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[5] + _data[2] * tensor._data[8];
  result._data[3] = _data[3] * tensor._data[0] + _data[4] * tensor._data[3] + _data[5] * tensor._data[6];
  result._data[4] = _data[3] * tensor._data[1] + _data[4] * tensor._data[4] + _data[5] * tensor._data[7];
  result._data[5] = _data[3] * tensor._data[2] + _data[4] * tensor._data[5] + _data[5] * tensor._data[8];
  result._data[6] = _data[6] * tensor._data[0] + _data[7] * tensor._data[3] + _data[8] * tensor._data[6];
  result._data[7] = _data[6] * tensor._data[1] + _data[7] * tensor._data[4] + _data[8] * tensor._data[7];
  result._data[8] = _data[6] * tensor._data[2] + _data[7] * tensor._data[5] + _data[8] * tensor._data[8];

  return result;
}

/*
@LABEL:Tensor2::operator*(SymTensor2)
@SHORT:Multiplication of 2 second order tensors.
@RETURN:Tensor2
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a second order tensor and $\B$ is a symmetric second order tensor.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::operator*(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[1] + _data[2] * tensor._data[2];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[3] + _data[2] * tensor._data[4];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[4] + _data[2] * tensor._data[5];
  result._data[3] = _data[3] * tensor._data[0] + _data[4] * tensor._data[1] + _data[5] * tensor._data[2];
  result._data[4] = _data[3] * tensor._data[1] + _data[4] * tensor._data[3] + _data[5] * tensor._data[4];
  result._data[5] = _data[3] * tensor._data[2] + _data[4] * tensor._data[4] + _data[5] * tensor._data[5];
  result._data[6] = _data[6] * tensor._data[0] + _data[7] * tensor._data[1] + _data[8] * tensor._data[2];
  result._data[7] = _data[6] * tensor._data[1] + _data[7] * tensor._data[3] + _data[8] * tensor._data[4];
  result._data[8] = _data[6] * tensor._data[2] + _data[7] * tensor._data[4] + _data[8] * tensor._data[5];

  return result;
}

/*
@LABEL:Tensor2::operator*(Vec3D)
@SHORT:Multiplication of a second order tensor by a vector.
@RETURN:Vec3D
  This method defines the product of a second order tensor by a vector.
  The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a second order tensor and $\overrightarrow{x}$ and $\overrightarrow{y}$ are two Vec3D.
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::operator*(const Vec3D &vector) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  result._data[0] = _data[0] * vector._data[0] + _data[1] * vector._data[1] + _data[2] * vector._data[2];
  result._data[1] = _data[3] * vector._data[0] + _data[4] * vector._data[1] + _data[5] * vector._data[2];
  result._data[2] = _data[6] * vector._data[0] + _data[7] * vector._data[1] + _data[8] * vector._data[2];

  return result;
}

/*
@LABEL:Tensor2::singleProduct(Tensor2)
@SHORT:Contracted product of two second order tensors.
@RETURN:Tensor2
This method defines a single contracted product of two second order tensors.
The result of this operation is also a second order tensor defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ and $\B$ are two second order tensors.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::singleProduct(const Tensor2 tensor) const
//-----------------------------------------------------------------------------
{
  //  return (*this) * tensor;
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[3] + _data[2] * tensor._data[6];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[4] + _data[2] * tensor._data[7];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[5] + _data[2] * tensor._data[8];
  result._data[3] = _data[3] * tensor._data[0] + _data[4] * tensor._data[3] + _data[5] * tensor._data[6];
  result._data[4] = _data[3] * tensor._data[1] + _data[4] * tensor._data[4] + _data[5] * tensor._data[7];
  result._data[5] = _data[3] * tensor._data[2] + _data[4] * tensor._data[5] + _data[5] * tensor._data[8];
  result._data[6] = _data[6] * tensor._data[0] + _data[7] * tensor._data[3] + _data[8] * tensor._data[6];
  result._data[7] = _data[6] * tensor._data[1] + _data[7] * tensor._data[4] + _data[8] * tensor._data[7];
  result._data[8] = _data[6] * tensor._data[2] + _data[7] * tensor._data[5] + _data[8] * tensor._data[8];

  return result;
}

/*
@LABEL:Tensor2::doubleProduct()
@SHORT:Double contracted product of a second order tensor by itself.
@RETURN:double
This method defines a double contracted product of a second order tensor by itself.
The result of this operation is a scalar defined by:
\begin{equation*}
s = \A : \A = \sum_{i=1}^{3} \sum_{j=1}^{3} A_{ij}\times A_{ij}
\end{equation*}
where $\A$ is a second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::doubleProduct() const
//-----------------------------------------------------------------------------
{
  return (_data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2] +
          _data[3] * _data[3] + _data[4] * _data[4] + _data[5] * _data[5] +
          _data[6] * _data[6] + _data[7] * _data[7] + _data[8] * _data[8]);
}

/*
@LABEL:Tensor2::doubleProduct(Tensor2)
@SHORT:Double contracted product of 2 second order tensors.
@RETURN:double
This method defines a double contracted product of two second order tensors.
The result of this operation is a scalar defined by:
\begin{equation*}
s = \A : \B = \sum_{i=1}^{3} \sum_{j=1}^{3} A_{ij}\times B_{ij}
\end{equation*}
where $\A$ and $\B$ are two second order tensors.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::doubleProduct(const Tensor2 tensor) const
//-----------------------------------------------------------------------------
{
  return (_data[0] * tensor._data[0] + _data[1] * tensor._data[1] + _data[2] * tensor._data[2] +
          _data[3] * tensor._data[3] + _data[4] * tensor._data[4] + _data[5] * tensor._data[5] +
          _data[6] * tensor._data[6] + _data[7] * tensor._data[7] + _data[8] * tensor._data[8]);
}

/*
@LABEL:Tensor2::getDeviator()
@SHORT:Deviatoric part of a second order tensor.
@RETURN:Tensor2
This method defines the deviatoric part of a second second order tensor.
The result of this operation is a second order tensor defined by the following equation:
\begin{equation*}
\Sig^d=\Sig-\frac{1}{3}\tr[\Sig].\Id
\end{equation*}
where $\Sig^d$ is the deviatoric part of the tensor, $\Sig$ is the tensor and $\Id$ is the unit tensor.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getDeviator() const
//-----------------------------------------------------------------------------
{
  Tensor2 result(*this);
  double pressure = getThirdTrace();

  result._data[0] -= pressure;
  result._data[4] -= pressure;
  result._data[8] -= pressure;

  return result;
}

/*
@LABEL:Tensor2::getTranspose()
@SHORT:Transpose of a second order tensor.
@RETURN:Tensor2
This method defines the transpose of a second second order tensor.
The result of this operation is a second order tensor defined by the following equation:
\begin{equation*}
\A=\B^T =\left[\begin{array}{ccc}
  B_{11} & B_{21} & B_{31}\\
  B_{12} & B_{22} & B_{32}\\
  B_{13} & B_{23} & B_{33}
  \end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getTranspose() const
//-----------------------------------------------------------------------------
{
  return Tensor2(_data[0], _data[3], _data[6], _data[1], _data[4], _data[7], _data[2], _data[5], _data[8]);
}

/*
@LABEL:Tensor2::rowSum()
@SHORT:Sum of the rows of a second order tensor.
@RETURN:Vec3D
This method returns a vector by computing the sum of the components on all rows of a second second order tensor.
The result of this operation is a vector defined by:
\begin{equation*}
v_{i}=\sum_{j=1}^{3} T_{ji}
\end{equation*}
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
@LABEL:Tensor2::columnSum()
@SHORT:Sum of the columns of a second order tensor.
@RETURN:Vec3D
This method returns a vector by computing the sum of the components on all columns of a second second order tensor.
The result of this operation is a vector defined by:
\begin{equation*}
v_{i}=\sum_{j=1}^{3}T_{ij}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::columnSum() const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + _data[3] + _data[6],
               _data[1] + _data[4] + _data[7],
               _data[2] + _data[5] + _data[8]);
}

/*
@LABEL:Tensor2::getSymetricPart()
@SHORT:Symmetric part of a second order tensor.
@RETURN:Tensor2
This method returns the symmetric part of a second second order tensor.
The result of this operation is a second second order tensor defined by:
\begin{equation*}
\B = \left[\begin{array}{ccc}
 A_{11} & \frac{A_{12} + A_{21}}{2} & \frac{A_{13} + A_{31}}{2}\\
 \frac{A_{12} + A_{21}}{2} & A_{22} & \frac {A_{23} + A_{32}}{2}\\
 \frac{A_{13} + A_{31}}{2} & \frac {A_{23} + A_{32}}{2} & A_{33}\end{array}
\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getSymetricPart() const
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
@LABEL:Tensor2::getSkewSymetricPart()
@SHORT:Skew-symmetric part of a second order tensor.
@RETURN:Tensor2
This method returns the skew-symmetric part of a second second order tensor.
The result of this operation is a second second order tensor defined by:
\begin{equation*}
\B = \left[\begin{array}{ccc}
 A_{11} & \frac{A_{12} - A_{21}}{2} & \frac{A_{13} - A_{31}}{2}\\
 -\frac{A_{12} -  A_{21}}{2} & A_{22} & \frac {A_{23} - A_{32}}{2}\\
 -\frac{A_{13} - A_{31}}{2} & -\frac {A_{23} - A_{32}}{2} & A_{33}\end{array}
\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getSkewSymetricPart() const
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
@LABEL:Tensor2::getRow(short)
@SHORT:Extraction of a row from a second order tensor.
@RETURN:Vec3D
This method returns a vector as part of a second second order tensor.
The result of this operation with the argument j is a vector defined by:
\begin{equation*}
v_{i} = T_{ij}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::getRow(short row) const
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
@LABEL:Tensor2::getColumn(short)
@SHORT:Extraction of a column from a second order tensor.
@RETURN:Vec3D
This method returns a vector as part of a second second order tensor.
The result of this operation with the argument j is a vector defined by:
\begin{equation*}
v_{i} = T_{ji}
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::getColumn(short col) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  for (short i = 0; i < 3; i++)
  {
    result(i) = _data[_internalIndexes.index[i][col]];
  }

  return result;
}

/*
  Test the equality of two second order tensors

  This method tests the equality of two second order tensors.
  It returns \ref true if all components of the two second order tensors are equals, \ref false on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - tensor Second second order tensor to use for the operation
*/
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

/*
  Test the inequality of two second order tensors

  This method tests the inequality of two second order tensors.
  It returns \ref false if all components of the two second order tensors are equals, \ref true on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool Tensor2::operator!=(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  return !(*this == tensor);
}

/*
  Writes a second order tensor in a binary flux for storage

  This method is used to store the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor2 _data;
  _data.write(pfile);
  \endcode
  - ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Tensor2::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)_data, 9 * sizeof(double));
}

/*
  Reads a second order tensor in a binary flux from storage

  This method is used to read the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor2 _data;
  _data.read(pfile);
  \endcode
  - ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void Tensor2::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)_data, 9 * sizeof(double));
}

/*
  Writes a second order tensor in a binary flux for storage

  This method is used to store the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor2 _data;
  pfile << _data;
  \endcode
  - os Output file stream to use for writting operation
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.write(os);
  return os;
}

/*
  Reads a second order tensor from a binary flux for storage

  This method is used to read the components of a second order tensor in a binary file.
  \code
  std::ifstream pfile("fichier");
  Tensor2 _data;
  pfile >> _data;
  \endcode
  - os Input file stream to use for reading operation
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.read(is);
  return is;
}

/*
@LABEL:Tensor2::maxValue()
@SHORT:Maximum component in a second order tensor.
@RETURN:double
This method returns the maximum component in a second second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::maxValue()
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
@LABEL:Tensor2::minValue()
@SHORT:Minimum component in a second order tensor.
@RETURN:double
This method returns the minimum component in a second second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::minValue()
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
@LABEL:Tensor2::maxAbsoluteValue()
@SHORT:Maximum absolute component in a second order tensor.
@RETURN:double
This method returns the maximum absolute component in a second second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::maxAbsoluteValue()
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
@LABEL:Tensor2::minAbsoluteValue()
@SHORT:Minimum absolute component in a second order tensor.
@RETURN:double
This method returns the minimum absolute component in a second second order tensor.
@END
*/
//-----------------------------------------------------------------------------
double Tensor2::minAbsoluteValue()
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
@LABEL:Tensor2::getInverse()
@SHORT:Inverse of a second order tensor.
@RETURN:Tensor2
This method returns the inverse of a second second order tensor.
The result of this operation is a second order tensor defined by:
\begin{equation*}
D = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33}
\end{equation*}
\begin{equation*}
T^{-1} = \frac {1}{D} \left[\begin{array}{ccc}
  T_{22}T_{33}-T_{23}T_{32}&T_{13}T_{32}-T_{12}T_{33}&T_{12}T_{23}-T_{13}T_{22}\\
  T_{23}T_{31}-T_{21}T_{33}&T_{11}T_{33}-T_{13}T_{31}&T_{13}T_{21}-T_{11}T_{23}\\
  T_{21}T_{32}-T_{22}T_{31}&T_{12}T_{31}-T_{11}T_{32}&T_{11}T_{22}-T_{12}T_{21}
  \end{array}
  \right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getInverse() const
//-----------------------------------------------------------------------------
{
  double t1 = _data[4] * _data[8];
  double t2 = _data[2] * _data[7];
  double t3 = _data[1] * _data[5];
  double t4 = _data[5] * _data[7];
  double t5 = _data[1] * _data[8];
  double t6 = _data[2] * _data[4];

  double unSurDeter = 1 / (_data[0] * t1 + _data[3] * t2 + _data[6] * t3 - _data[0] * t4 - _data[3] * t5 - _data[6] * t6);
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
  Returns the minor of a tensor
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getMinor() const
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
  Returns the cofactor of a tensor
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor2::getCofactor() const
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
  Solves a small system T.v = b
*/
//-----------------------------------------------------------------------------
Vec3D Tensor2::solve(const Vec3D &b) const
//-----------------------------------------------------------------------------
{
  return Vec3D(getInverse() * b);
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
  Polar decomposition of a second order tensor with computation of the \f$ ln[U] \f$ and \f$ R \f$ tensors.

  This method computes the polar decomposition of a second order tensor with computation of the \f$ ln[U] \f$ and \f$ R \f$ tensors as the returning arguments.
  The logarithm of a symmetric tensor is givent by the following formulation:
  \f[ \ln U =\sum _{i=1}^{3}\ln \lambda _{i}(u_{i}\otimes u_{i}) \f]
  - U Return second order tensor containing \f$ ln[U] \f$
  - R Return second order tensor containing \f$ R \f$
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
  Polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors.

  This method computes the polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors as the returning arguments.
  - U Return second order tensor containing \f$ U \f$
  - R Return second order tensor containing \f$ R \f$
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

/*
  Saves the content of a Tensor2 into a NumPy file

  This method saves the content of a Tensor2 object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor2 T;
  T.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor2::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {3, 3}, mode);
}

/*
  Saves the content of a Tensor2 into a NumPyZ file

  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor2 T;
  T.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor2::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {3, 3}, mode);
}

/*
  Read the content of a Tensor2 from a NumPy file

  This method reads the content of a vec3D object from a NumPy file defined by its filename.
  \code
  Tensor2 T;
  T.numpyRead("numpy.npy");
  \endcode
*/
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

/*
  Read the content of a Tensor2 from a NumPyZ file

  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.
  \code
  Tensor2 T;
  T.numpyReadZ("numpy.npz");
  \endcode
*/
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
