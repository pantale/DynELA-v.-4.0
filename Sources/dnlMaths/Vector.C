/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <Vector.h>
#include <Matrix.h>
#include <NumpyInterface.h>

/*
@LABEL:Vector::Vector(long l, double m)
@SHORT:Constructor of the Vector class with initialization.
@RETURN:Vector : The initialized vector.
@ARG:long & l & Length of the vector to create.
@ARG:double & m & Value to give to each element of the new vector.
This method creates a new vector of length $l$ where all values are initialized to the scalar value $m$.
\begin{equation*}
\overrightarrow{x}=\left[\begin{array}{c}
  x_{1}=m\\
  x_{2}=m\\
  \vdots\\
  x_{n}=m
  \end{array}\right]
\end{equation*}
where the $\overrightarrow{x}$ is the object itself and $m$ is scalar value defined by parameter m.
@END
*/
//-----------------------------------------------------------------------------
Vector::Vector(long len, double value)
//-----------------------------------------------------------------------------
{
  allocate(len);

  // filling the vector with the given value
  for (long i = 0; i < _dataLength; i++)
    _data[i] = value;
}

// Copy constructor
//-----------------------------------------------------------------------------
Vector::Vector(const Vector &vect)
//-----------------------------------------------------------------------------
{
  // memory allocation
  _dataLength = vect._dataLength;
  _data = new double[_dataLength];

  memcpy(_data, vect._data, _dataLength * sizeof(double));
}

/*
@LABEL:Vector::Vector(int l, double x1, double x2, ...)
@SHORT:Constructor of the vector class with initialization.
@RETURN:Vector : The initialized vector.
@ARG:int & l & Number of components.
@ARG:double & x1 & first component of the vector to create.
@ARG:double & x2 & second component of the vector to create.
@ARG:double & ... & $x^{th}$ component of the vector to create.
All components have to be defined in col order.
\begin{equation*}
\overrightarrow{x}=\left[\begin{array}{c}
  x_{1}=x1\\
  x_{2}=x2\\
  \vdots\\
  x_{n}=xn
  \end{array}\right]
\end{equation*}
@END
*/
//-----------------------------------------------------------------------------
Vector::Vector(int vectorLength, double firstValue, double secondValue, ...)
//-----------------------------------------------------------------------------
{
  // allocation dynamique de memoire
  _dataLength = vectorLength;
  _data = new double[vectorLength];

  // A place to store the list of arguments
  va_list arguments;

  // traitement de la premiere valeur
  _data[0] = firstValue;
  _data[1] = secondValue;

  // Store arguments values
  va_start(arguments, secondValue);

  for (int i = 2; i < vectorLength; i++)
  {
    _data[i] = va_arg(arguments, double);
  }

  // Cleans up the list
  va_end(arguments);
}

// Destructor
//-----------------------------------------------------------------------------
Vector::~Vector()
//-----------------------------------------------------------------------------
{
  desallocate();
}

// Memory allocation
//-----------------------------------------------------------------------------
void Vector::allocate(const long len)
//-----------------------------------------------------------------------------
{
  // memory allocation
  _dataLength = len;
  _data = new double[len];
}

// Memory deallocation
//-----------------------------------------------------------------------------
void Vector::desallocate()
//-----------------------------------------------------------------------------
{
  if (_dataLength >= 0)
    delete[] _data;

  _dataLength = 0;
}

/*
@LABEL:Vector::swap(Vector y)
@SHORT:Swap the content of two vectors.
@ARG:Vector & y & Second vector for the swap operation.
This method swaps the storage of two vectors.
The two vectors must have the exact same size.
@END
*/
//-----------------------------------------------------------------------------
void Vector::swap(Vector &V)
//-----------------------------------------------------------------------------
{
  double *tmp;

  // test the vector sizes
#ifdef VERIF_maths
  if (V._dataLength != _dataLength)
    fatalError("Vector::swap", "Only for same size vectors\n");
#endif

  // swap storages
  tmp = _data;
  _data = V._data;
  V._data = tmp;
}

/*
@LABEL:Vector::redim(long s)
@SHORT:Change the allocation size of a vector.
@ARG:long & s & New allocation size of the vector after the operation.
@WARNING:This method cleans the content of the vector.
This method changes the size of a vector.
If the new size is the same as the actual size, this method does nothing.
@END
*/
//-----------------------------------------------------------------------------
void Vector::redim(const long newSize)
//-----------------------------------------------------------------------------
{
  if (newSize == _dataLength)
    return;

  desallocate();
  allocate(newSize);
}

