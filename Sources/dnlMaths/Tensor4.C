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
  \file Tensor.C
  \brief Definition file for the fourth order tensor class

  This file is the declaration file for the fourth order tensor class. A fourth order tensor has the following form:
  \f[ T = T_{ijkl} \f]
  \ingroup dnlMaths
*/

#include <fstream>

#include <Tensor4.h>
#include <Tensor3.h>
#include <Tensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>

/*!
  \brief Constructor of the Tensor4 class

  This method is the default constructor of a fourth order tensor. All components are initialized to zero by default.
*/
//-----------------------------------------------------------------------------
Tensor4::Tensor4()
//-----------------------------------------------------------------------------
{
  // initialisation
  setToValue(0.);
}

/*!
  \brief Destructor of the Tensor4 class
*/
//-----------------------------------------------------------------------------
Tensor4::~Tensor4()
//-----------------------------------------------------------------------------
{
}

/*!
  \brief Send the content of a fourth order tensor to the output flux for display
  \code
  Tensor4 t;
  std::cout << t << endl;
  \endcode
  \param os Output flux
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

/*!
  \brief Print the content of a fourth order tensor to the output flux for display

  \param os Output flux
*/
//-----------------------------------------------------------------------------
void Tensor4::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i, j, k, l;
  os << "tensor 3x3x3\n";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 3; k++)
      {
        for (l = 0; l < 3; l++)
        {
          os << "T[" << i << "," << j << "," << k << "," << l << "]="
             << v[dnlTensor4Ind(i, j, k, l, 3)];
        }
      }
    }
  }
}

/*!
  \brief Returns an identity tensor

  This method transforms the current tensor to an identity tensor.
  \code
  Tensor4 t1;
  t1.setToUnity(); // Returns and identity tensor
  \endcode
  \warning This method modify it's argument
*/
//-----------------------------------------------------------------------------
void Tensor4::setToUnity()
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          v[dnlTensor4Ind(i, j, k, l, 3)] =
              (dnlKronecker(i, k) * dnlKronecker(j, l) +
               dnlKronecker(i, l) * dnlKronecker(j, k)) /
              2.;
}

