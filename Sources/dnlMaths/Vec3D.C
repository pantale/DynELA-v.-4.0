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
  \file Vec3D.C
  Declaration file for the 3D vector class

  This file is the declaration file for the 3D vector class. A 3D vector class is a vector with the following form:
  \f[ \overrightarrow{_data}=\left[\begin{array}{c}
  v_{1}\\
  v_{2}\\
  v_{3}
  \end{array}\right] \f]
  This type of data structure is useful for storing three-dimensional coordinates (for example nodal coordinates, vectors forces,...).

  \ingroup dnlMaths
*/

#include <fstream>

#include <Vec3D.h>
#include <NumpyInterface.h>
#include <Tensor2.h>
#include <SymTensor2.h>

// Constructor of the Vec3D class with initialization
/*
  This method is a  constructor of the Vec3D class. All components are initialized with given values.
  If all values are omitted, initial value is a zero vector, i.e. all components are set to zero.
  - v1 first value of the vector
  - v2 second value of the vector
  - v3 third value of the vector
*/
//-----------------------------------------------------------------------------
Vec3D::Vec3D(double v1, double v2, double v3)
//-----------------------------------------------------------------------------
{
    _data[0] = v1;
    _data[1] = v2;
    _data[2] = v3;
}

// Copy constructor of the Vec3D class
//-----------------------------------------------------------------------------
Vec3D::Vec3D(const Vec3D &vect)
//-----------------------------------------------------------------------------
{
    memcpy(_data, vect._data, 3 * sizeof(double));
}

// Destructor of the Vec3D class
//-----------------------------------------------------------------------------
Vec3D::~Vec3D()
//-----------------------------------------------------------------------------
{
}

// Multiplication of a vector by a scalar value
/*
  This method defines the multiplication of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \lambda . \overrightarrow{a} \f]

  Example :
  \code
  Vec3D t1, t2;
  double l;
  t2 = l * t1; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Vec3D operator*(const double lambda, const Vec3D &vect)
//-----------------------------------------------------------------------------
{
    Vec3D resu;

    resu._data[0] = lambda * vect._data[0];
    resu._data[1] = lambda * vect._data[1];
    resu._data[2] = lambda * vect._data[2];

    return resu;
}

// Multiplication of a vector by a scalar value
/*
  This method defines the multiplication of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} . \lambda \f]

  Example :
  \code
  Vec3D t1, t2;
  double l;
  t2=t1*l; // multiplication by a scalar
  \endcode
  - lambda Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator*(const double lambda) const
//-----------------------------------------------------------------------------
{
    return Vec3D(lambda * _data[0], lambda * _data[1], lambda * _data[2]);
}

// Division of a vector by a scalar value
/*
  This method defines the division of a vector by a scalar value
  The result of this operation is also a vector defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} / \lambda \f]

  Example :
  \code
  Vec3D t1, t2;
  double l;
  t1 = t2 / l; // division by a scalar
  \endcode
  - lambda Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::operator/(const double lambda) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_maths
    if (lambda == 0.)
    {
        fatalError("Vec3D:: operator /", "divide by zero");
    }
#endif

    return Vec3D(_data[0] / lambda, _data[1] / lambda, _data[2] / lambda);
}

// Test the equality of two vectors
/*
  This method tests the equality of two vectors.
  It returns \ref true if all components of the two vectors are equals, \ref false on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
bool Vec3D::operator==(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
    if (_data[0] != vect._data[0])
        return false;
    if (_data[1] != vect._data[1])
        return false;
    if (_data[2] != vect._data[2])
        return false;

    return true;
}

// Test the inequality of two vectors
/*
  This method tests the inequality of two vectors.
  It returns \ref true if all components of the two vectors are differents, \ref false on other case.
  Return : \ref true or \ref false depending on the result of the test.
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
bool Vec3D::operator!=(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
    return !(*this == vect);
}

// Maximum component in a vector
/*
  This method returns the maximum component of a vector
*/
//-----------------------------------------------------------------------------
double Vec3D::maxValue()
//-----------------------------------------------------------------------------
{
    if (_data[0] >= _data[1])
    {
        if (_data[0] >= _data[2])
            return _data[0];
        else
            return _data[2];
    }
    else
    {
        if (_data[1] >= _data[2])
            return _data[1];
        else
            return _data[2];
    }
}

// Minimum component in a vector
/*
  This method returns the minimum component of a vector
*/
//-----------------------------------------------------------------------------
double Vec3D::minValue()
//-----------------------------------------------------------------------------
{
    if (_data[0] <= _data[1])
    {
        if (_data[0] <= _data[2])
            return _data[0];
        else
            return _data[2];
    }
    else
    {
        if (_data[1] <= _data[2])
            return _data[1];
        else
            return _data[2];
    }
}