/*
@LABEL:Vector::resize(long s)
@SHORT:Change the size of a vector.
@ARG:long & s & New size of the vector after the operation.
This method is used to specify a new vector dimension of the one given during initialization by the constructor.
This method makes a copy of the previous vector according to the new size, \ie it can shrink or expand the vector.
If the new vector size is greater than the previous one, zeros are added at the end of the vector.
@END
*/
//-----------------------------------------------------------------------------
void Vector::resize(const long vLength)
//-----------------------------------------------------------------------------
{
  double *v2;

  if (_data == NULL)
  {
    // new memory allocation
    _data = new double[vLength];

    // fill with default a default value of zero
    for (long i = 0; i < _dataLength; i++)
      _data[i] = 0;
  }

  else
  {
    // new memory allocation
    v2 = new double[vLength];

    // Copy the old vector into the new one
    memcpy(v2, _data, vLength * sizeof(double));

    // Fill the rest with zeros
    if (vLength > _dataLength)
    {
      // initialisation
      for (long i = _dataLength; i < vLength; i++)
        v2[i] = 0;
    }

    // Delete the old part
    delete[] _data;

    // Switch to the new part
    _data = v2;
  }

  // set new size
  _dataLength = vLength;
}

// Send the content of a vector to the output flux for display
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Vector &vector)
//-----------------------------------------------------------------------------
{
  vector.print(os);
  return os;
}

// Print the content of a vector to the output flux for display
//-----------------------------------------------------------------------------
void Vector::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i;

  os << "vector n=" << _dataLength << " coords={";
  if (_dataLength != 0)
    os << _data[0];
  for (i = 1; i < _dataLength; i++)
    os << "," << _data[i];
  os << "}";
}

/*
@LABEL:Vector::printOut()
@SHORT:Displays the content of the vector on stdout.
This method displays the content of the vector on the screen in a predefined format.
The selection of the display format is made using the method setOutType().
The output type is defined by the variables listed in OutVector.
@END
*/
//-----------------------------------------------------------------------------
void Vector::printOut()
//-----------------------------------------------------------------------------
{
  switch (outType)
  {
  case outVectorTypeNormal:
  {
    long i;
    std::cout << "vector " << _dataLength << " coords={";
    if (_dataLength != 0)
      std::cout << _data[0];
    for (i = 1; i < _dataLength; i++)
      std::cout << "," << _data[i];
    std::cout << "}\n";
    std::flush(std::cout);
  }
  break;
  case outVectorTypeMupad:
  {
    long i;
    std::cout << "matrix([[";
    if (_dataLength != 0)
      std::cout << _data[0];
    for (i = 1; i < _dataLength; i++)
      std::cout << "],[" << _data[i];
    std::cout << "]])\n";
  }
  break;
  default:
    fatalError("Vector::printOut()", "Unknown type %d\n", outType);
  }
}

/*
@LABEL:Vector::setValue(double v)
@SHORT:Fill a vector with a scalar value.
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the vector class.
\begin{equation*}
\overrightarrow{x}=\left[\begin{array}{c}
  x_{1}=v\\
  x_{2}=v\\
  \vdots\\
  x_{n}=v
  \end{array}\right]
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $v$ is the scalar value defined by parameter v.
@END
*/
//-----------------------------------------------------------------------------
void Vector::setValue(double val)
//-----------------------------------------------------------------------------
{
  for (long index = 0; index < _dataLength; index++)
    _data[index] = val;
}

