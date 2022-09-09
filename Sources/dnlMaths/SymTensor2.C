/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file result.C
  Definition file for the second order tensor class

  This file is the declaration file for the second order tensor class. A second order tensor is a like a matrix with the following form:
  \f[ T=\left[\begin{array}{ccc}
  T_{11} & T_{12} & T_{13}\\
  T_{12} & T_{22} & T_{23}\\
  T_{13} & T_{23} & T_{33}
  \end{array}\right] \f]
  \ingroup dnlMaths
*/

#include <fstream>
#include <SymTensor2.h>
#include <NumpyInterface.h>
#include <Tensor2.h>
#include <Vec3D.h>

const Tensor2Index SymTensor2::_internalIndexes = {0, 1, 2, 1, 3, 4, 2, 4, 5};

/*
  Constructor of the SymTensor2 class

  This method is the default constructor of a second order tensor. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
SymTensor2::SymTensor2()
//-----------------------------------------------------------------------------
{
  setToValue(0.0);
}

/*
  Copy constructor of the SymTensor2 class
*/
//-----------------------------------------------------------------------------
SymTensor2::SymTensor2(const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  memcpy(_data, tensor._data, 6 * sizeof(double));
}

/*
  Constructor of the SymTensor2 class

  This method is a constructor of a second order tensor with initialization of the 6 values.
*/
//-----------------------------------------------------------------------------
SymTensor2::SymTensor2(double t1, double t2, double t3, double t4, double t5, double t6)
//-----------------------------------------------------------------------------
{
  _data[0] = t1;
  _data[1] = t2;
  _data[2] = t3;
  _data[3] = t4;
  _data[4] = t5;
  _data[5] = t6;
}

/*
  Destructor of the SymTensor2 class
*/
//-----------------------------------------------------------------------------
SymTensor2::~SymTensor2()
//-----------------------------------------------------------------------------
{
}

/*
  Send the content of a second order tensor to the output flux for display

  \code
  SymTensor2 t;
  std::cout << t << endl;
  \endcode
  - outputStream Output flux
  - tensor tensor
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &outputStream, const SymTensor2 &tensor)
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
void SymTensor2::print(std::ostream &outputStream) const
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
  Returns a unity tensor

  This method transforms the current tensor to a unity tensor.
  \code
  SymTensor2 tensor;
  tensor.setToUnity(); // Returns and identity tensor
  \endcode
  \warning This method modify it's argument
*/
//-----------------------------------------------------------------------------
void SymTensor2::setToUnity()
//-----------------------------------------------------------------------------
{
  _data[0] = 1.0;
  _data[1] = 0.0;
  _data[2] = 0.0;
  _data[3] = 1.0;
  _data[4] = 0.0;
  _data[5] = 1.0;
}

/*
  Sets all components of the tensor to zero
*/
//-----------------------------------------------------------------------------
void SymTensor2::setToZero()
//-----------------------------------------------------------------------------
{
  setToValue(0.0);
}