// Maximum absolute component in a vector
/*
  This method returns the maximum absolute component of a vector
*/
//-----------------------------------------------------------------------------
double Vec3D::maxAbsoluteValue()
//-----------------------------------------------------------------------------
{
    if (dnlAbs(_data[0]) >= dnlAbs(_data[1]))
    {
        if (dnlAbs(_data[0]) >= dnlAbs(_data[2]))
            return dnlAbs(_data[0]);
        else
            return dnlAbs(_data[2]);
    }
    else
    {
        if (dnlAbs(_data[1]) >= dnlAbs(_data[2]))
            return dnlAbs(_data[1]);
        else
            return dnlAbs(_data[2]);
    }
}

// Minimum absolute component in a vector
/*
  This method returns the minimum absolute component of a vector
*/
//-----------------------------------------------------------------------------
double Vec3D::minAbsoluteValue()
//-----------------------------------------------------------------------------
{
    if (dnlAbs(_data[0]) <= dnlAbs(_data[1]))
    {
        if (dnlAbs(_data[0]) <= dnlAbs(_data[2]))
            return dnlAbs(_data[0]);
        else
            return dnlAbs(_data[2]);
    }
    else
    {
        if (dnlAbs(_data[1]) <= dnlAbs(_data[2]))
            return dnlAbs(_data[1]);
        else
            return dnlAbs(_data[2]);
    }
}

// Test if a given point is inside a given box defined by two opposite points
/*
  This method returns true or false depending if the point is or not inside the bounding box.
  - vect_m first vector to use for the operation
  - vect_M second vector to use for the operation
  Return : true or false depending on the result of the test.
*/
//-----------------------------------------------------------------------------
bool Vec3D::isInsideBox(const Vec3D &vect_m, const Vec3D &vect_M) const
//-----------------------------------------------------------------------------
{
    if (_data[0] < vect_m._data[0] || vect_M._data[0] < _data[0])
        return false;
    if (_data[1] < vect_m._data[1] || vect_M._data[1] < _data[1])
        return false;
    if (_data[2] < vect_m._data[2] || vect_M._data[2] < _data[2])
        return false;
    return true;
}

// Dot product of two vectors
/*
  This method computes the dot product of two vectors defined by:
  \f[ s = \overrightarrow{a} . \overrightarrow{b} \f]
  - vect second vector to use for the operation
  Return : Result of the dot product of the two vectors
*/
//-----------------------------------------------------------------------------
double Vec3D::dotProduct(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
    return (_data[0] * vect._data[0] + _data[1] * vect._data[1] + _data[2] * vect._data[2]);
}

// Vectorial product of two vectors
/*
  This method computes the vectorialProduct product of two vectors defined by:
  \f[ \overrightarrow{_data} = \overrightarrow{a} \land \overrightarrow{b} \f]
  - vect second vector to use for the operation
  Return : Vector result of the vectorialProduct product of the two vectors
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::vectorialProduct(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
    return Vec3D(_data[1] * vect._data[2] - _data[2] * vect._data[1], _data[2] * vect._data[0] - _data[0] * vect._data[2], _data[0] * vect._data[1] - _data[1] * vect._data[0]);
}

// Components product of two vectors
/*
  This method computes the component product of two vectors defined by:
  \f[ v_i = a_i * b_i \f]
*/
//-----------------------------------------------------------------------------
Vec3D Vec3D::ewProduct(const Vec3D &vect) const
//-----------------------------------------------------------------------------
{
    return Vec3D(_data[0] * vect._data[0], _data[1] * vect._data[1], _data[2] * vect._data[2]);
}

// Components product of a vec3D and a Tensor2
/*
  This method computes the component product of a vec3D and a Tensor2:
  \f[ A_{ij} = a_i * B_{ij} \f]
  - v2 Second order tensor to use for the operation
  Return : A second order tensor resulting from the computation
*/
//-----------------------------------------------------------------------------
Tensor2 Vec3D::ewProduct(const Tensor2 &tensor) const
//-----------------------------------------------------------------------------
{
    Tensor2 result;

    result._data[0] = _data[0] * tensor._data[0];
    result._data[1] = _data[0] * tensor._data[1];
    result._data[2] = _data[0] * tensor._data[2];
    result._data[3] = _data[1] * tensor._data[3];
    result._data[4] = _data[1] * tensor._data[4];
    result._data[5] = _data[1] * tensor._data[5];
    result._data[6] = _data[2] * tensor._data[6];
    result._data[7] = _data[2] * tensor._data[7];
    result._data[8] = _data[2] * tensor._data[8];

    return result;
}

