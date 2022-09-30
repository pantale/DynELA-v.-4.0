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
  \file ElementPlane.C
  Definition file for the ElementPlane class

  This file is the definition file for the ElementPlane class.

  \ingroup dnlElements
*/

#include <ElTri3N2D.h>

const ElementData ElTri3N2D::_elementData = {
    "ElTri3N2D",            // name of the Element
    Element::ElTri3N2D,     // Type of the Element
    Element::Bidimensional, // Family of the Element
    2,                      // Number of dimensions of the Element
    2,                      // Number of degrees of freedom of the Element
    5,                      // VTK type of element

    3, // Number of nodes of the Element
    {
        // Node 1
        {
            Vec3D(0, 0, 0), // Coordinates of the node
            2,              // Number of neightbour nodes
            {1, 2},         // List of the neighbour nodes
            Vector()        // Extrapolates integration points values to nodal values
        },
        // Node 2
        {
            Vec3D(0, 0, 0), // Coordinates of the node
            2,              // Number of neightbour nodes
            {0, 2},         // List of the neighbour nodes
            Vector()        // Extrapolates integration points values to nodal values
        },
        // Node 3
        {
            Vec3D(0, 0, 0), // Coordinates of the node
            2,              // Number of neightbour nodes
            {1, 0},         // List of the neighbour nodes
            Vector()        // Extrapolates integration points values to nodal values
        }
        //
    },

    1, // Number of integration points of the Element
    {
        // Integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0), // Coordinates of the integration point
            1,                                              // Weight associated to the integration point
            Vector(),                                       // Shape functions at the integration point
            Matrix()                                        // Derivatives of the shape functions at the integration point
        }
        //
    },

    1, // Number of under integration points of the Element
    {
        // Under integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0), // Coordinates of the under integration point
            1,                                              // Weight associated to the under integration point
            Vector(),                                       // Shape functions at the under integration point
            Matrix()                                        // Derivatives of the shape functions at the under integration point
        }
        //
    },

    1, // Number of faces of the element
    {
        // Face number 1
        {
            3,        // Number of nodes on the face of the element
            {0, 1, 2} // List of the nodes composing the face of the element
        }
        //
    },
    /*
    1, // Number of mass integration points of the Element
    // THIS SHOULD BE 3
    {
        // Mass integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0), // Coordinates of the mass integration point
            1,                                              // Weight associated to the mass integration point
            Vector(),                                       // Shape functions at the mass integration point
            Matrix()                                        // Derivatives of the shape functions at the mass integration point
        }
        //
    },

    4, // Number of side faces of the element
    {
        // Side face number 1
        {
            2,     // Number of nodes on the side face of the element
            {0, 1} // List of the nodes composing the side face of the element
        },
        // Side face number 2
        {
            2,     // Number of nodes on the side face of the element
            {1, 2} // List of the nodes composing the side face of the element
        },
        // Side face number 3
        {
            2,     // Number of nodes on the side face of the element
            {2, 3} // List of the nodes composing the side face of the element
        },
        // Side face number 4
        {
            2,     // Number of nodes on the side face of the element
            {3, 0} // List of the nodes composing the side face of the element
        }
        //
    },

    4, // Number of edges of the element
    {
        // Edge number 1
        {
            2,     // Number of nodes on the edge of the element
            {0, 1} // List of the nodes composing the edge of the element
        },
        // Edge number 2
        {
            2,     // Number of nodes on the edge of the element
            {1, 2} // List of the nodes composing the edge of the element
        },
        // Edge number 3
        {
            2,     // Number of nodes on the edge of the element
            {2, 3} // List of the nodes composing the edge of the element
        },
        // Edge number 4
        {
            2,     // Number of nodes on the edge of the element
            {3, 0} // List of the nodes composing the edge of the element
        }
        //
    },*/
    // Check element
    Element::ElTri3N2D
    // END
};

//-----------------------------------------------------------------------------
ElTri3N2D::ElTri3N2D(long elementNumber) : Element2D(elementNumber)
//-----------------------------------------------------------------------------
{
    _globalToLocal.redim(2, 4);
    Element::_elementData = &_elementData;
    stiffnessMatrix.redim(8, 8);

    // Check the data structure
    if (_elementData.CheckElementData != Element::ElTri3N2D)
        fatalError("ElTri3N2D::ElTri3N2D", "Element Data currupted");
}

//-----------------------------------------------------------------------------
ElTri3N2D::ElTri3N2D(const ElTri3N2D &element) : Element2D(element)
//-----------------------------------------------------------------------------
{
    std::cout << "recopie d'un ElTri3N2D\n";
}

//-----------------------------------------------------------------------------
ElTri3N2D::~ElTri3N2D()
//-----------------------------------------------------------------------------
{
    nodes.flush();
}

//-----------------------------------------------------------------------------
void ElTri3N2D::getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);

    shapeFunctions(0) = (1.0 - ksi) * (1.0 - eta) / 4;
    shapeFunctions(1) = (1.0 + ksi) * (1.0 - eta) / 4;
    shapeFunctions(2) = (1.0 + ksi) * (1.0 + eta) / 4;
    shapeFunctions(3) = (1.0 - ksi) * (1.0 + eta) / 4;
}