/*
@LABEL:Vector::operator=(double v)
@SHORT:Fill a vector with a scalar value.
@RETURN:Vector
@ARG:double & v & Value to use for the operation.
This method is a surdefinition of the = operator for the vector class.
\begin{equation*}
\overrightarrow{x}=\left[\begin{array}{c}
  x_{1}=v\\
  x_{2}=v\\
  \vdots\\
  x_{n}=v
  \end{array}\right]
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $v$ is the scalar value defined by parameter v.
@END
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(double val)
//-----------------------------------------------------------------------------
{
  setValue(val);
  return *this;
}

// Fill a vector with a table of values
//-----------------------------------------------------------------------------
Vector &Vector::operator=(const double *vals)
//-----------------------------------------------------------------------------
{
  memcpy(_data, vals, _dataLength * sizeof(double));
  return *this;
}

// Copy the content of a vector into a new one
//-----------------------------------------------------------------------------
Vector &Vector::operator=(const Vector &vect)
//-----------------------------------------------------------------------------
{
  // redimension du vecteur resultat
  if (vect._dataLength != _dataLength)
  {
    delete[] _data;
    _dataLength = vect._dataLength;
    _data = new double[_dataLength];
  }

  // recopie des valeurs
  memcpy(_data, vect._data, _dataLength * sizeof(double));

  return *this;
}

/*
@LABEL:Vector::operator+(Vector y)
@SHORT:Addition of 2 vectors.
@ARG:Vector & y & Vector to add to the current one.
@RETURN:Vector : Result of the addition operation.
This method defines the addition of 2 vectors.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{z}=\overrightarrow{x}+\overrightarrow{y}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\overrightarrow{y}$ is the second vector defined by parameter y.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::operator+(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  Vector resu(_dataLength);

#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::operator +",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  // addition
  for (long i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] + vect._data[i];
  return resu;
}

/*
@LABEL:Vector::operator+=(Vector y)
@SHORT:Addition of 2 vectors.
@SMOD
@ARG:Vector & y & Vector to add to the current one.
This method defines the addition of 2 vectors.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{x}+=\overrightarrow{y}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\overrightarrow{y}$ is the second vector defined by parameter y.
@END
*/
//-----------------------------------------------------------------------------
void Vector::operator+=(const Vector &vect)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::operator +=",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  // addition
  for (long i = 0; i < _dataLength; i++)
    _data[i] += vect._data[i];
}

/*
@LABEL:Vector::operator-(Vector y)
@SHORT:Difference of 2 vectors.
@ARG:Vector & y & Vector to subtract to the current one.
@RETURN:Vector : Result of the difference operation.
This method defines the difference of 2 vectors.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{z}=\overrightarrow{x}-\overrightarrow{y}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\overrightarrow{y}$ is the second vector defined by parameter y.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::operator-(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  Vector resu(_dataLength);

#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::operator -",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  for (long i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] - vect._data[i];
  return resu;
}

/*
@LABEL:Vector::operator-=(Vector y)
@SHORT:Difference of 2 vectors.
@SMOD
@ARG:Vector & y & Vector to subtract to the current one.
This method defines the difference of 2 vectors.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{x}-=\overrightarrow{y}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\overrightarrow{y}$ is the second vector defined by parameter y.
@END
*/
//-----------------------------------------------------------------------------
void Vector::operator-=(const Vector &vect)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::operator -=",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  for (long i = 0; i < _dataLength; i++)
    _data[i] -= vect._data[i];
}

/*
@LABEL:Vector::operator-()
@SHORT:Opposite of a vector.
@RETURN:Vector : The opposite vector.
This method defines the opposite of a vector.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = - \overrightarrow{x}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::operator-() const
//-----------------------------------------------------------------------------
{
  Vector resu(_dataLength);

  for (long i = 0; i < _dataLength; i++)
    resu._data[i] = -_data[i];
  return resu;
}

/*
@LABEL:Vector::operator*(double l)
@SHORT:Multiplication of a vector by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Vector : Result of the multiplication operation.
This method defines the multiplication of a vector by a scalar value.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \lambda \overrightarrow{x}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::operator*(const double lambda) const
//-----------------------------------------------------------------------------
{
  long i;
  Vector resu(_dataLength);

  for (i = 0; i < _dataLength; i++)
    resu._data[i] = lambda * _data[i];
  return resu;
}

/*
@LABEL:Vector::operator*=(double l)
@SHORT:Multiplication of a vector by a scalar.
@SMOD
@ARG:double & l & Scalar value to use for the operation.
This method defines the multiplication of a vector by a scalar value.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{x} *= \lambda
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
void Vector::operator*=(const double lambda)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _dataLength; i++)
    _data[i] *= lambda;
}

/*
@LABEL:Vector::operator/(double l)
@SHORT:Division of a vector by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@RETURN:Vector : Result of the division operation.
This method defines the division of a vector by a scalar value.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} = \frac{1}{\lambda} \overrightarrow{x}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::operator/(const double lambda) const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_maths
  if (lambda == 0.0)
  {
    fatalError("Vector:: operator /", "divide by zero");
  }
#endif

  Vector resu(_dataLength);
  double unSurLambda = 1.0 / lambda;

  for (i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] * unSurLambda;
  return resu;
}

/*
@LABEL:Vector::operator/=(double l)
@SHORT:Division of a vector by a scalar.
@ARG:double & l & Scalar value to use for the operation.
@SMOD
This method defines the division of a vector by a scalar value.
The result of this operation is also a vector defined by:
\begin{equation*}
\overrightarrow{y} /= \lambda
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself and $\lambda$ is the scalar value defined by parameter l.
@END
*/
//-----------------------------------------------------------------------------
void Vector::operator/=(const double lambda)
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_maths
  if (lambda == 0.0)
  {
    fatalError("Vector:: operator /=", "divide by zero");
  }
