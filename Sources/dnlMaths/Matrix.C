/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*
  \file Matrix.C
  Declaration file for the Matrix class

  This file is the declaration file for the Matrix class. A Matrix class is a Matrix with the following form:
  \f[ M=\left[\begin{array}{cccc}
  M_{11}&M_{12}&...&M_{1n}\\
  M_{21}&M_{22}&...&M_{2n}\\
  ...\\
  M_{n1}&M_{n2}&...&M_{nn}\\
  \end{array}\right] \f]

  \ingroup dnlMaths
*/

#include <lapacke.h>
#include <cblas.h>

#include <Matrix.h>
#include <NumpyInterface.h>
#include <MatrixDiag.h>
#include <Tensor2.h>

//constructeur de la classe Matrix
/*
  Cette methode construit une matrice de rows/cols elements. Par defaut, le contenu de la matrice est mis à zero
  - rows nombre de rows
  - cols nombre de cols
*/
//-----------------------------------------------------------------------------
Matrix::Matrix(const long rows, const long cols, const double value)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(rows, cols);

  // remplissage initial
  setToValue(value);
}

//constructeur par recopie d'une matrice
/*
  Ce constructeur recopie les valeurs contenues dans une matrice. Selon la valeur du flag \ref MEM_funct, la copie est faite soit terme à terme (flag non defini) ou par appel à memcopy() (flag defini).
*/
//-----------------------------------------------------------------------------
Matrix::Matrix(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  allocate(mat._rows, mat._cols);

  memcpy(_data, mat._data, _dataLength * sizeof(double));
}

//-----------------------------------------------------------------------------
Matrix::Matrix(int rows, int cols, double firstValue, double secondValue, ...)
//-----------------------------------------------------------------------------
{
  // allocation memoire;
  allocate(rows, cols);

  // traitement de la premiere valeur
  _data[0] = firstValue;
  _data[1] = secondValue;

  // A place to store the list of arguments
  va_list arguments;

  // Store arguments values
  va_start(arguments, secondValue);

  long skip = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      if (skip >= 2)
        _data[skip] = va_arg(arguments, double);
      skip++;
    }

  //Cleans up the list
  va_end(arguments);
}

//destructeur de la classe Matrix
/*
*/
//-----------------------------------------------------------------------------
Matrix::~Matrix()
//-----------------------------------------------------------------------------
{
  // desallocation de memoire
  desallocate();
}

//-----------------------------------------------------------------------------
void Matrix::allocate(const long rows, const long cols)
//-----------------------------------------------------------------------------
{
  _rows = rows;
  _cols = cols;
  _dataLength = _rows * _cols;
  _data = new double[_dataLength];

#ifdef VERIF_alloc
  if (!_data)
  {
    internalFatalErrorLine("Matrix::allocate()",
                           "new double[%d] for a [%d,%d] Matrixrix allocation Error\n"
                           "Seems to have an overflow memory error\n"
                           "Check your memory getSize, and memory consumption first\n",
                           _dataLength, _rows, _cols);
  }
#endif
}

//-----------------------------------------------------------------------------
void Matrix::desallocate()
//-----------------------------------------------------------------------------
{
  if (_dataLength >= 0)
    delete[] _data;

  _rows = 0;
  _cols = 0;
  _dataLength = 0;
}

//redimensionne la matrice
/*
  Cette methode est utilisee pour specifier une nouvelle dimension de matrice de celle donnee lors de l'initialisation par le constructeur
  - rows nombre de rows
  - cols nombre de cols
*/
//-----------------------------------------------------------------------------
void Matrix::redim(const long rows, const long cols)
//-----------------------------------------------------------------------------
{
  if ((rows == _rows) && (cols == _cols))
    return;

  desallocate();
  allocate(rows, cols);
}

//affichage du contenu d'une matrice
/*
  Cette methode est une surdefinition de << pour les flux de sortie, son utilisation est donnee comme suit

  Exemple
  \code
  Matrix t;
  std::cout << t << endl;
  \endcode
  - os flux de sortie
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//affichage du contenu d'une matrice
/*
  Cette methode permet d'afficher le contenu d'une matrice sur la sortie std::ostream
  - os flux de sortie
*/
//-----------------------------------------------------------------------------
void Matrix::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i, j;
  os << "matrix " << _rows << "x" << _cols << " ={{";
  for (i = 0; i < _rows; i++)
  {
    for (j = 0; j < _cols; j++)
    {
      if (j != 0)
        os << ",";
      os << _data[Ind(i, j, _rows, _cols)];
    }
    if (i != _rows - 1)
      os << "},{";
  }
  os << "}}";
}

//affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'une matrice par un scalaire

  Exemple :
  \code
  Matrix t1;
  t1=setToValue(1.); // affecte 1 à toutes les composantes de la matrice
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::setToValue(double val)
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < _dataLength; i++)
    _data[i] = val;
}

//renvoie une matrice identite à partir d'une matrice quelconque
/*
  Cette methode prend en argument une matrice quelconque et renvoie une matrice identite

  Exemple :
  \code
  Matrix t1;
  t1.setToUnity(); // renvoie identite
  \endcode
  \warning Cette methode modifie son argument
*/
//-----------------------------------------------------------------------------
void Matrix::setToUnity()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  if (_rows != _cols)
    fatalError(" Matrix::setToUnity()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  long i, j;

  for (i = 0; i < _rows; i++)
    for (j = 0; j < _cols; j++)
      _data[Ind(i, j, _rows, _cols)] = (double)(i == j);
}

//Maximum component in a matrix
/*
  This method returns the maximum component of a matrix
*/
//-----------------------------------------------------------------------------
double Matrix::maxValue() const
//-----------------------------------------------------------------------------
{
  double max = _data[0];
  for (int i = 1; i < _dataLength; i++)
  {
    if (_data[i] > max)
      max = _data[i];
  }
  return max;
}

//Minimum component in a matrix
/*
  This method returns the minimum component of a matrix
*/
//-----------------------------------------------------------------------------
double Matrix::minValue() const
//-----------------------------------------------------------------------------
{
  double min = _data[0];
  for (int i = 1; i < _dataLength; i++)
  {
    if (_data[i] < min)
      min = _data[i];
  }
  return min;
}

//Maximum absolute component in a matrix
/*
  This method returns the maximum absolute component of a matrix
*/
//-----------------------------------------------------------------------------
double Matrix::maxAbsoluteValue() const
//-----------------------------------------------------------------------------
{
  double max = dnlAbs(_data[0]);
  for (int i = 1; i < _dataLength; i++)
  {
    if (dnlAbs(_data[i]) > max)
      max = dnlAbs(_data[i]);
  }
  return max;
}

//Minimum absolute component in a matrix
/*
  This method returns the minimum absolute component of a matrix
*/
//-----------------------------------------------------------------------------
double Matrix::minAbsoluteValue() const
//-----------------------------------------------------------------------------
{
  double min = dnlAbs(_data[0]);
  for (int i = 1; i < _dataLength; i++)
  {
    if (dnlAbs(_data[i]) < min)
      min = dnlAbs(_data[i]);
  }
  return min;
}

//affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'une matrice par un scalaire

  Exemple :
  \code
  Matrix t1;
  t1=1.; // affecte 1 à toutes les composantes de la matrice
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix &Matrix::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

//affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement l'affectation sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  t1=t2; // egalite de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix &Matrix::operator=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  // redimension de la matrice resultat
  redim(mat._rows, mat._cols);

  // recopie des valeurs
  memcpy(_data, mat._data, _dataLength * sizeof(double));

  return *this;
}

//affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement l'affectation sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  t1=t2; // egalite de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix &Matrix::operator=(const Tensor2 &tens)
//-----------------------------------------------------------------------------
{
  // redimension de la matrice resultat
  redim(3, 3);

  // recopie des valeurs
  memcpy(_data, tens._data, _dataLength * sizeof(double));

  return *this;
}

//addition de deux matrices
/*
  Cette methode permet de surdefinir l'operation d'addition des matrices et d'ecrire simplement la somme de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1+t2; // somme de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator+(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_math
  if ((_rows != mat._rows) && (_cols != mat._cols))
    fatalError("Matrix::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
#endif

  // creation d'une nouvelle matrice
  Matrix resu(mat._rows, mat._cols);

  // calcul de la somme
  for (i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] + mat._data[i];

  // renvoi de l'objet
  return resu;
}

//soustraction de deux matrices
/*
  Cette methode permet de surdefinir l'operation de soustraction des matrices et d'ecrire simplement la soustraction de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1-t2; // soustraction de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator-(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_math
  if ((_rows != mat._rows) && (_cols != mat._cols))
    fatalError("Matrix::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
#endif

  // creation d'une nouvelle matrice
  Matrix resu(_rows, _cols);

  // calcul de la somme
  for (i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] - mat._data[i];

  // renvoi de l'objet
  return resu;
}

//-----------------------------------------------------------------------------
Matrix Matrix::operator-() const
//-----------------------------------------------------------------------------
{
  long i;

  // creation d'une nouvelle matrice
  Matrix resu(_rows, _cols);

  // calcul de la somme
  for (i = 0; i < _dataLength; i++)
    resu._data[i] = -_data[i];

  // renvoi de l'objet
  return resu;
}

//addition de deux matrices
/*
  Cette methode permet de surdefinir l'operation d'addition des matrices et d'ecrire simplement la somme de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  t2+=t1; // somme de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::operator+=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_math
  if ((_rows != mat._rows) && (_cols != mat._cols))
    fatalError("Matrix::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
#endif

  // calcul de la somme
  for (i = 0; i < _dataLength; i++)
    _data[i] += mat._data[i];
}

//soustraction de deux matrices
/*
  Cette methode permet de surdefinir l'operation de soustraction des matrices et d'ecrire simplement la soustraction de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  t2-=t1; // soustraction de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::operator-=(const Matrix &mat)
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_math
  if ((_rows != mat._rows) && (_cols != mat._cols))
    fatalError("Matrix::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
#endif

  // calcul de la somme
  for (i = 0; i < _dataLength; i++)
    _data[i] -= mat._data[i];
}

//multiplication d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  double l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  long i;
  Matrix resu(_rows, _cols);

  for (i = 0; i < _dataLength; i++)
    resu._data[i] = lambda * _data[i];
  return resu;
}

//multiplication d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1;
  double l;
  t1*=l; // multiplication par un scalaire
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::operator*=(const double &lambda)
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < _dataLength; i++)
    _data[i] *= lambda;
}

//division d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de division des matrices et d'ecrire simplement la division d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1;
  double l;
  t1/=l; // division par un scalaire
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::operator/=(const double &lambda)
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < _dataLength; i++)
    _data[i] /= lambda;
}

//division d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de division des matrices et d'ecrire simplement la division d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Matrix t1,t2;
  double l;
  t2=t1/l; // division par un scalaire
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  long i;
  Matrix resu(_rows, _cols);

  for (i = 0; i < _dataLength; i++)
    resu._data[i] = _data[i] / lambda;
  return resu;
}

//multiplication d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication d'une matrice par un scalaire sous la forme donnee en exemple. Elle est identique à la forme precedente (et commutative).

  Exemple :
  \code
  Matrix t1,t2;
  double l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix operator*(const double &lambda, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  long i;
  Matrix resu(mat._rows, mat._cols);

  for (i = 0; i < mat._dataLength; i++)
    resu._data[i] = lambda * mat._data[i];
  return resu;
}

//multiplication de deux matrices
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication de deux matrice sous la forme donnee en exemple. Cette operation correspond à la notion de produit contracte de deux matrices.

  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1*t2; // produit contracte
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator*(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  return singleProduct(mat);
}

//-----------------------------------------------------------------------------
Matrix Matrix::singleProduct() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError(" Matrix::singleProduct()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  Matrix resu(_rows, _rows);

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, _rows, _rows, _rows, 1.0, _data, _rows, _data, _rows, 0.0, resu._data, _rows);

  return resu;
}

//-----------------------------------------------------------------------------
Matrix Matrix::singleProductTxN() const
//-----------------------------------------------------------------------------
{
  Matrix resu(_cols, _cols);

  cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, _cols, _cols, _rows, 1.0, _data, _cols, _data, _cols, 0.0, resu._data, _cols);

  return resu;
}

//-----------------------------------------------------------------------------
Matrix Matrix::singleProductNxT() const
//-----------------------------------------------------------------------------
{
  Matrix resu(_rows, _rows);

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, _rows, _rows, _cols, 1.0, _data, _cols, _data, _cols, 0.0, resu._data, _rows);

  return resu;
}

//-----------------------------------------------------------------------------
Matrix Matrix::singleProduct(const Matrix mat) const
//-----------------------------------------------------------------------------
{
  Matrix resu(_rows, mat._cols);

#ifdef VERIF_maths
  if (_cols != mat.rows())
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat.rows(), mat.columns());
#endif

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, _rows, mat._cols, _cols, 1.0, _data, _cols, mat._data, mat._cols, 0.0, resu._data, mat._cols);

  return resu;
}

//Double contracted product of 2 second order tensors
/*
  This method defines a double contracted product of two second order tensors.
  The result of this operation is a scalar defined by:
  \f[ s = A : B \f]
  Example :
  \code
  Tensor2 tensor1,tensor2;
  double s;
  s = tensor1.doubleProduct(tensor2); // double contracted product
  \endcode
  - tensor2 Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
double Matrix::doubleProduct() const
//-----------------------------------------------------------------------------
{
  double sum = 0.0;
  for (long i = 0; i < _rows; i++)
    for (long j = 0; j < _cols; j++)
      sum += _data[Ind(i, j, _rows, _cols)] * _data[Ind(i, j, _rows, _cols)];
  return (sum);
}

//Double contracted product of 2 second order tensors
/*
  This method defines a double contracted product of two second order tensors.
  The result of this operation is a scalar defined by:
  \f[ s = A : B \f]
  Example :
  \code
  Tensor2 tensor1,tensor;
  double s;
  s = tensor1.doubleProduct(tensor); // double contracted product
  \endcode
  - tensor Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
double Matrix::doubleProduct(const Matrix mat) const
//-----------------------------------------------------------------------------
{
  double sum = 0.0;
  for (long i = 0; i < _rows; i++)
    for (long j = 0; j < _cols; j++)
      sum += _data[Ind(i, j, _rows, _cols)] * mat._data[Ind(i, j, _rows, _cols)];
  return (sum);
}

//multiplication de deux matrices
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication de deux matrice sous la forme donnee en exemple. Cette operation correspond à la notion de produit contracte de deux matrices.

  Exemple :
  \code
  Matrix t1,t2,t3;
  t3=t1*t2; // produit contracte
  \endcode
*/
//-----------------------------------------------------------------------------
Matrix Matrix::operator*(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  long i, k;

  Matrix resu(_rows, mat.columns());

  for (i = 0; i < _rows; i++)
  {
    for (k = 0; k < mat.columns(); k++)
    {
      resu._data[Ind(i, k, _rows, mat.columns())] = _data[Ind(i, k, _rows, _cols)] * mat(k);
    }
  }

  return resu;
}

