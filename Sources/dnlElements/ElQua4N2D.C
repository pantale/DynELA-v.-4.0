/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file ElQua4N2D.C
  \brief Definition file for the ElQua4N2D class

  This file is the definition file for the ElQua4N2D class.

  \ingroup dnlElements
*/

#include <ElQua4N2D.h>
#include <Node.h>

const ElementData ElQua4N2D::_elementData = {
    "ElQua4N2D",            // name of the Element
    Element::ElQua4N2D,     // Type of the Element
    Element::Bidimensional, // Family of the Element
    2,                      // Number of dimensions of the Element
    2,                      // Number of degrees of freedom of the Element
    9,                      // VTK type of element

    4, // Number of nodes of the Element
    {
        // Node 1
        {
            Vec3D(-1, -1, 0),                                               // Coordinates of the node
            2,                                                              // Number of neightbour nodes
            {1, 3},                                                         // List of the neighbour nodes
            Vector(4, 1.0 + sqrt(3.0) / 2, -0.5, 1.0 - sqrt(3.0) / 2, -0.5) // Extrapolates integration points values to nodal values
        },
        // Node 2
        {
            Vec3D(+1, -1, 0),                                               // Coordinates of the node
            2,                                                              // Number of neightbour nodes
            {0, 2},                                                         // List of the neighbour nodes
            Vector(4, -0.5, 1.0 + sqrt(3.0) / 2, -0.5, 1.0 - sqrt(3.0) / 2) // Extrapolates integration points values to nodal values
        },
        // Node 3
        {
            Vec3D(+1, +1, 0),                                               // Coordinates of the node
            2,                                                              // Number of neightbour nodes
            {1, 3},                                                         // List of the neighbour nodes
            Vector(4, 1.0 - sqrt(3.0) / 2, -0.5, 1.0 + sqrt(3.0) / 2, -0.5) // Extrapolates integration points values to nodal values
        },
        // Node 4
        {
            Vec3D(-1, +1, 0),                                               // Coordinates of the node
            2,                                                              // Number of neightbour nodes
            {0, 2},                                                         // List of the neighbour nodes
            Vector(4, -0.5, 1.0 - sqrt(3.0) / 2, -0.5, 1.0 + sqrt(3.0) / 2) // Extrapolates integration points values to nodal values
        }
        //
    },

    4, // Number of integration points of the Element
    {
        // Integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0),                            // Coordinates of the integration point
            1,                                                                         // Weight associated to the integration point
            Vector(4, (2.0 + sqrt(3.0)) / 6, 1.0 / 6, (2.0 - sqrt(3.0)) / 6, 1.0 / 6), // Shape functions at the integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the integration point
                   (-3.0 - sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12,                   //
                   (-3.0 + sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12)                   //
        },
        // Integration point 2
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0),                            // Coordinates of the integration point
            1,                                                                         // Weight associated to the integration point
            Vector(4, 1.0 / 6, (2.0 + sqrt(3.0)) / 6, 1.0 / 6, (2.0 - sqrt(3.0)) / 6), // Shape functions at the integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the integration point
                   (-3.0 - sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12,                   //
                   (-3.0 + sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12)                   //
        },
        // Integration point 3
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), 0.0),                            // Coordinates of the integration point
            1,                                                                         // Weight associated to the integration point
            Vector(4, (2.0 - sqrt(3.0)) / 6, 1.0 / 6, (2.0 + sqrt(3.0)) / 6, 1.0 / 6), // Shape functions at the integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the integration point
                   (-3.0 + sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12,                   //
                   (-3.0 - sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12)                   //
        },
        // Integration point 4
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), 0.0),                            // Coordinates of the integration point
            1,                                                                         // Weight associated to the integration point
            Vector(4, 1.0 / 6, (2.0 - sqrt(3.0)) / 6, 1.0 / 6, (2.0 + sqrt(3.0)) / 6), // Shape functions at the integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the integration point
                   (-3.0 + sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12,                   //
                   (-3.0 - sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12)                   //
        }
        //
    },

    1, // Number of under integration points of the Element
    {
        // Under integration point 1
        {
            Vec3D(0.0, 0.0, 0.0),                              // Coordinates of the under integration point
            4.0,                                               // Weight associated to the under integration point
            Vector(4, +1.0 / 4, +1.0 / 4, +1.0 / 4, +1.0 / 4), // Shape functions at the under integration point
            Matrix(4, 2,                                       // Derivatives of the shape functions at the under integration point
                   -1.0 / 4, -1.0 / 4,                         //
                   +1.0 / 4, -1.0 / 4,                         //
                   +1.0 / 4, +1.0 / 4,                         //
                   -1.0 / 4, +1.0 / 4)                         //
        }
        //
    },

    1, // Number of faces of the element
    {
        // Face number 1
        {
            4,           // Number of nodes on the face of the element
            {0, 1, 2, 3} // List of the nodes composing the face of the element
        }
        //
    },
    /*
    4, // Number of mass integration points of the Element
    {
        // Mass integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0),                            // Coordinates of the mass integration point
            1,                                                                         // Weight associated to the mass integration point
            Vector(4, (2.0 + sqrt(3.0)) / 6, 1.0 / 6, (2.0 - sqrt(3.0)) / 6, 1.0 / 6), // Shape functions at the mass integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the mass integration point
                   (-3.0 - sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12,                   //
                   (-3.0 + sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12)                   //
        },
        // Mass integration point 2
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), 0.0),                            // Coordinates of the mass integration point
            1,                                                                         // Weight associated to the mass integration point
            Vector(4, 1.0 / 6, (2.0 + sqrt(3.0)) / 6, 1.0 / 6, (2.0 - sqrt(3.0)) / 6), // Shape functions at the mass integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the mass integration point
                   (-3.0 - sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12,                   //
                   (-3.0 + sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12)                   //
        },
        // Mass integration point 3
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), 0.0),                            // Coordinates of the mass integration point
            1,                                                                         // Weight associated to the mass integration point
            Vector(4, (2.0 - sqrt(3.0)) / 6, 1.0 / 6, (2.0 + sqrt(3.0)) / 6, 1.0 / 6), // Shape functions at the mass integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the mass integration point
                   (-3.0 + sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12,                   //
                   (-3.0 - sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12)                   //
        },
        // Mass integration point 4
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), 0.0),                            // Coordinates of the mass integration point
            1,                                                                         // Weight associated to the mass integration point
            Vector(4, 1.0 / 6, (2.0 - sqrt(3.0)) / 6, 1.0 / 6, (2.0 + sqrt(3.0)) / 6), // Shape functions at the mass integration point
            Matrix(4, 2,                                                               // Derivatives of the shape functions at the mass integration point
                   (-3.0 + sqrt(3.0)) / 12, (-3.0 - sqrt(3.0)) / 12,                   //
                   (+3.0 - sqrt(3.0)) / 12, (-3.0 + sqrt(3.0)) / 12,                   //
                   (+3.0 + sqrt(3.0)) / 12, (+3.0 - sqrt(3.0)) / 12,                   //
                   (-3.0 - sqrt(3.0)) / 12, (+3.0 + sqrt(3.0)) / 12)                   //
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
    10010110
    // END
};

//-----------------------------------------------------------------------------
ElQua4N2D::ElQua4N2D(long elementNumber) : Element2D(elementNumber)
//-----------------------------------------------------------------------------
{
    _globalToLocal.redim(2, 4);
    Element::_elementData = &_elementData;
    stiffnessMatrix.redim(8, 8);

    // Check the data structure
    if (_elementData.CheckElementData != 10010110)
        fatalError("ElQua4N2D::ElQua4N2D", "Element Data currupted");
}

//-----------------------------------------------------------------------------
ElQua4N2D::ElQua4N2D(const ElQua4N2D &element) : Element2D(element)
//-----------------------------------------------------------------------------
{
    std::cout << "recopie d'un ElQua4N2D\n";
}

//-----------------------------------------------------------------------------
ElQua4N2D::~ElQua4N2D()
//-----------------------------------------------------------------------------
{
    nodes.flush();
}

//-----------------------------------------------------------------------------
void ElQua4N2D::getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);

    shapeFunctions(0) = (1.0 - ksi) * (1.0 - eta) / 4.0;
    shapeFunctions(1) = (1.0 + ksi) * (1.0 - eta) / 4.0;
    shapeFunctions(2) = (1.0 + ksi) * (1.0 + eta) / 4.0;
    shapeFunctions(3) = (1.0 - ksi) * (1.0 + eta) / 4.0;
}

//-----------------------------------------------------------------------------
void ElQua4N2D::getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);

    derShapeFunctions(0, 0) = -(1.0 - eta) / 4.0;
    derShapeFunctions(1, 0) = +(1.0 - eta) / 4.0;
    derShapeFunctions(2, 0) = +(1.0 + eta) / 4.0;
    derShapeFunctions(3, 0) = -(1.0 + eta) / 4.0;

    derShapeFunctions(0, 1) = -(1.0 - ksi) / 4.0;
    derShapeFunctions(1, 1) = -(1.0 + ksi) / 4.0;
    derShapeFunctions(2, 1) = +(1.0 + ksi) / 4.0;
    derShapeFunctions(3, 1) = +(1.0 - ksi) / 4.0;
}

/*
//Calcul de la longueur caracteristique d'un element
//
//Cette methode calcule la longueur caracteristique d'un element à partir de la definition de la geometrie de cet element.
//La relation utilisee pour ce calcul est donnee par:
//\f[ l=\frac{x_{31}*y_{42}+x_{24}*y_{31}}{\sqrt{x_{24}^2+y_{42}^2+x_{31}^2+y_{31}^2}} \f] avec \f$ x_{ij} \f$ distance horizontale entre les points i et j et \f$ y_{ij} \f$ distance verticale entre les points i et j.
//\return longueur caracteristique de l'element
*/
//-----------------------------------------------------------------------------
double ElQua4N2D::getCharacteristicLength()
//-----------------------------------------------------------------------------
{
    double x31 = nodes(2)->coordinates(0) - nodes(0)->coordinates(0);
    double x24 = nodes(1)->coordinates(0) - nodes(3)->coordinates(0);
    double y42 = nodes(3)->coordinates(1) - nodes(1)->coordinates(1);
    double y31 = nodes(2)->coordinates(1) - nodes(0)->coordinates(1);

    return ((x31 * y42 + x24 * y31) / sqrt(2.0 * (dnlSquare(x24) + dnlSquare(y42) + dnlSquare(x31) + dnlSquare(y31))));
}

//-----------------------------------------------------------------------------
double ElQua4N2D::getArea()
//-----------------------------------------------------------------------------
{
    return sqrt(dnlSquare(nodes(1)->coordinates(0) * (nodes(0)->coordinates(1) - nodes(2)->coordinates(1)) + nodes(3)->coordinates(0) * (nodes(2)->coordinates(1) - nodes(0)->coordinates(1)) - (nodes(0)->coordinates(0) - nodes(2)->coordinates(0)) * (nodes(1)->coordinates(1) - nodes(3)->coordinates(1)))) / 2.0;
}