/*
  Fill a second order tensor with a scalar value

  This method is a surdefinition of the = operator for the second order tensor class.
  \code
  SymTensor2 tensor;
  tensor = 1.0; // All components of the tensor are set to 1.0
  \endcode
  - val double value to give to all components of the second order tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 &SymTensor2::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

/*
  Copy the content of a second order tensor into a new one

  This method is the so called = operator between two second order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.
  \code
  SymTensor2 tensor, SymTensor2;
  tensor = SymTensor2; // copy of the tensor
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
SymTensor2 &SymTensor2::operator=(const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  memcpy(_data, tensor._data, 6 * sizeof(double));
  return *this;
}

/*
  Addition of 2 second order tensors

  This method defines the addition of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A + B \f]
  \code
  SymTensor2 tensor,SymTensor2,t3;
  t3 = tensor + SymTensor2; // sum of 2 second order tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::operator+(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  SymTensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] + tensor._data[0];
  result._data[1] = _data[1] + tensor._data[1];
  result._data[2] = _data[2] + tensor._data[2];
  result._data[3] = _data[3] + tensor._data[3];
  result._data[4] = _data[4] + tensor._data[4];
  result._data[5] = _data[5] + tensor._data[5];

  // renvoi de l'objet
  return result;
}

/*
  Difference of 2 second order tensors

  This method defines the difference of 2 second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A - B \f]
  \code
  SymTensor2 tensor,SymTensor2,t3;
  t3 = tensor - SymTensor2; // difference of 2 second order tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::operator-(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  SymTensor2 result;

  // calcul de la somme
  result._data[0] = _data[0] - tensor._data[0];
  result._data[1] = _data[1] - tensor._data[1];
  result._data[2] = _data[2] - tensor._data[2];
  result._data[3] = _data[3] - tensor._data[3];
  result._data[4] = _data[4] - tensor._data[4];
  result._data[5] = _data[5] - tensor._data[5];

  // renvoi de l'objet
  return result;
}

/*
  Return the opposite tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::operator-() const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = -_data[0];
  result._data[1] = -_data[1];
  result._data[2] = -_data[2];
  result._data[3] = -_data[3];
  result._data[4] = -_data[4];
  result._data[5] = -_data[5];

  return result;
}

/*
  Multiplication of a second order tensor by a scalar value

  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . \lambda \f]
  \code
  SymTensor2 tensor,SymTensor2;
  double l;
  SymTensor2 = tensor * l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::operator*(const double lambda) const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = lambda * _data[0];
  result._data[1] = lambda * _data[1];
  result._data[2] = lambda * _data[2];
  result._data[3] = lambda * _data[3];
  result._data[4] = lambda * _data[4];
  result._data[5] = lambda * _data[5];

  return result;
}

/*
  Division of a second order tensor by a scalar value

  This method defines the division of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  \code
  SymTensor2 tensor,SymTensor2;
  double l;
  SymTensor2 = tensor / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::operator/(const double lambda) const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

#ifdef VERIF_maths
  if (lambda == 0.0)
  {
    fatalError("SymTensor2:: operator /", "divide by zero");
  }
#endif

  result._data[0] = _data[0] / lambda;
  result._data[1] = _data[1] / lambda;
  result._data[2] = _data[2] / lambda;
  result._data[3] = _data[3] / lambda;
  result._data[4] = _data[4] / lambda;
  result._data[5] = _data[5] / lambda;

  return result;
}

/*
  Multiplication of a second order tensor by a scalar value

  This method defines the multiplication of a second order tensor by a scalar value
  The result of this operation is also a second order tensor defined by:
  \f[ T = \lambda . A \f]
  \code
  SymTensor2 tensor,SymTensor2;
  double l;
  SymTensor2 = l * tensor; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
SymTensor2 operator*(const double &lambda, const SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = lambda * tensor._data[0];
  result._data[1] = lambda * tensor._data[1];
  result._data[2] = lambda * tensor._data[2];
  result._data[3] = lambda * tensor._data[3];
  result._data[4] = lambda * tensor._data[4];
  result._data[5] = lambda * tensor._data[5];

  return result;
}

/*
  Multiplication of 2 second order tensors

  This method defines a single contracted product of two second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  \code
  SymTensor2 tensor, SymTensor2, t3;
  t3 = tensor * SymTensor2; // product of two tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::singleProduct() const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = _data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2];
  result._data[1] = _data[0] * _data[1] + _data[1] * _data[3] + _data[2] * _data[4];
  result._data[2] = _data[0] * _data[2] + _data[1] * _data[4] + _data[2] * _data[5];
  result._data[3] = _data[1] * _data[1] + _data[3] * _data[3] + _data[4] * _data[4];
  result._data[4] = _data[1] * _data[2] + _data[3] * _data[4] + _data[4] * _data[5];
  result._data[5] = _data[2] * _data[2] + _data[4] * _data[4] + _data[5] * _data[5];

  return result;
}

/*
  Multiplication of 2 second order tensors

  This method defines a single contracted product of two second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  \code
  SymTensor2 tensor, SymTensor2, t3;
  t3 = tensor * SymTensor2; // product of two tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 SymTensor2::operator*(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[1] + _data[2] * tensor._data[2];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[3] + _data[2] * tensor._data[4];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[4] + _data[2] * tensor._data[5];
  result._data[3] = _data[1] * tensor._data[0] + _data[3] * tensor._data[1] + _data[4] * tensor._data[2];
  result._data[4] = _data[1] * tensor._data[1] + _data[3] * tensor._data[3] + _data[4] * tensor._data[4];
  result._data[5] = _data[1] * tensor._data[2] + _data[3] * tensor._data[4] + _data[4] * tensor._data[5];
  result._data[6] = _data[2] * tensor._data[0] + _data[4] * tensor._data[1] + _data[5] * tensor._data[2];
  result._data[7] = _data[2] * tensor._data[1] + _data[4] * tensor._data[3] + _data[5] * tensor._data[4];
  result._data[8] = _data[2] * tensor._data[2] + _data[4] * tensor._data[4] + _data[5] * tensor._data[5];

  return result;
}

/*
  Multiplication of 2 second order tensors

  This method defines a single contracted product of two second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  \code
  SymTensor2 tensor, SymTensor2, t3;
  t3 = tensor * SymTensor2; // product of two tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 SymTensor2::operator*(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[3] + _data[2] * tensor._data[6];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[4] + _data[2] * tensor._data[7];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[5] + _data[2] * tensor._data[8];
  result._data[3] = _data[1] * tensor._data[0] + _data[3] * tensor._data[3] + _data[4] * tensor._data[6];
  result._data[4] = _data[1] * tensor._data[1] + _data[3] * tensor._data[4] + _data[4] * tensor._data[7];
  result._data[5] = _data[1] * tensor._data[2] + _data[3] * tensor._data[5] + _data[4] * tensor._data[8];
  result._data[6] = _data[2] * tensor._data[0] + _data[5] * tensor._data[3] + _data[5] * tensor._data[6];
  result._data[7] = _data[2] * tensor._data[1] + _data[4] * tensor._data[4] + _data[5] * tensor._data[7];
  result._data[8] = _data[2] * tensor._data[2] + _data[4] * tensor._data[5] + _data[5] * tensor._data[8];

  return result;
}

/*
  Multiplication of 2 second order tensors
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::productByRxRT(const Tensor2 R) const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = R._data[0] * R._data[0] * _data[0] + 2 * R._data[0] * (R._data[1] * _data[1] + R._data[2] * _data[2]) + R._data[1] * R._data[1] * _data[3] + 2 * R._data[1] * R._data[2] * _data[4] + R._data[2] * R._data[2] * _data[5];
  result._data[1] = R._data[0] * (R._data[3] * _data[0] + R._data[4] * _data[1] + R._data[5] * _data[2]) + R._data[1] * (R._data[3] * _data[1] + R._data[4] * _data[3] + R._data[5] * _data[4]) + R._data[2] * (R._data[3] * _data[2] + R._data[4] * _data[4] + R._data[5] * _data[5]);
  result._data[2] = R._data[0] * (R._data[6] * _data[0] + R._data[7] * _data[1] + R._data[8] * _data[2]) + R._data[1] * (R._data[6] * _data[1] + R._data[7] * _data[3] + R._data[8] * _data[4]) + R._data[2] * (R._data[6] * _data[2] + R._data[7] * _data[4] + R._data[8] * _data[5]);
  result._data[3] = R._data[3] * R._data[3] * _data[0] + 2 * R._data[3] * (R._data[4] * _data[1] + R._data[5] * _data[2]) + R._data[4] * R._data[4] * _data[3] + 2 * R._data[4] * R._data[5] * _data[4] + R._data[5] * R._data[5] * _data[5];
  result._data[4] = R._data[3] * (R._data[6] * _data[0] + R._data[7] * _data[1] + R._data[8] * _data[2]) + R._data[4] * (R._data[6] * _data[1] + R._data[7] * _data[3] + R._data[8] * _data[4]) + R._data[5] * (R._data[6] * _data[2] + R._data[7] * _data[4] + R._data[8] * _data[5]);
  result._data[5] = R._data[6] * R._data[6] * _data[0] + 2 * R._data[6] * (R._data[7] * _data[1] + R._data[8] * _data[2]) + R._data[7] * R._data[7] * _data[3] + 2 * R._data[7] * R._data[8] * _data[4] + R._data[8] * R._data[8] * _data[5];

  return result;
}

/*
  Multiplication of 2 second order tensors
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::productByRTxR(const Tensor2 R) const
//-----------------------------------------------------------------------------
{
  SymTensor2 result;

  result._data[0] = R._data[0] * R._data[0] * _data[0] + 2 * R._data[0] * (R._data[3] * _data[1] + R._data[6] * _data[2]) + R._data[3] * R._data[3] * _data[3] + 2 * R._data[3] * R._data[6] * _data[4] + R._data[6] * R._data[6] * _data[5];
  result._data[1] = R._data[1] * R._data[3] * _data[1] + R._data[1] * R._data[6] * _data[2] + R._data[0] * (R._data[1] * _data[0] + R._data[4] * _data[1] + R._data[7] * _data[2]) + R._data[3] * R._data[4] * _data[3] + R._data[4] * R._data[6] * _data[4] + R._data[3] * R._data[7] * _data[4] + R._data[6] * R._data[7] * _data[5];
  result._data[2] = R._data[2] * R._data[3] * _data[1] + R._data[2] * R._data[6] * _data[2] + R._data[0] * (R._data[2] * _data[0] + R._data[5] * _data[1] + R._data[8] * _data[2]) + R._data[3] * R._data[5] * _data[3] + R._data[5] * R._data[6] * _data[4] + R._data[3] * R._data[8] * _data[4] + R._data[6] * R._data[8] * _data[5];
  result._data[3] = R._data[1] * R._data[1] * _data[0] + 2 * R._data[1] * (R._data[4] * _data[1] + R._data[7] * _data[2]) + R._data[4] * R._data[4] * _data[3] + 2 * R._data[4] * R._data[7] * _data[4] + R._data[7] * R._data[7] * _data[5];
  result._data[4] = R._data[2] * R._data[4] * _data[1] + R._data[2] * R._data[7] * _data[2] + R._data[1] * (R._data[2] * _data[0] + R._data[5] * _data[1] + R._data[8] * _data[2]) + R._data[4] * R._data[5] * _data[3] + R._data[5] * R._data[7] * _data[4] + R._data[4] * R._data[8] * _data[4] + R._data[7] * R._data[8] * _data[5];
  result._data[5] = R._data[2] * R._data[2] * _data[0] + 2 * R._data[2] * (R._data[5] * _data[1] + R._data[8] * _data[2]) + R._data[5] * R._data[5] * _data[3] + 2 * R._data[5] * R._data[8] * _data[4] + R._data[8] * R._data[8] * _data[5];

  return result;
}

/*
  Multiplication of a second order tensor by a vector

  This method defines the product of a second order tensor by a vector.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{b} = T . \overrightarrow{a} \f]
  \code
  SymTensor2 tensor;
  Vec3D v1,v2;
  v2 = tensor * v1; // product of the second order tensor by a vector
  \endcode
  - vector Vector to use for the operation
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::operator*(const Vec3D &vector) const
//-----------------------------------------------------------------------------
{
  Vec3D result;

  result._data[0] = _data[0] * vector._data[0] + _data[1] * vector._data[1] + _data[2] * vector._data[2];
  result._data[1] = _data[1] * vector._data[0] + _data[3] * vector._data[1] + _data[4] * vector._data[2];
  result._data[2] = _data[2] * vector._data[0] + _data[4] * vector._data[1] + _data[5] * vector._data[2];

  return result;
}

/*
  Multiplication of 2 second order tensors

  This method defines a single contracted product of two second order tensors.
  The result of this operation is also a second order tensor defined by:
  \f[ T = A . B \f]
  \code
  SymTensor2 tensor,SymTensor2,t3;
  t3 = tensor.singleProduct(SymTensor2); // product of two tensors
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 SymTensor2::singleProduct(const SymTensor2 tensor) const
//-----------------------------------------------------------------------------
{
  Tensor2 result;

  result._data[0] = _data[0] * tensor._data[0] + _data[1] * tensor._data[1] + _data[2] * tensor._data[2];
  result._data[1] = _data[0] * tensor._data[1] + _data[1] * tensor._data[3] + _data[2] * tensor._data[4];
  result._data[2] = _data[0] * tensor._data[2] + _data[1] * tensor._data[4] + _data[2] * tensor._data[5];
  result._data[3] = _data[1] * tensor._data[0] + _data[3] * tensor._data[1] + _data[4] * tensor._data[2];
  result._data[4] = _data[1] * tensor._data[1] + _data[3] * tensor._data[3] + _data[4] * tensor._data[4];
  result._data[5] = _data[1] * tensor._data[2] + _data[3] * tensor._data[4] + _data[4] * tensor._data[5];
  result._data[6] = _data[2] * tensor._data[0] + _data[4] * tensor._data[1] + _data[5] * tensor._data[2];
  result._data[7] = _data[2] * tensor._data[1] + _data[4] * tensor._data[3] + _data[5] * tensor._data[4];
  result._data[8] = _data[2] * tensor._data[2] + _data[4] * tensor._data[4] + _data[5] * tensor._data[5];

  return result;
}

/*
  Double contracted product of 2 second order tensors

  This method defines a double contracted product of two second order tensors.
  The result of this operation is a scalar defined by:
  \f[ s = A : B \f]
  \code
  SymTensor2 tensor1, tensor2;
  double s;
  s = tensor1.doubleProduct(tensor2); // double contracted product
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
double SymTensor2::doubleProduct(const SymTensor2 tensor) const
//-----------------------------------------------------------------------------
{
  return (_data[0] * tensor._data[0] + _data[3] * tensor._data[3] + _data[5] * tensor._data[5] +
          2.0 * (_data[1] * tensor._data[1] + _data[2] * tensor._data[2] + _data[4] * tensor._data[4]));
}

//Double contracted product of 2 second order tensors
/*
  This method defines a double contracted product of two second order tensors.
  The result of this operation is a scalar defined by:
  \f[ s = A : B \f]
  \code
  SymTensor2 tensor;
  double s;
  s = tensor.doubleProduct(); // double contracted product
  \endcode
*/
//-----------------------------------------------------------------------------
double SymTensor2::doubleProduct() const
//-----------------------------------------------------------------------------
{
  return (_data[0] * _data[0] + _data[3] * _data[3] + _data[5] * _data[5] +
          2.0 * (_data[1] * _data[1] + _data[2] * _data[2] + _data[4] * _data[4]));
}

