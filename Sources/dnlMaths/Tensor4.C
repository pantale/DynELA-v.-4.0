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
#include <Tensor4.h>
#include <Tensor3.h>
#include <Tensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>


/*
@LABEL:Tensor4::Tensor4()
@SHORT:Default constructor of the Tensor4 class.
@RETURN:Tensor4 : The new Tensor4 object created by the constructor.
This is the default constructor of the Tensor4 class, where all components are initialized to zero by default.
@END
*/
//-----------------------------------------------------------------------------
Tensor4::Tensor4()
//-----------------------------------------------------------------------------
{
  // initialisation
  setValue(0);
}

// Destructor
//-----------------------------------------------------------------------------
Tensor4::~Tensor4()
//-----------------------------------------------------------------------------
{
}

// Send the content of a fourth order tensor to the output flux for display
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

// Print the content of a fourth order tensor to the output flux for display
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
             << _data[dnlTensor4Ind(i, j, k, l, 3)];
        }
      }
    }
  }
}

/*
@LABEL:Tensor4::setToUnity()
@SHORT:Unity fourth order tensor.
@SMOD
This method transforms the current tensor to a fourth order unity tensor.
@END
*/
//-----------------------------------------------------------------------------
void Tensor4::setToUnity()
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          _data[dnlTensor4Ind(i, j, k, l, 3)] =
              (dnlKronecker(i, k) * dnlKronecker(j, l) +
               dnlKronecker(i, l) * dnlKronecker(j, k)) /
              2.;
}

/*
@LABEL:Tensor4::operator=(double v)
@SHORT:Fill a fourth order tensor with a scalar value.
@RETURN:Tensor4
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the fourth order tensor class.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setValue(val);
  return *this;
}

// Copy the content of a fourth order tensor into a new one
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const Tensor4 &T)
//-----------------------------------------------------------------------------
{
  memcpy(_data, T._data, 81 * sizeof(double));
  return *this;
}

/*
@LABEL:Tensor4::operator+(Tensor4 B)
@SHORT:Addition of 2 fourth order tensors.
@ARG:Tensor4 & B & Second order tensor to add to the current one.
@RETURN:Tensor4 : Result of the addition operation.
This method defines the addition of 2 fourth order tensors.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\T = \A + \B
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\B$ is the fourth order tensor value defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator+(const Tensor4 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2._data[i] = _data[i] + T._data[i];

  // renvoi de l'objet
  return t2;
}

/*
@LABEL:Tensor4::operator-(Tensor4 B)
@SHORT:Subtraction of 2 fourth order tensors.
@ARG:Tensor4 & B & Second order tensor to subtract to the current one.
@RETURN:Tensor4 : Result of the difference operation.
This method defines the subtraction of 2 fourth order tensors.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\T = \A - \B
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\B$ is the fourth order tensor value defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator-(const Tensor4 &T) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2._data[i] = _data[i] - T._data[i];

  // renvoi de l'objet
  return t2;
}

/*
@LABEL:Tensor4::operator*(double l)
@SHORT:Multiplication of a fourth order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor4 : Result of the multiplication operation.
This method defines the multiplication of a fourth order tensor by a scalar value.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator*(const double &l) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2._data[i] = l * _data[i];

  return t2;
}

/*
@LABEL:Tensor4::operator/(double l)
@SHORT:Division of a fourth order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Tensor4 : Result of the division operation.
This method defines the division of a fourth order tensor by a scalar value.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\T = \frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 Tensor4::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2._data[i] = _data[i] / lambda;

  return t2;
}

/*
@LABEL:operator*(double l, Tensor4 A)
@SHORT:Multiplication of a fourth order tensor by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@ARG:Tensor4 & A & Second order tensor to use for the operation.
@RETURN:Tensor4 : Result of the multiplication operation.
This method defines the multiplication of a fourth order tensor by a scalar value.
The result of this operation is also a fourth order tensor defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a fourth order tensor and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Tensor4 operator*(const double &l, const Tensor4 &T)
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2._data[i] = l * T._data[i];

  return t2;
}

/*
@LABEL:Tensor4::operator*(Vec3D V)
@SHORT:Multiplication of a fourth order tensor by a vector.
@RETURN:Vec3D : Result of the multiplication operation.
@ARG:Vec3D & V & Vec3D to use for the multiplication operation.
This method defines the product of a fourth order tensor by a vector.
The result of this operation is a third order tensor defined by:
\begin{equation*}
B = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\overrightarrow{x}$ is a Vec3D defined by parameter V.
@END
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor4::operator*(const Vec3D &V) const
//-----------------------------------------------------------------------------
{
  Tensor3 t3;

  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          t3(i, j, k) += _data[dnlTensor4Ind(i, j, k, l, 3)] * V(l);

  return t3;
}

/*
@LABEL:Tensor4::operator*(Tensor2 B)
@SHORT:Multiplication of a fourth order tensor by a second order tensor.
@RETURN:Tensor2 : Result of the multiplication operation.
@ARG:Tensor2 & B & Tensor2 to use for the multiplication operation.
This method defines the product of a fourth order tensor by a second order tensor.
The result of this operation is a second order tensor defined by:
\begin{equation*}
\C = \A \cdot \B
\end{equation*}
where $\A$ is a fourth order tensor defined by the object itself and $\B$ is a Vesecond order tensor defined by parameter B.
@END
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
          t3(i, j) += _data[dnlTensor4Ind(i, j, k, l, 3)] * t2(k, l);

  return t3;
}


//  Test the equality of two fourth order tensors
//-----------------------------------------------------------------------------
bool Tensor4::operator==(const Tensor4 &T) const
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < 81; i++)
    if (_data[i] != T._data[i])
      return false;
  return true;
}

//  Test the inequality of two fourth order tensors
//-----------------------------------------------------------------------------
bool Tensor4::operator!=(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//  Writes a fourth order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
void Tensor4::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)_data, 81 * sizeof(double));
}

//  Reads a fourth order tensor in a binary flux from storage
//-----------------------------------------------------------------------------
void Tensor4::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)_data, 81 * sizeof(double));
}

//  Writes a fourth order tensor in a binary flux for storage
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//  Reads a fourth order tensor from a binary flux for storage
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

//  Saves the content of a Tensor4 into a NumPy file
//-----------------------------------------------------------------------------
void Tensor4::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {3, 3, 3, 3}, mode);
}

//  Saves the content of a Tensor4 into a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor4::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {3, 3, 3, 3}, mode);
}

//  Read the content of a Tensor4 from a NumPy file
//-----------------------------------------------------------------------------
void Tensor4::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//  Read the content of a Tensor4 from a NumPyZ file
//-----------------------------------------------------------------------------
void Tensor4::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