//multiplication d'une matrices par une matrice diagonale
/*
  Cette methode permet de surdefinir l'operation de multiplication d'une matrice par une matrice diagonale et de stocker le resultat dans la matrice sur laquelle on applique la methode. Les deux matrices doivent etre de meme dimension pour que cette methode soit utilisable.
  Exemple :
  \code
  Matrix t1;
  MatrixDiag t2;
  t1.squareMultiplyBy(t2); // produit
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::squareMultiplyBy(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_cols != mat.rows())
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat.rows(), mat.columns());
  if (_cols != _rows)
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "Matrix must be a square one to do this operation");
#endif

  long k = 0;
  long i, j;
  for (i = 0; i < _rows; i++)
  {
    double m = mat._data[i];
    for (j = 0; j < _rows; j++)
    {
      _data[k++] *= m;
    }
  }
}

//multiplication d'une matrices par l'getInverse d'une matrice diagonale
/*
  Cette methode permet de surdefinir l'operation de multiplication d'une matrice par l'getInverse d'une matrice diagonale et de stocker le resultat dans la matrice sur laquelle on applique la methode. Les deux matrices doivent etre de meme dimension pour que cette methode soit utilisable.
  Exemple :
  \code
  Matrix t1;
  MatrixDiag t2;
  t1.squareDivideBy(t2); // produit
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::squareDivideBy(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_cols != mat.rows())
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat.rows(), mat.columns());
  if (_cols != _rows)
    fatalError("Matrix::squareMultiplyBy (const MatrixDiag & mat)",
               "matrix sizes incompatible\n"
               "Matrix must be a square one to do this operation");
#endif

  long k = _rows * _cols - 1;
  long i, j;
  for (i = _rows - 1; i >= 0; i--)
  {
    double m = 1.0 / mat._data[i];
    for (j = _rows - 1; j >= 0; j--)
    {
      _data[k--] *= m;
    }
  }
}

//multiplication d'une matrice par un vecteur
/*
  Cette methode calcule le produit d'une matrice A de taille N-x-M par un vecteur x de taille N. ceci genere un vecteur y de taille M. Cette methode utilise une routine Blas 2 et calcule le resultat de:
  \f[ y=A^{T}.x \f]
  Cette methode retourne un vecteur
  Exemple :
  \code
  Matrix t1;
  Vector v1,v2;
  v2=t1.trans_mult(v1); // produit
  \endcode
  - vec vecteur du second membre
  Return : vecteur resultant de l'operation de multiplication
*/
//-----------------------------------------------------------------------------
Vector Matrix::trans_mult(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != vec.getSize())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               _rows, _cols, vec.getSize());
#endif

  Vector resu(_cols);

  cblas_dgemv(CblasRowMajor, CblasTrans, _rows, _cols, 1, _data, _cols, vec._data, 1, 0, resu._data, 1);

  return resu;
}

//multiplication d'une matrice par un vecteur
/*
  Cette methode calcule le produit d'une matrice A de taille N-x-M par un vecteur x de taille M. ceci genere un vecteur y de taille N. Cette methode utilise une routine Blas 2 et calcule le resultat de:
  \f[ y=A^{T}.x \f]
  Cette methode retourne un vecteur
  Exemple :
  \code
  Matrix t1;
  Vector v1,v2;
  v2=t1*v1; // produit
  \endcode
  - vec vecteur du second membre
  Return : vecteur resultant de l'operation de multiplication
*/
//-----------------------------------------------------------------------------
Vector Matrix::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_cols != vec.getSize())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               _rows, _cols, vec.getSize());
#endif

  Vector resu(_rows);

  cblas_dgemv(CblasRowMajor, CblasNoTrans, _rows, _cols, 1, _data, _cols, vec._data, 1, 0, resu._data, 1);

  return resu;
}

//-----------------------------------------------------------------------------
void Matrix::productBy(Vector &resu) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError(" Matrix::productBy()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);

  if (_cols != resu.getSize())
    fatalError("Matrix::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
               _rows, _cols, resu.getSize());
#endif

  Vector vec = resu;

  cblas_dgemv(CblasRowMajor, CblasNoTrans, _rows, _cols, 1, _data, _rows, vec._data, 1, 0, resu._data, 1);
}

//calcule la trace d'une matrice
/*
  Cette methode calcule la trace d'une matrice carree
  Return : valeur de la trace de la matrice
*/
//-----------------------------------------------------------------------------
double Matrix::getTrace() const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError("Matrix::getTrace()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  double trace = 0.0;
  for (i = 0; i < _rows; i++)
    trace += _data[Ind(i, i, _rows, _cols)];
  return trace;
}

//calcule la moyenne de la trace d'une matrice
/*
  Cette methode calcule la moyenne de la trace d'une matrice carree
  Return : valeur de la moyenne de la trace de la matrice
*/
//-----------------------------------------------------------------------------
double Matrix::getAverageTrace() const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError("Matrix::getAverageTrace()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  double trace = 0.0;
  for (i = 0; i < _rows; i++)
    trace += _data[Ind(i, i, _rows, _cols)];
  return (trace / (double)_rows);
}

//transposee d'une matrice
/*
  Cette methode renvoie la transposee d'une matrice
  Return : transposee de la matrice
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getTranspose() const
//-----------------------------------------------------------------------------
{
  Matrix resu(_cols, _rows);
  long i, j;

  for (i = 0; i < _rows; i++)
    for (j = 0; j < _cols; j++)
      resu._data[Ind(j, i, _cols, _rows)] = _data[Ind(i, j, _rows, _cols)];
  return resu;
}

//sommation des rows d'une matrice
/*
  Cette methode calcule la somme des termes sur les rows d'une matrice et renvoie un vecteur correspondant
  Return : vecteur contenant les sommes sur les rows
*/
//-----------------------------------------------------------------------------
Vector Matrix::rowSum() const
//-----------------------------------------------------------------------------
{
  Vector resu(_rows);

  for (long i = 0; i < _rows; i++)
    for (long j = 0; j < _cols; j++)
      resu(i) += _data[Ind(i, j, _rows, _cols)];

  return resu;
}