/*
  Deviator of a second order tensor

  This method defines the getDeviator of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ s=\sigma-\frac{1}{3}\tr[\sigma].I \f]
  Return : Deviator of a second order tensor
*/
//-----------------------------------------------------------------------------
void SymTensor2::computeDeviator(SymTensor2 &deviator, double &pressure) const
//-----------------------------------------------------------------------------
{
  pressure = getThirdTrace();

  deviator._data[0] = _data[0] - pressure;
  deviator._data[3] = _data[3] - pressure;
  deviator._data[5] = _data[5] - pressure;
  deviator._data[1] = _data[1];
  deviator._data[2] = _data[2];
  deviator._data[4] = _data[4];
}

/*
  Deviator of a second order tensor
/*
  This method defines the getDeviator of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ s=\sigma-\frac{1}{3}\tr[\sigma].I \f]
  Return : Deviator of a second order tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getDeviator() const
//-----------------------------------------------------------------------------
{
  SymTensor2 result(*this);
  double pressure = getThirdTrace();

  result._data[0] -= pressure;
  result._data[3] -= pressure;
  result._data[5] -= pressure;

  return result;
}

/*
  getTranspose of a second order tensor

  This method defines the transpose of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ T_2 = T_1^T \f]
  Return : getTranspose of a second order tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getTranspose() const
//-----------------------------------------------------------------------------
{
  return *this;
}

/*
  Sum of the rows of a second order tensor

  This method returns a vector by computing the sum of the components on all rows of a second second order tensor.
  The result of this operation is a vector defined by:
  \f[ v_{i}=\sum_{j=1}^{3}T_{ji} \f]
  Return : Vector containing the sum of the rows of a second order tensor
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::rowSum() const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + _data[1] + _data[2], _data[1] + _data[3] + _data[4], _data[2] + _data[4] + _data[5]);
}

/*
  Sum of the columns of a second order tensor

  This method returns a vector by computing the sum of the components on all columns of a second second order tensor.
  The result of this operation is a vector defined by:
  \f[ v_{i}=\sum_{j=1}^{3}T_{ij} \f]
  Return : Vector containing the sum of the rows of a second order tensor
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::columnSum() const
//-----------------------------------------------------------------------------
{
  return Vec3D(_data[0] + _data[1] + _data[2], _data[1] + _data[3] + _data[4], _data[2] + _data[4] + _data[5]);
}

/*
  Symmetric part of a second order tensor

  This method returns the symmetric part of a second second order tensor.
  The result of this operation is a second second order tensor defined by:
  \f[
  s = \left[\begin{array}{ccc}
 v_{11} & \frac{v{12} + v{21}}{2} & \frac{v{13} + v{31}}{2}\\
 \frac{v{12} + v{21}}{2} & v_{22} & \frac {v{23} + v{32}}{2}\\
 \frac{v{13} + v{31}}{2} & \frac {v{23} + v{32}}{2} & v_{33}\end{array}
\right]
  \f]
  - s_v Returned symmetric part of a second order tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getSymetricPart() const
//-----------------------------------------------------------------------------
{
  return *this;
}

/*
  Skew-symmetric part of a second order tensor

  This method returns the skew-symmetric part of a second second order tensor.
  The result of this operation is a second second order tensor defined by:
  \f[
  s = \left[\begin{array}{ccc}
 v_{11} & \frac{v{12} - v{21}}{2} & \frac{v{13} - v{31}}{2}\\
 -\frac{v{12} -  v{21}}{2} & v_{22} & \frac {v{23} - v{32}}{2}\\
 -\frac{v{13} - v{31}}{2} & -\frac {v{23} - v{32}}{2} & v_{33}\end{array}
\right]
  \f]
  - a_v Returned skew-symmetric part of a second order tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getSkewSymetricPart() const
//-----------------------------------------------------------------------------
{
  return SymTensor2();
}

/*
  Extraction of a row from a second order tensor

  This method returns a vector as part of a second second order tensor.
  The result of this operation with the argument j is a vector defined by:
  \f[ v_{i} = T_{ij} \f]
  - row row line number to extract.
  Return : vecteur containing the corresponding row
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::getRow(short row) const
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
  Extraction of a column from a second order tensor

  This method returns a vector as part of a second second order tensor.
  The result of this operation with the argument j is a vector defined by:
  \f[ v_{i} = T_{ji} \f]
  - col col line number to extract.
  Return : vecteur containing the corresponding column
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::getColumn(short col) const
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
bool SymTensor2::operator==(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  short i;

  for (i = 0; i < 6; i++)
    if (_data[i] != tensor._data[i])
    {
      return false;
    }
  return true;
}

/*
  Test the equality of two second order tensors

  This method tests the equality of two second order tensors.
  It returns \ref false if all components of the two second order tensors are equals, \ref true on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool SymTensor2::operator!=(const SymTensor2 &tensor) const
//-----------------------------------------------------------------------------
{
  return !(*this == tensor);
}

/*
  Writes a second order tensor in a binary flux for storage

  This method is used to store the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  SymTensor2 t;
  t.write(pfile);
  \endcode
  - ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void SymTensor2::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)_data, 6 * sizeof(double));
}

/*
  Reads a second order tensor in a binary flux from storage

  This method is used to read the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  SymTensor2 t;
  t.read(pfile);
  \endcode
  - ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void SymTensor2::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)_data, 6 * sizeof(double));
}

/*
  Writes a second order tensor in a binary flux for storage

  This method is used to store the components of a second order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  SymTensor2 t;
  pfile << t;
  \endcode
  - os Output file stream to use for writting operation
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const SymTensor2 &tensor)
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
  SymTensor2 t;
  pfile >> t;
  \endcode
  - os Input file stream to use for reading operation
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, SymTensor2 &tensor)
//-----------------------------------------------------------------------------
{
  tensor.read(is);
  return is;
}

/*
  Maximum component in a vector

  This method returns the maximum component of a vector
*/
//-----------------------------------------------------------------------------
double SymTensor2::maxValue()
//-----------------------------------------------------------------------------
{
  double max = _data[0];
  for (short i = 1; i < 6; i++)
  {
    if (_data[i] > max)
      max = _data[i];
  }
  return max;
}

