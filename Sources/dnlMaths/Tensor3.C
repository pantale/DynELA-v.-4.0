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
#include <Tensor3.h>
#include <Tensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>

/*
@LABEL:Tensor3::Tensor3()
@SHORT:Default constructor of the Tensor3 class.
@RETURN:Tensor3 : The new Tensor3 object created by the constructor.
This is the default constructor of the Tensor3 class, where all components are initialized to zero by default.
@END
*/
//-----------------------------------------------------------------------------
Tensor3::Tensor3()
//-----------------------------------------------------------------------------
{
  // initialisation
  setToValue(0);
}

// Copy constructor
//-----------------------------------------------------------------------------
Tensor3::Tensor3(const Tensor3 &tensor)
//-----------------------------------------------------------------------------
{
  memcpy(_data, tensor._data, 27 * sizeof(double));
}

//-----------------------------------------------------------------------------
Tensor3::~Tensor3()
//-----------------------------------------------------------------------------
{
}

// Send the content of a third order tensor to the output flux for display
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

// Print the content of a third order tensor to the output flux for display
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
        os << "T[" << i << "," << j << "," << k << "]=" << _data[dnlTensor3Ind(i, j, k, 3)];
      }
    }
  }
}

/*
@LABEL:Tensor3::setToUnity()
@SHORT:Unity third order tensor.
@WARNING:This method modifies its own argument
This method transforms the current tensor to a unity tensor.
\begin{equation*}
\T=\left[\begin{array}{ccc}
1&0&0\\
0&1&0\\
0&0&1
\end{array}\right]
\end{equation*}
where $\T$ is a third order tensor defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Tensor3::setToUnity()
//-----------------------------------------------------------------------------
{
  setToValue(0);
  _data[dnlTensor3Ind(0, 1, 2, 3)] = 1;
  _data[dnlTensor3Ind(1, 2, 0, 3)] = 1;
  _data[dnlTensor3Ind(2, 0, 1, 3)] = 1;
  _data[dnlTensor3Ind(2, 1, 0, 3)] = -1;
  _data[dnlTensor3Ind(1, 0, 2, 3)] = -1;
  _data[dnlTensor3Ind(0, 2, 1, 3)] = -1;
}

/*
@LABEL:Tensor3::operator=(double v)
@SHORT:Fill a third order tensor with a scalar value.
@RETURN:Tensor3
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the third order tensor class.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

// Copy the content of a third order tensor into a new one
//-----------------------------------------------------------------------------
Tensor3 &Tensor3::operator=(const Tensor3 &T)
//-----------------------------------------------------------------------------
{
  memcpy(_data, T._data, 27 * sizeof(double));
  return *this;
}

/*
@LABEL:Tensor3::operator+(Tensor3 B)
@SHORT:Addition of 2 third order tensors.
@ARG:Tensor3 & B & Third order tensor to add to the current one.
@RETURN:Tensor3 : Result of the addition operation.
This method defines the addition of 2 third order tensors.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\T = \A + \B
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\B$ is the third order tensor value defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator+(const Tensor3 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (short i = 0; i < 27; i++)
  {
    t2._data[i] = _data[i] + T._data[i];
  }

  // renvoi de l'objet
  return t2;
}

/*
@LABEL:Tensor3::operator-(Tensor3 B)
@SHORT:Subtraction of 2 third order tensors.
@ARG:Tensor3 & B & Third order tensor to subtract to the current one.
@RETURN:Tensor3 : Result of the difference operation.
This method defines the subtraction of 2 third order tensors.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\T = \A - \B
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\B$ is the third order tensor value defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator-(const Tensor3 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor3 t2;

  // calcul de la somme
  for (short i = 0; i < 27; i++)
  {
    t2._data[i] = _data[i] - T._data[i];
  }

  // renvoi de l'objet
  return t2;
}

/*
@LABEL:Tensor3::operator*(double l)
@SHORT:Multiplication of a third order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor3 : Result of the multiplication operation.
This method defines the multiplication of a third order tensor by a scalar value.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2._data[i] = lambda * _data[i];
  }

  return t2;
}

/*
@LABEL:Tensor3::operator/(double l)
@SHORT:Division of a third order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor3 : Result of the division operation.
This method defines the division of a third order tensor by a scalar value.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\T = \frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor3::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2._data[i] = _data[i] / lambda;
  }

  return t2;
}

/*
@LABEL:operator*(double l, Tensor3 A)
@SHORT:Multiplication of a third order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@ARG:Tensor3 & A & Third order tensor to use for the operation.
@RETURN:Tensor3 : Result of the multiplication operation.
This method defines the multiplication of a third order tensor by a scalar value.
The result of this operation is also a third order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a third order tensor and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 operator*(const double &l, const Tensor3 &T)
//-----------------------------------------------------------------------------
{
  Tensor3 t2;

  for (short i = 0; i < 27; i++)
  {
    t2._data[i] = l * T._data[i];
  }

  return t2;
}

/*
@LABEL:Tensor3::operator*(Vec3D V)
@SHORT:Multiplication of a third order tensor by a vector.
@RETURN:Tensor2 : Result of the multiplication operation.
@ARG:Vec3D & V & Vec3D to use for the multiplication operation.
This method defines the product of a third order tensor by a vector.
The result of this operation is a second order tensor defined by:
\begin{equation*}
\B = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a third order tensor defined by the object itself and $\overrightarrow{x}$ is a Vec3D defined by parameter V.
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Tensor3::operator*(const Vec3D &V) const
//-----------------------------------------------------------------------------
{
  Tensor2 t2;

  for (short i = 0; i < 3; i++)
    for (short j = 0; j < 3; j++)
      for (short k = 0; k < 3; k++)
      {
        t2(i, j) += _data[dnlTensor3Ind(i, j, k, 3)] * V(k);
      }

  return t2;
}

//  Test the equality of two third order tensors
//-----------------------------------------------------------------------------
bool Tensor3::operator==(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  short i;

  for (i = 0; i < 27; i++)
    if (_data[i] != t1._data[i])
    {
      return false;
    }
  return true;
}

//  Test the inequality of two third order tensors
//-----------------------------------------------------------------------------
bool Tensor3::operator!=(const Tensor3 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//  Writes a third order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
void Tensor3::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)_data, 27 * sizeof(double));
}

//  Reads a third order tensor in a binary flux from storage
//-----------------------------------------------------------------------------
void Tensor3::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)_data, 27 * sizeof(double));
}

//  Writes a third order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//  Reads a third order tensor from a binary flux for storage
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor3 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

//  Saves the content of a Tensor3 into a NumPy file
//-----------------------------------------------------------------------------
void Tensor3::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {3, 3, 3}, mode);
}

//  Saves the content of a Tensor3 into a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor3::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {3, 3, 3}, mode);
}

//  Read the content of a Tensor3 from a NumPy file
//-----------------------------------------------------------------------------
void Tensor3::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 27)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//  Read the content of a Tensor3 from a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor3::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 27)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
