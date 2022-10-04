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
  \file MatrixDiag.C
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

#include <MatrixDiag.h>
#include <NumpyInterface.h>

// constructeur de la classe MatrixDiag
/*
  Cette methode construit une matrice de lignes/colonnes elements. Par defaut, le contenu de la matrice est misàzerho
  - numberOfTerms nombre de lignes
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

// constructeur par recopie d'une matrice
/*
  Ce constructeur recopie les valeurs contenues dans une matrice. Selon la valeur du flag \ref MEM_funct, la copie est faite soit termeàterme (flag non defini) ou par appelàmemcopy() (flag defini).
*/
//-----------------------------------------------------------------------------
MatrixDiag::MatrixDiag(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  allocate(mat._rows);

  memcpy(_data, mat._data, _rows * sizeof(double));
}

// destructeur de la classe MatrixDiag
/*
 */
//-----------------------------------------------------------------------------
MatrixDiag::~MatrixDiag()
//-----------------------------------------------------------------------------
{
  desallocate();
}

// Maximum _data in a MatrixDiag
/*
  This method returns the maximum _data of a MatrixDiag
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

// Minimum _data in a MatrixDiag
/*
  This method returns the minimum _data of a MatrixDiag
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

// Absolute maximum _data in a MatrixDiag
/*
  This method returns the absolute maximum _data of a MatrixDiag
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

// Absolute minimum _data in a MatrixDiag
/*
  This method returns the absolute minimum _data of a MatrixDiag
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

//-----------------------------------------------------------------------------
void MatrixDiag::allocate(const long numberOfTerms)
//-----------------------------------------------------------------------------
{
  _rows = numberOfTerms;
  _cols = numberOfTerms;

  // allocation dynamique de memoire
  _data = new double[_rows];
}

//-----------------------------------------------------------------------------
void MatrixDiag::desallocate()
//-----------------------------------------------------------------------------
{
  _rows = 0;
  _cols = 0;

  delete[] _data;
}

// redimensionne la matrice
/*
  Cette methode est utilisee pour specifier une nouvelle dimension de matrice de celle donnee lors de l'initialisation par le constructeur
  - newSize nombre de lignes
*/
//-----------------------------------------------------------------------------
void MatrixDiag::redim(const long newSize)
//-----------------------------------------------------------------------------
{
  if (newSize == _rows)
    return;

  desallocate();
  allocate(newSize);
}

// affichage du contenu d'une matrice
/*
  Cette methode est une surdefinition de << pour les flux de sortie, son utilisation est donnee comme suit

  Exemple
  \code
  MatrixDiag t;
  cout << t << endl;
  \endcode
  - os flux de sortie
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

// affichage du contenu d'une matrice
/*
  Cette methode permet d'afficher le contenu d'une matrice sur la sortie std::ostream
  - os flux de sortie
*/
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
      os << (i == j ? _data[i] : 0.0);
    }
    if (i != _rows - 1)
      os << "},{";
  }
  os << "}}";
}

// affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'une matrice par un scalaire

  Exemple :
  \code
  MatrixDiag t1;
  t1=setValue(1.); // affecte 1àtoutes les composantes de la matrice
  \endcode
*/
//-----------------------------------------------------------------------------
void MatrixDiag::setValue(double val)
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _rows; i++)
    _data[i] = val;
}

// renvoie une matrice identiteàpartir d'une matrice quelconque
/*
  Cette methode prend en argument une matrice quelconque et renvoie une matrice identite. Par definition une matrice diagonale identite est entierement remplie de 1.

  Exemple :
  \code
  MatrixDiag t1;
  t1.setToUnity(); // renvoie identite
  \endcode
  \warning Cette methode modifie son argument
*/
//-----------------------------------------------------------------------------
void MatrixDiag::setToUnity()
//-----------------------------------------------------------------------------
{
  long i;
  for (i = 0; i < _rows; i++)
    _data[i] = 1.0;
}

// affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'une matrice par un scalaire

  Exemple :
  \code
  MatrixDiag t1;
  t1=1.; // affecte 1àtoutes les composantes de la matrice
  \endcode