//sommation des cols d'une matrice
/*
  Cette methode calcule la somme des termes sur les cols d'une matrice et renvoie un vecteur correspondant
  Return : vecteur contenant les sommes sur les rows
*/
//-----------------------------------------------------------------------------
Vector Matrix::columnSum() const
//-----------------------------------------------------------------------------
{
  Vector resu(_cols);

  for (long i = 0; i < _rows; i++)
    for (long j = 0; j < _cols; j++)
      resu(j) += _data[Ind(i, j, _rows, _cols)];

  return resu;
}

//partie symetrique d'une matrice
/*
  Cette methode extraie la partie symetrique d'une matrice et la renvoie
  \warning Cette methode n'est disponible que pour les matrices de taille 2 et 3.
  Return : partie symetrique de la matrice
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getSymetricPart() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError("Matrix::getSymetricPart()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  Matrix result(_rows, _rows);

  for (long j = 0; j < _rows; j++)
    for (long i = 0; i < _rows; i++)
      result._data[Ind(i, j, _rows, _rows)] = (_data[Ind(i, j, _rows, _rows)] + _data[Ind(j, i, _rows, _rows)]) / 2;

  return result;
}

//partie anti-symetrique d'une matrice
/*
  Cette methode extraie la partie anti-symetrique d'une matrice et la renvoie
  \warning Cette methode n'est disponible que pour les matrices de taille 2 et 3.
  Return : partie anti-symetrique de la matrice
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getSkewSymetricPart() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError("Matrix::getSkewSymetricPart()",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  Matrix result(_rows, _rows);

  for (long j = 0; j < _rows; j++)
    for (long i = 0; i < _rows; i++)
      result._data[Ind(i, j, _rows, _rows)] = (_data[Ind(i, j, _rows, _rows)] - _data[Ind(j, i, _rows, _rows)]) / 2;

  return result;
}

//extraction d'une ligne d'une matrice
/*
  Cette methode extraie une ligne d'une matrice et la renvoie
  - row numero de la ligne de la matrice (origine 0).
  Return : vecteur contenant la ligne \c row de la matrice
*/
//-----------------------------------------------------------------------------
Vector Matrix::getRow(long row) const
//-----------------------------------------------------------------------------
{
  indexOK(row, 0);

  Vector resu(_cols);

  for (long j = 0; j < _cols; j++)
    resu(j) = _data[Ind(row, j, _rows, _cols)];

  return resu;
}

//extraction d'une colonne d'une matrice
/*
  Cette methode extraie une colonne d'une matrice et la renvoie
  - col numero de la colonne de la matrice (origine 0).
  Return : vecteur contenant la colonne \c col de la matrice
*/
//-----------------------------------------------------------------------------
Vector Matrix::getColumn(long col) const
//-----------------------------------------------------------------------------
{
  indexOK(0, col);

  Vector resu(_rows);

  for (long i = 0; i < _rows; i++)
    resu(i) = _data[Ind(i, col, _rows, _cols)];

  return resu;
}

//egalite de deux matrices
/*
  Cette methode teste l'egalite de deux matrices
  Return : true si les deux matrices sont identiques, false dans la cas contraire
*/
//-----------------------------------------------------------------------------
bool Matrix::operator==(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  long i;

#ifdef VERIF_maths
  if ((_rows != mat._rows) && (_cols != mat._cols))
    fatalError("Matrix::operator ==",
               "The dimensions of the two matrix are'nt corresponding\n"
               "they are respectively a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
#endif

  for (i = 0; i < _dataLength; i++)
    if (_data[i] != mat._data[i])
      return false;
  return true;
}

//inegalite de deux matrices
/*
  Cette methode teste l'inegalite de deux matrices
  Return : true si les deux matrices sont differentes, false dans la cas contraire
*/
//-----------------------------------------------------------------------------
bool Matrix::operator!=(const Matrix &mat) const
//-----------------------------------------------------------------------------
{
  return !(*this == mat);
}

//sortie sur flux std::ofstream
/*
  Cette methode permet d'ecrire une matrice dans un fichier (notament) binaire

  Exemple :
  \code
  std::ofstream pfile("fichier");
  Matrix t;
  t.write(pfile);
  t.close();
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&_rows, sizeof(long));
  ofs.write((char *)&_cols, sizeof(long));
  ofs.write((char *)_data, _dataLength * sizeof(double));
}

//lecture sur flux std::ifstream
/*
  Cette methode permet de lire une matrice depuis un fichier (notament) binaire

  Exemple :
  \code
  std::ifstream pfile("fichier");
  Matrix t;
  t.read(pfile);
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  long rows, cols;

  ifs.read((char *)&rows, sizeof(long));
  ifs.read((char *)&cols, sizeof(long));
  redim(rows, cols);
  ifs.read((char *)_data, _dataLength * sizeof(double));
}

//sortie sur flux std::ofstream
/*
  Cette methode permet d'ecrire une matrice dans un fichier (notament) binaire

  Exemple :
  \code
  std::ofstream pfile("fichier");
  Matrix t;
  pfile << t;
  \endcode
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.write(os);
  return os;
}

//lecture sur flux std::ifstream
/*
  Cette methode permet de lire une matrice depuis un fichier (notament) binaire

  Exemple :
  \code
  std::ifstream pfile("fichier");
  Matrix t;
  pfile >> t;
  \endcode
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Matrix &mat)
//-----------------------------------------------------------------------------
{
  mat.read(is);
  return is;
}

//-----------------------------------------------------------------------------
long Matrix::Memory() const
//-----------------------------------------------------------------------------
{
  return (_dataLength * sizeof(double));
}

//-----------------------------------------------------------------------------
double Matrix::getDeterminant2x2() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if ((_rows != 2) || (_cols != 2))
    fatalError("Matrix::getDeterminant",
               "Your matrix is not a 2x2 square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif
  return _data[0] * _data[3] - _data[2] * _data[1];
}

//-----------------------------------------------------------------------------
double Matrix::getDeterminant3x3() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if ((_rows != 3) || (_cols != 3))
    fatalError("Matrix::getDeterminant",
               "Your matrix is not a 3x3 square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif
  return _data[1] * _data[5] * _data[6] -
         _data[2] * _data[4] * _data[6] +
         _data[2] * _data[3] * _data[7] -
         _data[0] * _data[5] * _data[7] -
         _data[1] * _data[3] * _data[8] +
         _data[0] * _data[4] * _data[8];
}

//determinant d'une matrice carree
/*
  Cette methode calcule le determinant d'une matrice carree en utilisant la routine DGETRF de la librairie Lapack. Cette routine calcule la decomposition LU d'une matrice generale A de taille M-x-N en utilisant des pivots partiels avec echange de ligne. La factorisation a la forme  A = P * L * U avec P matrice de permutations, L la partie triangulaire inferieure avec elements unitaires sur la diagonale et U la partie triangulaire superieure. On a alors
  \f[
  getDeterminant A = tr [U]
  \f]
  Return : valeur du determinant
*/
//-----------------------------------------------------------------------------
double Matrix::getDeterminant() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if (_rows != _cols)
    fatalError("Matrix::getDeterminant",
               "Your matrix is not a square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
#endif

  lapack_int n, m, info, lda;
  double Det = 1.0;

  n = _rows;
  m = _rows;
  lda = _rows;
  lapack_int *ipiv = new lapack_int[_rows];

  // init a matrix
  double *A = new double[_dataLength];
  memcpy(A, _data, _dataLength * sizeof(double));

  info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, A, lda, ipiv);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getDeterminant",
                             "parameter %d of function dgetrf_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getDeterminant",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  for (int i = 0; i < _rows; i++)
  {
    if (ipiv[i] == i + 1)
      Det *= A[Ind(i, i, _rows, _cols)];
    else
      Det *= -A[Ind(i, i, _rows, _cols)];
  }

  delete[] ipiv;
  delete[] A;

  return Det;
}

