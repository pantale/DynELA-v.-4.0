/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*
  \file DiscreteFunction.C
  Definition file for the discrete function class

  This file is the definition file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#include <Macros.h>
#include <Exception.h>

#include <DiscreteFunction.h>
#include <NumpyInterface.h>

/*
  Default constructor for the coupleReal class
*/
//-----------------------------------------------------------------------------
coupleReal::coupleReal()
//-----------------------------------------------------------------------------
{
}

/*
  Constructor for the coupleReal class with initailisation of the coords of the point
  - abscissa : X coordinate of the new point
  - ordinate : Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
coupleReal::coupleReal(double abscissa, double ordinate)
//-----------------------------------------------------------------------------
{
    x = abscissa;
    y = ordinate;
}

/*
  Copy constructor for the coupleReal class
*/
//-----------------------------------------------------------------------------
coupleReal::coupleReal(const coupleReal &X)
//-----------------------------------------------------------------------------
{
}

/*
  Destructor for the coupleReal class
*/
//-----------------------------------------------------------------------------
coupleReal::~coupleReal()
//-----------------------------------------------------------------------------
{
}

/*
  Default constructor for the DiscreteFunction class
*/
//-----------------------------------------------------------------------------
DiscreteFunction::DiscreteFunction(char *newName) : Function(newName)
//-----------------------------------------------------------------------------
{
    style = 0;
    style2 = 0;
    Sort = true;
}

/*
  Copy constructor for the DiscreteFunction class
*/
//-----------------------------------------------------------------------------
DiscreteFunction::DiscreteFunction(const DiscreteFunction &X)
//-----------------------------------------------------------------------------
{
}

/*
  Add a new point (X,Y) to the current discrete function

  This method add a new point to the current discrete function.
  Depending of the \ref Sort variable, the new point is added at the end of the curve or inserted at it's right place into the curve depending on it's X value.
  - abscissa : X coordinate of the new point
  - ordinate : Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::add(double abscissa, double ordinate)
//-----------------------------------------------------------------------------
{
    // init memoire
    coupleReal *ppoint = new coupleReal;

    // affectation des coords
    ppoint->x = abscissa;
    ppoint->y = ordinate;

    // ajout a la liste
    lpoints << ppoint;

    if ((lpoints.size() > 1) && (Sort))
        if (abscissa < lpoints(lpoints.size() - 2)->x)
        {
            lpoints.sort(compare);
        }
}

/*
  Add a new point (X,Y) to the current discrete function

  This method add a new point as the first point of the current discrete function.
  Depending of the \ref Sort variable, the new point is added at the beginning of the curve or inserted at it's right place into the curve depending on it's X value.
  - abscissa : X coordinate of the new point
  - ordinate : Y coordinate of the new point
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::startPointAdd(double abscissa, double ordinate)
//-----------------------------------------------------------------------------
{
    // if autosort then add at the end simply
    if (Sort)
    {
        add(abscissa, ordinate);
        return;
    }

    // init memoire
    coupleReal *ppoint = new coupleReal;

    // affectation des coords
    ppoint->x = abscissa;
    ppoint->y = ordinate;

    // ajout à la liste au debut
    lpoints.insert(ppoint, 0);
}

/*
  Destructor of the DiscreteFunction class
*/
//-----------------------------------------------------------------------------
DiscreteFunction::~DiscreteFunction()
//-----------------------------------------------------------------------------
{
}

/*
  Returns the number of points defining the discrete function

  \see getNumberOfPoints()
*/
//-----------------------------------------------------------------------------
long DiscreteFunction::size()
//-----------------------------------------------------------------------------
{
    return lpoints.size();
}

/*
  Verify that the given X coordinate is in the X range of the function.

  - abscissa : X coordinate of the point to test
  Return : true if the given coordinate X in in the correct range
*/
//-----------------------------------------------------------------------------
bool DiscreteFunction::intoBoundsX(double abscissa)
//-----------------------------------------------------------------------------
{
    return (abscissa >= lpoints(0)->x && abscissa <= lpoints(lpoints.size() - 1)->x);
}