/*
  Minimum component in a vector

  This method returns the minimum component of a vector
*/
//-----------------------------------------------------------------------------
double SymTensor2::minValue()
//-----------------------------------------------------------------------------
{
  double min = _data[0];
  for (short i = 1; i < 6; i++)
  {
    if (_data[i] < min)
      min = _data[i];
  }
  return min;
}

/*
  Maximum absolute component in a vector

  This method returns the maximum absolute component of a vector
*/
//-----------------------------------------------------------------------------
double SymTensor2::maxAbsoluteValue()
//-----------------------------------------------------------------------------
{
  double max = dnlAbs(_data[0]);
  for (short i = 1; i < 6; i++)
  {
    if (dnlAbs(_data[i]) > max)
      max = dnlAbs(_data[i]);
  }
  return max;
}

/*
  Minimum absolute component in a vector

  This method returns the minimum absolute component of a vector
*/
//-----------------------------------------------------------------------------
double SymTensor2::minAbsoluteValue()
//-----------------------------------------------------------------------------
{
  double min = dnlAbs(_data[0]);
  for (short i = 1; i < 6; i++)
  {
    if (dnlAbs(_data[i]) < min)
      min = dnlAbs(_data[i]);
  }
  return min;
}

/*
  Inverse of a second order tensor

  This method returns the getInverse of a second second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ D = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33} \f]
  and
  \f[ T^{-1} = \frac {1}{D} \left[\begin{array}{ccc}
  -(T{23} * T{32}) + T{22} * T{33} & T{13} * T{32} - T{12} * T{33} & -(T{13} * T{22}) + T{12} * T{23}\\
  T{23} * T{31} - T{21} * T{33}& -(T{13} * T{31}) + T{11} * T{33} & T{13} * T{21} - T{11} * T{23}\\
  -(T{22} * T{31}) + T{21} * T{32}&T{12} * T{31} - T{11} * T{32} & -(T{12} * T{21}) + T{11} * T{22}\end{array}
  \right]
  \f]
  Return : Tensor containing the getInverse
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getInverse() const
//-----------------------------------------------------------------------------
{

  double t1 = _data[3] * _data[5];
  double t2 = _data[2] * _data[4];
  double t4 = _data[4] * _data[4];
  double t5 = _data[1] * _data[5];
  double t6 = _data[2] * _data[3];

  double unSurDeter = 1.0 / (_data[0] * t1 + 2.0 * _data[1] * t2 - _data[0] * t4 - _data[1] * t5 - _data[2] * t6);
  return SymTensor2((t1 - t4) * unSurDeter,
                    (t2 - t5) * unSurDeter,
                    (_data[1] * _data[4] - t6) * unSurDeter,
                    (_data[0] * _data[5] - _data[2] * _data[2]) * unSurDeter,
                    (_data[2] * _data[1] - _data[0] * _data[4]) * unSurDeter,
                    (_data[0] * _data[3] - _data[1] * _data[1]) * unSurDeter);
}

/*
  Returns the minor of a tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getMinor() const
//-----------------------------------------------------------------------------
{
  return SymTensor2(_data[3] * _data[5] - _data[4] * _data[4],
                    _data[5] * _data[1] - _data[4] * _data[2],
                    _data[1] * _data[4] - _data[2] * _data[3],
                    _data[5] * _data[0] - _data[2] * _data[2],
                    _data[0] * _data[4] - _data[2] * _data[1],
                    _data[0] * _data[3] - _data[1] * _data[1]);
}

/*
  Returns the cofactor of a tensor
*/
//-----------------------------------------------------------------------------
SymTensor2 SymTensor2::getCofactor() const
//-----------------------------------------------------------------------------
{
  return SymTensor2(_data[3] * _data[5] - _data[4] * _data[4],
                    _data[4] * _data[2] - _data[5] * _data[1],
                    _data[1] * _data[4] - _data[2] * _data[3],
                    _data[5] * _data[0] - _data[2] * _data[2],
                    _data[2] * _data[1] - _data[0] * _data[4],
                    _data[0] * _data[3] - _data[1] * _data[1]);
}