//-----------------------------------------------------------------------------
Matrix Matrix::getCofactor() const
//-----------------------------------------------------------------------------
{
  Matrix result = getInverse();
  return (result.getTranspose() * getDeterminant());
}

//-----------------------------------------------------------------------------
void Matrix::computeInverse2x2(double det, Matrix &inverse) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if ((_rows != 2) || (_cols != 2))
    fatalError("Matrix::getDeterminant",
               "Your matrix is not a 2x2 square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
  if ((inverse._rows != 2) || (inverse._cols != 2))
    fatalError("Matrix::getDeterminant",
               "The inverse matrix is not a 2x2 square matrix, it's a [%d,%d] matrix",
               inverse._rows, inverse._cols);
#endif

  inverse._data[0] = _data[3] / det;
  inverse._data[1] = -_data[1] / det;
  inverse._data[2] = -_data[2] / det;
  inverse._data[3] = _data[0] / det;
}

//-----------------------------------------------------------------------------
void Matrix::computeInverse3x3(double det, Matrix &inverse) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
  if ((_rows != 3) || (_cols != 3))
    fatalError("Matrix::getDeterminant",
               "Your matrix is not a 3x3 square matrix, it's a [%d,%d] matrix",
               _rows, _cols);
  if ((inverse._rows != 3) || (inverse._cols != 3))
    fatalError("Matrix::getDeterminant",
               "The inverse matrix is not a 3x3 square matrix, it's a [%d,%d] matrix",
               inverse._rows, inverse._cols);

#endif

  inverse._data[0] = (_data[4] * _data[8] - _data[5] * _data[7]) / det;
  inverse._data[1] = (_data[2] * _data[7] - _data[1] * _data[8]) / det;
  inverse._data[2] = (_data[1] * _data[5] - _data[2] * _data[4]) / det;
  inverse._data[3] = (_data[5] * _data[6] - _data[3] * _data[8]) / det;
  inverse._data[4] = (_data[0] * _data[8] - _data[2] * _data[6]) / det;
  inverse._data[5] = (_data[2] * _data[3] - _data[0] * _data[5]) / det;
  inverse._data[6] = (_data[3] * _data[7] - _data[4] * _data[6]) / det;
  inverse._data[7] = (_data[1] * _data[6] - _data[0] * _data[7]) / det;
  inverse._data[8] = (_data[0] * _data[4] - _data[1] * _data[3]) / det;
}

//getInverse d'une matrice
/*
  Cette methode calcule l'getInverse d'une matrice et le renvoie. Elle utilise les fonctions DGETRF et DGETRI de la librairie Lapack. La matrice initiale est preservee par cette methode.
  Return : valeur de l'getInverse d'une matrice
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getInverse() const
//-----------------------------------------------------------------------------
{
  Matrix inv(_rows, _cols);
  lapack_int n, m, info, lda;
  lapack_int *ipiv;
  n = _rows;
  m = _rows;
  lda = _rows;
  ipiv = new lapack_int[_rows];

  // copie de la matrice
  inv = *this;

  info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, inv._data, lda, ipiv);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getInverse",
                             "parameter (%d) of function dgetrf_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getInverse",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  info = LAPACKE_dgetri(LAPACK_ROW_MAJOR, n, inv._data, lda, ipiv);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getInverse",
                             "parameter %d of function dgetri_ has illegal value",
                             -info);
    }
  }

  delete[] ipiv;

  // and return the result
  return inv;
}

//resolution d'un systeme lineaire
/*
  Cette methode calcule la solution du systeme lineaire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N. Cette methode utilise les routines DGETRF et DGETRS de la librairie Lapack.

  Cette methode utilise des copies de vecteurs et matrices donnes en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel.
  - x vecteur du second membre
  Return : vecteur solution du systeme lineaire

*/
//-----------------------------------------------------------------------------
Vector Matrix::getSolve(const Vector &vect) const
//-----------------------------------------------------------------------------
{
  Vector result(_rows);
  lapack_int info;
  lapack_int *ipiv;

  ipiv = new lapack_int[_rows];
  result = vect;

  // init a matrix
  double *A = new double[_dataLength];
  memcpy(A, _data, _dataLength * sizeof(double));

  info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, _rows, 1, A, _rows, ipiv, result._data, 1);
  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getSolve",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getSolve",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  delete[] ipiv;
  delete[] A;

  return result;
}

//resolution d'un systeme lineaire
/*
  Cette methode calcule la solution du systeme lineaire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N. Cette methode utilise les routines DGETRF et DGETRS de la librairie Lapack.

  Cette methode utilise directement les vecteurs et matrices donnes en argument et modifie les valeurs contenues dans ceux-ci au cours de l'appel. Cette methode est un peu plus rapide que la methode getSolve(const Vector& x) pour les larges matrices mais possede un avantage concernant les besoins memoire.
  - x vecteur du second membre
  Return : vecteur solution du systeme lineaire

*/
//-----------------------------------------------------------------------------
void Matrix::solve(Vector &b)
//-----------------------------------------------------------------------------
{
  lapack_int info;
  lapack_int *ipiv = new lapack_int[_rows];

  info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, _rows, 1, _data, _rows, ipiv, b._data, 1);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::solveLinearPivoting",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::solveLinearPivoting",
                 "pivot (%d) is exactly zero\n"
                 "The factorization has been completed, but this factor is exactly singular, and division by zero will occur if it is used to solve a system of equations",
                 info);
    }
  }

  delete[] ipiv;
}