#endif

  double unSurLambda = 1.0 / lambda;
  for (i = 0; i < _dataLength; i++)
    _data[i] *= unSurLambda;
}

// Multiplication of a vector by a scalar value
//-----------------------------------------------------------------------------
Vector operator*(const double lambda, const Vector &vect)
//-----------------------------------------------------------------------------
{
  long i;
  Vector resu(vect._dataLength);

  for (i = 0; i < vect._dataLength; i++)
    resu._data[i] = lambda * vect._data[i];
  return resu;
}

/*
@LABEL:Vector::norm()
@SHORT:Returns the norm of a vector.
@RETURN:double : The norm of the vector.
This method returns the norm $s$ of a vector $\overrightarrow{x}$ defined by:
\begin{equation*}
s = \left\Vert \overrightarrow{x} \right\Vert = \sqrt {x_{1}^2 + x_{2}^2 + ... + x_{n}^2}
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
double Vector::norm()
//-----------------------------------------------------------------------------
{
  long i;
  double norm = 0.;

  for (i = 0; i < _dataLength; i++)
    norm += dnlSquare(_data[i]);
  return sqrt(norm);
}

/*
@LABEL:Vector::dot()
@SHORT:Returns the dot product of a vector by itself.
@RETURN:double : The dot product of the vector by itsefl.
This method returns dot product of a vector by itself defined by:
\begin{equation*}
 \left\Vert \overrightarrow{x} \right\Vert  = x_{1}^2 + x_{2}^2 + ... + x_{n}^2
\end{equation*}
where $\overrightarrow{x}$ is a vector defined by the object itself.
@END
*/
//-----------------------------------------------------------------------------
double Vector::dot()
//-----------------------------------------------------------------------------
{
  long i;
  double norm = 0.;

  for (i = 0; i < _dataLength; i++)
    norm += dnlSquare(_data[i]);
  return norm;
}

/*
@LABEL:Vector::dyadic()
@SHORT:Dyadic product of a vector by itsefl.
@RETURN:Matrix : The matrix result of the dyadic product.
This method returns the dyadic product of two vectors defined by the following equation:
\begin{equation*}
\M = \overrightarrow{x}\otimes\overrightarrow{x},
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
The result of this operation is a symmetric matrix.
@END
*/
//-----------------------------------------------------------------------------
Matrix Vector::dyadic()
//-----------------------------------------------------------------------------
{
  Matrix result(_dataLength, _dataLength);

  for (long i = 0; i < _dataLength; i++)
    for (long j = 0; j < _dataLength; j++)
      result._data[Ind(i, j, result._rows, result._cols)] = _data[i] * _data[j];

  return result;
}