/*
  Compute the Y coordinate corresponding to the given X coordinate

  This method computes the Y coordinate corresponding to the given X coordinate using a linear interpolation method.
  - abscissa : X coordinate of the point to compute
  Return : Y coordinate of the corresponding point
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::getValue(double abscissa)
//-----------------------------------------------------------------------------
{
    long i, nb;
    coupleReal *pt;
    coupleReal *pt0;

    if (abscissa < lpoints(0)->x || abscissa > lpoints(lpoints.size() - 1)->x)
        fatalError("DiscreteFunction::getValue(double abscissa)", "value %lf is out of bounds [%lf,%lf]\n", abscissa, lpoints(0)->x, lpoints(lpoints.size() - 1)->x);

    nb = lpoints.size();
    pt0 = lpoints.first();

    for (i = 1; i < nb; i++)
    {
        pt = lpoints(i);
        if (pt->x >= abscissa)
        {
            if (pt->x == abscissa)
            {
                return pt->y;
            }
            return ((abscissa - pt0->x) / (pt->x - pt0->x)) * (pt->y - pt0->y) + pt0->y;
        }
        pt0 = pt;
    }

    return 0;
}

/*
  Compute the X coordinate corresponding to the given Y coordinate

  This method computes the X coordinate corresponding to the given Y coordinate using a linear interpolation method.
  - abscissa Y coordinate of the point to compute
  Return : X coordinate of the corresponding point
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::inverseValue(double ordinate)
//-----------------------------------------------------------------------------
{
    long i, start = 0, stop = 0;

    if (ordinate > maxY() || ordinate < minY())
        fatalError("DiscreteFunction::inverseValue(double abscissa)", "value %lf is out of bounds [%lf,%lf]\n", ordinate, minY(), maxY());

    if (lpoints(0)->y == ordinate)
    {
        return lpoints(0)->x;
    }
    if (lpoints(0)->y < ordinate)
    {
        for (i = 1; i < lpoints.size(); i++)
        {
            if (lpoints(i)->y >= ordinate)
            {
                if (lpoints(i)->y == ordinate)
                {
                    return lpoints(i)->x;
                }
                start = i - 1;
                i = lpoints.size();
            }
        }
    }
    else
    {
        for (i = 1; i < lpoints.size(); i++)
        {
            if (lpoints(i)->y <= ordinate)
            {
                if (lpoints(i)->y == ordinate)
                {
                    return lpoints(i)->x;
                }
                start = i - 1;
                i = lpoints.size();
            }
        }
    }

    stop = start + 1;

    // sinon, on interpole
    return ((ordinate - lpoints(start)->y) / (lpoints(stop)->y - lpoints(start)->y)) * (lpoints(stop)->x - lpoints(start)->x) + lpoints(start)->x;
}

/*
  Compute the slope of the discrete function at a given X coordinate

  This method is used to compute the slope of the discrete function at a given X coordinate.
  The slope of the function is more or less equal to the derivative of the function at the given point.
  - abscissa : X coordinate of the point to compute
  Return : Slope of the function at the corresponding point
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::getSlope(double abscissa)
//-----------------------------------------------------------------------------
{
    long i, nb;
    coupleReal *pt;
    coupleReal *pt0;

    if (abscissa < lpoints(0)->x || abscissa > lpoints(lpoints.size() - 1)->x)
        fatalError("DiscreteFunction::getSlope(double abscissa)", "value %lf is out of bounds [%lf,%lf]\n", abscissa, lpoints(0)->x, lpoints(lpoints.size() - 1)->x);

    nb = lpoints.size();
    pt0 = lpoints.first();

    for (i = 1; i < nb; i++)
    {
        pt = lpoints(i);
        if (pt->x >= abscissa)
        {
            return (pt->y - pt0->y) / (pt->x - pt0->x);
        }
        pt0 = pt;
    }

    return 0;
}

/*
  Printout the discrete function definition
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::print()
//-----------------------------------------------------------------------------
{
    long i;
    printf("Discrete Fonction : %s\n", name.c_str());
    for (i = 0; i < lpoints.size(); i++)
    {
        printf("point %ld [%f,%f]\n", i + 1, lpoints(i)->x, lpoints(i)->y);
    }
}

/*
  Returns the maximum X value of the discrete function

  Return : Maximum X value of the discrete function
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::maxX()
//-----------------------------------------------------------------------------
{
    return (lpoints(lpoints.size() - 1))->x;
}

/*
  Returns the minimum X value of the discrete function

  Return : Minimum X value of the discrete function
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::minX()
//-----------------------------------------------------------------------------
{
    return (lpoints(0))->x;
}

/*
  Returns the maximum Y value of the discrete function

  Return : Maximum Y value of the discrete function
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::maxY()
//-----------------------------------------------------------------------------
{
    double val;
    val = (lpoints(0))->y;
    for (long i = 1; i < lpoints.size(); i++)
    {
        val = ((lpoints(i))->y > val ? (lpoints(i))->y : val);
    }
    return val;
}

/*
  Returns the minimum Y value of the discrete function

  Return : Minimum Y value of the discrete function
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::minY()
//-----------------------------------------------------------------------------
{
    double val;
    val = (lpoints(0))->y;
    for (long i = 1; i < lpoints.size(); i++)
    {
        val = ((lpoints(i))->y < val ? (lpoints(i))->y : val);
    }
    return val;
}

//-----------------------------------------------------------------------------
void DiscreteFunction::sort()
//-----------------------------------------------------------------------------
{
    lpoints.sort(compare);
}

//-----------------------------------------------------------------------------
bool compare(coupleReal *t1, coupleReal *t2)
//-----------------------------------------------------------------------------
{
    // comparaison sur x
    return (t1->x > t2->x);
}

/*
  Save the current discrete function for using with the Gnuplot software

  This method saves the current discrete function for using with the Gnuplot software.
  - filename Name of the datafile to use for writting the discrete function
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::toGnuplot(String filename)
//-----------------------------------------------------------------------------
{
    FILE *pfile;

    pfile = fopen(filename.c_str(), "w");
    if (pfile == NULL)
    {
        fatalError("open Gnuplot file", "failed to open file for write");
    }
    fprintf(pfile, "#DynELA_plot :%s\n", name.c_str());
    fprintf(pfile, "#plotted :%s\n", name.c_str());

    for (long i = 0; i < lpoints.size(); i++)
    {
        fprintf(pfile, "%10.6E %10.6E\n", lpoints(i)->x, lpoints(i)->y);
    }
    fclose(pfile);
}

/*
  Read a discrete function from an ASCII file

  This method reads a discrete function from an ASCII file.
  The X and Y values of the curve are in separate columns on the same line.
  Several values may be given on the line, the number of values on a line is specified by the number of columns that contain data on X and Y axes.
  - filename Name of the datafile to use for reading the discrete function
  - xCol Reference number of the column containing the values in the abscissa
  - yCol Reference number of the column containing the values in the ordinate
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::fromFile(String filename, int xCol, int yCol)
//-----------------------------------------------------------------------------
{
    FILE *pfile;
    double x, y;
    String st;

    try
    {
        pfile = fopen(filename.c_str(), "r");
        IfException(pfile == NULL, StandardExceptions::FileNotFound);
    }
    catch (Exception &e)
    {
        e.print();
        exit(-1);
    }

    while (st.scanFileLine(pfile))
    {
        if (st[0] != '#')
        {
            x = st.getRealAtPos(xCol);
            y = st.getRealAtPos(yCol);
            add(x, y);
        }
    }
    fclose(pfile);
}

/*
  Remap the discrete function on X or Y axis

  This method is used to remap a discrete function on X or Y axis.
  Ranges of the X and Y axis are modified by the user.
  This method returns a new discrete function with the same name as the current one + "_remap".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_remap".
  - xm Minimum value on the X axis
  - xM Maximum value on the X axis
  - ym Minimum value on the Y axis
  - yM Maximum value on the Y axis
  Return : Remapped discrete function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::remapCurve(double xm, double xM, double ym, double yM)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *remapped = new DiscreteFunction;
    double x, y;
    double mx, Mx, my, My;
    mx = minX();
    Mx = maxX();
    my = minY();
    My = maxY();
    long sz = lpoints.size();

    // recopie de la fonction
    remapped->name = name + "_remap";

    for (long i = 0; i < sz; i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        if (xm != xM)
        {
            x = (x - mx) * (xM - xm) / (Mx - mx) + xm;
        }
        if (ym != yM)
        {
            y = (y - my) * (yM - ym) / (My - my) + ym;
        }

        remapped->add(x, y);
    }

    return remapped;
}

/*
  Derivate a discrete function

  This method computes the derivative of a discrete function.
  This method returns a new discrete function with the same name as the current one + "_derivative".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_derivative".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 2)
        fatalError("Derivate function",
                   "can't derivate a function with less than two points");

    derive->add(lpoints(0)->x, (lpoints(1)->y - lpoints(0)->y) / (lpoints(1)->x - lpoints(0)->x));

    for (long i = 1; i < sz - 1; i++)
        derive->add(lpoints(i)->x, (lpoints(i)->y - lpoints(i - 1)->y) / (lpoints(i)->x - lpoints(i - 1)->x));

    derive->add(lpoints(sz - 1)->x, (lpoints(sz - 1)->y - lpoints(sz - 2)->y) / (lpoints(sz - 1)->x - lpoints(sz - 2)->x));

    derive->name = name + "_derivate";

    return derive;
}

/*
  Euler Backward derivate a discrete function

  This method computes the Euler Backward derivative of a discrete function.
  The Euler Backward derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x)-f(x-dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_EBderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_EBderivate".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::EulerBackwardDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 2)
        fatalError("Euler Backward Derivate function",
                   "can't derivate a function with less than two points");

    for (long i = 1; i < sz; i++)
        derive->add(lpoints(i)->x, (lpoints(i)->y - lpoints(i - 1)->y) / (lpoints(i)->x - lpoints(i - 1)->x));

    derive->name = name + "_EBderivate";

    return derive;
}

/*
  Euler Forward derivate a discrete function

  This method computes the Euler Forward derivative of a discrete function.
  The Euler Forward derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x+dx)-f(x)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_EFderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_EFderivate".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::EulerForwardDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 2)
        fatalError("Euler Forward Derivate function",
                   "can't derivate a function with less than two points");

    for (long i = 0; i < sz - 1; i++)
        derive->add(lpoints(i)->x, (lpoints(i + 1)->y - lpoints(i)->y) / (lpoints(i + 1)->x - lpoints(i)->x));

    derive->name = name + "_EFderivate";

    return derive;
}

/*
  Central-Difference derivate a discrete function

  This method computes the Central-Difference derivative of a discrete function.
  The Central-Difference derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{f(x+dx)-f(x-dx)}{2dx} \f]
  This method returns a new discrete function with the same name as the current one + "_CDderivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_CDderivate".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::CentralDifferenceDerivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 3)
        fatalError("Central Difference Derivate function",
                   "can't derivate a function with less than two points");

    for (long i = 1; i < sz - 1; i++)
        derive->add(lpoints(i)->x, (lpoints(i + 1)->y - lpoints(i - 1)->y) / (lpoints(i + 1)->x - lpoints(i - 1)->x));

    derive->name = name + "_CDderivate";

    return derive;
}

/*
  Zero Forcing 1 derivate a discrete function

  This method computes the Zero Forcing 1 derivative of a discrete function.
  The Zero Forcing 1 derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{0.2047f(x+dx)+0.886f(x)-1.386f(x-dx)+0.2953f(x-2dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_ZF1derivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_ZF1derivate".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::ZeroForcing1Derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 4)
        fatalError("Zero forcing # 1 Derivate function",
                   "can't derivate a function with less than two points");

    for (long i = 2; i < sz - 1; i++)
        derive->add(lpoints(i)->x, 3.0 * (0.2047 * lpoints(i + 1)->y + 0.886 * lpoints(i)->y - 1.386 * lpoints(i - 1)->y + 0.2953 * lpoints(i - 2)->y) / (lpoints(i + 1)->x - lpoints(i - 2)->x));

    derive->name = name + "_ZF1derivate";

    return derive;
}

/*
  Zero Forcing 2 derivate a discrete function

  This method computes the Zero Forcing 2 derivative of a discrete function.
  The Zero Forcing 2 derivate of a function is defined by the following equation: \f[ \frac{df(x)}{dx}=\frac{-0.2047f(x+dx)-0.886f(x)+1.386f(x-dx)-0.2953f(x-2dx)}{dx} \f]
  This method returns a new discrete function with the same name as the current one + "_ZF2derivate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_ZF2derivate".
  Return : Derivative of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::ZeroForcing2Derivate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *derive = new DiscreteFunction;
    long sz = lpoints.size();

    if (sz < 4)
        fatalError("Zero forcing # 2 Derivate function",
                   "can't derivate a function with less than two points");

    for (long i = 1; i < sz - 2; i++)
        derive->add(lpoints(i)->x, 3.0 * (-0.2047 * lpoints(i - 1)->y - 0.886 * lpoints(i)->y + 1.386 * lpoints(i + 1)->y - 0.2953 * lpoints(i + 2)->y) / (lpoints(i + 2)->x - lpoints(i - 1)->x));

    derive->name = name + "_ZF2derivate";

    return derive;
}

/*
  Integration of a discrete function

  This method computes the integral of a discrete function using the Simpson method.
  This method returns a new discrete function with the same name as the current one + "_integrate".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_integrate".
  Return : Integrate of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::integrate()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *integr = new DiscreteFunction;
    long sz = lpoints.size();
    double intg;

    if (sz < 2)
        fatalError("Integrate function",
                   "can't integrate a function with less than two points");

    integr->add(lpoints(0)->x, 0.0);
    intg = 0.0;

    for (long i = 1; i < sz; i++)
    {
        intg += (lpoints(i)->x - lpoints(i - 1)->x) * (lpoints(i)->y + lpoints(i - 1)->y) / 2;
        integr->add(lpoints(i)->x, intg);
    }

    integr->name = name + "_integrate";

    return integr;
}

/*
  Mobile average of a discrete function

  This method computes the mobile average of a discrete function.
  If \f$ f(x) \f$ is the original discrete function, the the mobile average curve \f$ f_{m}(x) \f$ is defined by the following:
  \f[ f_{m}(x)=\frac{1}{n_{1}+n_{2}+1}\sum ^{i=x+n_{2}}_{i=x-n_{1}}f(i) \f]
  where \f$ n_{1} \f$ et \f$ n_{2} \f$ are corresponding to the given parameters \c before et \c after respectively.
  If \f$ n_{1}=n_{2} \f$ the mobile average is centered, else it will be non-centered.
  The number of points of the filtered curve is less than the number of points of the original curve and is defined by \f$ n=1+n_{1}+n_{2} \f$.
  This method returns a new discrete function with the same name as the current one + "_movingAverage".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_movingAverage".
  Return : Mobile average of the function
  - before Number of points to consider on the left side
  - after Number of points to consider on the right side
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::movingAverage(int before, int after)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *average = new DiscreteFunction;
    long sz = lpoints.size();
    double valeur;

    for (long i = before; i < sz - after; i++)
    {
        valeur = 0.0;
        for (long j = i - before; j <= i + after; j++)
        {
            valeur += lpoints(j)->y;
        }
        average->add(lpoints(i)->x, valeur / (after + before + 1));
    }

    average->name = name + "_movingAverage";

    return average;
}

/*
  Combine two functions into a new one

  This method combines two functions into a new one.
  From the given two curves \f$ y_{1}=f_{1}(x) \f$ and \f$ y_{2}=f_{2}(x) \f$ we create the new curve \f$ y_{2}=f(y_{1}) \f$
  This method returns a new discrete function with the same name as the current ones + "_vs_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_vs_curve2".
  Return : Combined discrete function
  - fonction Second function to combine with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::xyMerge(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(getValue(x), fonction->getValue(x));
    }

    mix->name = fonction->name + "_vs_" + name;
    return mix;
}

/*
  Absolute value of a discrete function

  This method creates a new fonction from the given one as the absolute value of the function defined by \f$ f2=ABS(f1) \f$
  This method returns a new discrete function with the same name as the current one + "_abs".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_abs".
  Return : Absolute value of the discrete function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::absoluteValueCurve()
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (long i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, dnlAbs(lpoints(i)->y));
    }

    // apply the name
    mix->name = "abs_" + name;

    // returns the function
    return mix;
}

/*
  Multiplication by a scalar value of a function

  This method creates a new fonction from the given one as the product of the given function by a scalar value \f$ f2=\lambda.f1 \f$
  This method returns a new discrete function with the same name as the current one + "scale_".
  Therefore, if the current function name is "curve" then the new created function name will be "scale_curve".
  Return : The new function
  - factor Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::scaleCurve(double factor)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (long i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, factor * lpoints(i)->y);
    }

    // apply the name
    mix->name = "scale_" + name;

    // returns the function
    return mix;
}

/*
  Offset of a discrete function

  This method creates a new fonction from the given one with a offset on the Y axis by a scalar value \f$ f2=f1+Cte \f$
  This method returns a new discrete function with the same name as the current one + "offset_".
  Therefore, if the current function name is "curve" then the new created function name will be "offset_curve".
  Return : The new function
  - offset Offset of the function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::offsetCurve(double offset)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;

    for (long i = 0; i < lpoints.size(); i++)
    {
        mix->add(lpoints(i)->x, offset + lpoints(i)->y);
    }

    // apply the name
    mix->name = "offset_" + name;

    // returns the function
    return mix;
}

/*
  Add two discrete functions

  This method creates a new fonction from the given two ones as the sum of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)+f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_plus_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_plus_curve2".
  Return : The new function
  - fonction Second function to work with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::addCurves(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) + fonction->getValue(x));
    }

    mix->name = name + "_plus_" + fonction->name;
    return mix;
}

/*
  Substract two discrete functions

  This method creates a new fonction from the given two ones as the sustraction of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)-f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_minus_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_minus_curve2".
  Return : The new function
  - fonction Second function to work with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::substractCurves(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) - fonction->getValue(x));
    }

    mix->name = name + "_minus_" + fonction->name;
    return mix;
}

/*
  Multiply two discrete functions

  This method creates a new fonction from the given two ones as the product of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=f_1(x)*f_2(x) \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_mult_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_mult_curve2".
  Return : The new function
  - fonction Second function to work with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *
DiscreteFunction::multiplyCurves(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) * fonction->getValue(x));
    }

    mix->name = name + "_mult_" + fonction->name;
    return mix;
}

/*
  Divides two discrete functions

  This method creates a new fonction from the given two ones as the division of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=\frac{f_1(x)}{f_2(x)} \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_div_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_div_curve2".
  Return : The new function
  \warning Function \f$ f_2(x) \f$ must not cut the X axis or it will generate a division by zero.
  - fonction Second function to work with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::divideCurves(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(x, getValue(x) / fonction->getValue(x));
    }

    mix->name = name + "_div_" + fonction->name;
    return mix;
}

/*
  Relative error of two discrete functions

  This method creates a new fonction from the given two ones as the relative error of the two functions on the common X range.
  Therefore, if the \f$ f_1(x) \f$ function is defined on the range [0;10] and \f$ f_2(x) \f$ is defined on the range [2;15] the resulting function \f$ f(x)=\frac {(f_2(x) - f_1(x))}{ f_1(x)} \f$ will be defined on the range [2;10].
  This method returns a new discrete function with the same name as the current ones + "_error_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_error_curve2".
  Return : The new function
  - fonction Second function to work with
  - points Number of points to generate for the new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::errorCurves(DiscreteFunction *fonction, long points)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *mix = new DiscreteFunction;
    double mX, MX;
    double dx;

    mX = dnlMax(minX(), fonction->minX());
    MX = dnlMin(maxX(), fonction->maxX());

    dx = (MX - mX) / points;

    for (double x = mX; x <= MX; x += dx)
    {
        mix->add(x, (fonction->getValue(x) - getValue(x)) / getValue(x));
    }

    mix->name = name + "_error_" + fonction->name;
    return mix;
}

/*
  Minimal distance between two curves on the X axis

  This method creates a new fonction from the given two ones as the minimal distance between the two curves on the X axis.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear abscissa in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distanceX_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distanceX_curve2".
  - fonction Second function to work with
  Return : The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurvesX(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    double dist;
    double dist2;
    double x, y;
    double x0 = 0.0; //,y0=0.0;

    for (long i = 0; i < lpoints.size(); i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = dnlSquare(x - fonction->lpoints(0)->x) + dnlSquare(y - fonction->lpoints(0)->y);
        for (long ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = dnlSquare(x - fonction->lpoints(ii)->x) + dnlSquare(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                x0 = (fonction->lpoints(ii)->x + x) / 2;
            }
        }
        distfunct->add(x0, sqrt(dist));
    }
    distfunct->name = name + "_distanceX_" + fonction->name;
    return distfunct;
}

/*
  Minimal distance between two curves on the Y axis

  This method creates a new fonction from the given two ones as the minimal distance between the two curves on the Y axis.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear ordinate in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distanceY_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distanceY_curve2".
  - fonction Second function to work with
  Return : The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurvesY(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    double dist;
    double dist2;
    double x, y;
    double y0 = 0.0;

    for (long i = 0; i < lpoints.size(); i++)
    {
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = dnlSquare(x - fonction->lpoints(0)->x) + dnlSquare(y - fonction->lpoints(0)->y);
        for (long ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = dnlSquare(x - fonction->lpoints(ii)->x) + dnlSquare(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                y0 = (fonction->lpoints(ii)->y + y) / 2;
            }
        }
        distfunct->add(y0, sqrt(dist));
    }
    distfunct->name = name + "_distanceY_" + fonction->name;
    return distfunct;
}

/*
  Minimal distance between two curves

  This method creates a new fonction from the given two ones as the minimal distance between the two curves.
  This method calculates a minimum distance between two curves and returns a curve having the curvilinear ordinate in abscissa and in ordinate the minimum distance between two curves at the point considered.
  This method returns a new discrete function with the same name as the current ones + "_distance_".
  Therefore, if the current functions names are "curve1" and "curve2" then the new created function name will be "curve1_distance_curve2".
  - fonction Second function to work with
  Return : The new function
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::distanceCurves(DiscreteFunction *fonction)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *distfunct = new DiscreteFunction;
    double absc = 0.0;
    double dist;
    double dist2;
    double x, y;
    double x0 = 0.0, y0 = 0.0, x1, y1;

    for (long i = 0; i < lpoints.size(); i++)
    {
        // mise à jour de l'abcisse
        if (i > 0)
        {
            absc += sqrt(dnlSquare(lpoints(i)->x - lpoints(i - 1)->x) + dnlSquare(lpoints(i)->y - lpoints(i - 1)->y));
        }
        x = lpoints(i)->x;
        y = lpoints(i)->y;
        dist = dnlSquare(x - fonction->lpoints(0)->x) + dnlSquare(y - fonction->lpoints(0)->y);
        for (long ii = 1; ii < fonction->lpoints.size(); ii++)
        {
            dist2 = dnlSquare(x - fonction->lpoints(ii)->x) + dnlSquare(y - fonction->lpoints(ii)->y);
            if (dist2 < dist)
            {
                dist = dist2;
                x1 = (fonction->lpoints(ii)->x + x) / 2;
                y1 = (fonction->lpoints(ii)->y + y) / 2;
            }
        }
        if (i != 0)
        {
            absc += sqrt(dnlSquare(x1 - x0) + dnlSquare(y1 - y0));
        }
        distfunct->add(absc, sqrt(dist));
        x0 = x1;
        y0 = y1;
    }
    distfunct->name = name + "_distance_" + fonction->name;
    return distfunct;
}

/*
  Order 1 low pass filter of a function

  This method creates a new fonction from the given one using an order 1 low pass filter. This type of filter has low frequency gain of 1. It is defined by the following equation:
  \f[ f(p)=\frac{1}{ap+1} \f]
  where \f$ p \f$ is the Laplace operator, \f$ p=j\omega \f$ with \f$ j^{2}=-1 \f$. Using a Backward Euler derivative the filtered form \f$ f_{f}(x) \f$ is therefore given by:
  \f[ f_{f}(x)=\frac{f(x)+\frac{a}{dx}f_{f}(x-dx)}{\frac{a}{dx}+1} \f]
  with the initial conditions on \f$ f_{f}(-dx) \f$
  This method returns a new discrete function with the same name as the current one + "_lowPass_1".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_lowPass_1".
  Return : The new function
  - A Cutting pulse for the filter
  - points Number of points to generate for the new function
  - Init Definition of the first point for the filter (\ref df_noInit: first value is 0, \ref df_averageInit: first value is the average value of the function, \ref df_fisrtValueInit: first value is the first value of the non filtered function)
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::lowPass_1(double A, long points, int Init)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *filtered = new DiscreteFunction;
    double dx;
    double vfilter;

    dx = (maxX() - minX()) / points;

    vfilter = (Init == averageInit ? getAverage() : (Init == fisrtValueInit ? lpoints(0)->y : 0.0));

    for (double x = minX(); x <= maxX(); x += dx)
    {
        vfilter = (getValue(x) + A / dx * vfilter) / (A / dx + 1.0);
        filtered->add(x, vfilter);
    }

    filtered->name = name + "_lowPass_1";
    return filtered;
}

/*
  Order 2 low pass filter of a function

  This method creates a new fonction from the given one using an order 2 low pass filter. This type of filter has low frequency gain of 1. It is defined by the following equation:
  \f[ f(p)=\frac{1}{ap^{2}+bp+1} \f]
  where \f$ p \f$ is the Laplace operator, \f$ p=j\omega \f$ with \f$ j^{2}=-1 \f$. Using a Backward Euler derivative the filtered form \f$ f_{f}(x) \f$ is therefore given by:
  \f[ f_{f}(x)=\frac{f(x)+(\frac{2a}{dx^{2}}+\frac{b}{dx})f_{f}(x-dx)-\frac{a}{dx^{2}}f_{f}(x-2dx)}{\frac{a}{dx^{2}}+\frac{b}{dx}+1} \f]
  with the initial conditions on \f$ f_{f}(-2dx) \f$ and \f$ f_{f}(-dx) \f$
  This method returns a new discrete function with the same name as the current one + "_lowPass_2".
  Therefore, if the current function name is "curve" then the new created function name will be "curve_lowPass_2".
  Return : The new function
  - A Cutting pulse for the filter
  - Q Over value (usually between 0.7 and 1.0)
  - points Number of points to generate for the new function
  - Init Definition of the first point for the filter (\ref df_noInit: first value is 0, \ref df_averageInit: first value is the average value of the function, \ref df_fisrtValueInit: first value is the first value of the non filtered function)
*/
//-----------------------------------------------------------------------------
DiscreteFunction *DiscreteFunction::lowPass_2(double A, double Q, long points, int Init)
//-----------------------------------------------------------------------------
{
    DiscreteFunction *filtered = new DiscreteFunction;
    double dx;
    double vfilter1, vfilter2, vfilter;
    double B;

    B = sqrt(A) / Q;

    dx = (maxX() - minX()) / points;

    vfilter1 = (Init == averageInit ? getAverage() : (Init == fisrtValueInit ? lpoints(0)->y : 0.0));
    vfilter2 = vfilter1;

    for (double x = minX(); x <= maxX(); x += dx)
    {
        vfilter = (getValue(x) + (2.0 * A / dnlSquare(dx) + B / dx) * vfilter1 - A / dnlSquare(dx) * vfilter2) / (A / dnlSquare(dx) + B / dx + 1.0);
        filtered->add(x, vfilter);
        vfilter2 = vfilter1;
        vfilter1 = vfilter;
    }

    filtered->name = name + "_lowPass_2";
    return filtered;
}