// Dyadic product of two vectors
/*
  This method defines the dyadic product of 2 vectors that gives a second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ T = \overrightarrow{a} \otimes \overrightarrow{b} \f]

  Exemple :
  \code
  Vec3D v1,v2;
  Tensor2 t1 = v1.dyadicProduct(v2); // dyadic product
  \endcode
  - v2 Second vector to use for the operation
  Return : A second order tensor resulting from the computation
*/
//-----------------------------------------------------------------------------
Tensor2 Vec3D::dyadicProduct(const Vec3D &vector) const
//-----------------------------------------------------------------------------
{
    Tensor2 result;

    result._data[0] = _data[0] * vector._data[0];
    result._data[1] = _data[0] * vector._data[1];
    result._data[2] = _data[0] * vector._data[2];
    result._data[3] = _data[1] * vector._data[0];
    result._data[4] = _data[1] * vector._data[1];
    result._data[5] = _data[1] * vector._data[2];
    result._data[6] = _data[2] * vector._data[0];
    result._data[7] = _data[2] * vector._data[1];
    result._data[8] = _data[2] * vector._data[2];

    return result;
}

// Dyadic product of two vectors
/*
  This method defines the dyadic product of 2 vectors that gives a second order tensor.
  The result of this operation is a second order tensor defined by:
  \f[ T = \overrightarrow{a} \otimes \overrightarrow{b} \f]

  Exemple :
  \code
  Vec3D v1,v2;
  Tensor2 t1 = v1.dyadicProduct(v2); // dyadic product
  \endcode
  - v2 Second vector to use for the operation
  Return : A second order tensor resulting from the computation
*/
//-----------------------------------------------------------------------------
SymTensor2 Vec3D::dyadicProduct() const
//-----------------------------------------------------------------------------
{
    SymTensor2 result;

    result._data[0] = _data[0] * _data[0];
    result._data[1] = _data[0] * _data[1];
    result._data[2] = _data[0] * _data[2];
    result._data[3] = _data[1] * _data[1];
    result._data[4] = _data[1] * _data[2];
    result._data[5] = _data[2] * _data[2];

    return result;
}

// Writes a vector in a binary flux for storage
/*
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Vec3D t;
  t.write(pfile);
  t.close();
  \endcode
  - ofs Output file stream to use for writting operation
*/
//-----------------------------------------------------------------------------
void Vec3D::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
    ofs.write((char *)_data, 3 * sizeof(double));
}

// Reads a vector from a binary flux for storage
/*
  This method is used to read the components of a vector in a binary file.

  Example :
  \code
  ifstream pfile("file");
  Vec3D t;
  t.read(pfile);
  \endcode
  - ifs Input file stream to use for reading operation
*/
//-----------------------------------------------------------------------------
Vec3D &Vec3D::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
    ifs.read((char *)_data, 3 * sizeof(double));
    return *this;
}

/*
  This method is used to store the components of a vector in a binary file.

  Example :
  \code
  ofstream pfile("file");
  Vec3D t;
  pfile << t;
  \endcode
  - os Output file stream to use for writting operation
  - vect Second second order tensor to use for the operation
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Vec3D &vect)
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
  Vec3D t;
  pfile >> t;
  \endcode
  - os Input file stream to use for reading operation
  - vect Second vector to use for the operation
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Vec3D &vect)
//-----------------------------------------------------------------------------
{
    vect.read(is);
    return is;
}

// Display the content of a Vec3D
/*
  This method is a surdefintion of the << operator

  Example
  \code
  Vec3D t;
  cout << t << endl;
  \endcode
  - os Output flux
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Vec3D &vector)
//-----------------------------------------------------------------------------
{
    vector.print(os);
    return os;
}

// Display the content of a Vec3D
/*
  This method is a surdefintion of the << operator

  Example
  \code
  Vec3D t;
  t.print(os);
  \endcode
  - os Output flux
*/
//-----------------------------------------------------------------------------
void Vec3D::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
    os << "Vec3D coordinates={";
    os << _data[0];
    for (int i = 1; i < 3; i++)
    {
        os << "," << _data[i];
    }
    os << "}";
}

// Saves the content of a Vec3D into a NumPy file
/*
  This method saves the content of a vec3D object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Vec3D t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Vec3D::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
    std::string mode = "a";
    if (initialize)
        mode = "w";
    NumpyInterface::npySave(filename, &_data[0], {3}, mode);
}

// Saves the content of a Vec3D into a NumPyZ file
/*
  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Vec3D t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Vec3D::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
    std::string mode = "a";
    if (initialize)
        mode = "w";
    NumpyInterface::npzSave(filename, name, &_data[0], {3}, mode);
}

// Read the content of a Vec3D from a NumPy file
/*
  This method reads the content of a vec3D object from a NumPy file defined by its filename.

  Example
  \code
  Vec3D t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void Vec3D::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
    NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
    if (arr.num_vals != 3)
    {
        std::cout << "ERROR\n";
    }
    memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}

// Read the content of a Vec3D from a NumPyZ file
/*
  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.

  Example
  \code
  Vec3D t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void Vec3D::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
    NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
    if (arr.num_vals != 3)
    {
        std::cout << "ERROR\n";
    }
    memcpy(_data, arr.data<double *>(), arr.num_vals * arr.word_size);
}