/*
@LABEL:Vector::dyadic(Vector y)
@SHORT:Dyadic product of two Vector.
@RETURN:Tensor2
@ARG:Vector&y&Vector $\overrightarrow{y}$ to use for the dyadic product operation.
This method returns the dyadic product of two Vector defined by the following equation:
\begin{equation*}
\M = \overrightarrow{x}\otimes\overrightarrow{y},
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Matrix Vector::dyadic(const Vector &vec)
//-----------------------------------------------------------------------------
{
  Matrix result(_dataLength, vec._dataLength);

  for (long i = 0; i < _dataLength; i++)
    for (long j = 0; j < vec._dataLength; j++)
      result._data[Ind(i, j, result._rows, result._cols)] = _data[i] * vec._data[j];

  return result;
}

/*
@LABEL:Vector::normalize()
@SHORT:Normalize the Vector.
@SMOD
This method modifies the given vector and makes its norm equal to $1$.
\begin{equation*}
\overrightarrow{x} = \frac{\overrightarrow{x}}{\sqrt {x_{1}^2 + x_{2}^2 + ... + x_{n}^2}}
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
void Vector::normalize()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double _norm = norm();

  // recalcul des composantes
  for (long i = 0; i < _dataLength; i++)
    _data[i] /= _norm;
}

/*
@LABEL:Vector::getNormalized()
@SHORT:Normalize the Vector.
@RETURN: Vector : The result of the nomalization.
This method scales a vector and makes its norm equal to $1$.
\begin{equation*}
\overrightarrow{y} = \frac{\overrightarrow{x}}{\sqrt {x_{1}^2 + x_{2}^2 + ... + x_{n}^2}}
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::getNormalized()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double _norm = norm();

  Vector vect(*this);

  // recalcul des composantes
  for (long i = 0; i < _dataLength; i++)
    vect._data[i] /= _norm;

  return vect;
}

/*
@LABEL:Vector::maxVal()
@SHORT:Maximum value in a vector.
@RETURN:double : The maximum component of the vector.
This method returns the maximum value in a vector.
@END
*/
//-----------------------------------------------------------------------------
double Vector::maxVal()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _dataLength; i++)
    if (_data[i] > val)
      val = _data[i];

  return val;
}

/*
@LABEL:Vector::minVal()
@SHORT:Minumum value in a vector.
@RETURN:double : The minimum component of the vector.
This method returns the minimum value in a vector.
@END
*/
//-----------------------------------------------------------------------------
double Vector::minVal()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _dataLength; i++)
    if (_data[i] < val)
      val = _data[i];

  return val;
}

/*
@LABEL:Vector::maxAbs()
@SHORT:Maximum absolute value in a vector.
@RETURN:double : The maximum component of the vector.
This method returns the maximum absolute value in a vector.
@END
*/
//-----------------------------------------------------------------------------
double Vector::maxAbs()
//-----------------------------------------------------------------------------
{
  double val = dnlAbs(_data[0]);

  for (long i = 1; i < _dataLength; i++)
    if (dnlAbs(_data[i]) > val)
      val = dnlAbs(_data[i]);

  return val;
}

/*
@LABEL:Vector::minAbs()
@SHORT:Minumum absolute value in a vector.
@RETURN:double : The minimum component of the vector.
This method returns the minimum absolute value in a vector.
@END
*/
//-----------------------------------------------------------------------------
double Vector::minAbs()
//-----------------------------------------------------------------------------
{
  double val = dnlAbs(_data[0]);

  for (long i = 1; i < _dataLength; i++)
    if (dnlAbs(_data[i]) < val)
      val = dnlAbs(_data[i]);

  return val;
}