/*
  Average value of a function

  This method computes the average value of a discrete function.
  Return : Average value of a function
*/
//-----------------------------------------------------------------------------
double DiscreteFunction::getAverage()
//-----------------------------------------------------------------------------
{
    double avrge = 0.0;

    if (lpoints.size() == 0)
    {
        return avrge;
    }

    for (long i = 1; i < lpoints.size(); i++)
    {
        avrge += (lpoints(i)->y + lpoints(i - 1)->y) / 2 * (lpoints(i)->x - lpoints(i - 1)->x);
    }
    avrge /= (maxX() - minX());

    return avrge;
}

/*
  Output format to DynELA source file

  This method is used to store the architecture of the discrete function using a DynELA source file format.
  The returned string can be written directly to the source file of the DynELA FEM code.
  Return : The string containing the definition of the discrete function according to the DynELA source file format
*/
//-----------------------------------------------------------------------------
String DiscreteFunction::convertToDynELASourceFile()
//-----------------------------------------------------------------------------
{
    String str = "";
    char sstr[1000];

    sprintf(sstr, "DiscreteFunction %s;\n", name.c_str());
    str += sstr;

    for (long i = 0; i < getNumberOfPoints(); i++)
    {
        sprintf(sstr, "%s.add(%12.7E, %12.7E);\n", name.c_str(), getXPoint(i), getYPoint(i));
        str += sstr;
    }
    return str;
}