//evaluation du Null Space d'une matrice
/*
  Cette methode calcule le Null Space d'une matrice \f$ \textbf{A} \f$. Le null-space de  \f$ \textbf{A}=N.S.(\textbf{A})\subset\mathcal{W} \f$  est l'ensemble des elements de \f$ \mathcal{V} \f$ tels que \f$ \textbf{A}(\overrightarrow{_data})=\overrightarrow{0} \f$. Cette methode utilise la routine DGESVD de la librairie Lapack.

  Cette methode utilise des copies de vecteurs et matrices donnes en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel.
  - relative Parametre booleen indiquant si la tolerance de recherche est relative ou non. Dans le cas d'une tolerance relative, la tolerance de recherche de la plus patite valeur preopres est recalculee par rapport à la plus grande valeur propre.
  - tol tolerance de recherche du Null-Space par defaut 1e-10.
  Return : matrice contenant les vecteurs du Null-Space en cols (taille N x nNs) avce nNs nombre de "modes rigides"
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getNullSpace(bool relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector S(_rows);
  lapack_int n, m, info, lda;
  lapack_int ldu, ldvt;
  double *U = NULL;
  double *VT = NULL;
  double *superb;

  n = _rows;
  m = _rows;
  lda = _rows;
  //  ldb = _rows;
  ldu = _rows;
  ldvt = _rows;

  // init a matrix
  Matrix A = *this;

  //  DGESVD computes the singular value decomposition (SVD) of a double
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are double and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  //lwork = -1;
  //dgesvd_("N", "O", &m, &n, &A(0,0), &lda, &S(0), U, &ldu, VT, &ldvt, &worksize, &lwork, &info);
  // lwork = (int)worksize;

  // allocate memory for the work buffer
  superb = new double[m];

  // compute eigenvalues and eigenvectors
  //dgesvd_("N", "O", &m, &n, &A(0,0), &lda, &S(0), U, &ldu, VT, &ldvt, work, &lwork, &info);
  info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'N', 'O', m, n, &A(0, 0), lda, &S(0), U, ldu, VT, ldvt, superb);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  // on recalcule la tolerance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= S(0);

  // on compte le nombre de null cols
  long ic = 0;
  for (long i = n; i > 0;)
    if (S(--i) <= tol)
      ic++;

  // dimension de la matrice de sortie
  Matrix ret(n, ic);

  // remplissage du Null-Space
  ic = 0;
  for (long i = 0; i < n; i++)
  {
    if (S(i) < tol)
    {
      // copy the last rows
      for (long j = 0; j < n; j++)
        ret(j, ic) = A(i, j);
      ic++;
    }
  }

  delete[] superb;

  return ret;
}

//-----------------------------------------------------------------------------
Matrix Matrix::getNullSpace2(bool relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector S(_rows);
  lapack_int n, m, info, lda;
  lapack_int ldu, ldvt;
  Matrix U;
  Matrix VT;

  n = _rows;
  m = _rows;
  lda = _rows;
  //  ldb = _rows;
  ldu = _rows;
  ldvt = _rows;

  // init a matrix
  Matrix A = *this;

  U.redim(ldu, m);
  VT.redim(ldvt, n);

  //  DGESVD computes the singular value decomposition (SVD) of a double
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are double and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  //iwork = new int[8 * m];

  // lwork = -1;
  //dgesdd_("A", &m, &n, &A(0, 0), &lda, &S(0), &U(0, 0), &ldu, &VT(0, 0), &ldvt, &worksize, &lwork, iwork, &info);
  // lwork = (int)worksize;

  // allocate memory for the work buffer
  // work = new double[lwork];

  // compute eigenvalues and eigenvectors
  //dgesdd_("A", &m, &n, &A(0, 0), &lda, &S(0), &U(0, 0), &ldu, &VT(0, 0), &ldvt, work, &lwork, iwork, &info);
  info = LAPACKE_dgesdd(LAPACK_ROW_MAJOR, 'A', m, n, &A(0, 0), lda, &S(0), &U(0, 0), ldu, &VT(0, 0), ldvt);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::getNullSpace",
                             "parameter %d of function dgesdd_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::getNullSpace",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }

  // on recalcule la tolerance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= S(0);

  // on compte le nombre de null cols
  long ic = 0;
  for (long i = n; i > 0;)
    if (S(--i) <= tol)
      ic++;

  // dimension de la matrice de sortie
  Matrix ret(n, ic);

  // remplissage du Null-Space
  ic = 0;
  for (long i = 0; i < n; i++)
  {
    if (S(i) < tol)
    {
      // copy the last rows
      for (long j = 0; j < n; j++)
        ret(j, ic) = VT(i, j);
      ic++;
    }
  }

  return ret;
}

//evaluation valeurs propres et des vecteurs propres à droite et à gauche
/*
  Cette methode calcule les valeurs propres et les vecteurs propres à droite et à gauche d'une matrice \f$ \textbf{A} \f$. Cette methode utilise la routine DGESVD de la librairie Lapack.

  Cette methode utilise des copies de vecteurs et matrices donnes en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donnes en argument n'on pas besoin d'etre dimensionnes à un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning la matrice rightEigenVectors contient les vecteurs propres de la matrice sous une forme TRANSPOSEE.
  - eigenValues vecteur contenant en retour les valeurs propres de la matrice
  - leftEigenVectors Matrice contenant en retour les vecteurs propres à gauche de la matrice (TRANSPOSES)
  - rightEigenVectors Matrice contenant en retour les vecteurs propres à droite de la matrice
*/
//-----------------------------------------------------------------------------
void Matrix::computeSVD(Vector &eigenValues, Matrix &leftEigenVectors, Matrix &rightEigenVectors)
//-----------------------------------------------------------------------------
{
  lapack_int info, lwork;
  double *superb;

  Matrix A = *this;
  leftEigenVectors.redim(_rows, _rows);
  rightEigenVectors.redim(_cols, _cols);
  eigenValues.redim(_cols);

  superb = new double[_rows];

  info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', _rows, _cols, &A(0, 0), _cols, &eigenValues(0), &leftEigenVectors(0, 0), _rows, &rightEigenVectors(0, 0), _cols, superb);

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::computeSVD",
                             "parameter %d of function LAPACKE_dgesvd has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::computeSVD",
                 "SVD incorrect",
                 info);
    }
  }

  delete[] superb;
}

//-----------------------------------------------------------------------------
void Matrix::computeEigenVectors2(Vector &eigenValues, Matrix &eigenVectors)
//-----------------------------------------------------------------------------
{
  lapack_int n, m, info, lda;
  lapack_int ldu, ldvt, lwork;

  n = _rows;
  m = _rows;
  lda = _rows;
  ldu = _rows;
  ldvt = _rows;

  // init a matrix
  eigenVectors = *this;

  // redim the matrices and vectors
  //  leftEigenVectors.redim(n, n);
  //  rightEigenVectors.redim(n, n);
  eigenValues.redim(n);

  //  DGESVD computes the singular value decomposition (SVD) of a double
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are double and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  //lwork = -1;
  //dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, &worksize, &lwork, &info);
  //lwork = (int)worksize;

  // allocate memory for the work buffer
  //  work = new double[lwork];
  // superb = new double[m];

  // compute eigenvalues and eigenvectors
  //dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, work, &lwork, &info);
  // info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', m, n, &A(0, 0), lda, &eigenValues(0), &leftEigenVectors(0, 0), ldu, &rightEigenVectors(0, 0), ldvt, superb);
  info = LAPACKE_dsyevd(LAPACK_ROW_MAJOR, 'V', 'U', n, &eigenVectors(0, 0), lda, &eigenValues(0));

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::computeSVD",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::computeSVD",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }
}

