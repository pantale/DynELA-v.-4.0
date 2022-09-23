/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file Vector.C
  Declaration file for the Vector class

  This file is the declaration file for the Vector class. A Vector class is a vector with the following form:
  \f[ \overrightarrow{_data}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  ...\\
  v_{n}
  \end{array}\right] \f]

  \ingroup dnlMaths
*/

#include <Vector.h>
#include <Matrix.h>
#include <NumpyInterface.h>

// Constructor of the Vector class
/*
  This method is a constructor of the Vector class with a null size.
*/
/* //-----------------------------------------------------------------------------
Vector::Vector()
//-----------------------------------------------------------------------------
{
  _dataLength = 0;
  _data = NULL;
}
 */
// Constructor of the Vector class with memory allocation
/*
  This method is a constructor of the Vector class with a given size. If the initial value is not provided, the initial value of all components of the vector is set to zero.
  - len Number of components of the vector
  - value Initial value for the components of the vector
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

// Copy constructor of the Vector class
//-----------------------------------------------------------------------------
Vector::Vector(const Vector &vect)
//-----------------------------------------------------------------------------
{
  // memory allocation
  _dataLength = vect._dataLength;
  _data = new double[_dataLength];

  memcpy(_data, vect._data, _dataLength * sizeof(double));
}

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

// Destructor of the Vector class
//-----------------------------------------------------------------------------
Vector::~Vector()
//-----------------------------------------------------------------------------
{
  desallocate();
}

//-----------------------------------------------------------------------------
void Vector::allocate(const long len)
//-----------------------------------------------------------------------------
{
  // memory allocation
  _dataLength = len;
  _data = new double[len];
}

//-----------------------------------------------------------------------------
void Vector::desallocate()
//-----------------------------------------------------------------------------
{
  if (_dataLength >= 0)
    delete[] _data;

  _dataLength = 0;
}

// Swap two vectors of the same size
/*
  This method swaps the storage of two vectors. The twio vectors must have the exact same size.
*/
//-----------------------------------------------------------------------------
void Vector::swapWith(Vector &vec)
//-----------------------------------------------------------------------------
{
  double *tmp;

  // test the vector sizes
#ifdef VERIF_maths
  if (vec._dataLength != _dataLength)
    fatalError("Vector::swapWith", "Only for same size vectors\n");
#endif

  // swap storages
  tmp = _data;
  _data = vec._data;
  vec._data = tmp;
}

//-----------------------------------------------------------------------------
void Vector::redim(const long newSize)
//-----------------------------------------------------------------------------
{
  if (newSize == _dataLength)
    return;

  desallocate();
  allocate(newSize);
}

// Resize a vector
/*
  This method is used to specify a new vector dimension of the one given during initialization by the constructor.
  This method makes a copy of the prevoius vector according to the new size, i.e. it can shrink or expand the vector.
  If the new vector size is greater than the previous one, zeros are added at the end of the vector.
  - new_vectorLength new size off the vector
*/
//-----------------------------------------------------------------------------
void Vector::resizeVector(const long new_vectorLength)
//-----------------------------------------------------------------------------
{
  double *v2;

  if (_data == NULL)
  {
    // new memory allocation
    _data = new double[new_vectorLength];

    // fill with default a default value of zero
    for (long i = 0; i < _dataLength; i++)
      _data[i] = 0.0;
  }

  else
  {
    // new memory allocation
    v2 = new double[new_vectorLength];
    memcpy(v2, _data, new_vectorLength * sizeof(double));

    if (new_vectorLength > _dataLength)
    {
      // initialisation
      for (long i = _dataLength; i < new_vectorLength; i++)
        v2[i] = 0.;
    }
    delete[] _data;
    _data = v2;
  }

  // set new size
  _dataLength = new_vectorLength;
}

// Display the content of a Vector
/*
  This method is a surdefintion of the << operator

  Example
  \code
  Vector t;
  cout << t << endl;
  \endcode
  - os Output flux
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Vector &vector)
//-----------------------------------------------------------------------------
{
  vector.print(os);
  return os;
}

// Display the content of a Vector
/*
  This method is a surdefintion of the << operator

  Example
  \code
  Vector t;
  t.print(os);
  \endcode
  - os Output flux
*/
//-----------------------------------------------------------------------------
void Vector::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i;

  os << "vector n=" << _dataLength << " coordinates={";
  if (_dataLength != 0)
    os << _data[0];
  for (i = 1; i < _dataLength; i++)
    os << "," << _data[i];
  os << "}";
}