//-----------------------------------------------------------------------------
void ElTri3N2D::getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);

    derShapeFunctions(0, 0) = -(1.0 - eta) / 4;
    derShapeFunctions(0, 1) = -(1.0 - ksi) / 4;
    derShapeFunctions(1, 0) = +(1.0 - eta) / 4;
    derShapeFunctions(1, 1) = -(1.0 + ksi) / 4;
    derShapeFunctions(2, 0) = +(1.0 + eta) / 4;
    derShapeFunctions(2, 1) = +(1.0 + ksi) / 4;
    derShapeFunctions(3, 0) = -(1.0 + eta) / 4;
    derShapeFunctions(3, 1) = +(1.0 - ksi) / 4;
}

// Calcul de la longueur caracteristique d'un element
/*
Cette methode calcule la longueur caracteristique d'un element à partir de la definition de la geometrie de cet element.
La relation utilisee pour ce calcul est donnee par:
\f[ l=\frac{x_{31}*y_{42}+x_{24}*y_{31}}{\sqrt{x_{24}^2+y_{42}^2+x_{31}^2+y_{31}^2}} \f] avec \f$ x_{ij} \f$ distance horizontale entre les points i et j et \f$ y_{ij} \f$ distance verticale entre les points i et j.
Return : longueur caracteristique de l'element
*/
/*
//-----------------------------------------------------------------------------
double ElTri3N2D::getCharacteristicLength ()
//-----------------------------------------------------------------------------
{
  double x31, y42, x24, y31;
  x31 = nodes (2)->coords (0) - nodes (0)->coords (0);
  x24 = nodes (1)->coords (0) - nodes (3)->coords (0);
  y42 = nodes (3)->coords (1) - nodes (1)->coords (1);
  y31 = nodes (2)->coords (1) - nodes (0)->coords (1);

  // modif DynELA 0.9.5
  // correction d'un bug, merci Maxima
  return ((x31 * y42 +	   x24 * y31) / sqrt (2. * (dnlSquare (x24) + dnlSquare (y42) + dnlSquare (x31) +				    dnlSquare (y31))));
}

//-----------------------------------------------------------------------------
double ElTri3N2D::getVolume ()
//-----------------------------------------------------------------------------
{
  double l1, l2, l3, l4, l5;

  l1 = nodes (0)->coords.distance (nodes (1)->coords);
  l2 = nodes (1)->coords.distance (nodes (2)->coords);
  l3 = nodes (2)->coords.distance (nodes (3)->coords);
  l4 = nodes (3)->coords.distance (nodes (0)->coords);
  l5 = nodes (0)->coords.distance (nodes (2)->coords);

  return (dnlSurfTriangle (l1, l2, l5) + dnlSurfTriangle (l3, l4, l5));
}




// //-----------------------------------------------------------------------------
// void ElTri3N2D::getIntgtoNodes (Vector & N, const Vec3D & point) const
// //-----------------------------------------------------------------------------
// {
//   const double pos = 1.0 / sqrt (3.0);
//   double ksi = point (0);
//   double eta = point (1);

//   N (0) = 3. * (pos - ksi) * (pos - eta) / 4;
//   N (1) = 3. * (pos + ksi) * (pos - eta) / 4;
//   N (2) = 3. * (pos + ksi) * (pos + eta) / 4;
//   N (3) = 3. * (pos - ksi) * (pos + eta) / 4;
// }

//-----------------------------------------------------------------------------
void ElTri3N2D::computeGlob2Loc ()
//-----------------------------------------------------------------------------
{
  Matrix nds (getNumberOfNodes(), getNumberOfNodes());
  Vector crds (getNumberOfNodes());
  long i, j;
  Node *pnd;

  // calcul de la matrice
  for (i = 0; i < getNumberOfNodes(); i++)
    {
      pnd = nodes (i);
      nds (i, 0) = 1.;
      nds (i, 1) = pnd->coords (0);
      nds (i, 2) = pnd->coords (1);
      nds (i, 3) = pnd->coords (0) * pnd->coords (1);
    }

  // inversion de la matrice
  Matrix inv = nds.inverse ();
  // cout << inv;

  // init
  _globalToLocal = 0.;

  // calcul des coefficients en x
  crds (0) = -1.;
  crds (1) = +1.;
  crds (2) = +1.;
  crds (3) = -1.;
  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < getNumberOfNodes(); j++)
      _globalToLocal (0, i) += inv (i, j) * crds (j);

  // calcul des coefficients en y
  crds (0) = -1.;
  crds (1) = -1.;
  crds (2) = +1.;
  crds (3) = +1.;
  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < getNumberOfNodes(); j++)
      _globalToLocal (1, i) += inv (i, j) * crds (j);
}

//-----------------------------------------------------------------------------
void ElTri3N2D::glob2Loc (const Vec3D & point, Vec3D & local)
//-----------------------------------------------------------------------------
{
  local (0) =
    _globalToLocal (0, 0) + point (0) * _globalToLocal (0, 1) + point (1) * _globalToLocal (0,
                                      2) +
    point (0) * point (1) * _globalToLocal (0, 3);
  local (1) =
    _globalToLocal (1, 0) + point (0) * _globalToLocal (1, 1) + point (1) * _globalToLocal (1,
                                      2) +
    point (0) * point (1) * _globalToLocal (1, 3);
  local (2) = 0.;
}
*/
