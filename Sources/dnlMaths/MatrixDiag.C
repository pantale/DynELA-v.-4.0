/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <MatrixDiag.h>
#include <NumpyInterface.h>

/*
@LABEL:MatrixDiag::MatrixDiag(long n, double m)
@SHORT:Constructor of the MatrixDiag class with initialization.
@RETURN:MatrixDiag : The initialized matrix.
@ARG:long & n & Number of rows and cols of the matrix to create.
@ARG:double & m & Value to give to each element of the new matrix.
This method creates a new matrix of size $n \times n$ where all values are initialized to the scalar value $m$.
\begin{equation*}
\M=\left[\begin{array}{cccc}
  M_{11}=m & 0 & \hdots & 0\\
  0 & M_{22}=m & \hdots & 0\\
  \vdots & \vdots & \hdots & \vdots\\
  0 & 0 & \hdots & M_{nn}=m
  \end{array}\right]
\end{equation*}
where the $\M$ is the object itself and $m$ is scalar value defined by parameter m.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag::MatrixDiag(const long numberOfTerms, double value)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(numberOfTerms);

  // initialisation
  setValue(value);
}

// Copy constructor
//-----------------------------------------------------------------------------
MatrixDiag::MatrixDiag(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  allocate(mat._rows);

  memcpy(_data, mat._data, _rows * sizeof(double));
}

// Destructor
//-----------------------------------------------------------------------------
MatrixDiag::~MatrixDiag()
//-----------------------------------------------------------------------------
{
  deallocate();
}

/*
@LABEL:MatrixDiag::maxVal()
@SHORT:Maximum component in a diagonal matrix.
@RETURN:double : The maximum component of the diagonal matrix.
This method returns the maximum component in a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
double MatrixDiag::maxVal()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _rows; i++)
    if (_data[i] > val)
      val = _data[i];

  return val;
}

/*
@LABEL:MatrixDiag::minVal()
@SHORT:Minimum component in a diagonal matrix.
@RETURN:double : The minimum component of the diagonal matrix.
This method returns the minimum component in a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
double MatrixDiag::minVal()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _rows; i++)
    if (_data[i] < val)
      val = _data[i];

  return val;
}

/*
@LABEL:MatrixDiag::maxAbs()
@SHORT:Maximum absolute component in a diagonal matrix.
@RETURN:double : The maximum component of the diagonal matrix.
This method returns the maximum absolute component in a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
double MatrixDiag::maxAbs()
//-----------------------------------------------------------------------------
{
  double val = dnlAbs(_data[0]);

  for (long i = 1; i < _rows; i++)
    if (dnlAbs(_data[i]) > val)
      val = dnlAbs(_data[i]);

  return val;
}

/*
@LABEL:MatrixDiag::minAbs()
@SHORT:Minimum absolute component in a diagonal matrix.
@RETURN:double : The minimum component of the diagonal matrix.
This method returns the minimum absolute component in a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
double MatrixDiag::minAbs()
//-----------------------------------------------------------------------------
{
  double val = dnlAbs(_data[0]);

  for (long i = 1; i < _rows; i++)
    if (dnlAbs(_data[i]) < val)
      val = dnlAbs(_data[i]);

  return val;
}

// Memory allocation
//-----------------------------------------------------------------------------
void MatrixDiag::allocate(const long numberOfTerms)
//-----------------------------------------------------------------------------
{
  _rows = numberOfTerms;
  _cols = numberOfTerms;

  // allocation dynamique de memoire
  _data = new double[_rows];
}

// Memory deallocation
//-----------------------------------------------------------------------------
void MatrixDiag::deallocate()
//-----------------------------------------------------------------------------
{
  _rows = 0;
  _cols = 0;

  delete[] _data;
}

/*
@LABEL:MatrixDiag::redim(long n)
@SHORT:Change the allocation size of a diagonal matrix.
@ARG:long & n & Number of rows and cols of the diagonal matrix.
@WARNING:This method cleans the content of the diagonal matrix.
This method changes the size of a diagonal matrix.
If the new size is the same as the actual size, this method does nothing.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::redim(const long newSize)
//-----------------------------------------------------------------------------
{
  if (newSize == _rows)
    return;

  deallocate();
  allocate(newSize);
}

// Send the content of a diagonal matrix to the output flux for display
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

// Print the content of a diagonal matrix to the output flux for display
//-----------------------------------------------------------------------------
void MatrixDiag::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i, j;

  os << "diagonal matrix " << _rows << "x" << _cols << " ={{";
  for (i = 0; i < _rows; i++)
  {
    for (j = 0; j < _cols; j++)
    {
      if (j != 0)
        os << ",";
      os << (i == j ? _data[i] : 00);
    }
    if (i != _rows - 1)
      os << "},{";
  }
  os << "}}";
}

/*
@LABEL:MatrixDiag::setValue(double v)
@SHORT:Fill a diagonal matrix with a scalar value.
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the diagonal matrix class.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::setValue(double val)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _rows; i++)
    _data[i] = val;
}

/*
@LABEL:MatrixDiag::setToUnity()
@SHORT:Unity diagonal matrix.
@SMOD
This method transforms the current diagonal matrix to a unity diagonal matrix.
\begin{equation*}
\M=\left[\begin{array}{cccc}
  M_{11}=1 & 0 & \hdots & 0\\
  0 & M_{22}=1 & \hdots & 0\\
  \vdots & \vdots & \hdots & \vdots\\
  0 & 0 & \hdots & M_{nn}=1
  \end{array}\right]
\end{equation*}
where $\M$ is a diagonal matrix defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::setToUnity()
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _rows; i++)
    _data[i] = 1;
}

/*
@LABEL:MatrixDiag::operator=(double m)
@SHORT:Fill a diagonal matrix with a scalar value.
@RETURN:MatrixDiag
@ARG:double & m & Value to use for the operation.
This method is a surdefinition of the = operator for the diagonal matrix class.
\begin{equation*}
\M=\left[\begin{array}{cccc}
  M_{11}=m & 0 & \hdots & 0\\
  0 & M_{22}=m & \hdots & 0\\
  \vdots & \vdots & \hdots & \vdots\\
  0 & 0 & \hdots & M_{nn}=m
  \end{array}\right]
\end{equation*}
where $\M$ is a diagonal matrix defined by the object itself and $m$ is the scalar value defined by parameter m.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag &MatrixDiag::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setValue(val);
  return *this;
}

// Copy the content of a matrix into a new one
//-----------------------------------------------------------------------------
MatrixDiag &MatrixDiag::operator=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  // redimension de la matrice resultat
  redim(mat._rows);

  // recopie des valeurs
  memcpy(_data, mat._data, _rows * sizeof(double));

  return *this;
}

/*
@LABEL:MatrixDiag::operator+(MatrixDiag B)
@SHORT:Addition of 2 diagonal matrices.
@ARG:MatrixDiag & B & MatrixDiag to add to the current one.
@RETURN:MatrixDiag : Result of the addition operation.
This method defines the addition of 2 diagonal matrices.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \A + \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator+(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator +",
               "diagonal matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] diagonal matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // creation d'une nouvelle matrice
  MatrixDiag resu(mat._rows, mat._cols);

  // calcul de la somme
  for (long i = 0; i < _rows; i++)
    resu._data[i] = _data[i] + mat._data[i];

  // renvoi de l'objet
  return resu;
}

/*
@LABEL:MatrixDiag::operator-(MatrixDiag B)
@SHORT:Difference of 2 diagonal matrices.
@ARG:MatrixDiag & B & MatrixDiag to subtract to the current one.
@RETURN:MatrixDiag : Result of the difference operation.
This method defines the difference of 2 diagonal matrices.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \A - \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator-(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator -",
               "diagonal matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] diagonal matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // creation d'une nouvelle matrice
  MatrixDiag resu(_rows, _cols);

  // calcul de la difference
  for (long i = 0; i < _rows; i++)
    resu._data[i] = _data[i] - mat._data[i];

  // renvoi de l'objet
  return resu;
}

/*
@LABEL:MatrixDiag::operator-()
@SHORT:Opposite of a diagonal matrix.
@RETURN:MatrixDiag : The opposite diagonal matrix.
This method defines the opposite of a diagonal matrix.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = - \A
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator-() const
//-----------------------------------------------------------------------------
{
  // creation d'une nouvelle matrice
  MatrixDiag resu(_rows, _cols);

  // calcul de la difference
  for (long i = 0; i < _rows; i++)
    resu._data[i] = -_data[i];

  // renvoi de l'objet
  return resu;
}

/*
@LABEL:MatrixDiag::operator+=(MatrixDiag B)
@SHORT:Addition of 2 diagonal matrices.
@ARG:MatrixDiag & B & Second diagonal matrix to add to the current one.
@RETURN:MatrixDiag : Result of the addition operation.
This method defines the addition of 2 diagonal matrices.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\A += \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator+=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator +=",
               "diagonal matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] diagonal matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // calcul de la somme
  for (long i = 0; i < _rows; i++)
    _data[i] += mat._data[i];
}

/*
@LABEL:MatrixDiag::operator-=(MatrixDiag B)
@SHORT:Difference of 2 diagonal matrices.
@ARG:MatrixDiag & B & Second diagonal matrix to add to the current one.
@RETURN:MatrixDiag : Result of the difference operation.
This method defines the difference of 2 diagonal matrices.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\A -= \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator-=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator -=",
               "diagonal matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] diagonal matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // calcul de la difference
  for (long i = 0; i < _rows; i++)
    _data[i] -= mat._data[i];
}

/*
@LABEL:MatrixDiag::operator*(double l)
@SHORT:Multiplication of a diagonal matrix by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:MatrixDiag : Result of the multiplication operation.
This method defines the multiplication of a diagonal matrix by a scalar value.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(_rows, _cols);

  for (long i = 0; i < _rows; i++)
    resu._data[i] = lambda * _data[i];
  return resu;
}

/*
@LABEL:MatrixDiag::operator/(double l)
@SHORT:Division of a diagonal matrix by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:MatrixDiag : Result of the division operation.
This method defines the division of a diagonal matrix by a scalar value.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \frac{1}{\lambda} \A
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(_rows, _cols);

  for (long i = 0; i < _rows; i++)
    resu._data[i] = _data[i] / lambda;
  return resu;
}

/*
@LABEL:operator*(double l, MatrixDiag A)
@SHORT:Multiplication of a diagonal matrix by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@ARG:MatrixDiag & A & Second matrix to use for the operation.
@RETURN:MatrixDiag : Result of the multiplication operation.
This method defines the multiplication of a diagonal matrix by a scalar value.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \lambda \A
\end{equation*}
where $\A$ is a diagonal matrix and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag operator*(const double &lambda, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  MatrixDiag resu(mat._rows, mat._cols);

  for (long i = 0; i < mat._rows; i++)
    resu._data[i] = lambda * mat._data[i];
  return resu;
}

/*
@LABEL:MatrixDiag::operator*(MatrixDiag B)
@SHORT:Single contracted product of two diagonal matrices.
@RETURN:MatrixDiag : Result of the multiplication operation.
@ARG:MatrixDiag & B & Second matrix for the multiplication operation.
This method defines a single contracted product of two diagonal matrices.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator*(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  return dot(mat);
}

/*
@LABEL:MatrixDiag::dot(MatrixDiag B)
@SHORT:Single contracted product of two matrixs.
@RETURN:MatrixDiag : Result of the multiplication operation.
@ARG:MatrixDiag & B & Second matrix for the multiplication operation.
This method defines a single contracted product of two matrixs.
The result of this operation is also a diagonal matrix defined by:
\begin{equation*}
\T = \A \cdot \B
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\B$ is the diagonal matrix defined by parameter B.
This method uses the Blas \textsf{dgemm} Fortran subroutine to perform the operation.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::dot(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  if (_cols != mat._rows)
  {
    fatalError("MatrixDiag::operator *",
               "diagonal matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] diagonal matrix",
               _rows, _cols, mat._rows, mat._cols);
  }

  MatrixDiag resu(_rows, mat._cols);

  for (long i = 0; i < _rows; i++)
    resu._data[i] = _data[i] * mat._data[i];

  return resu;
}

/*
@LABEL:MatrixDiag::operator*(Vector V)
@SHORT:Multiplication of a diagonal matrix by a vector.
@RETURN:Vector : Result of the multiplication operation.
@ARG:Vector & V & Vector to use for the multiplication operation.
This method defines the product of a diagonal matrix by a vector.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\overrightarrow{x}$ is a Vector defined by parameter V.
@END
*/
//-----------------------------------------------------------------------------
Vector MatrixDiag::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  if (_cols != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "diagonal matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] diagonal matrix and a [%d] vector",
               _rows, _cols, vec.size());
  }

  Vector resu(_rows);
  double *Res = resu._data;
  double *Mat = _data;
  double *Vec = vec._data;

  for (long i = 0; i < _rows; i++)
    *Res++ = *Mat++ * *Vec++;

  return resu;
}

