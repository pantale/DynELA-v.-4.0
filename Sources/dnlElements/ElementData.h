/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
\file ElementData.h
\brief Declaration file for the ElementData class

This file is the declaration file for the ElementData class.

\ingroup dnlElements
*/

#ifndef __dnlElements_ElementData_h__
#define __dnlElements_ElementData_h__

#define maxNumberOfEdges 12                 //!< Maximum number of edges for the elements
#define maxNumberOfFaces 6                  //!< Maximum number of faces for the elements
#define maxNumberOfIntegrationPoints 8      //!< Maximum number of integration points for the elements
#define maxNumberOfNodes 10                 //!< Maximum number of nodes for the elements
#define maxNumberOfNodesByFace 6            //!< Maximum number of nodes by face for the elements
#define maxNumberOfNodesNeighbour 3         //!< Maximum number of neighbour nodes for the elements
#define maxNumberOfUnderIntegrationPoints 1 //!< Maximum number of under integration points for the elements
//#define maxNumberOfNodesByEdge 3            //!< Maximum number of nodes connected to an edge for the elements
//#define maxNumberOfNodesBySideFace 6        //!< Maximum number of nodes by sideface for the elements
//#define maxNumberOfSideFaces 6              //!< Maximum number of sidefaces for the elements

/*!
  \brief Element structure
*/
struct ElementNode
{
  Vec3D localCoords;                          //!< Coordinates of the node
  short numberOfNeighbourNodes;               //!< Number of neightbour nodes
  short neighbour[maxNumberOfNodesNeighbour]; //!< List of the neighbour nodes
  Vector integrationPointsToNode;             //!< Extrapolates integration points values to nodal values
};

/*!
  \brief Integration points data
*/
struct IntegrationPointData
{
  Vec3D coords;            //!< Coordinates of the integration point
  double weight;           //!< Weight associated to the integration point
  Vector shapeFunction;    //!< Shape functions at the integration point
  Matrix derShapeFunction; //!< Derivatives of the shape functions at the integration point
};

/*!
  \brief Faces of the element
*/
struct ElementFace
{
  short numberOfNodes;                  //!< Number of nodes on the face of the element
  short number[maxNumberOfNodesByFace]; //!< List of the nodes composing the face of the element
};

/* struct ElementSideFace
{
    short numberOfNodes;                      //!< Number of nodes on the side face of the element
    short number[maxNumberOfNodesBySideFace]; //!< List of the nodes composing the side face of the element
};
 */
/* struct ElementEdge
{
    short numberOfNodes;                  //!< Number of nodes on the edge of the element
    short number[maxNumberOfNodesByEdge]; //!< List of the nodes composing the edge of the element
};
 */
/*!
  \brief Storage structure for Element database
*/
struct ElementData
{
  String name;                                                                   //!< name of the Element
  short type;                                                                    //!< Type of the Element
  short family;                                                                  //!< Family of the Element
  short numberOfDimensions;                                                      //!< Number of dimensions of the Element
  short numberOfDDL;                                                             //!< Number of degrees of freedom of the Element
  short vtkType;                                                                 //!< VTK type of element
  int numberOfNodes;                                                             //!< Number of nodes of the Element
  ElementNode nodes[maxNumberOfNodes];                                           //!< Lists of the nodes of the Element
  int numberOfIntegrationPoints;                                                 //!< Number of integration points of the Element
  IntegrationPointData integrationPoint[maxNumberOfIntegrationPoints];           //!< Lists of integration points of the Element
                                                                                 //    int numberOfMassIntegrationPoints;                                        // Number of under integration points of the Element
                                                                                 //    IntegrationPointData massIntegrationPoint[maxNumberOfIntegrationPoints];  // Lists of integration points of the Element
  int numberOfUnderIntegrationPoints;                                            //!< Number of under integration points of the Element
  IntegrationPointData underIntegrationPoint[maxNumberOfUnderIntegrationPoints]; //!< Lists of under integration points of the Element
  int numberOfFaces;                                                             //!< Number of faces of the element
  ElementFace faces[maxNumberOfFaces];                                           //!< List of the faces of the element
                                                                                 //    int numberOfSideFaces;                                                    // Number of side faces of the element
                                                                                 //    ElementSideFace sideFaces[maxNumberOfSideFaces];                          // List of the side faces of the element
                                                                                 //    int numberOfEdges;                                                        // Number of edges of the element
                                                                                 //    ElementEdge edges[maxNumberOfEdges];                                      // List of the edges of the element
  int CheckElementData;                                                          //!< Check
};

#endif