/*
  Solves a small system T.v = b
*/
//-----------------------------------------------------------------------------
Vec3D SymTensor2::solve(const Vec3D &b) const
//-----------------------------------------------------------------------------
{
  return Vec3D(getInverse() * b);
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
void SymTensor2::polarDecomposeLnU(SymTensor2 &LnU, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  short p, q;
  double alpha, t, Ctheta, Stheta;
  double _tmp1, _tmp2, _tmp3, _tmp4, _tmp5, _tmp6, _tmp7, _tmp8;
  double Rot11, Rot12, Rot13, Rot21, Rot22, Rot23, Rot31, Rot32, Rot33;
  SymTensor2 FF;
  short it = 1;

  // calcul de FF=F(T).F
  FF._data[0] = dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]);
  FF._data[1] = _data[0] * _data[1] + _data[1] * _data[3] + _data[2] * _data[4];
  FF._data[2] = _data[0] * _data[2] + _data[1] * _data[4] + _data[2] * _data[5];
  FF._data[3] = dnlSquare(_data[1]) + dnlSquare(_data[3]) + dnlSquare(_data[4]);
  FF._data[4] = _data[1] * _data[2] + _data[3] * _data[4] + _data[4] * _data[5];
  FF._data[5] = dnlSquare(_data[2]) + dnlSquare(_data[4]) + dnlSquare(_data[5]);

  // initialisation de Rot
  Rot11 = 1.0;
  Rot22 = 1.0;
  Rot33 = 1.0;
  Rot12 = 0.0;
  Rot13 = 0.0;
  Rot21 = 0.0;
  Rot23 = 0.0;
  Rot31 = 0.0;
  Rot32 = 0.0;

  while (it < 30)
  {
    // recherche du terme maxi
    q = (dnlAbs(FF._data[1]) > dnlAbs(FF._data[2]) ? 1 : 2);
    p = (dnlAbs(FF._data[q]) > dnlAbs(FF._data[4]) ? 0 : 1);
    if (p == 1)
      q = 2;

    // test de convergence
    if (dnlAbs(FF._data[_internalIndexes.index[p][q]]) < precisionPolarDecompose)
    {
      _tmp1 = 0.5 * log(FF._data[0]);
      _tmp2 = 0.5 * log(FF._data[3]);
      _tmp3 = 0.5 * log(FF._data[5]);
      LnU._data[0] = dnlSquare(Rot11) * _tmp1 + dnlSquare(Rot12) * _tmp2 + dnlSquare(Rot13) * _tmp3;
      LnU._data[3] = dnlSquare(Rot21) * _tmp1 + dnlSquare(Rot22) * _tmp2 + dnlSquare(Rot23) * _tmp3;
      LnU._data[5] = dnlSquare(Rot31) * _tmp1 + dnlSquare(Rot32) * _tmp2 + dnlSquare(Rot33) * _tmp3;
      LnU._data[1] = (Rot11 * Rot21 * _tmp1 + Rot12 * Rot22 * _tmp2 + Rot13 * Rot23 * _tmp3);
      LnU._data[2] = (Rot11 * Rot31 * _tmp1 + Rot12 * Rot32 * _tmp2 + Rot13 * Rot33 * _tmp3);
      LnU._data[4] = (Rot21 * Rot31 * _tmp1 + Rot22 * Rot32 * _tmp2 + Rot23 * Rot33 * _tmp3);

      _tmp1 = 1.0 / sqrt(FF._data[0]);
      _tmp2 = 1.0 / sqrt(FF._data[3]);
      _tmp3 = 1.0 / sqrt(FF._data[5]);
      _tmp4 = (Rot13 * _data[0] + Rot23 * _data[1] + Rot33 * _data[2]) * _tmp3;
      _tmp5 = (Rot12 * _data[0] + Rot22 * _data[1] + Rot32 * _data[2]) * _tmp2;
      _tmp6 = (Rot11 * _data[0] + Rot21 * _data[1] + Rot31 * _data[2]) * _tmp1;
      R._data[0] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[1] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[2] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[1] + Rot23 * _data[3] + Rot33 * _data[4]) * _tmp3;
      _tmp5 = (Rot12 * _data[1] + Rot22 * _data[3] + Rot32 * _data[4]) * _tmp2;
      _tmp6 = (Rot11 * _data[1] + Rot21 * _data[3] + Rot31 * _data[4]) * _tmp1;
      R._data[3] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[4] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[5] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[2] + Rot23 * _data[4] + Rot33 * _data[5]) * _tmp3;
      _tmp5 = (Rot12 * _data[2] + Rot22 * _data[4] + Rot32 * _data[5]) * _tmp2;
      _tmp6 = (Rot11 * _data[2] + Rot21 * _data[4] + Rot31 * _data[5]) * _tmp1;
      R._data[6] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[7] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[8] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      return;
    }

    // calcul de l'angle
    alpha = (FF._data[_internalIndexes.index[q][q]] - FF._data[_internalIndexes.index[p][p]]) / (2. * FF._data[_internalIndexes.index[p][q]]);
    t = (alpha != 0.0 ? 1.0 / (alpha + sqrt(alpha * alpha + 1.0) * (alpha > 0.0 ? 1.0 : -1.0)) : 1.0);

    // calcul des cosinus
    Ctheta = 1.0 / (sqrt(1.0 + t * t));
    Stheta = t * Ctheta;

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
  fatalError("SymTensor2::polarDecomposeLnU", "No convergence");
}