//-----------------------------------------------------------------------------
void Matrix::computeEigenVectors2(Vector &eigenValues)
//-----------------------------------------------------------------------------
{
  lapack_int n, m, info, lda;
  lapack_int ldu, ldvt, lwork;

  n = _rows;
  m = _rows;
  lda = _rows;
  ldu = _rows;
  ldvt = _rows;

  // init a matrix
  Matrix A = *this;

  // redim the matrices and vectors
  //  leftEigenVectors.redim(n, n);
  //  rightEigenVectors.redim(n, n);
  eigenValues.redim(n);

  //  DGESVD computes the singular value decomposition (SVD) of a double
  //  M-by-N matrix A, optionally computing the left and/or right singular
  //  vectors. The SVD is written

  //       A = U * SIGMA * transpose(V)

  //  where SIGMA is an M-by-N matrix which is zero except for its
  //  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  //  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
  //  are the singular values of A; they are double and non-negative, and
  //  are returned in descending order.  The first min(m,n) columns of
  //  U and V are the left and right singular vectors of A.

  //  Note that the routine returns V**T, not V.

  // computes the work length vector
  //lwork = -1;
  //dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, &worksize, &lwork, &info);
  //lwork = (int)worksize;

  // allocate memory for the work buffer
  //  work = new double[lwork];
  // superb = new double[m];

  // compute eigenvalues and eigenvectors
  //dgesvd_("A", "A", &m, &n, &A(0, 0), &lda, &eigenValues(0), &leftEigenVectors(0, 0), &ldu, &rightEigenVectors(0, 0), &ldvt, work, &lwork, &info);
  // info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', m, n, &A(0, 0), lda, &eigenValues(0), &leftEigenVectors(0, 0), ldu, &rightEigenVectors(0, 0), ldvt, superb);
  info = LAPACKE_dsyevd(LAPACK_ROW_MAJOR, 'V', 'U', n, &A(0, 0), lda, &eigenValues(0));

  if (info != 0)
  {
    if (info < 0)
    {
      internalFatalErrorLine("Matrix::computeSVD",
                             "parameter %d of function dgesv_ has illegal value",
                             -info);
    }
    if (info > 0)
    {
      fatalError("Matrix::computeSVD",
                 "Eigenvalues decomposition incorrect",
                 info);
    }
  }
}

//evaluation de la pseudo getInverse d'une matrice
/*
  Cette methode calcule la pseudoInverse d'une matrice \f$ \textbf{A} \f$. Cette methode utilise la routine DGESVD de la librairie Lapack.

  La pseudo getInverse d'une matrice, egalement appelee getInverse de Moore-Penrose est generalement notee par \f$ A^{+} \f$

  Cette methode utilise des copies de vecteurs et matrices donnes en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donnes en argument n'on pas besoin d'etre dimensionnes à un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning Pour plus d'efficacite, cette methode est (Me semble-t-il !!!) restreinte au calcul des matrices carrees symetriques uniquement. Dans le cas d'une matrice non symetrique, il conviendra de verifier le comportement.
  - relative Parametre booleen indiquant si la tolerance de recherche est relative ou non. Dans le cas d'une tolerance relative, la tolerance de recherche de la plus patite valeur preopres est recalculee par rapport à la plus grande valeur propre.
  - tol tolerance de recherche du Null-Space par defaut 1e-10.
  Return : pseudo getInverse de la matrice donnee en argument
*/
//-----------------------------------------------------------------------------
Matrix Matrix::getPseudoInverse(bool relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector ev;
  Matrix U;
  Matrix V;
  Matrix result(_cols, _rows);
  long i;

  // compute all eigenvalues end eigenvectors
  computeSVD(ev, U, V);

  // on recalcule la tolerance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= ev(0);

  // calcul des inverses des valeurs propres (si dans la tolerance)
  for (i = 0; i < ev.getSize(); i++)
  {
    result(i, i) = (ev(i) >= tol ? 1.0 / ev(i) : 0.0);
  }

  // compute the pseudo getInverse
  result = (V.getTranspose() * result * U.getTranspose());

  // retour
  return result;
}

//evaluation de la pseudo getInverse d'une matrice et du Null-Space
/*
  Cette methode calcule la pseudoInverse et le Null-Space d'une matrice \f$ \textbf{A} \f$. Cette methode utilise la routine DGESVD de la librairie Lapack.

  La pseudo getInverse d'une matrice, egalement appelee getInverse de Moore-Penrose est generalement notee par \f$ A^{+} \f$.

  Cette methode utilise des copies de vecteurs et matrices donnes en argument et ne modifie pas les valeurs contenues dans ceux-ci au cours de l'appel. De plus, les matrices et vecteurs de retour donnes en argument n'on pas besoin d'etre dimensionnes à un taille correcte avant appel (ceci est fait en interne dans la routine).
  \warning Pour plus d'efficacite, cette methode est (Me semble-t-il !!!) restreinte au calcul des matrices carrees symetriques uniquement. Dans le cas d'une matrice non symetrique, il conviendra de verifier le comportement.
  - Kplus Matrice de retour pour la pseudo-getInverse de la matrice donnee en argument
  - NS Matrice de retour pour le Null-Space de la matrice donnee en argument
  - relative Parametre booleen indiquant si la tolerance de recherche est relative ou non. Dans le cas d'une tolerance relative, la tolerance de recherche de la plus patite valeur preopres est recalculee par rapport à la plus grande valeur propre.
  - tol tolerance de recherche du Null-Space par defaut 1e-10.
*/
//-----------------------------------------------------------------------------
void Matrix::computePseudoInverse(Matrix &Kplus, Matrix &NS, bool relative, double tol)
//-----------------------------------------------------------------------------
{
  Vector ev;
  Matrix U;
  Matrix V;
  long nFloats;
  long i;

  // compute all ev end eigenvectors
  computeSVD(ev, U, V);

  // on recalcule la tolerance relative en la multipliant par la plus grande valeur propre
  if (relative)
    tol *= ev(0);

  // dimension de Kplus
  Kplus.redim(_cols, _rows);

  // init de nFloats
  nFloats = 0;

  // calcul des inverses des valeurs propres (si dans la tolerance)
  for (i = 0; i < ev.getSize(); i++)
  {
    if (ev(i) > tol)
    {
      Kplus(i, i) = 1.0 / ev(i);
    }
    else
    {
      Kplus(i, i) = 0.0;
      nFloats++;
    }
  }

  // compute the pseudo getInverse
  Kplus = (V.getTranspose() * Kplus * U.getTranspose());

  // dimension de la matrice de Null-Space
  NS.redim(_rows, nFloats);

  // remplissage du Null-Space
  long ic = 0;
  for (long i = 0; i < ev.getSize(); i++)
  {
    if (ev(i) <= tol)
    {
      // copy the last rows
      for (long j = 0; j < _rows; j++)
        NS(j, ic) = V(i, j);
      ic++;
    }
  }
}