// Displays the content of the vector on stdout
/*
  This method displays the content of the vector on the screen in a predefined format. The selection of the display format is made using the method \ref setOutType().
 The output type is defined by the variables listed in \ref OutVector.
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
    std::cout << "vector " << _dataLength << " coordinates={";
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

// Fill a vector with a scalar value
/*
  This method affect a value to a vector class.

  Example :
  \code
  Vector v1;
  v1 = setToValue(1.0); // All components of the vector are set to 1.0
  \endcode
  - val double value to give to all components of the vector.
*/
//-----------------------------------------------------------------------------
void Vector::setToValue(double val)
//-----------------------------------------------------------------------------
{
  for (long index = 0; index < _dataLength; index++)
    _data[index] = val;
}

// Fill a vector with a scalar value
/*
  This method is a surdefinition of the = operator for the vector class.

  Example :
  \code
  Vector v1;
  v1 = 1.0; // All components of the vector are set to 1.0
  \endcode
  - val double value to give to all components of the vector
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(double val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

// Fill a vector with a table of values
/*
  This method is a surdefinition of the equality operator used to setToValue a vector with a table of values.

  Exemple :
  \code
  Vector t1(10);
  double t2[10];
  t1 = t2; // copy values
  \endcode
*/
//-----------------------------------------------------------------------------
Vector &Vector::operator=(const double *vals)
//-----------------------------------------------------------------------------
{
  memcpy(_data, vals, _dataLength * sizeof(double));
  return *this;
}

// Copy the content of a vector into a new one
/*
  This method is the so called = operator between two vectors.

  Example :
  \code
  Vector v1, v2;
  v1 = v2; // copy of vector
  \endcode
  - vect Second vector to use for the operation
*/
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

// Addition of 2 vectors
/*
  This method defines the addition of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} + \overrightarrow{b} \f]

  Example :
  \code
  Vector t1, t2, t3;
  t3 = t1 + t2; // sum of the vectors
  \endcode
  - vect Second vector to use for the operation
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

// Addition of 2 vectors
/*
  This method defines the addition of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{a} = \overrightarrow{a} + \overrightarrow{b} \f]

  Example :
  \code
  Vector t1, t2;
  t2 += t1 ; // sum of the vectors
  \endcode
  - vect Second vector to use for the operation
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

// Difference of 2 vectors
/*
  This method defines the difference of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} - \overrightarrow{b} \f]

  Example :
  \code
  Vector t1, t2, t3;
  t3 = t1 - t2; // difference of the vectors
  \endcode
  - vect Second vector to use for the operation
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

// Difference of 2 vectors
/*
  This method defines the difference of 2 vectors.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{a} = \overrightarrow{a} - \overrightarrow{b} \f]

  Example :
  \code
  Vector t1, t2;
  t2 -= t1; // difference of the vectors
  \endcode
  - vect Second vector to use for the operation
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

// Opposite value of a vector
/*
  This method defines the opposite of a vector.
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = - \overrightarrow{a} \f]

  Example :
  \code
  Vec3D t1, t2;
  t2 = - t1; // opposite of the vector t1
  \endcode
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

// Multiplication of a vector by a scalar value
/*
  This method defines the multiplication of a vector by a scalar value

  Example :
  \code
  Vector t1, t2;
  double l;
  t2 = t1 * l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
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

// Multiplication of a vector by a scalar value
/*
  This method defines the multiplication of a vector by a scalar value

  Example :
  \code
  Vector t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
void Vector::operator*=(const double lambda)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _dataLength; i++)
    _data[i] *= lambda;
}

// Division of a vector by a scalar value
/*
  This method defines the division of a vector by a scalar value

  Example :
  \code
  Tensor2 t1, t2;
  double l;
  t2 = t1 / l; // division by a scalar
  \endcode
  - lambda Scalar value to use for the division
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

// Division of a vector by a scalar value
/*
  This method defines the division of a vector by a scalar value

  Example :
  \code
  Tensor2 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  - lambda Scalar value to use for the division
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
/*
  This method defines the multiplication of a vector by a scalar value

  Example :
  \code
  Vector t1, t2;
  double l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
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

// Returns the norm of a vector
/*
  This method returns norm of a vector defined by:
  \f[ \left\Vert \overrightarrow{_data} \right\Vert = \sqrt {v_{1}^2 + v_{2}^2 + ... + v_{n}^2} \f]
  Return : norm of a vector
*/
//-----------------------------------------------------------------------------
double Vector::getNorm()
//-----------------------------------------------------------------------------
{
  long i;
  double getNorm = 0.;

  for (i = 0; i < _dataLength; i++)
    getNorm += dnlSquare(_data[i]);
  return sqrt(getNorm);
}

// Returns the inner product of a vector by itself
/*
  This method returns inner product of a vector by itself defined by:
  \f[ \left\Vert \overrightarrow{_data} \right\Vert  = v_{1}^2 + v_{2}^2 + ... + v_{n}^2 \f]
  Return : inner product of a vector by itself
*/
//-----------------------------------------------------------------------------
double Vector::innerProduct()
//-----------------------------------------------------------------------------
{
  long i;
  double getNorm = 0.;

  for (i = 0; i < _dataLength; i++)
    getNorm += dnlSquare(_data[i]);
  return getNorm;
}