// Assemblage d'un vecteur dans un autre
/*
  Cette methode effectue la phase d'assemblage d'ue vecteur dans un autre conformement à ce qui se fait classiquement en Elements Finis.

  - V Vecteur elementaire à assembler dans le grand vecteur
  - int tableau d'long correspondant au nombre de NOEUDS de l'element et contenant les Id des noeuds que l'on utilise
  - numberOfDimensions nombre de dimensions du probleme
  \code
  Vector Ve;
  long numberOfDimensions=...; // nombre de dimensions du probleme
  long *ind=new long[loop_I]; // allocation memoire pour ind
  for (long I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.gatherFrom(Ve, ind, numberOfDimensions);
  delete [] ind;
  \endcode

  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Vector::gatherFrom(const Vector &V, long *ind0, int numberOfDimensions)
//-----------------------------------------------------------------------------
{
  long loop_I = V._dataLength / numberOfDimensions;
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
      _data[ind[I]] += V._data[I];
    }
  }
  break;
  case 2:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *gl = &_data[ind[I]];
      double *lo = &V._data[I * 2];
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
      double *lo = &V._data[I * 3];
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
        (*this)(globI + Ii) += V(i + Ii);
    }
  }
  }
  delete[] ind;
}

// Assemblage d'un vecteur dans un autre
/*
  Cette methode effectue la phase d'assemblage d'ue vecteur dans un autre conformement à ce qui se fait classiquement en Elements Finis.

  - V Vecteur elementaire à assembler dans le grand vecteur
  - int tableau d'long correspondant au nombre de NOEUDS de l'element et contenant les Id des noeuds que l'on utilise
  - numberOfDimensions nombre de dimensions du probleme
  \code
  Vector Ve;
  long numberOfDimensions=...; // nombre de dimensions du probleme
  long *ind=new long[loop_I]; // allocation memoire pour ind
  for (long I=0; I < loop_I; I++) ind[I]=pel->nodes (I)->Id; // affectation des indices
  K.scatterFrom(Ve, ind, numberOfDimensions);
  delete [] ind;
  \endcode

  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Vector::scatterFrom(const Vector &V, long *ind0, int numberOfDimensions)
//-----------------------------------------------------------------------------
{
  // long mr=_dataLength;
  long loop_I = _dataLength / numberOfDimensions;
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
      _data[I] += V._data[ind[I]];
    }
  }
  break;
  case 2:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *lo = &V._data[ind[I]];
      double *gl = &_data[I * 2];
      *gl++ += *lo++;
      *gl += *lo;
    }
  }
  break;
  case 3:
  {
    for (long I = loop_I - 1; I >= 0; I--)
    {
      double *lo = &V._data[ind[I]];
      double *gl = &_data[I * 3];
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
        (*this)(i + Ii) += V(globI + Ii);
    }
  }
  }
  delete[] ind;
}

// Test the equality of two vectors
//-----------------------------------------------------------------------------
bool Vector::operator==(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::operator ==",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  for (long i = 0; i < _dataLength; i++)
    if (_data[i] != vect._data[i])
      return false;
  return true;
}

// Test the inequality of two vectors
//-----------------------------------------------------------------------------
bool Vector::operator!=(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  return !(*this == vec);
}

/*
@LABEL:Vector::distance(Vector y)
@SHORT:Distance between two points.
@RETURN:double : The distance.
This method computes the distance between two points using an Euclidian norm.
\begin{equation*}
d = \left\Vert \overrightarrow{y} - \overrightarrow{x} \right\Vert
\end{equation*}
where the $\overrightarrow{x}$ is the object itself and $\overrightarrow{y}$ is the second vector of the operation.
@END
*/
//-----------------------------------------------------------------------------
double Vector::distance(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::distance",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  Vector x = vect - *this;
  return x.norm();
}

/*
@LABEL:Vector::squareDistance(Vector y)
@SHORT:Square of distance between two points.
@RETURN:double : The distance.
This method computes the square of the distance between two points using an Euclidian norm.
\begin{equation*}
d = {\left\Vert \overrightarrow{y} - \overrightarrow{x} \right\Vert}^2
\end{equation*}
where the $\overrightarrow{x}$ is the object itself and $\overrightarrow{y}$ is the second vector of the operation.
@END
*/
//-----------------------------------------------------------------------------
double Vector::squareDistance(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::squareDistance",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  Vector x = vect - *this;
  return x.dot();
}

/*
@LABEL:Vector::dot(Vector y)
@SHORT:Dot product of two vectors.
@RETURN:double : The dot product.
@ARG:Vector & y & Vector $\overrightarrow{y}$ to use for the dot product operation.
This method returns the dot product of two vectors defined by the following equation:
\begin{equation*}
m = \overrightarrow{x}\cdot\overrightarrow{y},
\end{equation*}
where the $\overrightarrow{x}$ is the object itself and $\overrightarrow{y}$ is given by the parameter y.
@END
*/
//-----------------------------------------------------------------------------
double Vector::dot(const Vector &V) const
//-----------------------------------------------------------------------------
{
  double prod = 0.0;

#ifdef VERIF_maths
  if (_dataLength != V._dataLength)
  {
    fatalErrorLine("Vector::dot",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   V._dataLength);
  }
#endif

  for (long i = 0; i < _dataLength; i++)
    prod += _data[i] * V._data[i];
  return prod;
}