//Assemblage d'une matrice dans une autre
/*
  Cette methode effectue la phase d'assemblage d'une matrice dans une autre conformement à ce qui se fait classiquement en Elements Finis.

  - M Matrice elementaire à assembler dans la grande matrice
  - int tableau d'long correspondant au nombre de NOEUDS de l'element et contenant les Id des noeuds que l'on utilise
  - numberOfDim nombre de dimensions du probleme
  \code
  Matrix stiffnessMatrix;
  long numberOfDim=...; // nombre de dimensions du probleme
  long *ind=new long[loop_I]; // allocation memoire pour ind
  for (long Iloop=0; Iloop < loop_I; Iloop++) ind[Iloop]=pel->nodes (Iloop)->Id; // affectation des indices
  K.gatherFrom(stiffnessMatrix, ind, numberOfDim);
  delete [] ind;
  \endcode

  \version 1.1.0
*/
//-----------------------------------------------------------------------------
void Matrix::gatherFrom(const Matrix &M, long *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  //   long mr=M.rows();
  //   long mc=M.columns();
  long mr = M._rows;
  long mc = M._cols;
  long loop_I = mc / numberOfDim;
  long loop_J = mr / numberOfDim;
  long *ind = new long[loop_I];

  // preparation
  long *pind = ind;
  long *pind0 = ind0;
  for (long Iloop = loop_I; Iloop > 0; Iloop--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  for (long Iloop = loop_I - 1; Iloop >= 0; Iloop--)
  {
    for (long Jloop = loop_J - 1; Jloop >= 0; Jloop--)
    {
      switch (numberOfDim)
      {
      case 1:
      {
        double *gl = &_data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *lo = &M._data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl += *lo;
      }
      break;
      case 2:
      {
        double *gl = &_data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *lo = &M._data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl-- += *lo--;
        *gl += *lo;
      }
      break;
      case 3:
      {
        double *gl = &_data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *lo = &M._data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl-- += *lo--;
        *gl-- += *lo--;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
      }
      break;

        // cas general
      default:
      {
        // boucle sur les ddl
        long i = Iloop * numberOfDim;
        long j = Jloop * numberOfDim;
        long globI = ind[Iloop];
        long globJ = ind[Jloop];
        for (long Ii = 0; Ii < numberOfDim; Ii++)
          for (long Jj = 0; Jj < numberOfDim; Jj++)
            (*this)(globI + Ii, globJ + Jj) += M(i + Ii, j + Jj);
      }
      }
    }
  }
  delete[] ind;

#ifdef VERIF_assert
  assert(ind == NULL);
#endif
}

//-----------------------------------------------------------------------------
void Matrix::scatterFrom(const Matrix &M, long *ind0, int numberOfDim)
//-----------------------------------------------------------------------------
{
  //   long mr=M.rows();
  //   long mc=M.columns();
  long mr = _rows;
  long mc = _cols;
  long loop_I = mc / numberOfDim;
  long loop_J = mr / numberOfDim;
  long *ind = new long[loop_I];

  // preparation
  long *pind = ind;
  long *pind0 = ind0;
  for (long Iloop = loop_I; Iloop > 0; Iloop--)
    *pind++ = *pind0++ * numberOfDim;

  // boucle principale
  for (long Iloop = loop_I - 1; Iloop >= 0; Iloop--)
  {
    for (long Jloop = loop_J - 1; Jloop >= 0; Jloop--)
    {
      switch (numberOfDim)
      {
      case 1:
      {
        double *lo = &M._data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *gl = &_data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl += *lo;
      }
      break;
      case 2:
      {
        double *lo = &M._data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *gl = &_data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl-- += *lo--;
        *gl += *lo;
      }
      break;
      case 3:
      {
        double *lo = &M._data[Ind(ind[Iloop], ind[Jloop], _rows, _cols)];
        double *gl = &_data[Ind(Iloop * numberOfDim, Jloop * numberOfDim, mr, mc)];
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl-- += *lo--;
        *gl-- += *lo--;
        *gl += *lo;
        gl += (_rows);
        lo += (mr);
        *gl++ += *lo++;
        *gl++ += *lo++;
        *gl += *lo;
      }
      break;

        // cas general
      default:
      {
        // boucle sur les ddl
        long i = Iloop * numberOfDim;
        long j = Jloop * numberOfDim;
        long globI = ind[Iloop];
        long globJ = ind[Jloop];
        for (long Ii = 0; Ii < numberOfDim; Ii++)
          for (long Jj = 0; Jj < numberOfDim; Jj++)
            (*this)(i + Ii, j + Jj) += M(globI + Ii, globJ + Jj);
      }
      }
    }
  }
  delete[] ind;

#ifdef VERIF_assert
  assert(ind == NULL);
#endif
}

//affichage à l'ecran suivant format predefini
/*
  Cette methode affiche le contenu de la matrice à l'ecran selon un format predefini. La selection du format d'affichage est faite en utilisant la methode \ref setOutType(). Le type de sortie est definit par les variable enumerees dans \ref OutMatrix.
*/
//-----------------------------------------------------------------------------
void Matrix::printOut()
//-----------------------------------------------------------------------------
{
  switch (outType)
  {
  case outMatrixTypeNormal:
  {
    std::cout << "matrix " << _rows << "x" << _cols << " ={{";
    for (long i = 0; i < _rows; i++)
    {
      for (long j = 0; j < _cols; j++)
      {
        if (j != 0)
          std::cout << ",";
        std::cout << (*this)(i, j);
      }
      if (i != _rows - 1)
        std::cout << "},{";
    }
    std::cout << "}}\n";
  }
  break;
  case outMatrixTypeMupad:
  {
    std::cout << "matrix([[";
    for (long i = 0; i < _rows; i++)
    {
      for (long j = 0; j < _cols; j++)
      {
        if (j != 0)
          std::cout << ",";
        std::cout << _data[Ind(i, j, _rows, _cols)];
      }
      if (i != _rows - 1)
        std::cout << "],[";
    }
    std::cout << "]])\n";
  }
  break;
  default:
    fatalError("Matrices::printOut()", "unknown type %d\n", outType);
  }
}

//Saves the content of a Matrix into a NumPy file
/*
  This method saves the content of a Matrix object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Matrix t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {_rows, _cols}, mode);
}

//Saves the content of a Matrix into a NumPyZ file
/*
  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Matrix t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {_rows, _cols}, mode);
}

//Read the content of a Matrix from a NumPy file
/*
  This method reads the content of a vec3D object from a NumPy file defined by its filename.

  Example
  \code
  Matrix t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);

  redim(arr.shape[0], arr.shape[1]);

  if (arr.num_vals != _dataLength)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//Read the content of a Matrix from a NumPyZ file
/*
  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.

  Example
  \code
  Matrix t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void Matrix::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);

  redim(arr.shape[0], arr.shape[1]);

  if (arr.num_vals != _dataLength)
  {
    std::cout << "ERROR\n";
  }
  memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