/*!
  \brief Fill a fourth order tensor with a scalar value

  This method is a surdefinition of the = operator for the fourth order tensor class.
  \code
  Tensor4 t1;
  t1 = 1.0; // All components of the tensor are set to 1.0
  \endcode
  \param val double value to give to all components of the fourth order tensor
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

/*!
  \brief Copy the content of a fourth order tensor into a new one

  This method is the so called = operator between two fourth order tensors. If the \ref MEM_funct is set, the \ref memcpy function is used for the copy.
  \code
  Tensor4 t1, t2;
  t1 = t2; // copy of the tensor
  \endcode
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  memcpy(v, t1.v, 81 * sizeof(double));
  return *this;
}

/*!
  \brief Addition of 2 fourth order tensors

  This method defines the addition of 2 fourth order tensors.
  The result of this operation is also a fourth order tensor defined by:
  \f[ T = A + B \f]
  \code
  Tensor4 t1,t2,t3;
  t3 = t1 + t2; // sum of 2 fourth order tensors
  \endcode
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator+(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] + t1.v[i];

  // renvoi de l'objet
  return t2;
}

/*!
  \brief Difference of 2 fourth order tensors

  This method defines the difference of 2 fourth order tensors.
  The result of this operation is also a fourth order tensor defined by:
  \f[ T = A - B \f]
  \code
  Tensor4 t1,t2,t3;
  t3 = t1 - t2; // difference of 2 fourth order tensors
  \endcode
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator-(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] - t1.v[i];

  // renvoi de l'objet
  return t2;
}

/*!
  \brief Multiplication of a fourth order tensor by a scalar value

  This method defines the multiplication of a fourth order tensor by a scalar value
  The result of this operation is also a fourth order tensor defined by:
  \f[ T = A . \lambda \f]
  \code
  Tensor4 t1,t2;
  double l;
  t2 = t1 * l; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = lambda * v[i];

  return t2;
}

/*!
  \brief Division of a fourth order tensor by a scalar value

  This method defines the division of a fourth order tensor by a scalar value
  The result of this operation is also a fourth order tensor defined by:
  \f[ T = \frac{A}{\lambda} \f]
  \code
  Tensor4 t1,t2;
  double l;
  t2 = t1 / l; // division by a scalar
  \endcode
  \warning This is not a commutative operation, be also warn not to divide by zero.
  \param lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] / lambda;

  return t2;
}

/*!
  \brief Multiplication of a fourth order tensor by a scalar value

  This method defines the multiplication of a fourth order tensor by a scalar value
  The result of this operation is also a fourth order tensor defined by:
  \f[ T = \lambda . A \f]
  \code
  Tensor4 t1,t2;
  double l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  \param lambda Scalar value to use for the multiplication
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor4 operator*(const double &lambda, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = lambda * t1.v[i];

  return t2;
}

/*!
  \brief Multiplication of a fourth order tensor by a vector

  This method defines the product of a fourth order tensor by a vector.
  The result of this operation is also a vector defined by:
  \f[ B = T . \overrightarrow{a} \f]
  \code
  Tensor4 t1;
  Vec3D v1;
  Tensor 2 t2;
  t2 = t1 * v1; // product of the fourth order tensor by a vector
  \endcode
  \param v1 Vector to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor4::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Tensor3 t3;

  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          t3(i, j, k) += v[dnlTensor4Ind(i, j, k, l, 3)] * v1(l);

  return t3;
}

/*!
  \brief Multiplication of a fourth order tensor by a tensor

  This method defines the product of a fourth order tensor by a tensor.
  The result of this operation is also a tensor defined by:
  \f[ B = T . \overrightarrow{a} \f]
  \code
  Tensor4 t1;
  Vec3D v1;
  Tensor 2 t2;
  t2 = t1 * v1; // product of the fourth order tensor by a tensor
  \endcode
  \param t2 tensor to use for the operation
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor4::operator*(const Tensor2 &t2) const
//-----------------------------------------------------------------------------
{
  Tensor2 t3;

  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          t3(i, j) += v[dnlTensor4Ind(i, j, k, l, 3)] * t2(k, l);

  return t3;
}

/*!
  \brief Test the equality of two fourth order tensors

  This method tests the equality of two fourth order tensors.
  It returns \ref true if all components of the two fourth order tensors are equals, \ref false on other case.
  \return \ref true or \ref false depending on the result of the test.
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool Tensor4::operator==(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < 81; i++)
    if (v[i] != t1.v[i])
      return false;
  return true;
}

/*!
  \brief Test the equality of two fourth order tensors

  This method tests the equality of two fourth order tensors.
  It returns \ref false if all components of the two fourth order tensors are equals, \ref true on other case.
  \return \ref true or \ref false depending on the result of the test.
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
bool Tensor4::operator!=(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

/*!
  \brief Writes a fourth order tensor in a binary flux for storage

  This method is used to store the components of a fourth order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor4 t;
  t.write(pfile);
  \endcode
  \param ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Tensor4::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 81 * sizeof(double));
}

/*!
  \brief Reads a fourth order tensor in a binary flux from storage

  This method is used to read the components of a fourth order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor4 t;
  t.read(pfile);
  \endcode
  \param ofs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
void Tensor4::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 81 * sizeof(double));
}

/*!
  \brief Writes a fourth order tensor in a binary flux for storage

  This method is used to store the components of a fourth order tensor in a binary file.
  \code
  std::ofstream pfile("file");
  Tensor4 t;
  pfile << t;
  \endcode
  \param os Output file stream to use for writting operation
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

/*!
  \brief Reads a fourth order tensor from a binary flux for storage

  This method is used to read the components of a fourth order tensor in a binary file.
  \code
  std::ifstream pfile("fichier");
  Tensor4 t;
  pfile >> t;
  \endcode
  \param os Input file stream to use for reading operation
  \param t1 Second fourth order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

/*!
  \brief Saves the content of a Tensor4 into a NumPy file

  This method saves the content of a Tensor4 object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor4 t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &v[0], {3, 3, 3, 3}, mode);
}

/*!
  \brief Saves the content of a Tensor4 into a NumPyZ file

  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.
  \code
  Tensor4 t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &v[0], {3, 3, 3, 3}, mode);
}

/*!
  \brief Read the content of a Tensor4 from a NumPy file

  This method reads the content of a vec3D object from a NumPy file defined by its filename.
  \code
  Tensor4 t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}

/*!
  \brief Read the content of a Tensor4 from a NumPyZ file

  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.
  \code
  Tensor4 t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}