//-----------------------------------------------------------------------------
Matrix Vector::dyadicProduct()
//-----------------------------------------------------------------------------
{
  Matrix result(_dataLength, _dataLength);

  for (long i = 0; i < _dataLength; i++)
    for (long j = 0; j < _dataLength; j++)
      result._data[Ind(i, j, result._rows, result._cols)] = _data[i] * _data[j];

  return result;
}

//-----------------------------------------------------------------------------
Matrix Vector::dyadicProduct(const Vector &vec)
//-----------------------------------------------------------------------------
{
  Matrix result(_dataLength, vec._dataLength);

  for (long i = 0; i < _dataLength; i++)
    for (long j = 0; j < vec._dataLength; j++)
      result._data[Ind(i, j, result._rows, result._cols)] = _data[i] * vec._data[j];

  return result;
}

// Normalization of a 3D vector
/*
  This method modifies the given vector and makes its norm equal to 1.0
*/
//-----------------------------------------------------------------------------
void Vector::normalize()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double norm = getNorm();

  // recalcul des composantes
  for (long i = 0; i < _dataLength; i++)
    _data[i] /= norm;
}

// Nomalized vector
/*
  This method returns an colinear vector with a unary norm.
  Return : colinear vector with a norm equal to 1.
*/
//-----------------------------------------------------------------------------
Vector Vector::getNormalized()
//-----------------------------------------------------------------------------
{
  // calcul de la norme
  double norm = getNorm();

  Vector vect(*this);

  // recalcul des composantes
  for (long i = 0; i < _dataLength; i++)
    vect._data[i] /= norm;

  return vect;
}

// Maximum _data in a vector
/*
  This method returns the maximum _data of a vector
*/
//-----------------------------------------------------------------------------
double Vector::maxValue()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _dataLength; i++)
    if (_data[i] > val)
      val = _data[i];

  return val;
}

// Minimum _data in a vector
/*
  This method returns the minimum _data of a vector
*/
//-----------------------------------------------------------------------------
double Vector::minValue()
//-----------------------------------------------------------------------------
{
  double val = _data[0];

  for (long i = 1; i < _dataLength; i++)
    if (_data[i] < val)
      val = _data[i];

  return val;
}

// Absolute maximum _data in a vector
/*
  This method returns the absolute maximum _data of a vector
*/
//-----------------------------------------------------------------------------
double Vector::maxAbsoluteValue()
//-----------------------------------------------------------------------------
{
  double val = dnlAbs(_data[0]);

  for (long i = 1; i < _dataLength; i++)
    if (dnlAbs(_data[i]) > val)
      val = dnlAbs(_data[i]);

  return val;
}

// Absolute minimum _data in a vector
/*
  This method returns the absolute minimum _data of a vector
*/
//-----------------------------------------------------------------------------
double Vector::minAbsoluteValue()
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
      double *lo = &V._data[I * numberOfDimensions];
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
      double *lo = &V._data[I * numberOfDimensions];
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
      double *lo = &V._data[ind[I]];
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
        (*this)(i + Ii) += V(globI + Ii);
    }
  }
  }
  delete[] ind;
}

// Test the equality of two vectors
/*
  This method tests the equality of two vectors.
  It returns \ref true if all components of the two vectors are equals, \ref false on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - vec Second vector to use for the operation
*/
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
/*
  This method tests the inequality of two vectors.
  It returns \ref true if all components of the two vectors are differents, \ref false on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - vec Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
bool Vector::operator!=(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  return !(*this == vec);
}

// Distance between two vectors
/*
  This method computes the distance between two vectors using an Euclidian getNorm.
  - vect Second vector to use
  \f[ d = \left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert \f]
  Return : Distance between both vectors
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
  return x.getNorm();
}

// Square value of the distance between two vectors
/*
  This method computes the distance between two vectors using an Euclidian getNorm and returns the square value of this distance.
  - vect Second vector to use
  \f[ d = {\left\Vert \overrightarrow{v2} - \overrightarrow{v1} \right\Vert}^2 \f]
  Return : Square value of the distance between two vectors
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
  return x.innerProduct();
}

// Dot product of two vectors
/*
  This method computes the dot product of two vectors defined by:
  \f[ s = \overrightarrow{a} . \overrightarrow{b} \f]
  - vect second vector to use for the operation
  Return : Result of the dot product of the two vectors
*/
//-----------------------------------------------------------------------------
double Vector::dotProduct(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  double prod = 0.0;

#ifdef VERIF_maths
  if (_dataLength != vect._dataLength)
  {
    fatalErrorLine("Vector::dotProduct",
                   "non compatible sizes of vectors v1(%d) and v2(%d)", _dataLength,
                   vect._dataLength);
  }
#endif

  for (long i = 0; i < _dataLength; i++)
    prod += _data[i] * vect._data[i];
  return prod;
}