/*
  Saves the content of a DiscreteFunction into a NumPy file

  This method saves the content of a DiscreteFunction object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  DiscreteFunction t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
    double buffer[2 * lpoints.size()];
    double *ptbuff = buffer;
    for (long i = 0; i < lpoints.size(); i++)
    {
        *ptbuff++ = lpoints(i)->x;
        *ptbuff++ = lpoints(i)->y;
    }

    std::string mode = "a";
    if (initialize)
        mode = "w";
    NumpyInterface::npySave(filename, &buffer[0], {(unsigned long)lpoints.size(), 2}, mode);
}

/*
  Saves the content of a DiscreteFunction into a NumPyZ file

  This method saves the content of a DiscreteFunction object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  DiscreteFunction t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
    double buffer[2 * lpoints.size()];
    double *ptbuff = buffer;
    for (long i = 0; i < lpoints.size(); i++)
    {
        *ptbuff++ = lpoints(i)->x;
        *ptbuff++ = lpoints(i)->y;
    }

    std::string mode = "a";
    if (initialize)
        mode = "w";
    NumpyInterface::npzSave(filename, name, &buffer[0], {(unsigned long)lpoints.size(), 2}, mode);
}

/*
  Read the content of a DiscreteFunction from a NumPy file

  This method reads the content of a DiscreteFunction object from a NumPy file defined by its filename.

  Example
  \code
  DiscreteFunction t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
    NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
    flush();
    for (long i = 0; i < arr.num_vals; i += 2)
    {
        add(arr.data<double>()[i], arr.data<double>()[i + 1]);
    }
}

/*
  Read the content of a DiscreteFunction from a NumPyZ file

  This method reads the content of a DiscreteFunction object from a NumPyZ file defined by its filename.

  Example
  \code
  DiscreteFunction t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void DiscreteFunction::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
    NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
    flush();
    for (long i = 0; i < arr.num_vals; i += 2)
    {
        add(arr.data<double>()[i], arr.data<double>()[i + 1]);
    }
}
