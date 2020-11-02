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
  \file ElTet10N3D.C
  \brief Definition file for the ElTet10N3D class

  This file is the definition file for the ElTet10N3D class.

  \ingroup dnlElements
*/

#include <ElTet10N3D.h>

const ElementData ElTet10N3D::_elementData = {
    "ElTet10N3D",              // name of the Element
    Element::ElTet10N3D,       // Type of the Element
    Element::Threedimensional, // Family of the Element
    3,                         // Number of dimensions of the Element
    3,                         // Number of degrees of freedom of the Element
    24,                        // VTK type of element

    10, // Number of nodes of the Element
    {
        // Node 1
        {
            Vec3D(0.0, 0.0, 0.0),                                                                                       // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {4, 6, 7},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 2
        {
            Vec3D(1.0, 0.0, 0.0),                                                                                       // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {4, 5, 8},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 3
        {
            Vec3D(0.0, 1.0, 0.0),                                                                                       // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {6, 5, 9},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 4
        {
            Vec3D(0.0, 0.0, 1.0),                                                                                       // Coordinates of the node
            3,                                                                                                          // Number of neightbour nodes
            {7, 8, 9},                                                                                                  // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + 3.0 * sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 5
        {
            Vec3D(0.5, 0.0, 0.0),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {0, 1},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 + sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 6
        {
            Vec3D(0.5, 0.5, 0.0),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {1, 2},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 7
        {
            Vec3D(0.0, 0.5, 0.0),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {0, 2},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 8
        {
            Vec3D(0.0, 0.0, 0.5),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {0, 3},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 9
        {
            Vec3D(0.5, 0.0, 0.5),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {1, 3},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        },
        // Node 10
        {
            Vec3D(0.0, 0.5, 0.5),                                                                                 // Coordinates of the node
            2,                                                                                                    // Number of neightbour nodes
            {2, 3},                                                                                               // List of the neighbour nodes
            Vector(4, (1.0 - sqrt(5.0)) / 4, (1.0 - sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4, (1.0 + sqrt(5.0)) / 4) // Extrapolates integration points values to nodal values
        }
        //
    },

    4, // Number of integration points of the Element
    {
        // Integration point 1
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0),  // Coordinates of the integration point
            1.0 / 24.0,                                                                           // Weight associated to the integration point
            Vector(10, 0.1, -0.1, -0.1, -0.1,                                                     // Shape functions at the integration point
                   (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0,  //
                   (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0), //
            Matrix(10, 3,                                                                         // Derivatives of the shape functions at the integration point
                   -3.0 / sqrt(5.0), -3.0 / sqrt(5.0), -3.0 / sqrt(5.0),                          //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                  //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                  //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                  //
                   4 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                 //
                   1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                             //
                   -1.0 + 1.0 / sqrt(5.0), 4 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                 //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 4 / sqrt(5.0),                 //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                             //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                             //
        },

        // Integration point 2
        {
            Vec3D((5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0), // Coordinates of the integration point
            1.0 / 24.0,                                                                                // Weight associated to the integration point
            Vector(10, -0.1, 0.1, -0.1, -0.1,                                                          // Shape functions at the integration point
                   (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0,       //
                   (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   3.0 / sqrt(5.0), 0.0, 0.0,                                                          //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                       //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                       //
                   -4 / sqrt(5.0), -1 - 3.0 / sqrt(5.0), -1 - 3.0 / sqrt(5.0),                         //
                   1.0 - 1.0 / sqrt(5.0), 1 + 3.0 / sqrt(5.0), 0.0,                                    //
                   -1.0 + 1.0 / sqrt(5.0), 0.0, -1.0 + 1.0 / sqrt(5.0),                                //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 0.0,                                //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1 + 3.0 / sqrt(5.0),                                    //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                                  //
        },

        // Integration point 3
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0), // Coordinates of the integration point
            1.0 / 24.0,                                                                                // Weight associated to the integration point
            Vector(10, -0.1, -0.1, 0.1, -0.1,                                                          // Shape functions at the integration point
                   (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0,       //
                   (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                       //
                   0.0, 3.0 / sqrt(5.0), 0.0,                                                          //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                       //
                   0.0, -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                                //
                   1 + 3.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                                    //
                   -1 - 3.0 / sqrt(5.0), -4 / sqrt(5.0), -1 - 3.0 / sqrt(5.0),                         //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 0.0,                                //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                                  //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1 + 3.0 / sqrt(5.0))                                    //
        },

        // Integration point 4
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0), // Coordinates of the integration point
            1.0 / 24.0,                                                                                // Weight associated to the integration point
            Vector(10, -0.1, -0.1, -0.1, 0.1,                                                          // Shape functions at the integration point
                   (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0,       //
                   (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                       //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                       //
                   0.0, 0.0, 3.0 / sqrt(5.0),                                                          //
                   0.0, -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                                //
                   1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                                  //
                   -1.0 + 1.0 / sqrt(5.0), 0.0, -1.0 + 1.0 / sqrt(5.0),                                //
                   -1 - 3.0 / sqrt(5.0), -1 - 3.0 / sqrt(5.0), -4 / sqrt(5.0),                         //
                   1 + 3.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                                    //
                   0.0, 1 + 3.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                                    //
        }
        //
    },

    1, // Number of under integration points of the Element
    {
        // Under integration point 1
        {
            Vec3D(1.0 / 4, 1.0 / 4, 1.0 / 4),                             // Coordinates of the under integration point
            1.0 / 6,                                                      // Weight associated to the under integration point
            Vector(10, -1.0 / 8, -1.0 / 8, -1.0 / 8, -1.0 / 8,            // Shape functions at the under integration point
                   1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4), //
            Matrix(4, 3,                                                  // Derivatives of the shape functions at the under integration point
                   0.0, 0.0, 0.0,                                         //
                   0.0, 0.0, 0.0,                                         //
                   0.0, 0.0, 0.0,                                         //
                   0.0, 0.0, 0.0,                                         //
                   0.0, -1.0, -1.0,                                       //
                   +1.0, +1.0, 0.0,                                       //
                   -1.0, 0.0, -1.0,                                       //
                   -1.0, -1.0, 0.0,                                       //
                   +1.0, 0.0, +1.0,                                       //
                   0.0, +1.0, 1.0)                                        //
        }
        //
    },

    4, // Number of faces of the element
    {
        // Face number 1
        {
            6,                 // Number of nodes on the face of the element
            {0, 4, 1, 8, 3, 7} // List of the nodes composing the face of the element
        },
        // Face number 2
        {
            6,                 // Number of nodes on the face of the element
            {0, 6, 2, 5, 1, 4} // List of the nodes composing the face of the element
        },
        // Face number 3
        {
            6,                 // Number of nodes on the face of the element
            {0, 7, 3, 9, 2, 6} // List of the nodes composing the face of the element
        },
        // Face number 4
        {
            6,                 // Number of nodes on the face of the element
            {2, 9, 3, 8, 1, 5} // List of the nodes composing the face of the element
        }
        //
    },
    /*
    4, // Number of mass integration points of the Element
    {
        // Mass integration point 1
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0),  // Coordinates of the mass integration point
            1.0 / 24.0,                                                                           // Weight associated to the mass integration point
            Vector(10, 0.1, -0.1, -0.1, -0.1,                                                     // Shape functions at the mass integration point
                   (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0,  //
                   (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0), //
            Matrix(10, 3,                                                                         // Derivatives of the shape functions at the mass integration point
                   -3.0 / sqrt(5.0), -3.0 / sqrt(5.0), -3.0 / sqrt(5.0),                          //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                  //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                  //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                  //
                   4 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                 //
                   1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                             //
                   -1.0 + 1.0 / sqrt(5.0), 4 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                 //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 4 / sqrt(5.0),                 //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                             //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                             //
        },

        // Mass integration point 2
        {
            Vec3D((5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0), // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                // Weight associated to the mass integration point
            Vector(10, -0.1, 0.1, -0.1, -0.1,                                                          // Shape functions at the mass integration point
                   (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0,       //
                   (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the mass integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   3.0 / sqrt(5.0), 0.0, 0.0,                                                          //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                       //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                       //
                   -4 / sqrt(5.0), -1 - 3.0 / sqrt(5.0), -1 - 3.0 / sqrt(5.0),                         //
                   1.0 - 1.0 / sqrt(5.0), 1 + 3.0 / sqrt(5.0), 0.0,                                    //
                   -1.0 + 1.0 / sqrt(5.0), 0.0, -1.0 + 1.0 / sqrt(5.0),                                //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 0.0,                                //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1 + 3.0 / sqrt(5.0),                                    //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                                  //
        },

        // Mass integration point 3
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0), // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                // Weight associated to the mass integration point
            Vector(10, -0.1, -0.1, 0.1, -0.1,                                                          // Shape functions at the mass integration point
                   (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0,       //
                   (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the mass integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                       //
                   0.0, 3.0 / sqrt(5.0), 0.0,                                                          //
                   0.0, 0.0, -(1.0 / sqrt(5.0)),                                                       //
                   0.0, -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                                //
                   1 + 3.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                                    //
                   -1 - 3.0 / sqrt(5.0), -4 / sqrt(5.0), -1 - 3.0 / sqrt(5.0),                         //
                   -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0), 0.0,                                //
                   1.0 - 1.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                                  //
                   0.0, 1.0 - 1.0 / sqrt(5.0), 1 + 3.0 / sqrt(5.0))                                    //
        },

        // Mass integration point 4
        {
            Vec3D((5.0 - sqrt(5.0)) / 20.0, (5.0 - sqrt(5.0)) / 20.0, (5.0 + 3.0 * sqrt(5.0)) / 20.0), // Coordinates of the mass integration point
            1.0 / 24.0,                                                                                // Weight associated to the mass integration point
            Vector(10, -0.1, -0.1, -0.1, 0.1,                                                          // Shape functions at the mass integration point
                   (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0, (3.0 - sqrt(5.0)) / 10.0,       //
                   (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0, (1.0 + sqrt(5.0)) / 10.0),      //
            Matrix(10, 3,                                                                              // Derivatives of the shape functions at the mass integration point
                   1.0 / sqrt(5.0), 1.0 / sqrt(5.0), 1.0 / sqrt(5.0),                                  //
                   -(1.0 / sqrt(5.0)), 0.0, 0.0,                                                       //
                   0.0, -(1.0 / sqrt(5.0)), 0.0,                                                       //
                   0.0, 0.0, 3.0 / sqrt(5.0),                                                          //
                   0.0, -1.0 + 1.0 / sqrt(5.0), -1.0 + 1.0 / sqrt(5.0),                                //
                   1.0 - 1.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0), 0.0,                                  //
                   -1.0 + 1.0 / sqrt(5.0), 0.0, -1.0 + 1.0 / sqrt(5.0),                                //
                   -1 - 3.0 / sqrt(5.0), -1 - 3.0 / sqrt(5.0), -4 / sqrt(5.0),                         //
                   1 + 3.0 / sqrt(5.0), 0.0, 1.0 - 1.0 / sqrt(5.0),                                    //
                   0.0, 1 + 3.0 / sqrt(5.0), 1.0 - 1.0 / sqrt(5.0))                                    //
        }
        //
    },

    4, // Number of side faces of the element
    {
        // Side face number 1
        {
            6,                 // Number of nodes on the side face of the element
            {0, 4, 1, 8, 3, 7} // List of the nodes composing the side face of the element
        },

        // Side face number 2
        {
            6,                 // Number of nodes on the side face of the element
            {0, 6, 2, 5, 1, 4} // List of the nodes composing the side face of the element
        },

        // Side face number 3
        {
            6,                 // Number of nodes on the side face of the element
            {0, 7, 3, 9, 2, 6} // List of the nodes composing the side face of the element
        },

        // Side face number 4
        {
            6,                 // Number of nodes on the side face of the element
            {2, 9, 3, 8, 1, 5} // List of the nodes composing the side face of the element
        }
        //
    },

    6, // Number of edges of the element
    {
        // Edge number 1
        {
            3,        // Number of nodes on the edge of the element
            {0, 4, 1} // List of the nodes composing the edge of the element
        },

        // Edge number 2
        {
            3,        // Number of nodes on the edge of the element
            {1, 5, 2} // List of the nodes composing the edge of the element
        },

        // Edge number 3
        {
            3,        // Number of nodes on the edge of the element
            {2, 6, 0} // List of the nodes composing the edge of the element
        },

        // Edge number 4
        {
            3,        // Number of nodes on the edge of the element
            {0, 7, 3} // List of the nodes composing the edge of the element
        },

        // Edge number 5
        {
            3,        // Number of nodes on the edge of the element
            {1, 8, 3} // List of the nodes composing the edge of the element
        },

        // Edge number 6
        {
            3,        // Number of nodes on the edge of the element
            {2, 9, 3} // List of the nodes composing the edge of the element
        }
        //
    },*/
    // Check element
    10010110
    // END
};

//-----------------------------------------------------------------------------
ElTet10N3D::ElTet10N3D(long elementNumber) : Element3D(elementNumber)
//-----------------------------------------------------------------------------
{
    // redim du _globalToLocal ?
    _globalToLocal.redim(3, 10);
    Element::_elementData = &_elementData;
    stiffnessMatrix.redim(30, 30);

    // Check the data structure
    if (_elementData.CheckElementData != 10010110)
        fatalError("ElTet10N3D::ElTet10N3D", "Element Data currupted");
}

//-----------------------------------------------------------------------------
ElTet10N3D::ElTet10N3D(const ElTet10N3D &el) : Element3D(el)
//-----------------------------------------------------------------------------
{
    std::cout << "recopie d'un ElTet10N3D\n";
}

//-----------------------------------------------------------------------------
ElTet10N3D::~ElTet10N3D()
//-----------------------------------------------------------------------------
{
    nodes.flush();
}

//-----------------------------------------------------------------------------
void ElTet10N3D::getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);
    double zeta = point(2);
    double xez = 1.0 - ksi - eta - zeta;

    shapeFunctions(0) = xez * (2.0 * xez - 1.0);
    shapeFunctions(1) = ksi * (2.0 * ksi - 1.0);
    shapeFunctions(2) = eta * (2.0 * eta - 1.0);
    shapeFunctions(3) = zeta * (2.0 * zeta - 1.0);
    shapeFunctions(4) = 4.0 * xez * ksi;
    shapeFunctions(5) = 4.0 * ksi * eta;
    shapeFunctions(6) = 4.0 * xez * eta;
    shapeFunctions(7) = 4.0 * xez * zeta;
    shapeFunctions(8) = 4.0 * ksi * zeta;
    shapeFunctions(9) = 4.0 * eta * zeta;
}

//-----------------------------------------------------------------------------
void ElTet10N3D::getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);
    double zeta = point(2);

    derShapeFunctions(0, 0) = 1.0 - 4.0 * (1.0 - ksi - eta - zeta);
    derShapeFunctions(1, 0) = 4.0 * ksi - 1.0;
    derShapeFunctions(2, 0) = 0.0;
    derShapeFunctions(3, 0) = 0.0;
    derShapeFunctions(4, 0) = 4.0 * (1.0 - 2.0 * ksi - eta - zeta);
    derShapeFunctions(5, 0) = 4.0 * eta;
    derShapeFunctions(6, 0) = -4.0 * eta;
    derShapeFunctions(7, 0) = -4.0 * zeta;
    derShapeFunctions(8, 0) = 4.0 * zeta;
    derShapeFunctions(9, 0) = 0.0;

    derShapeFunctions(0, 1) = 1.0 - 4.0 * (1.0 - ksi - eta - zeta);
    derShapeFunctions(1, 1) = 0.0;
    derShapeFunctions(2, 1) = 4.0 * eta - 1.0;
    derShapeFunctions(3, 1) = 0.0;
    derShapeFunctions(4, 1) = -4.0 * ksi;
    derShapeFunctions(5, 1) = 4.0 * ksi;
    derShapeFunctions(6, 1) = 4.0 * (1.0 - ksi - 2.0 * eta - zeta);
    derShapeFunctions(7, 1) = -4.0 * zeta;
    derShapeFunctions(8, 1) = 0.0;
    derShapeFunctions(9, 1) = 4.0 * zeta;

    derShapeFunctions(0, 2) = 1.0 - 4.0 * (1.0 - ksi - eta - zeta);
    derShapeFunctions(1, 2) = 0.0;
    derShapeFunctions(2, 2) = 0.0;
    derShapeFunctions(3, 2) = 4.0 * zeta - 1.0;
    derShapeFunctions(4, 2) = -4.0 * ksi;
    derShapeFunctions(5, 2) = 0.0;
    derShapeFunctions(6, 2) = -4.0 * eta;
    derShapeFunctions(7, 2) = 4.0 * (1.0 - ksi - eta - 2.0 * zeta);
    derShapeFunctions(8, 2) = 4.0 * ksi;
    derShapeFunctions(9, 2) = 4.0 * eta;
}

/*
//-----------------------------------------------------------------------------
double ElTet10N3D::getCharacteristicLength()
//-----------------------------------------------------------------------------
{
  double l1,l2,l3,l4,l5,l6,ll;
  l1=nodes(0)->coordinates.distance(nodes(1)->coordinates);
  l2=nodes(0)->coordinates.distance(nodes(2)->coordinates);
  l3=nodes(1)->coordinates.distance(nodes(2)->coordinates);
  l4=nodes(0)->coordinates.distance(nodes(3)->coordinates);
  l5=nodes(1)->coordinates.distance(nodes(3)->coordinates);
  l6=nodes(2)->coordinates.distance(nodes(3)->coordinates);
  ll=dnlMin(l1,l2);
  ll=dnlMin(ll,l3);
  ll=dnlMin(ll,l4);
  ll=dnlMin(ll,l5);
  ll=dnlMin(ll,l6);
  return ll;
}

//-----------------------------------------------------------------------------
double ElTet10N3D::getVolume()
//-----------------------------------------------------------------------------
{
  cerr << "ElTet10N3D::getVolume() erreur ElHex8N3D non implemente\n";
  exit(-1);

  double l1,l2,l3,l4,l5;
  l1=nodes(1)->coordinates.distance(nodes(2)->coordinates);
  l2=nodes(2)->coordinates.distance(nodes(3)->coordinates);
  l3=nodes(3)->coordinates.distance(nodes(4)->coordinates);
  l4=nodes(4)->coordinates.distance(nodes(1)->coordinates);
  l5=nodes(1)->coordinates.distance(nodes(3)->coordinates);
  return (dnlSurfTriangle(l1,l2,l5)+dnlSurfTriangle(l3,l4,l5));
}





// //-----------------------------------------------------------------------------
// void ElTet10N3D::getIntgtoNodes(Vector& N,const Vec3D& point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElTet10N3D::getIntgtoNodes\n";
//   exit(-1);
// }

//-----------------------------------------------------------------------------
void ElTet10N3D::computeGlob2Loc()
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
      nds (i, 1) = pnd->coordinates (0);
      nds (i, 2) = pnd->coordinates (1);
      nds (i, 3) = pnd->coordinates (2);
    }

  // inversion de la matrice
  Matrix inv = nds.getInverse ();

  for (i = 0; i < getNumberOfNodes(); i++)
    for (j = 0; j < 3; j++)
      cords(i,j) = getLocalNodeCoords(i)(j);

  _globalToLocal = (inv * cords).getTranspose();
}

//-----------------------------------------------------------------------------
void ElTet10N3D::glob2Loc(const Vec3D& point,Vec3D& local)
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