*/
//-----------------------------------------------------------------------------
MatrixDiag &MatrixDiag::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setValue(val);
  return *this;
}

// affectation d'egalite
/*
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement l'affectation sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2;
  t1=t2; // egalite de deux matrices
  \endcode
*/
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

// addition de deux matrices
/*
  Cette methode permet de surdefinir l'operation d'addition des matrices et d'ecrire simplement la somme de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1+t2; // somme de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator+(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator +",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
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

// soustraction de deux matrices
/*
  Cette methode permet de surdefinir l'operation de soustraction des matrices et d'ecrire simplement la soustraction de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1-t2; // soustraction de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator-(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator -",
               "matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] matrix",
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

// addition de deux matrices
/*
  Cette methode permet de surdefinir l'operation d'addition des matrices et d'ecrire simplement la somme de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2;
  t2+=t1; // somme de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator+=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  // test si egalite de dimensions
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator +=",
               "matrix sizes incompatible\n"
               "You're about to add a [%d,%d] and a [%d,%d] matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // calcul de la somme
  for (long i = 0; i < _rows; i++)
    _data[i] += mat._data[i];
}

// soustraction de deux matrices
/*
  Cette methode permet de surdefinir l'operation de soustraction des matrices et d'ecrire simplement la soustraction de deux matrices sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2;
  t2-=t1; // soustraction de deux matrices
  \endcode
*/
//-----------------------------------------------------------------------------
void MatrixDiag::operator-=(const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  if (_rows != mat._rows)
  {
    fatalError("MatrixDiag::operator -=",
               "matrix sizes incompatible\n"
               "You're about to substract [%d,%d] and [%d,%d] matrix",
               _rows,
               _cols, mat._rows, mat._cols);
  }

  // calcul de la difference
  for (long i = 0; i < _rows; i++)
    _data[i] -= mat._data[i];
}

// multiplication d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1,t2;
  double l;
  t2=t1*l; // multiplication par un scalaire
  \endcode
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

// division d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de division des matrices et d'ecrire simplement la division d'une matrice par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  MatrixDiag t1;
  double l;
  t1/=l; // division par un scalaire
  \endcode
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

// multiplication d'une matrice par un scalaire
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication d'une matrice par un scalaire sous la forme donnee en exemple. Elle est identiqueàla forme precedente (et commutative).

  Exemple :
  \code
  MatrixDiag t1,t2;
  double l;
  t2=l*t1; // multiplication par un scalaire
  \endcode
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