/*
@LABEL:MatrixDiag::dot(Vector V)
@SHORT:Multiplication of a diagonal matrix by a vector.
@ARG:Vector & V & Vector to use for the multiplication operation.
@WARNING: The result of the operation is the parameter V itself.
This method defines the product of a diagonal matrix by a vector.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\overrightarrow{x}$ is a Vector defined by parameter V.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::dot(Vector &vec) const
//-----------------------------------------------------------------------------
{
  if (_cols != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               _rows, _cols, vec.size());
  }

  double *Vec = vec._data;
  double *Mat = _data;
  for (long i = _rows - 1; i >= 0; i--)
    *Vec++ *= *Mat++;
}

//-----------------------------------------------------------------------------
void MatrixDiag::divideBy(Vector &vec) const
//-----------------------------------------------------------------------------
{
  if (_cols != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               _rows, _cols, vec.size());
  }

  double *Vec = vec._data;
  double *Mat = _data;
  for (long i = _rows - 1; i >= 0; i--)
    *Vec++ /= *Mat++;
}

/*
@LABEL:MatrixDiag::trace()
@SHORT:Returns the trace of a diagonal matrix.
@RETURN:double : The trace of the diagonal matrix.
This method returns the trace of a diagonal matrix, i.e. the sum $s$ of all the terms of the diagonal:
\begin{equation*}
s = \tr[\M] = M_{11}+M_{22}+\hdots+M_{nn}
\end{equation*}
where $\M$ is a diagonal matrix defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
double MatrixDiag::trace()
//-----------------------------------------------------------------------------
{
  double trace = 0;
  for (long i = 0; i < _rows; i++)
    trace += _data[i];
  return trace;
}

/*
@LABEL:MatrixDiag::transpose()
@SHORT:Transpose of a diagonal matrix.
@RETURN:Matrix : The transpose of the diagonal matrix.
This method defines the transpose of a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::transpose()
//-----------------------------------------------------------------------------
{
  return *this;
}

/*
@LABEL:MatrixDiag::getSolve(Vector x)
@SHORT:Solves a linear system $\A\cdot \overrightarrow{x} = \overrightarrow{b}$.
@RETURN:Vector : The solution of the linear system.
This method returns the solution of a linear system with the following form:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\overrightarrow{x}$ is a vector defined by parameter x.
@END
*/
//-----------------------------------------------------------------------------
Vector MatrixDiag::getSolve(Vector &vec)
//-----------------------------------------------------------------------------
{
  Vector resu(_rows);

  if (_rows != vec.size())
  {
    fatalError("MatrixDiag::Solve",
               "diagonal matrix and vector sizes incompatible\n"
               "You're about to Solve a linear system with a [%d,%d]"
               " diagonal matrix and a [%d] vector",
               _rows, _cols, vec.size());
  }

  for (long component = 0; component < _rows; component++)
    resu(component) = vec(component) / _data[component];

  return resu;
}