/*
@LABEL:Vector::vectorProduct(Vector y)
@SHORT:Vector product of two vectors.
@RETURN:Vector : Result of the operation.
@WARNING:This method only works for two vectors with 3 components.
@ARG:Vector & y & Vector $\overrightarrow{y}$ to use for the vector product operation.
This method returns the vector product of two Vector defined by the following equation:
\begin{equation*}
\overrightarrow{w} = \overrightarrow{x}\land\overrightarrow{y},
\end{equation*}
where the $\overrightarrow{x}$ is the object itself and $\overrightarrow{y}$ is given by the parameter y.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::vectorProduct(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != 3)
  {
    fatalErrorLine("Vector::vectorProduct",
                   "size of v1(%d) not allowed for this operation", _dataLength);
  }
  if (vect._dataLength != 3)
  {
    fatalErrorLine("Vector::vectorProduct",
                   "size of v2(%d) not allowed for this operation", vect._dataLength);
  }
#endif

  Vector resu(3);

  resu._data[0] = _data[1] * vect._data[2] - _data[2] * vect._data[1];
  resu._data[1] = _data[2] * vect._data[0] - _data[0] * vect._data[2];
  resu._data[2] = _data[0] * vect._data[1] - _data[1] * vect._data[0];

  return resu;
}

/*
@LABEL:Vector::ewInverse()
@SHORT:Element-wise inverse of a vector
@RETURN:Vector : Result of the operation.
This method returns a vector containing the inverse of all elements of the vector.
\begin{equation*}
y_i = 1/x_i,
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/

//-----------------------------------------------------------------------------
Vector Vector::ewInverse()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = 1 / _data[i];
  return result;
}

/*
@LABEL:Vector::ewExp()
@SHORT:Element-wise exponential of a vector
@RETURN:Vector : Result of the operation.
This method returns a vector containing the exponential of all elements of the vector.
\begin{equation*}
y_i = \exp(x_i),
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::ewExp()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = exp(_data[i]);
  return result;
}

/*
@LABEL:Vector::ewProduct(Vector y)
@SHORT:Element-wise product of two vectors.
@RETURN:Vector : Result of the operation.
@ARG:Vector&y&Vector $\overrightarrow{y}$ to use for the element-wise product operation.
This method returns the vector product of two Vector defined by the following equation:
\begin{equation*}
w_i = x_i y_i,
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::ewProduct(const Vector &v2)
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = _data[i] * v2._data[i];
  return result;
}

/*
@LABEL:Vector::ewSquare()
@SHORT:Element-wise square of a vector
@RETURN:Vector : Result of the operation.
This method returns a vector containing the square of all elements of the vector.
\begin{equation*}
y_i = (x_i)^2,
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::ewSquare()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = _data[i] * _data[i];
  return result;
}

/*
@LABEL:Vector::ewSquare(double m)
@SHORT:Element-wise addition of a real to a vector
@ARG:double&m&Value to add to all components of the vector
@RETURN:Vector : Result of the operation.
This method returns a vector containing all elements of the given vector plus a quantity.
\begin{equation*}
y_i = x_i + m,
\end{equation*}
where the $\overrightarrow{x}$ is the object itself.
@END
*/
//-----------------------------------------------------------------------------
Vector Vector::ewAddReal(const double val)
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = val + _data[i];
  return result;
}

// Writes a vector in a binary flux for storage
//-----------------------------------------------------------------------------
void Vector::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&_dataLength, sizeof(long));
  ofs.write((char *)_data, _dataLength * sizeof(double));
}

// Reads a vector from a binary flux for storage
//-----------------------------------------------------------------------------
Vector &Vector::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  long new_vectorLength;

  ifs.read((char *)&new_vectorLength, sizeof(long));
  redim(new_vectorLength);
  ifs.read((char *)_data, _dataLength * sizeof(double));
  return *this;
}

// This method is used to store the components of a vector in a binary file.
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.write(os);
  return os;
}

// Reads a vector from a binary flux for storage
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.read(is);
  return is;
}

// Saves the content of a vector into a NumPy file
//-----------------------------------------------------------------------------
void Vector::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {_dataLength}, mode);
}

// Saves the content of a vector into a NumPyZ file
//-----------------------------------------------------------------------------
void Vector::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {_dataLength}, mode);
}

// Read the content of a vector from a NumPy file
//-----------------------------------------------------------------------------
void Vector::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  redim(arr.num_vals);

  if (arr.num_vals != _dataLength)
  {
    std::cout << "ERROR\n";
  }

  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

// Read the content of a vector from a NumPyZ file
//-----------------------------------------------------------------------------
void Vector::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  redim(arr.num_vals);

  if (arr.num_vals != _dataLength)
  {
    std::cout << "ERROR\n";
  }

  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