// multiplication de deux matrices
/*
  Cette methode permet de surdefinir l'operation de multiplication des matrices et d'ecrire simplement la multiplication de deux matrice sous la forme donnee en exemple. Cette operation correspondàla notion de produit contracte de deux matrices.

  Exemple :
  \code
  MatrixDiag t1,t2,t3;
  t3=t1*t2; // produit contracte
  \endcode
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::operator*(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  return dot(mat);
}

//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::dot(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  if (_cols != mat._rows)
  {
    fatalError("MatrixDiag::operator *",
               "matrix sizes incompatible\n"
               "You're about to multiply a [%d,%d] and a [%d,%d] matrix",
               _rows, _cols, mat._rows, mat._cols);
  }

  MatrixDiag resu(_rows, mat._cols);

  for (long i = 0; i < _rows; i++)
    resu._data[i] = _data[i] * mat._data[i];

  return resu;
}

// multiplication d'une matrice par un vecteur
/*
  Cette methode calcule le produit d'une matrice A de taille N-x-N par un vecteur x de taille N. ceci genere un vecteur y de taille N.
  Cette methode retourne un vecteur
  Exemple :
  \code
  MatrixDiag t1;
  Vector v1,v2;
  v2=t1*v1; // produit
  \endcode
  - vec vecteur du second membre
  Return : vecteur resultant de l'operation de multiplication
*/
//-----------------------------------------------------------------------------
Vector MatrixDiag::operator*(const Vector &vec) const
//-----------------------------------------------------------------------------
{
  if (_cols != vec.size())
  {
    fatalError("MatrixDiag::operator *",
               "matrix and vector sizes incompatible\n"
               "You're about to multiply a [%d,%d] matrix and a [%d] vector",
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

//-----------------------------------------------------------------------------
void MatrixDiag::productBy(Vector &vec) const
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

// calcule la trace d'une matrice
/*
  Cette methode calcule la trace d'une matrice carree
  Return : valeur de la trace de la matrice
*/
//-----------------------------------------------------------------------------
double MatrixDiag::trace()
//-----------------------------------------------------------------------------
{
  double trace = 0.;
  for (long i = 0; i < _rows; i++)
    trace += _data[i];
  return trace;
}

// transposee d'une matrice
/*
  Cette methode renvoie la transposee d'une matrice
  Return : transposee de la matrice
*/
//-----------------------------------------------------------------------------
MatrixDiag MatrixDiag::transpose()
//-----------------------------------------------------------------------------
{
  return *this;
}

// resolution d'un systeme lineaire
/*
  Cette methode calcule la solution du systeme lineaire  \f[ A x = b \f] avec A matrice N-x-N et x et y vecteurs de taille N.

  - x vecteur du second membre
  Return : vecteur solution du systeme lineaire

*/
//-----------------------------------------------------------------------------
Vector MatrixDiag::getSolve(Vector &vec)
//-----------------------------------------------------------------------------
{
  Vector resu(_rows);

  if (_rows != vec.size())
  {
    fatalError("MatrixDiag::Solve",
               "matrix and vector sizes incompatible\n"
               "You're about to Solve a linear system with a [%d,%d]"
               " matrix and a [%d] vector",
               _rows, _cols, vec.size());
  }

  for (long component = 0; component < _rows; component++)
    resu(component) = vec(component) / _data[component];

  return resu;
}

//-----------------------------------------------------------------------------
void MatrixDiag::solve(Vector &vec)
//-----------------------------------------------------------------------------
{
  for (long component = 0; component < _rows; component++)
    vec(component) /= _data[component];
}

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

// egalite de deux matrices
/*
  Cette methode teste l'egalite de deux matrices
  Return : true si les deux matrices sont identiques, false dans la cas contraire
*/
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

// inegalite de deux matrices
/*
  Cette methode teste l'inegalite de deux matrices
  Return : true si les deux matrices sont differentes, false dans la cas contraire
*/
//-----------------------------------------------------------------------------
bool MatrixDiag::operator!=(const MatrixDiag &mat) const
//-----------------------------------------------------------------------------
{
  return !(*this == mat);
}

// sortie sur flux std::ofstream
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
void MatrixDiag::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)&_rows, sizeof(long));
  ofs.write((char *)_data, _rows * sizeof(double));
}

// lecture sur flux std::ifstream
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
void MatrixDiag::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  long rows;

  ifs.read((char *)&rows, sizeof(long));
  redim(rows);
  ifs.read((char *)_data, _rows * sizeof(double));
}

// sortie sur flux std::ofstream
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
std::ofstream &operator<<(std::ofstream &os, const MatrixDiag &mat)
//-----------------------------------------------------------------------------
{
  mat.write(os);
  return os;
}

// lecture sur flux std::ifstream
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

// Saves the content of a Matrix into a NumPy file
/*
  This method saves the content of a Matrix object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Matrix t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void MatrixDiag::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &_data[0], {_rows}, mode);
}

// Saves the content of a MatrixDiag into a NumPyZ file
/*
  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  MatrixDiag t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void MatrixDiag::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &_data[0], {_rows}, mode);
}

// Read the content of a MatrixDiag from a NumPy file
/*
  This method reads the content of a vec3D object from a NumPy file defined by its filename.

  Example
  \code
  MatrixDiag t;
  t.numpyRead("numpy.npy");
  \endcode
*/
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

// Read the content of a MatrixDiag from a NumPyZ file
/*
  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.

  Example
  \code
  MatrixDiag t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
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
