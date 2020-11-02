/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*!
  \file Tensor3.C
  \brief Definition file for the third order tensor class

  This file is the declaration file for the third order tensor class. A third order tensor has the following form:
  \f[ T = T_{ijk} \f]
  \ingroup dnlMaths
*/

#include <fstream>

#include <Tensor3.h>
#include <Tensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>

/*!
  \brief Constructor of the Tensor3 class

  This method is the default constructor of a third order tensor. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
Tensor3::Tensor3()
//-----------------------------------------------------------------------------
{
  // initialisation
  setToValue(0.);
}

/*!
  \brief Destructor of the Tensor3 class
*/
//-----------------------------------------------------------------------------
Tensor3::~Tensor3()
//-----------------------------------------------------------------------------
{
}

/*!
  \brief Send the content of a third order tensor to the output flux for display
  \code
  Tensor3 t;
  std::cout << t << endl;
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

/*!
  \brief Print the content of a third order tensor to the output flux for display

  \param os Output flux
*/
//-----------------------------------------------------------------------------
void Tensor3::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  short i, j, k;
  os << "tensor 3x3x3\n";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 3; k++)
      {
        os << "T[" << i << "," << j << "," << k << "]=" << v[dnlTensor3Ind(i, j, k, 3)];
      }
    }
  }
}

/*!
  \brief Returns an identity tensor

  This method transforms the current tensor to an identity tensor.
  \code
  Tensor3 t1;
  t1.setToUnity(); // Returns and identity tensor
  \endcode
  \warning This method modify it's argument
*/
//-----------------------------------------------------------------------------
void Tensor3::setToUnity()
//-----------------------------------------------------------------------------
{
  setToValue(0.);
  v[dnlTensor3Ind(0, 1, 2, 3)] = 1.;
  v[dnlTensor3Ind(1, 2, 0, 3)] = 1.;
  v[dnlTensor3Ind(2, 0, 1, 3)] = 1.;
  v[dnlTensor3Ind(2, 1, 0, 3)] = -1.;
  v[dnlTensor3Ind(1, 0, 2, 3)] = -1.;
  v[dnlTensor3Ind(0, 2, 1, 3)] = -1.;
}

/*!
  \brief Fill a third order tensor with a scalar value

  This method is a surdefinition of the = operator for the third order tensor class.
  \code
  Tensor3 t1;
  t1 = 1.0; // All components of the tensor are set to 1.0
  \endcode
  \param val double value to give to all components of the third order tensor
*/
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

/*!
  \brief Copy the content of a third order tensor into a new one

  This method is the so called = operator between two third order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.
  \code
  Tensor3 t1, t2;
  t1 = t2; // copy of the tensor
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  memcpy(v, t1.v, 27 * sizeof(double));
  return *this;
}

/*!
  \brief Addition of 2 third order tensors

  This method defines the addition of 2 third order tensors.
  The result of this operation is also a third order tensor defined by:
  \f[ T = A + B \f]
  \code
  Tensor3 t1,t2,t3;
  t3 = t1 + t2; // sum of 2 third order tensors
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator+(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (short i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] + t1.v[i];
  }

  // renvoi de l'objet
  return t2;
}

/*!
  \brief Difference of 2 third order tensors

  This method defines the difference of 2 third order tensors.
  The result of this operation is also a third order tensor defined by:
  \f[ T = A - B \f]
  \code
  Tensor3 t1,t2,t3;
  t3 = t1 - t2; // difference of 2 third order tensors
  \endcode
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator-(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (short i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] - t1.v[i];
  }

  // renvoi de l'objet
  return t2;
}

/*!
  \brief Multiplication of a third order tensor by a scalar value

  This method defines the multiplication of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = A . \lambda \f]
  \code
  Tensor3 t1,t2;
  double l;
  t2 = t1 * l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2.v[i] = lambda * v[i];
  }

  return t2;
}

/*!
  \brief Division of a third order tensor by a scalar value

  This method defines the division of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  \code
  Tensor3 t1,t2;
  double l;
  t2 = t1 / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2.v[i] = v[i] / lambda;
  }

  return t2;
}

/*!
  \brief Multiplication of a third order tensor by a scalar value

  This method defines the multiplication of a third order tensor by a scalar value
  The result of this operation is also a third order tensor defined by:
  \f[ T = \lambda . A \f]
  \code
  Tensor3 t1,t2;
  double l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 operator*(const double &lambda, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2.v[i] = lambda * t1.v[i];
  }

  return t2;
}

/*!
  \brief Multiplication of a third order tensor by a vector

  This method defines the product of a third order tensor by a vector.
  The result of this operation is also a vector defined by:
  \f[ B = T . \overrightarrow{a} \f]
  \code
  Tensor3 t1;
  Vec3D v1;
  Tensor 2 t2;
  t2 = t1 * v1; // product of the third order tensor by a vector
  \endcode
  \param v1 Vector to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor3::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  for (short i = 0; i < 3; i++)
    for (short j = 0; j < 3; j++)
      for (short k = 0; k < 3; k++)
      {
        t2(i, j) += v[dnlTensor3Ind(i, j, k, 3)] * v1(k);
      }

  return t2;
}

/*!
  \brief Test the equality of two third order tensors

  This method tests the equality of two third order tensors.
  It returns \ref true if all components of the two third order tensors are equals, \ref false on other case.
  \return \ref true or \ref false depending on the result of the test.
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool Tensor3::operator==(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  short i;

  for (i = 0; i < 27; i++)
    if (v[i] != t1.v[i])
    {
      return false;
    }
  return true;
}

/*!
  \brief Test the equality of two third order tensors

  This method tests the equality of two third order tensors.
  It returns \ref false if all components of the two third order tensors are equals, \ref true on other case.
  \return \ref true or \ref false depending on the result of the test.
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool Tensor3::operator!=(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

/*!
  \brief Writes a third order tensor in a binary flux for storage

  This method is used to store the components of a third order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor3 t;
  t.write(pfile);
  \endcode
  \param ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Tensor3::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 27 * sizeof(double));
}

/*!
  \brief Reads a third order tensor in a binary flux from storage

  This method is used to read the components of a third order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor3 t;
  t.read(pfile);
  \endcode
  \param ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void Tensor3::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 27 * sizeof(double));
}

/*!
  \brief Writes a third order tensor in a binary flux for storage

  This method is used to store the components of a third order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor3 t;
  pfile << t;
  \endcode
  \param os Output file stream to use for writting operation
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

/*!
  \brief Reads a third order tensor from a binary flux for storage

  This method is used to read the components of a third order tensor in a binary file.
  \code
  std::ifstream pfile("fichier");
  Tensor3 t;
  pfile >> t;
  \endcode
  \param os Input file stream to use for reading operation
  \param t1 Second third order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

/*!
  \brief Saves the content of a Tensor3 into a NumPy file

  This method saves the content of a Tensor3 object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor3 t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor3::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &v[0], {3, 3, 3}, mode);
}

/*!
  \brief Saves the content of a Tensor3 into a NumPyZ file

  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor3 t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor3::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &v[0], {3, 3, 3}, mode);
}

/*!
  \brief Read the content of a Tensor3 from a NumPy file

  This method reads the content of a vec3D object from a NumPy file defined by its filename.
  \code
  Tensor3 t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor3::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 27)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}

/*!
  \brief Read the content of a Tensor3 from a NumPyZ file

  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.
  \code
  Tensor3 t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor3::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 27)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}