/*
  Polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors.

  This method computes the polar decomposition of a second order tensor with computation of the \f$ U \f$ and \f$ R \f$ tensors as the returning arguments.

  - U Return second order tensor containing \f$ U \f$
  - R Return second order tensor containing \f$ R \f$
*/
//-----------------------------------------------------------------------------
void SymTensor2::polarDecompose(SymTensor2 &U, Tensor2 &R) const
//-----------------------------------------------------------------------------
{
  short p, q;
  double alpha, t, Ctheta, Stheta;
  double _tmp1, _tmp2, _tmp3, _tmp4, _tmp5, _tmp6, _tmp7, _tmp8;
  double Rot11, Rot12, Rot13, Rot21, Rot22, Rot23, Rot31, Rot32, Rot33;
  SymTensor2 FF;
  short it = 1;

  // calcul de FF=F(T).F
  FF._data[0] = dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]);
  FF._data[1] = _data[0] * _data[1] + _data[1] * _data[3] + _data[2] * _data[4];
  FF._data[2] = _data[0] * _data[2] + _data[1] * _data[4] + _data[2] * _data[5];
  FF._data[3] = dnlSquare(_data[1]) + dnlSquare(_data[3]) + dnlSquare(_data[4]);
  FF._data[4] = _data[1] * _data[2] + _data[3] * _data[4] + _data[4] * _data[5];
  FF._data[5] = dnlSquare(_data[2]) + dnlSquare(_data[4]) + dnlSquare(_data[5]);

  // initialisation de Rot
  Rot11 = 1.0;
  Rot22 = 1.0;
  Rot33 = 1.0;
  Rot12 = 0.0;
  Rot13 = 0.0;
  Rot21 = 0.0;
  Rot23 = 0.0;
  Rot31 = 0.0;
  Rot32 = 0.0;

  while (it < 30)
  {
    // recherche du terme maxi
    q = (dnlAbs(FF._data[1]) > dnlAbs(FF._data[2]) ? 1 : 2);
    p = (dnlAbs(FF._data[q]) > dnlAbs(FF._data[4]) ? 0 : 1);
    if (p == 1)
      q = 2;

    // test de convergence
    if (dnlAbs(FF._data[_internalIndexes.index[p][q]]) < precisionPolarDecompose)
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

      _tmp1 = 1.0 / sqrt(FF._data[0]);
      _tmp2 = 1.0 / sqrt(FF._data[3]);
      _tmp3 = 1.0 / sqrt(FF._data[5]);
      _tmp4 = (Rot13 * _data[0] + Rot23 * _data[1] + Rot33 * _data[2]) * _tmp3;
      _tmp5 = (Rot12 * _data[0] + Rot22 * _data[1] + Rot32 * _data[2]) * _tmp2;
      _tmp6 = (Rot11 * _data[0] + Rot21 * _data[1] + Rot31 * _data[2]) * _tmp1;
      R._data[0] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[1] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[2] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[1] + Rot23 * _data[3] + Rot33 * _data[4]) * _tmp3;
      _tmp5 = (Rot12 * _data[1] + Rot22 * _data[3] + Rot32 * _data[4]) * _tmp2;
      _tmp6 = (Rot11 * _data[1] + Rot21 * _data[3] + Rot31 * _data[4]) * _tmp1;
      R._data[3] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[4] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[5] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      _tmp4 = (Rot13 * _data[2] + Rot23 * _data[4] + Rot33 * _data[5]) * _tmp3;
      _tmp5 = (Rot12 * _data[2] + Rot22 * _data[4] + Rot32 * _data[5]) * _tmp2;
      _tmp6 = (Rot11 * _data[2] + Rot21 * _data[4] + Rot31 * _data[5]) * _tmp1;
      R._data[6] = Rot13 * _tmp4 + Rot12 * _tmp5 + Rot11 * _tmp6;
      R._data[7] = Rot23 * _tmp4 + Rot22 * _tmp5 + Rot21 * _tmp6;
      R._data[8] = Rot33 * _tmp4 + Rot32 * _tmp5 + Rot31 * _tmp6;
      return;
    }

    // calcul de l'angle
    alpha = (FF._data[_internalIndexes.index[q][q]] - FF._data[_internalIndexes.index[p][p]]) / (2. * FF._data[_internalIndexes.index[p][q]]);
    t = (alpha != 0.0 ? 1.0 / (alpha + sqrt(alpha * alpha + 1.0) * (alpha > 0.0 ? 1.0 : -1.0)) : 1.0);

    // calcul des cosinus
    Ctheta = 1.0 / (sqrt(1.0 + t * t));
    Stheta = t * Ctheta;

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
  fatalError("SymTensor2::polarDecompose", "No convergence");
}