/*
@LABEL:MatrixDiag::solve(Vector x)
@SHORT:Solves a linear system $\A\cdot \overrightarrow{x} = \overrightarrow{b}$.
@WARNING: The result of the operation is the parameter x itself.
This method returns the solution of a small linear system with the following form:
\begin{equation*}
\overrightarrow{y} = \A \cdot \overrightarrow{x}
\end{equation*}
where $\A$ is a diagonal matrix defined by the object itself and $\overrightarrow{x}$ is a vector defined by parameter x.
@END
*/
//-----------------------------------------------------------------------------
void MatrixDiag::solve(Vector &vec)
//-----------------------------------------------------------------------------
{
  for (long component = 0; component < _rows; component++)
    vec(component) /= _data[component];
}

/*
@LABEL:MatrixDiag::inverse()
@SHORT:Inverse of a diagonal matrix.
@RETURN:MatrixDiag : The inverse of the diagonal matrix.
This method returns the inverse of a diagonal matrix.
@END
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::inverse() const
//-----------------------------------------------------------------------------
{
  MatrixDiag res(_rows, _rows);

  double *Res = res._data;
  double *Loc = _data;

  for (long i = _rows; i > 0; i--)
    *Res++ = 1.0 / (*Loc++);

  return res;
}

//  Test the equality of two matrices
//-----------------------------------------------------------------------------
bool MatrixDiag::operator==(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  if ((_rows != mat._rows) && (_cols != mat._cols))
  {
    fatalError("MatrixDiag::operator ==",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
  }

  for (long i = 0; i < _rows; i++)
    if (_data[i] != mat._data[i])
      return false;
  return true;
}

//  Test the inequality of two matrices
//-----------------------------------------------------------------------------
bool MatrixDiag::operator!=(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  return !(*this == mat);
}

//  Writes a matrix in a binary flux for storage
//-----------------------------------------------------------------------------
void MatrixDiag::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&_rows, sizeof(long));
  ofs.write((char *)_data, _rows * sizeof(double));
}

//  Reads a matrix in a binary flux from storage
//-----------------------------------------------------------------------------
void MatrixDiag::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  long rows;

  ifs.read((char *)&rows, sizeof(long));
  redim(rows);
  ifs.read((char *)_data, _rows * sizeof(double));
}

//  Writes a matrix in a binary flux for storage
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.write(os);
  return os;
}

//  Reads a matrix from a binary flux for storage
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.read(is);
  return is;
}

//-----------------------------------------------------------------------------
long MatrixDiag::Memory() const
//-----------------------------------------------------------------------------
{
  return (0);
}

//  Saves the content of a Matrix into a NumPy file
//-----------------------------------------------------------------------------
void MatrixDiag::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {_rows}, mode);
}

//  Saves the content of a Matrix into a NumPyZ file
//-----------------------------------------------------------------------------
void MatrixDiag::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {_rows}, mode);
}

//  Read the content of a Matrix from a NumPy file
//-----------------------------------------------------------------------------
void MatrixDiag::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  redim(arr.num_vals);

  if (arr.num_vals != _rows)
  {
    std::cout << "ERROR\n";
  }

  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//  Read the content of a Matrix from a NumPyZ file
//-----------------------------------------------------------------------------
void MatrixDiag::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  redim(arr.num_vals);

  if (arr.num_vals != _rows)
  {
    std::cout << "ERROR\n";
  }

  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

// Assemblage d'un vecteur dans un autre
/*
  Cette methode effectue la phase d'assemblage d'ue vecteur dans un autre conformement à ce qui se fait classiquement en Elements Finis.

  - M Vecteur elementaire à assembler dans le grand vecteur
  - int tableau d'long correspondant au nombre de NOEUDS de l'element et contenant les Id des noeuds que l'on utilise
  - numberOfDimensions nombre de dimensions du probleme
  \code
  MatrixDiag Ve;
  long numberOfDimensions=...; // nombre de dimensions du probleme
  long *ind=new long[loop_I]; // allocation memoire pour ind
  for (long I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.gatherFrom(Ve, ind, numberOfDimensions);
  delete [] ind;
  \endcode

  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void MatrixDiag::gatherFrom(const MatrixDiag &M, long *ind0, int numberOfDimensions)
//-----------------------------------------------------------------------------
{
  long loop_I = M._rows / numberOfDimensions;
  long *ind = new long[loop_I];

  // preparation
  long *pind = ind;
  long *pind0 = ind0;
  for (long I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDimensions;

  // boucle principale
  switch (numberOfDimensions)
  {
  case 1:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      _data[ind[I]] += M._data[I];
    }
  }
  break;
  case 2:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *gl = &_data[ind[I]];
      double *lo = &M._data[I * numberOfDimensions];
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;
  case 3:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *gl = &_data[ind[I]];
      double *lo = &M._data[I * numberOfDimensions];
      *gl++ += *lo++;
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;

    // cas general
  default:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      // boucle sur les ddl
      long i = I * numberOfDimensions;
      long globI = ind[I];
      for (long Ii = 0; Ii < numberOfDimensions; Ii++)
        (*this)(globI + Ii) += M(i + Ii);
    }
  }
  }
  delete[] ind;
}

// Assemblage d'un vecteur dans un autre
/*
  Cette methode effectue la phase d'assemblage d'ue vecteur dans un autre conformement à ce qui se fait classiquement en Elements Finis.

  - M Vecteur elementaire à assembler dans le grand vecteur
  - int tableau d'long correspondant au nombre de NOEUDS de l'element et contenant les Id des noeuds que l'on utilise
  - numberOfDimensions nombre de dimensions du probleme
  \code
  MatrixDiag Ve;
  long numberOfDimensions=...; // nombre de dimensions du probleme
  long *ind=new long[loop_I]; // allocation memoire pour ind
  for (long I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.scatterFrom(Ve, ind, numberOfDimensions);
  delete [] ind;
  \endcode

  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void MatrixDiag::scatterFrom(const MatrixDiag &M, long *ind0, int numberOfDimensions)
//-----------------------------------------------------------------------------
{
  // long mr=_rows;
  long loop_I = _rows / numberOfDimensions;
  long *ind = new long[loop_I];

  // preparation
  long *pind = ind;
  long *pind0 = ind0;
  for (long I = loop_I; I > 0; I--)
    *pind++ = *pind0++ * numberOfDimensions;

  // boucle principale
  switch (numberOfDimensions)
  {
  case 1:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      _data[I] += M._data[ind[I]];
    }
  }
  break;
  case 2:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *lo = &M._data[ind[I]];
      double *gl = &_data[I * numberOfDimensions];
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;
  case 3:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *lo = &M._data[ind[I]];
      double *gl = &_data[I * numberOfDimensions];
      *gl++ += *lo++;
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;

    // cas general
  default:
  {
    // boucle sur les ddl
    for (long I = loop_I - 1; I >= 0; I--)
    {
      long i = I * numberOfDimensions;
      long globI = ind[I];
      for (long Ii = 0; Ii < numberOfDimensions; Ii++)
        (*this)(i + Ii) += M(globI + Ii);
    }
  }
  }
  delete[] ind;
}
