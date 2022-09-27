/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_Node_h__
#define __dnlElements_Node_h__

#include <List.h>
#include <Vec3D.h>

class NodalField;
class BoundaryCondition;
class Element;

/*
@LABEL:Node::Node
@SHORT:Finite Element Node class.
This class is used to store information for Finite Element Nodes.
@ARG:BoundaryCondition&*boundary&Boundary conditions on the current node.
@ARG:double&mass&Nodal mass.
@ARG:List<Element *>&elements&List of the elements that contains a reference to the current node.
@ARG:long&number&Identification number of the node.
@ARG:NodalField&*currentField&Nodal field of the node.
@ARG:NodalField&*newField&Nodal field of the node.
@ARG:Vec3D&coordinates&Coordinates of the corresponding node.
@ARG:Vec3D&displacement&Displacement at the current node $\overrightarrow{d}$.
@END
*/
class Node
{
  friend class ListIndex<Node *>; // To be able to use ListIndex
  long _listIndex;                // Local index used for the ListIndex management.

public:
  // double initialTemperature;     // Initial Temperature. This field is used to store the reference value of the temperature of the node at the begining of the calculus
  // NodeMotion *motion;          // Node motion. This pointer reference the method used to move the point.
  // Vec3D initialCoordinates;      // Coordinates. Initial coordinates of the corresponding point.
  // Vec3D normal;                // Normal vector. This vector represents the normal vector of the current point in 3D space. Such normal vector is compted from the values of the normals of the faces connected to this point.
  BoundaryCondition *boundary; // Boundary conditions. This pointer reference the list of the boundary conditions on the current node.
  double mass;                 // Mass. This field is used to store the value of the getJ2 nodal mass.
  List<Element *> elements;    // Element list. This pointer reference the list of the elements that contains a reference to the current node (ie: the list of the elements that includes this node).
  long number;                 // Identification number. This field represents the external identification number of the current node (ie. user representation).
  NodalField *currentField;    // Nodal field of the node, ie. nodal field at the begining of the current increment
  NodalField *newField;        // New Nodal field of the node, ie. nodal field at the end of the current increment
  Vec3D coordinates;           // Coordinates of the corresponding node.
  Vec3D displacement = 0.0;    // Displacement at the current node \f$ \overrightarrow{d} \f$

public:
  Node(long = 1, double = 0.0, double = 0.0, double = 0.0);
  Node(const Node &);
  ~Node();

  // Interface methods excluded from SWIG
#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Node &);
  friend std::ofstream &operator<<(std::ofstream &, const Node &);
  Node &read(std::ifstream &);
  friend std::ostream &operator<<(std::ostream &, Node &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  /*   bool operator<(const Node &node) const;
  bool operator>(const Node &node) const;
 */
  bool operator!=(const Node &) const;
  bool operator==(const Node &) const;
  double getNodalValue(short);
  long &internalNumber();
  long objectSize();
  NodalField *getNodalField(short);
  SymTensor2 getNodalSymTensor(short);
  Tensor2 getNodalTensor(short);
  Vec3D getNodalVec3D(short);
  void copyNodalFieldToNew();
  void swapNodalFields();

  /**Attach an element. This method is used to add a new reference to an element in the list of the elements connected to the current point. There is no limit in the number of elements connected to the current node, so there is no verification procedure to see if this is correct for the structure.*/
  /*


 */
  /**.Number of elements. This method returns the number of elements referenced in the list of the elements connected to the current node.*/
  //  long NoElements ();

  /***/
  //

  /**Node motion. This method is used to attach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool attachNodeMotion ( NodeMotion *motion );

  /**Node motion. This method is used to detach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool detachNodeMotion ();

  /**Node motion. This method is used to delete a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool deleteNodeMotion ();

  /**Nodal field transfer. This method is used to transfer the newField nodal field to the currentField one. In fact, this swaps the two fields.*/

  /**Get a nodal field. This method returns a pointer with a reference to one of the nodal fields depending on the given parameter time. Settings time=0 returns the currentField nodal field, time=1 returns the newField nodal field.*/

  // fonctions entree sortie
  //   void toFile ( FILE *pfile );
  //   void toFileBound ( FILE *pfile );
  //}
};

bool compareNodesNumber(Node *node1, Node *node2);
long substractNodesNumber(Node *node1, const long number);

/*
//-----------------------------------------------------------------------------
inline long Node::NoElements ()
//-----------------------------------------------------------------------------
{
    return elements.getSize ();
}
*/

// Internal number. This method gives a direct access to the internal number of the current node.
/*
  Return : Internal number of the current node.
*/
//-----------------------------------------------------------------------------
inline long &Node::internalNumber()
//-----------------------------------------------------------------------------
{
  return (_listIndex);
}

//-----------------------------------------------------------------------------
inline NodalField *Node::getNodalField(short time)
//-----------------------------------------------------------------------------
{
  return (time == 0 ? currentField : newField);
}

#endif
