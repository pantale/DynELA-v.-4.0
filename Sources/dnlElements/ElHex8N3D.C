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
  \file ElHex8N3D.C
  Definition file for the ElHex8N3D class

  This file is the definition file for the ElHex8N3D class.

  \ingroup dnlElements
*/

#include <ElHex8N3D.h>
#include <Node.h>

const ElementData ElHex8N3D::_elementData = {
    "ElHex8N3D",               // name of the Element
    Element::ElHex8N3D,        // Type of the Element
    Element::Threedimensional, // Family of the Element
    3,                         // Number of dimensions of the Element
    3,                         // Number of degrees of freedom of the Element
    12,                        // VTK type of element

    8, // Number of nodes of the Element
    {
        // Node 1
        {
            Vec3D(-1, -1, -1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {1, 3, 4},                                                                                                     // List of the neighbour nodes
            Vector(8, (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 2
        {
            Vec3D(+1, -1, -1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {0, 5, 2},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 3
        {
            Vec3D(+1, +1, -1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {1, 6, 3},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 4
        {
            Vec3D(-1, +1, -1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {2, 0, 7},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 5
        {
            Vec3D(-1, -1, +1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {0, 5, 7},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 6
        {
            Vec3D(+1, -1, +1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {1, 4, 6},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 7
        {
            Vec3D(+1, +1, +1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {5, 7, 2},                                                                                                     // List of the neighbour nodes
            Vector(8, (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4)},  //
                                                                                                                          // Node 8
        {
            Vec3D(-1, +1, +1),                                                                                             // Coordinates of the node
            3,                                                                                                             // Number of neightbour nodes
            {3, 6, 4},                                                                                                     // List of the neighbour nodes
            Vector(8, (-1.0 + sqrt(3.0)) / 4, (5.0 - 3.0 * sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, // Extrapolates integration points values to nodal values
                   (-1.0 - sqrt(3.0)) / 4, (-1.0 + sqrt(3.0)) / 4, (-1.0 - sqrt(3.0)) / 4, (5.0 + 3.0 * sqrt(3.0)) / 4)}   //
    },

    8, // Number of integration points of the Element
    {
        // Integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                         //
                   (2.0 + sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                      //
                   1.0 / 12, 1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                       //
                   -1.0 / 12, (2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   -1.0 / 12, -1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                      //
                   1.0 / 12, (-2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                       //
                   (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                            //
                   (-2.0 + sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12)                                                       //
        },

        // Integration point 2
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   (-2.0 - sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                     //
                   (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                          //
                   1.0 / 12, (2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                       //
                   -1.0 / 12, 1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                      //
                   -1.0 / 12, (-2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                      //
                   1.0 / 12, -1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                       //
                   (2.0 - sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12,                                                        //
                   (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12)                           //
        },

        // Integration point 3
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the integration point
                   (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   -1.0 / 12, -1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                     //
                   1.0 / 12, (-2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                           //
                   (-2.0 - sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                      //
                   (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                          //
                   (2.0 - sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                       //
                   1.0 / 12, 1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                        //
                   -1.0 / 12, (2.0 - sqrt(3.0)) / 12, 1.0 / 12)                                                       //
        },

        // Integration point 4
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   -1.0 / 12, (-2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                     //
                   1.0 / 12, -1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                       //
                   (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                          //
                   (-2.0 + sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                           //
                   1.0 / 12, (2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                        //
                   -1.0 / 12, 1.0 / 12, (2.0 + sqrt(3.0)) / 12)                                                       //
        },

        // Integration point 5
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the integration point
                   (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   -1.0 / 12, -1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                     //
                   1.0 / 12, (-2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                           //
                   (-2.0 + sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                      //
                   (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                          //
                   (2.0 + sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                       //
                   1.0 / 12, 1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                        //
                   -1.0 / 12, (2.0 + sqrt(3.0)) / 12, 1.0 / 12)                                                       //
        },

        // Integration point 6
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   -1.0 / 12, (-2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                     //
                   1.0 / 12, -1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                       //
                   (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                          //
                   (-2.0 - sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                           //
                   1.0 / 12, (2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                        //
                   -1.0 / 12, 1.0 / 12, (2.0 - sqrt(3.0)) / 12)                                                       //
        },

        // Integration point 7
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                         //
                   (2.0 - sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                      //
                   1.0 / 12, 1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                       //
                   -1.0 / 12, (2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   -1.0 / 12, -1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                      //
                   1.0 / 12, (-2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                       //
                   (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                            //
                   (-2.0 - sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12)                                                       //
        },

        // Integration point 8
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the integration point
            1,                                                                                                        // Weight associated to the integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the integration point
                   (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the integration point
                   (-2.0 + sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                     //
                   (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                          //
                   1.0 / 12, (2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                       //
                   -1.0 / 12, 1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                      //
                   -1.0 / 12, (-2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                      //
                   1.0 / 12, -1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                       //
                   (2.0 + sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12,                                                        //
                   (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12)                           //
        }
        //
    },

    1, // Number of under integration points of the Element
    {
        // Under integration point 1
        {
            Vec3D(0.0, 0.0, 0.0),                             // Coordinates of the under integration point
            8.0,                                              // Weight associated to the under integration point
            Vector(8, +1.0 / 8, +1.0 / 8, +1.0 / 8, +1.0 / 8, // Shape functions at the under integration point
                   +1.0 / 8, +1.0 / 8, +1.0 / 8, +1.0 / 8),   //
            Matrix(8, 3,                                      // Derivatives of the shape functions at the under integration point
                   -1.0 / 8, -1.0 / 8, -1.0 / 8,              //
                   1.0 / 8, -1.0 / 8, -1.0 / 8,               //
                   1.0 / 8, 1.0 / 8, -1.0 / 8,                //
                   -1.0 / 8, 1.0 / 8, -1.0 / 8,               //
                   -1.0 / 8, -1.0 / 8, 1.0 / 8,               //
                   1.0 / 8, -1.0 / 8, 1.0 / 8,                //
                   1.0 / 8, 1.0 / 8, 1.0 / 8,                 //
                   -1.0 / 8, 1.0 / 8, 1.0 / 8)                //
        },
        //
    },

    6, // Number of faces of the element
    {
        // Face number 1
        {
            4,           // Number of nodes on the face of the element
            {0, 1, 5, 4} // List of the nodes composing the face of the element
        },
        // Face number 2
        {
            4,           // Number of nodes on the face of the element
            {1, 2, 6, 5} // List of the nodes composing the face of the element
        },
        // Face number 3
        {
            4,           // Number of nodes on the face of the element
            {2, 3, 7, 6} // List of the nodes composing the face of the element
        },
        // Face number 4
        {
            4,           // Number of nodes on the face of the element
            {3, 0, 4, 7} // List of the nodes composing the face of the element
        },
        // Face number 5
        {
            4,           // Number of nodes on the face of the element
            {7, 4, 5, 6} // List of the nodes composing the face of the element
        },
        // Face number 6
        {
            4,           // Number of nodes on the face of the element
            {2, 1, 0, 3} // List of the nodes composing the face of the element
        }
        //
    },
    /*
    8, // Number of mass integration points of the Element
    {
        // Mass integration point 1
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                         //
                   (2.0 + sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                      //
                   1.0 / 12, 1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                       //
                   -1.0 / 12, (2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   -1.0 / 12, -1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                      //
                   1.0 / 12, (-2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                       //
                   (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                            //
                   (-2.0 + sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12)                                                       //
        },

        // Mass integration point 2
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   (-2.0 - sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                     //
                   (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                          //
                   1.0 / 12, (2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                       //
                   -1.0 / 12, 1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                      //
                   -1.0 / 12, (-2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                      //
                   1.0 / 12, -1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                       //
                   (2.0 - sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12,                                                        //
                   (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12)                           //
        },

        // Mass integration point 3
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   -1.0 / 12, -1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                     //
                   1.0 / 12, (-2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                           //
                   (-2.0 - sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                      //
                   (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                          //
                   (2.0 - sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                       //
                   1.0 / 12, 1.0 / 12, (2.0 + sqrt(3.0)) / 12,                                                        //
                   -1.0 / 12, (2.0 - sqrt(3.0)) / 12, 1.0 / 12)                                                       //
        },

        // Mass integration point 4
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), -1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   -1.0 / 12, (-2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                     //
                   1.0 / 12, -1.0 / 12, (-2.0 + sqrt(3.0)) / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                       //
                   (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12,                          //
                   (-2.0 + sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12,                           //
                   1.0 / 12, (2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                        //
                   -1.0 / 12, 1.0 / 12, (2.0 + sqrt(3.0)) / 12)                                                       //
        },

        // Mass integration point 5
        {
            Vec3D(-1.0 / sqrt(3.0), -1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   -1.0 / 12, -1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                     //
                   1.0 / 12, (-2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                           //
                   (-2.0 + sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                      //
                   (-2.0 - sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                          //
                   (2.0 + sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                       //
                   1.0 / 12, 1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                        //
                   -1.0 / 12, (2.0 + sqrt(3.0)) / 12, 1.0 / 12)                                                       //
        },

        // Mass integration point 6
        {
            Vec3D(+1.0 / sqrt(3.0), -1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   -1.0 / 12, (-2.0 + sqrt(3.0)) / 12, -1.0 / 12,                                                     //
                   1.0 / 12, -1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                      //
                   (2.0 - sqrt(3.0)) / 12, 1.0 / 12, -1.0 / 12,                                                       //
                   (-2.0 + sqrt(3.0)) / 12, (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                          //
                   (-2.0 - sqrt(3.0)) / 12, -1.0 / 12, 1.0 / 12,                                                      //
                   (2.0 + sqrt(3.0)) / 12, (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                           //
                   1.0 / 12, (2.0 + sqrt(3.0)) / 12, 1.0 / 12,                                                        //
                   -1.0 / 12, 1.0 / 12, (2.0 - sqrt(3.0)) / 12)                                                       //
        },

        // Mass integration point 7
        {
            Vec3D(+1.0 / sqrt(3.0), +1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                         //
                   (2.0 - sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                      //
                   1.0 / 12, 1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                       //
                   -1.0 / 12, (2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                      //
                   -1.0 / 12, -1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                      //
                   1.0 / 12, (-2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                       //
                   (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12,                            //
                   (-2.0 - sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12)                                                       //
        },

        // Mass integration point 8
        {
            Vec3D(-1.0 / sqrt(3.0), +1.0 / sqrt(3.0), +1.0 / sqrt(3.0)),                                              // Coordinates of the mass integration point
            1,                                                                                                        // Weight associated to the mass integration point
            Vector(8, (3 - sqrt(3.0)) / 36, (9.0 - 5.0 * sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, // Shape functions at the mass integration point
                   (3 + sqrt(3.0)) / 36, (3 - sqrt(3.0)) / 36, (3 + sqrt(3.0)) / 36, (9.0 + 5.0 * sqrt(3.0)) / 36),   //
            Matrix(8, 3,                                                                                              // Derivatives of the shape functions at the mass integration point
                   (-2.0 + sqrt(3.0)) / 12, -1.0 / 12, -1.0 / 12,                                                     //
                   (2.0 - sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12, (-2.0 + sqrt(3.0)) / 12,                          //
                   1.0 / 12, (2.0 - sqrt(3.0)) / 12, -1.0 / 12,                                                       //
                   -1.0 / 12, 1.0 / 12, (-2.0 - sqrt(3.0)) / 12,                                                      //
                   -1.0 / 12, (-2.0 - sqrt(3.0)) / 12, 1.0 / 12,                                                      //
                   1.0 / 12, -1.0 / 12, (2.0 - sqrt(3.0)) / 12,                                                       //
                   (2.0 + sqrt(3.0)) / 12, 1.0 / 12, 1.0 / 12,                                                        //
                   (-2.0 - sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12, (2.0 + sqrt(3.0)) / 12)                           //
        }
        //
    },

    6, // Number of side faces of the element
    {
        // Side face number 1
        {
            4,           // Number of nodes on the side face of the element
            {0, 1, 5, 4} // List of the nodes composing the side face of the element
        },
        // Side face number 2
        {
            4,           // Number of nodes on the side face of the element
            {1, 2, 6, 5} // List of the nodes composing the side face of the element
        },
        // Side face number 3
        {
            4,           // Number of nodes on the side face of the element
            {2, 3, 7, 6} // List of the nodes composing the side face of the element
        },
        // Side face number 4
        {
            4,           // Number of nodes on the side face of the element
            {3, 0, 4, 7} // List of the nodes composing the side face of the element
        },
        // Side face number 5
        {
            4,           // Number of nodes on the side face of the element
            {7, 4, 5, 6} // List of the nodes composing the side face of the element
        },
        // Side face number 6
        {
            4,           // Number of nodes on the side face of the element
            {2, 1, 0, 3} // List of the nodes composing the side face of the element
        }
        //
    },

    12, // Number of edges of the element
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
        },
        // Edge number 5
        {
            2,     // Number of nodes on the edge of the element
            {4, 5} // List of the nodes composing the edge of the element
        },
        // Edge number 6
        {
            2,     // Number of nodes on the edge of the element
            {5, 6} // List of the nodes composing the edge of the element
        },
        // Edge number 7
        {
            2,     // Number of nodes on the edge of the element
            {6, 7} // List of the nodes composing the edge of the element
        },
        // Edge number 8
        {
            2,     // Number of nodes on the edge of the element
            {7, 4} // List of the nodes composing the edge of the element
        },
        // Edge number 9
        {
            2,     // Number of nodes on the edge of the element
            {0, 4} // List of the nodes composing the edge of the element
        },
        // Edge number 10
        {
            2,     // Number of nodes on the edge of the element
            {1, 5} // List of the nodes composing the edge of the element
        },
        // Edge number 11
        {
            2,     // Number of nodes on the edge of the element
            {2, 6} // List of the nodes composing the edge of the element
        },
        // Edge number 12
        {
            2,     // Number of nodes on the edge of the element
            {3, 7} // List of the nodes composing the edge of the element
        }
        //
    },*/
    // Check element
    Element::ElHex8N3D
    // END
};

//-----------------------------------------------------------------------------
ElHex8N3D::ElHex8N3D(long elementNumber) : Element3D(elementNumber)
//-----------------------------------------------------------------------------
{
    _globalToLocal.redim(3, 8);
    Element::_elementData = &_elementData;
    stiffnessMatrix.redim(24, 24);

    // Check the data structure
    if (_elementData.CheckElementData != Element::ElHex8N3D)
        fatalError("ElHex8N3D::ElHex8N3D", "Element Data currupted");
}

//-----------------------------------------------------------------------------
ElHex8N3D::ElHex8N3D(const ElHex8N3D &element) : Element3D(element)
//-----------------------------------------------------------------------------
{
    std::cout << "recopie d'un ElHex8N3D\n";
}

//-----------------------------------------------------------------------------
ElHex8N3D::~ElHex8N3D()
//-----------------------------------------------------------------------------
{
    nodes.flush();
}

//-----------------------------------------------------------------------------
void ElHex8N3D::getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);
    double zeta = point(2);

    shapeFunctions(0) = (1.0 - ksi) * (1.0 - eta) * (1.0 - zeta) / 8;
    shapeFunctions(1) = (1.0 + ksi) * (1.0 - eta) * (1.0 - zeta) / 8;
    shapeFunctions(2) = (1.0 + ksi) * (1.0 + eta) * (1.0 - zeta) / 8;
    shapeFunctions(3) = (1.0 - ksi) * (1.0 + eta) * (1.0 - zeta) / 8;
    shapeFunctions(4) = (1.0 - ksi) * (1.0 - eta) * (1.0 + zeta) / 8;
    shapeFunctions(5) = (1.0 + ksi) * (1.0 - eta) * (1.0 + zeta) / 8;
    shapeFunctions(6) = (1.0 + ksi) * (1.0 + eta) * (1.0 + zeta) / 8;
    shapeFunctions(7) = (1.0 - ksi) * (1.0 + eta) * (1.0 + zeta) / 8;
}

//-----------------------------------------------------------------------------
void ElHex8N3D::getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const
//-----------------------------------------------------------------------------
{
    double ksi = point(0);
    double eta = point(1);
    double zeta = point(2);

    derShapeFunctions(0, 0) = -(1.0 - eta) * (1.0 - zeta) / 8;
    derShapeFunctions(1, 0) = +(1.0 - eta) * (1.0 - zeta) / 8;
    derShapeFunctions(2, 0) = +(1.0 + eta) * (1.0 - zeta) / 8;
    derShapeFunctions(3, 0) = -(1.0 + eta) * (1.0 - zeta) / 8;
    derShapeFunctions(4, 0) = -(1.0 - eta) * (1.0 + zeta) / 8;
    derShapeFunctions(5, 0) = +(1.0 - eta) * (1.0 + zeta) / 8;
    derShapeFunctions(6, 0) = +(1.0 + eta) * (1.0 + zeta) / 8;
    derShapeFunctions(7, 0) = -(1.0 + eta) * (1.0 + zeta) / 8;

    derShapeFunctions(0, 1) = -(1.0 - ksi) * (1.0 - zeta) / 8;
    derShapeFunctions(1, 1) = -(1.0 + ksi) * (1.0 - zeta) / 8;
    derShapeFunctions(2, 1) = +(1.0 + ksi) * (1.0 - zeta) / 8;
    derShapeFunctions(3, 1) = +(1.0 - ksi) * (1.0 - zeta) / 8;
    derShapeFunctions(4, 1) = -(1.0 - ksi) * (1.0 + zeta) / 8;
    derShapeFunctions(5, 1) = -(1.0 + ksi) * (1.0 + zeta) / 8;
    derShapeFunctions(6, 1) = +(1.0 + ksi) * (1.0 + zeta) / 8;
    derShapeFunctions(7, 1) = +(1.0 - ksi) * (1.0 + zeta) / 8;

    derShapeFunctions(0, 2) = -(1.0 - ksi) * (1.0 - eta) / 8;
    derShapeFunctions(1, 2) = -(1.0 + ksi) * (1.0 - eta) / 8;
    derShapeFunctions(2, 2) = -(1.0 + ksi) * (1.0 + eta) / 8;
    derShapeFunctions(3, 2) = -(1.0 - ksi) * (1.0 + eta) / 8;
    derShapeFunctions(4, 2) = +(1.0 - ksi) * (1.0 - eta) / 8;
    derShapeFunctions(5, 2) = +(1.0 + ksi) * (1.0 - eta) / 8;
    derShapeFunctions(6, 2) = +(1.0 + ksi) * (1.0 + eta) / 8;
    derShapeFunctions(7, 2) = +(1.0 - ksi) * (1.0 + eta) / 8;
}

#define AERT(res, p1, p2, p3, p4)                                                                                                                          \
    {                                                                                                                                                      \
        double x1 = nodes(p1)->coords(0);                                                                                                             \
        double x2 = nodes(p2)->coords(0);                                                                                                             \
        double x3 = nodes(p3)->coords(0);                                                                                                             \
        double x4 = nodes(p4)->coords(0);                                                                                                             \
        double y1 = nodes(p1)->coords(1);                                                                                                             \
        double y2 = nodes(p2)->coords(1);                                                                                                             \
        double y3 = nodes(p3)->coords(1);                                                                                                             \
        double y4 = nodes(p4)->coords(1);                                                                                                             \
        double z1 = nodes(p1)->coords(2);                                                                                                             \
        double z2 = nodes(p2)->coords(2);                                                                                                             \
        double z3 = nodes(p3)->coords(2);                                                                                                             \
        double z4 = nodes(p4)->coords(2);                                                                                                             \
        res = sqrt(4 * (dnlSquare(x1 - x3) + dnlSquare(y1 - y3) + dnlSquare(z1 - z3)) * (dnlSquare(x2 - x4) + dnlSquare(y2 - y4) + dnlSquare(z2 - z4)) -   \
                   4 * dnlSquare(-(x2 * x3) + x1 * (x2 - x4) + x3 * x4 + y1 * y2 - y2 * y3 - y1 * y4 + y3 * y4 + z1 * z2 - z2 * z3 - z1 * z4 + z3 * z4)) / \
              4;                                                                                                                                           \
    }

//-----------------------------------------------------------------------------
double ElHex8N3D::getCharacteristicLength()
//-----------------------------------------------------------------------------
{
    double A1, A2, A3, A4, A5, A6;
    AERT(A1, 0, 1, 2, 3)
    AERT(A2, 0, 1, 5, 4)
    AERT(A3, 1, 2, 6, 5)
    AERT(A4, 3, 2, 6, 7)
    AERT(A5, 0, 3, 7, 4)
    AERT(A6, 4, 5, 6, 7)
    A1 = dnlMax3(A1, A2, A3);
    A2 = dnlMax3(A4, A5, A6);
    A1 = dnlMax(A1, A2);
    return getVolume() / A1;
}

//-----------------------------------------------------------------------------
double ElHex8N3D::getVolume()
//-----------------------------------------------------------------------------
{
    double base;
    Vec3D norm;
    double height;
    double volume = 0.0;

    base = (((nodes(1)->coords - nodes(0)->coords).vectorProduct(nodes(2)->coords - nodes(0)->coords)).getNorm() +
            ((nodes(3)->coords - nodes(0)->coords).vectorProduct(nodes(2)->coords - nodes(0)->coords)).getNorm()) /
           2.0;
    norm = (nodes(1)->coords - nodes(0)->coords).vectorProduct(nodes(3)->coords - nodes(0)->coords);
    norm.normalize();
    height = dnlAbs(norm.dot(nodes(7)->coords - nodes(0)->coords));
    volume += base * height / 3.0;

    base = (((nodes(1)->coords - nodes(0)->coords).vectorProduct(nodes(5)->coords - nodes(0)->coords)).getNorm() +
            ((nodes(4)->coords - nodes(0)->coords).vectorProduct(nodes(5)->coords - nodes(0)->coords)).getNorm()) /
           2.0;
    norm = (nodes(1)->coords - nodes(0)->coords).vectorProduct(nodes(4)->coords - nodes(0)->coords);
    norm.normalize();
    height = dnlAbs(norm.dot(nodes(7)->coords - nodes(0)->coords));
    volume += base * height / 3.0;

    base = (((nodes(2)->coords - nodes(1)->coords).vectorProduct(nodes(6)->coords - nodes(1)->coords)).getNorm() +
            ((nodes(5)->coords - nodes(1)->coords).vectorProduct(nodes(6)->coords - nodes(1)->coords)).getNorm()) /
           2.0;
    norm = (nodes(2)->coords - nodes(1)->coords).vectorProduct(nodes(5)->coords - nodes(1)->coords);
    norm.normalize();
    height = dnlAbs(norm.dot(nodes(7)->coords - nodes(1)->coords));
    volume += base * height / 3.0;

    return volume;
}

/*


//-----------------------------------------------------------------------------
double ElHex8N3D::getVolume ()
//-----------------------------------------------------------------------------
{
  cerr << "erreur ElHex8N3D non implemente\n";
  exit (-1);

  double l1, l2, l3, l4, l5;
  l1 = nodes (1)->coords.distance (nodes (2)->coords);
  l2 = nodes (2)->coords.distance (nodes (3)->coords);
  l3 = nodes (3)->coords.distance (nodes (4)->coords);
  l4 = nodes (4)->coords.distance (nodes (1)->coords);
  l5 = nodes (1)->coords.distance (nodes (3)->coords);
  return (dnlSurfTriangle (l1, l2, l5) + dnlSurfTriangle (l3, l4, l5));
}



// //-----------------------------------------------------------------------------
// void ElHex8N3D::getIntgtoNodes (Vector & N, const Vec3D & point) const
// //-----------------------------------------------------------------------------
// {
//   cout << N << point << endl;
//   cerr << "unknown ElHex8N3D::getIntgtoNodes\n";
//   exit (-1);
// }

//-----------------------------------------------------------------------------
void ElHex8N3D::computeGlob2Loc ()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ElHex8N3D::glob2Loc (const Vec3D & point, Vec3D & local)
//-----------------------------------------------------------------------------
{
  cout << point << local << endl;
}
 */