/*
  Saves the content of a SymTensor2 into a NumPy file

  This method saves the content of a SymTensor2 object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  SymTensor2 t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void SymTensor2::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  double nst[9];
  nst[0] = _data[0];
  nst[1] = _data[1];
  nst[2] = _data[2];
  nst[3] = _data[1];
  nst[4] = _data[3];
  nst[5] = _data[4];
  nst[6] = _data[2];
  nst[7] = _data[4];
  nst[8] = _data[5];
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &nst[0], {3, 3}, mode);
}

/*
  Saves the content of a SymTensor2 into a NumPyZ file

  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  SymTensor2 t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void SymTensor2::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  double nst[9];
  nst[0] = _data[0];
  nst[1] = _data[1];
  nst[2] = _data[2];
  nst[3] = _data[1];
  nst[4] = _data[3];
  nst[5] = _data[4];
  nst[6] = _data[2];
  nst[7] = _data[4];
  nst[8] = _data[5];
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &nst[0], {3, 3}, mode);
}

/*
  Read the content of a SymTensor2 from a NumPy file

  This method reads the content of a vec3D object from a NumPy file defined by its filename.
  \code
  SymTensor2 t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void SymTensor2::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 9)
  {
    std::cout << "ERROR\n";
  }
  _data[0] = arr.data<double>()[0];
  _data[1] = arr.data<double>()[1];
  _data[2] = arr.data<double>()[2];
  _data[3] = arr.data<double>()[4];
  _data[4] = arr.data<double>()[5];
  _data[5] = arr.data<double>()[8];

  // memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

/*
  Read the content of a SymTensor2 from a NumPyZ file

  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.
  \code
  SymTensor2 t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void SymTensor2::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 9)
  {
    std::cout << "ERROR\n";
  }
  _data[0] = arr.data<double>()[0];
  _data[1] = arr.data<double>()[1];
  _data[2] = arr.data<double>()[2];
  _data[3] = arr.data<double>()[4];
  _data[4] = arr.data<double>()[5];
  _data[5] = arr.data<double>()[8];
}
