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
  \file ElTet4N3D.C
  Definition file for the ElTet4N3D class

  This file is the definition file for the ElTet4N3D class.

  \ingroup dnlElements
*/

#include <ElTet4N3D.h>

const ElementData ElTet4N3D::_elementData = {
    "ElTet4N3D",               // name of the Element
    Element::ElTet4N3D,        // Type of the Element
    Element::Threedimensional, // Family of the Element
    3,                         // Number of dimensions of the Element
    3,                         // Number of degrees of freedom of the Element
    10,                        // VTK type of element

    4, // Number of nodes of the Element
    {
        // Node 1
        {
            Vec3D(0, 0, 0),                                                                                             // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {1, 2, 3},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 2
        {
            Vec3D(1, 0, 0),                                                                                             // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {0, 2, 3},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 3
        {
            Vec3D(0, 1, 0),                                                                                             // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {0, 1, 3},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 4
        {
            Vec3D(0, 0, 1),                                                                                             // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {0, 1, 2},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        }
        //
    },

    1, // Number of integration points of the Element
    {
        {
            Vec3D(1.0 / 4, 1.0 / 4, 1.0 / 4),                                           // Coordinates of the integration point
            1.0 / 6,                                                                    // Weight associated to the integration point
            Vector(4, 1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4),                              // Shape functions at the integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0) // Derivatives of the shape functions at the integration point
        },
        //
    },

    1, // Number of under integration points of the Element
    {
        {
            Vec3D(1.0 / 4, 1.0 / 4, 1.0 / 4),                                           // Coordinates of the under integration point
            1.0 / 6,                                                                    // Weight associated to the under integration point
            Vector(4, 1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4),                              // Shape functions at the under integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0) // Derivatives of the shape functions at the under integration point
        },
        //
    },

    4, // Number of faces of the element
    {
        // Face number 1
        {
            3,        // Number of nodes on the face of the element
            {0, 1, 3} // List of the nodes composing the face of the element
        },
        // Face number 2
        {
            3,        // Number of nodes on the face of the element
            {0, 2, 1} // List of the nodes composing the face of the element
        },
        // Face number 3
        {
            3,        // Number of nodes on the face of the element
            {0, 3, 2} // List of the nodes composing the face of the element
        },
        // Face number 4
        {
            3,        // Number of nodes on the face of the element
            {2, 3, 1} // List of the nodes composing the face of the element
        }
        //
    },
    /*
    4, // Number of mass integration points of the Element
    {
        // Mass integration point 1
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0),                                 // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                                          // Weight associated to the mass integration point
            Vector(4, (5.0 + 3.0 * sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20.), // Shape functions at the mass integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)                                          // Derivatives of the shape functions at the mass integration point
        },

        // Mass integration point 2
        {
            Vec3D((5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0),                           // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                                          // Weight associated to the mass integration point
            Vector(4, (5.0 - sqrt(5.0)) / 20., (5.0 + 3.0 * sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20.), // Shape functions at the mass integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)                                          // Derivatives of the shape functions at the mass integration point
        },

        // Mass integration point 3
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0),                           // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                                          // Weight associated to the mass integration point
            Vector(4, (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 + 3.0 * sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20.), // Shape functions at the mass integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)                                          // Derivatives of the shape functions at the mass integration point
        },

        // Mass integration point 4
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0),                           // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                                          // Weight associated to the mass integration point
            Vector(4, (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 - sqrt(5.0)) / 20., (5.0 + 3.0 * sqrt(5.0)) / 20.), // Shape functions at the mass integration point
            Matrix(4, 3, -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)                                          // Derivatives of the shape functions at the mass integration point
        },
        //
    },

    4, // Number of side faces of the element
    {
        // Side face number 1
        {
            3,        // Number of nodes on the side face of the element
            {0, 1, 3} // List of the nodes composing the side face of the element
        },
        // Side face number 2
        {
            3,        // Number of nodes on the side face of the element
            {0, 2, 1} // List of the nodes composing the side face of the element
        },
        // Side face number 3
        {
            3,        // Number of nodes on the side face of the element
            {0, 3, 2} // List of the nodes composing the side face of the element
        },
        // Side face number 4
        {
            3,        // Number of nodes on the side face of the element
            {2, 3, 1} // List of the nodes composing the side face of the element
        }
        //
    },

    6, // Number of edges of the element
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
            {2, 0} // List of the nodes composing the edge of the element
        },
        // Edge number 4
        {
            2,     // Number of nodes on the edge of the element
            {0, 3} // List of the nodes composing the edge of the element
        },
        // Edge number 5
        {
            2,     // Number of nodes on the edge of the element
            {1, 3} // List of the nodes composing the edge of the element
        },
        // Edge number 6
        {
            2,     // Number of nodes on the edge of the element
            {2, 3} // List of the nodes composing the edge of the element
        }
        //
    },*/
    // Check element
    Element::ElTet4N3D
    // END
};

//-----------------------------------------------------------------------------
ElTet4N3D::ElTet4N3D(long elementNumber) : Element3D(elementNumber)
//-----------------------------------------------------------------------------
{
    _globalToLocal.redim(3, 4);
    Element::_elementData = &_elementData;
    stiffnessMatrix.redim(12, 12);

    // Check the data structure
    if (_elementData.CheckElementData != Element::ElTet4N3D)
        fatalError("ElTet4N3D::ElTet4N3D", "Element Data currupted");
}

//-----------------------------------------------------------------------------
ElTet4N3D::ElTet4N3D(const ElTet4N3D &element) : Element3D(element)
//-----------------------------------------------------------------------------
{
    std::cout << "recopie d'un ElTet4N3D\n";
}

//-----------------------------------------------------------------------------
ElTet4N3D::~ElTet4N3D()
//-----------------------------------------------------------------------------
{
    nodes.flush();
}

//-----------------------------------------------------------------------------
void ElTet4N3D::getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);
    double zeta = point(2);

    shapeFunctions(0) = 1 - ksi - eta - zeta;
    shapeFunctions(1) = ksi;
    shapeFunctions(2) = eta;
    shapeFunctions(3) = zeta;
}

//-----------------------------------------------------------------------------
void ElTet4N3D::getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    derShapeFunctions(0, 0) = -1.;
    derShapeFunctions(0, 1) = -1.;
    derShapeFunctions(0, 2) = -1.;
    derShapeFunctions(1, 0) = 1.;
    derShapeFunctions(1, 1) = 0.;
    derShapeFunctions(1, 2) = 0.;
    derShapeFunctions(2, 0) = 0.;
    derShapeFunctions(2, 1) = 1.;
    derShapeFunctions(2, 2) = 0.;
    derShapeFunctions(3, 0) = 0.;
    derShapeFunctions(3, 1) = 0.;
    derShapeFunctions(3, 2) = 1.;
}

/*
//-----------------------------------------------------------------------------
double ElTet4N3D::getCharacteristicLength()
//-----------------------------------------------------------------------------
{
  double l1,l2,l3,l4,l5,l6,ll;
  l1=nodes(0)->coords.distance(nodes(1)->coords);
  l2=nodes(0)->coords.distance(nodes(2)->coords);
  l3=nodes(1)->coords.distance(nodes(2)->coords);
  l4=nodes(0)->coords.distance(nodes(3)->coords);
  l5=nodes(1)->coords.distance(nodes(3)->coords);
  l6=nodes(2)->coords.distance(nodes(3)->coords);
  ll=dnlMin(l1,l2);
  ll=dnlMin(ll,l3);
  ll=dnlMin(ll,l4);
  ll=dnlMin(ll,l5);
  ll=dnlMin(ll,l6);
  return ll;
}

//-----------------------------------------------------------------------------
double ElTet4N3D::getVolume()
//-----------------------------------------------------------------------------
{
  cerr << "ElTet4N3D::getVolume() erreur ElHex8N3D non implemente\n";
  exit(-1);

  double l1,l2,l3,l4,l5;
  l1=nodes(1)->coords.distance(nodes(2)->coords);
  l2=nodes(2)->coords.distance(nodes(3)->coords);
  l3=nodes(3)->coords.distance(nodes(4)->coords);
  l4=nodes(4)->coords.distance(nodes(1)->coords);
  l5=nodes(1)->coords.distance(nodes(3)->coords);
  return (dnlSurfTriangle(l1,l2,l5)+dnlSurfTriangle(l3,l4,l5));
}


// //-----------------------------------------------------------------------------
// void ElTet4N3D::getIntgtoNodes(Vector& N,const Vec3D& point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElTet4N3D::getIntgtoNodes\n";
//   exit(-1);
// }

//-----------------------------------------------------------------------------
void ElTet4N3D::computeGlob2Loc()
//-----------------------------------------------------------------------------
{
  Matrix nds (getNumberOfNodes(), getNumberOfNodes());
  Matrix cords(getNumberOfNodes(),3);
  long i,j;
  Node *pnd;

  // calcul de la matrice
  for (i = 0; i < getNumberOfNodes(); i++)
    {
      pnd = nodes (i);
      nds (i, 0) = 1;
      nds (i, 1) = pnd->coords (0);
      nds (i, 2) = pnd->coords (1);
      nds (i, 3) = pnd->coords (2);
    }

  // inversion de la matrice
  Matrix inv = nds.inverse ();

  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < 3; j++)
      cords(i,j) = getLocalNodeCoords(i)(j);

  _globalToLocal = (inv * cords).transpose();
}

//-----------------------------------------------------------------------------
void ElTet4N3D::glob2Loc(const Vec3D& point,Vec3D& local)
//-----------------------------------------------------------------------------
{
  local (0) =
    _globalToLocal (0, 0) + point (0) * _globalToLocal (0, 1) + point (1) * _globalToLocal (0, 2) + point (2) * _globalToLocal (0, 3);
  local (1) =
    _globalToLocal (1, 0) + point (0) * _globalToLocal (1, 1) + point (1) * _globalToLocal (1, 2) + point (2) * _globalToLocal (1, 3);
  local (2) =
    _globalToLocal (2, 0) + point (0) * _globalToLocal (2, 1) + point (1) * _globalToLocal (2, 2) + point (2) * _globalToLocal (2, 3);
}
*/
