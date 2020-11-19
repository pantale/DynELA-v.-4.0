/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file DiscreteFunction.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class.
  A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_DiscreteFunction_h__
#define __dnlMaths_DiscreteFunction_h__

#include <List.h>
#include <Function.h>

/*!
  \brief Private class used in the DiscreteFunction class to define a couple of points.

  This one is not directly refered by the user.
  \ingroup dnlMaths
*/
class coupleReal
{
  friend class List<coupleReal *>;
  friend class DiscreteFunction;
  friend bool compare(coupleReal *t1, coupleReal *t2);
  double x; //!< Coordinate X of the point
  double y; //!< Coordinate Y of the point

  coupleReal();
  coupleReal(double x, double y);
  coupleReal(const coupleReal &X);
  ~coupleReal();
};

// fonction de comparison pour le tri des points
bool compare(coupleReal *t1, coupleReal *t2);

/*!
  \class DiscreteFunction
  \brief Definition of a discrete function in the DynELA FEM code

  This class defines a discrete function for using in the DynELA FEM code. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  These functions are for example used in the DynELA computer code to create boundaries conditions to varying with time limits.
  Combining times to the variable x, we can define a function to vary the boundary condition over time.
  One can also use this function to create nonlinear behavior laws of discretely defined materials with the following form: \f[ \sigma=f(\varepsilon) \f]
  Of course it's also used in the application \b DynELA \b curves for managing digital results as curves.
  \ingroup dnlMaths
*/
class DiscreteFunction : public Function
{
public:
  enum
  {
    noInit,
    averageInit,
    fisrtValueInit
  };

protected:
  List<coupleReal *> lpoints; //!< List of points
  bool Sort;                  //!< Boolean flage telling if the points are sorted or not

public:
  char style;  //!< First style variable for the discrete function. This is useful for storing information about this feature
  char style2; //!< Second style variable for the discrete function. This is useful for storing information about this feature

private:
  void sort();

public:
  DiscreteFunction(char *newName = NULL);
  DiscreteFunction(const DiscreteFunction &);
  ~DiscreteFunction();

#ifndef SWIG
  void print();
#endif

  bool intoBoundsX(double X);
  DiscreteFunction *absoluteValueCurve();
  DiscreteFunction *addCurves(DiscreteFunction *, long);
  DiscreteFunction *CentralDifferenceDerivate();
  DiscreteFunction *derivate();
  DiscreteFunction *distanceCurves(DiscreteFunction *);
  DiscreteFunction *distanceCurvesX(DiscreteFunction *);
  DiscreteFunction *distanceCurvesY(DiscreteFunction *);
  DiscreteFunction *divideCurves(DiscreteFunction *, long);
  DiscreteFunction *errorCurves(DiscreteFunction *, long);
  DiscreteFunction *EulerBackwardDerivate();
  DiscreteFunction *EulerForwardDerivate();
  DiscreteFunction *integrate();
  DiscreteFunction *lowPass_1(double, long, int);
  DiscreteFunction *lowPass_2(double, double, long, int);
  DiscreteFunction *movingAverage(int, int);
  DiscreteFunction *multiplyCurves(DiscreteFunction *, long);
  DiscreteFunction *offsetCurve(double);
  DiscreteFunction *remapCurve(double, double, double, double);
  DiscreteFunction *scaleCurve(double);
  DiscreteFunction *substractCurves(DiscreteFunction *, long);
  DiscreteFunction *xyMerge(DiscreteFunction *, long);
  DiscreteFunction *ZeroForcing1Derivate();
  DiscreteFunction *ZeroForcing2Derivate();
  double getAverage();
  double getInverseValue(double);
  double getSlope(double);
  double getValue(double);
  double getXPoint(long);
  double getYPoint(long);
  double maxX();
  double maxY();
  double minX();
  double minY();
  long getNumberOfPoints();
  long getSize();
  String convertToDynELASourceFile();
  void add(double, double);
  void autoSort(bool);
  void flush();
  void fromFile(String filename, int xCol, int yCol);
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void startPointAdd(double x, double y);
  void toGnuplot(String);
};

/*!
  \brief Auto sort x coordinates (default value is \b true)
  \param setSorted Boolean defining if coordinates are sorted
*/
//-----------------------------------------------------------------------------
inline void DiscreteFunction::autoSort(bool setSorted)
//-----------------------------------------------------------------------------
{
  Sort = setSorted;
}

/*!
  \brief Returns the number of points defining the discrete function
*/
//-----------------------------------------------------------------------------
inline long DiscreteFunction::getNumberOfPoints()
//-----------------------------------------------------------------------------
{
  return lpoints.getSize();
}

/*!
  \brief Delete all the points in the current dicrete function
*/
//-----------------------------------------------------------------------------
inline void DiscreteFunction::flush()
//-----------------------------------------------------------------------------
{
  lpoints.flush();
}

/*!
  \brief Return the X coordinate of the ith
  \param point refers the ith point
*/
//-----------------------------------------------------------------------------
inline double DiscreteFunction::getXPoint(long point)
//-----------------------------------------------------------------------------
{
  return lpoints(point)->x;
}

/*!
  \brief Return the Y coordinate of the ith
  \param point refers the ith point
*/
//-----------------------------------------------------------------------------
inline double DiscreteFunction::getYPoint(long point)
//-----------------------------------------------------------------------------
{
  return lpoints(point)->y;
}

#endif