// Vectorial product of two vectors
/*
  This method computes the vectorialProduct product of two vectors defined by:
  \f[ \overrightarrow{v} = \overrightarrow{a} \land \overrightarrow{b} \f]
  \warning This method only works for two vectors with 3 components.
  - vect second vector to use for the operation
  Return : Vector result of the vectorialProduct product of the two vectors
*/
//-----------------------------------------------------------------------------
Vector Vector::vectorialProduct(const Vector &vect) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_dataLength != 3)
  {
    fatalErrorLine("Vector::vectorialProduct",
                   "getSize of v1(%d) not allowed for this operation", _dataLength);
  }
  if (vect._dataLength != 3)
  {
    fatalErrorLine("Vector::vectorialProduct",
                   "getSize of v2(%d) not allowed for this operation", vect._dataLength);
  }
#endif

  Vector resu(3);

  resu._data[0] = _data[1] * vect._data[2] - _data[2] * vect._data[1];
  resu._data[1] = _data[2] * vect._data[0] - _data[0] * vect._data[2];
  resu._data[2] = _data[0] * vect._data[1] - _data[1] * vect._data[0];

  return resu;
}

// Element Wise inversion of a vector
/*
  This method computes the element wise inverse of a vector
  Return : Vector result of the element wise inverse
*/
//-----------------------------------------------------------------------------
Vector Vector::EWInverse()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = 1 / _data[i];
  return result;
}

// Element Wise exp of a vector
/*
  This method computes the element wise exp of a vector
  Return : Vector result of the element wise exp
*/
//-----------------------------------------------------------------------------
Vector Vector::EWExp()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = exp(_data[i]);
  return result;
}

// Element Wise multiplication of two vectors
/*
  This method computes the element wise multiplication of two vectors
  - v2 : second vector to use for the operation
  Return : Vector result of the element wise multiplication of two vectors
*/
//-----------------------------------------------------------------------------
Vector Vector::EWMultiply(const Vector &v2)
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = _data[i] * v2._data[i];
  return result;
}

// Element Wise square of a vector
/*
  This method computes the element wise square of a vector
  Return : Vector result of the element wise square
*/
//-----------------------------------------------------------------------------
Vector Vector::EWSquare()
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = _data[i] * _data[i];
  return result;
}

// Element Wise add a value to a vector
/*
  This method computes the element wise add a value to a vector
  Return : Vector result of the element wise add a value
*/
//-----------------------------------------------------------------------------
Vector Vector::EWAddReal(const double val)
//-----------------------------------------------------------------------------
{
  Vector result(_dataLength);
  for (long i = 0; i < _dataLength; i++)
    result._data[i] = val + _data[i];
  return result;
}

// Writes a vector in a binary flux for storage
/*
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Vector t;
  t.write(pfile);
  t.close();
  \endcode
  - ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Vector::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&_dataLength, sizeof(long));
  ofs.write((char *)_data, _dataLength * sizeof(double));
}

// Reads a vector from a binary flux for storage
/*
  This method is used to read the components of a vector in a binary file.

  Example :
  \code
  ifstream pfile("file");
  Vector t;
  t.read(pfile);
  \endcode
  - ifs Input file stream to use for reading operation
*/
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

/*
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Vector t;
  pfile << t;
  \endcode
  - os Output file stream to use for writting operation
  - vect Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.write(os);
  return os;
}

// Reads a vector from a binary flux for storage
/*
  This method is used to read the components of a vector in a binary file.

  Example :
  \code
  ifstream pfile("file");
  Vector t;
  pfile >> t;
  \endcode
  - os Input file stream to use for reading operation
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Vector &vect)
//-----------------------------------------------------------------------------
{
  vect.read(is);
  return is;
}

// Saves the content of a Vector into a NumPy file
/*
  This method saves the content of a Vector object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Vector t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Vector::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {_dataLength}, mode);
}

// Saves the content of a Vector into a NumPyZ file
/*
  This method saves the content of a Vector object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Vector t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Vector::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {_dataLength}, mode);
}

// Read the content of a Vector from a NumPy file
/*
  This method reads the content of a Vector object from a NumPy file defined by its filename.

  Example
  \code
  Vector t;
  t.numpyRead("numpy.npy");
  \endcode
*/
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

// Read the content of a Vector from a NumPyZ file
/*
  This method reads the content of a Vector object from a NumPyZ file defined by its filename.

  Example
  \code
  Vector t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
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
