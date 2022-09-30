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
  \file Node.C
  Definition file for the Node class

  This file is the definition file for the Node class.

  \ingroup dnlElements
*/

#include "Node.h"
#include <Vec3D.h>
#include <Tensor2.h>
#include <NodalField.h>
#include <Field.h>
#include <Element.h>
#include <IntegrationPoint.h>

/*
@LABEL:Node::Node(long n, double x, double y, double z)
@SHORT:Constructor of the Node class with initialization.
@RETURN:Node
@ARG:long&n&Node number to create.
@ARG:double&x&X coordinate of the node to create.
@ARG:double&y&Y coordinate of the node to create.
@ARG:double&z&Z coordinate of the node to create.
@END
*/
//-----------------------------------------------------------------------------
Node::Node(long n, double x, double y, double z)
//-----------------------------------------------------------------------------
{
  // Initialization of the node number
  number = n;

  // Initialization of data
  mass = 0.;

  // Initialization of coords
  coords.setValue(x, y, z);

  // Initialization of the nodal fields
  field1 = new NodalField;
  field0 = new NodalField;

  // No boundary condition by default on a new Node
  boundary = NULL;
}

/* //-----------------------------------------------------------------------------
Node::Node(const Node &node)
//-----------------------------------------------------------------------------
{
  exit(-1);
  std::cout << "Copy of a Node" << std::endl;

  coords = node.coords;
}
 */
//-----------------------------------------------------------------------------
Node::~Node()
//-----------------------------------------------------------------------------
{
  // Delete Both nodal fields
  delete field1;
  delete field0;
}

//-----------------------------------------------------------------------------
void Node::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "node n=" << number;
  os << " coords=(" << coords << ")";
}

//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, Node &node)
//-----------------------------------------------------------------------------
{
  node.print(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Node &nd)
//-----------------------------------------------------------------------------
{
  nd.write(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Node &nd)
//-----------------------------------------------------------------------------
{
  nd.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void Node::write(std::ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  pfile << coords;
  field0->write(pfile);
}

//-----------------------------------------------------------------------------
Node &Node::read(std::ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile >> coords;
  field0->read(pfile);

  return *this;
}

//-----------------------------------------------------------------------------
long Node::objectSize()
//-----------------------------------------------------------------------------
{
  long sz;
  sz = sizeof(*this);
  sz += field0->objectSize();
  sz += field1->objectSize();
  return sz;
}

//-----------------------------------------------------------------------------
bool Node::operator==(const Node &node) const
//-----------------------------------------------------------------------------
{
  if (_listIndex != node._listIndex)
    return (false);
  if (coords != node.coords)
    return (false);
  return (true);
}

//-----------------------------------------------------------------------------
bool Node::operator!=(const Node &node) const
//-----------------------------------------------------------------------------
{
  return !(*this == node);
}

/*
@LABEL:Node::swapFields()
@SHORT:Swap the two fields of the node.
This method swaps the two fields of the node. So that the field0 becomes the field1 and vice versa.
@END
*/
//-----------------------------------------------------------------------------
void Node::swapFields()
//-----------------------------------------------------------------------------
{
  // swap both fields
  NodalField *tmp = field1;
  field1 = field0;
  field0 = tmp;
}

/* //-----------------------------------------------------------------------------
void Node::copyNodalFieldToNew()
//-----------------------------------------------------------------------------
{
  // field1->density = field0->density;
  // field1->densityInc = field0->densityInc;
  // field1->energy = field0->energy;
  // field1->energyInc = field0->energyInc;
  // field1->u = field0->u;
  field1->u = field0->u;
  //  field1->flux = field0->flux;
  // field1->force = field0->force;
  field1->speed = field0->speed;
  field1->acceleration = field0->acceleration;
} */

//-----------------------------------------------------------------------------
bool compareNN(Node *node1, Node *node2)
//-----------------------------------------------------------------------------
{
  return (node1->number > node2->number); // comparaison
}

//-----------------------------------------------------------------------------
long substractNN(Node *node1, const long number)
//-----------------------------------------------------------------------------
{
  return (node1->number - number); // comparaison
}

//-----------------------------------------------------------------------------
// Nodal values
//-----------------------------------------------------------------------------
#define _getFromNodal(FIELD, VAR) \
  if (field == Field::FIELD)      \
    return VAR;

#define _getScalarFromNodalVec3D(FIELD, VAR) \
  if (field == Field::FIELD)                 \
    return VAR.getNorm();                    \
  if (field == Field::FIELD##X)              \
    return VAR(0);                           \
  if (field == Field::FIELD##Y)              \
    return VAR(1);                           \
  if (field == Field::FIELD##Z)              \
    return VAR(2);

#define _getScalarFromNodalTensor2(FIELD, VAR) \
  if (field == Field::FIELD)                   \
    return VAR.getNorm();                      \
  if (field == Field::FIELD##XX)               \
    return VAR(0, 0);                          \
  if (field == Field::FIELD##XY)               \
    return VAR(0, 1);                          \
  if (field == Field::FIELD##XZ)               \
    return VAR(0, 2);                          \
  if (field == Field::FIELD##YX)               \
    return VAR(1, 0);                          \
  if (field == Field::FIELD##YY)               \
    return VAR(1, 1);                          \
  if (field == Field::FIELD##YZ)               \
    return VAR(1, 2);                          \
  if (field == Field::FIELD##ZX)               \
    return VAR(2, 0);                          \
  if (field == Field::FIELD##ZY)               \
    return VAR(2, 1);                          \
  if (field == Field::FIELD##ZZ)               \
    return VAR(2, 2);

#define _getFromIntegrationPoint(FIELD, VAR, TYPE)                                                                        \
  if (field == Field::FIELD)                                                                                              \
  {                                                                                                                       \
    Element *element;                                                                                                     \
    long elementId, intPt;                                                                                                \
    TYPE VAR;                                                                                                             \
    VAR = 0.0;                                                                                                            \
    for (elementId = 0; elementId < elements.size(); elementId++)                                                         \
    {                                                                                                                     \
      element = elements(elementId);                                                                                      \
      long loc = element->nodes.IAppN(_listIndex);                                                                        \
      for (intPt = 0; intPt < element->getNumberOfIntegrationPoints(); intPt++)                                           \
      {                                                                                                                   \
        VAR += element->_elementData->nodes[loc].integrationPointsToNode(intPt) * element->integrationPoints(intPt)->VAR; \
      }                                                                                                                   \
    }                                                                                                                     \
    VAR = VAR / elements.size();                                                                                          \
    return VAR;                                                                                                           \
  }

#define _getScalarFromIntegrationPointTensor2(FIELD, VAR)                                                                    \
  if ((Field::FIELD <= field) && (field <= Field::FIELD##ZZ))                                                                \
  {                                                                                                                          \
    Element *element;                                                                                                        \
    long elementId, intPt;                                                                                                   \
    SymTensor2 tensor;                                                                                                       \
    tensor = 0.0;                                                                                                            \
    for (elementId = 0; elementId < elements.size(); elementId++)                                                            \
    {                                                                                                                        \
      element = elements(elementId);                                                                                         \
      long loc = element->nodes.IAppN(_listIndex);                                                                           \
      for (intPt = 0; intPt < element->getNumberOfIntegrationPoints(); intPt++)                                              \
      {                                                                                                                      \
        tensor += element->_elementData->nodes[loc].integrationPointsToNode(intPt) * element->integrationPoints(intPt)->VAR; \
      }                                                                                                                      \
    }                                                                                                                        \
    tensor = tensor / elements.size();                                                                                       \
    if (field == Field::FIELD)                                                                                               \
      return tensor.getNorm();                                                                                               \
    if (field == Field::FIELD##XX)                                                                                           \
      return tensor(0, 0);                                                                                                   \
    if (field == Field::FIELD##XY)                                                                                           \
      return tensor(0, 1);                                                                                                   \
    if (field == Field::FIELD##XZ)                                                                                           \
      return tensor(0, 2);                                                                                                   \
    if (field == Field::FIELD##YX)                                                                                           \
      return tensor(1, 0);                                                                                                   \
    if (field == Field::FIELD##YY)                                                                                           \
      return tensor(1, 1);                                                                                                   \
    if (field == Field::FIELD##YZ)                                                                                           \
      return tensor(1, 2);                                                                                                   \
    if (field == Field::FIELD##ZX)                                                                                           \
      return tensor(2, 0);                                                                                                   \
    if (field == Field::FIELD##ZY)                                                                                           \
      return tensor(2, 1);                                                                                                   \
    if (field == Field::FIELD##ZZ)                                                                                           \
      return tensor(2, 2);                                                                                                   \
  }

/*
@LABEL:Node::fieldScalar(short field)
@SHORT:Get back a nodalField value.
@RETURN:double
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
double Node::fieldScalar(short field)
//-----------------------------------------------------------------------------
{
  // Nodal values
  // _getFromNodal(T0, T0);
  _getFromNodal(mass, mass);
  _getScalarFromNodalVec3D(coords, coords);
  //_getScalarFromNodalVec3D(coords0, initialCoordinates);
  //_getScalarFromNodalVec3D(normal, normal);
  _getScalarFromNodalVec3D(disp, disp);

  // NodalField values
  //_getFromNodal(energy, field0->energy);
  //_getFromNodal(energyInc, field0->energyInc);
  _getScalarFromNodalVec3D(dispInc, field0->u);
  // _getScalarFromNodalVec3D(flux, field0->flux);
  // _getScalarFromNodalVec3D(force, field0->force);
  _getScalarFromNodalVec3D(speed, field0->speed);
  _getScalarFromNodalVec3D(speedInc, field0->acceleration);

  // Integration point field
  _getFromIntegrationPoint(density, density, double);
  _getFromIntegrationPoint(plasticStrain, plasticStrain, double);
  _getFromIntegrationPoint(plasticStrainRate, plasticStrainRate, double);
  _getFromIntegrationPoint(gamma, gamma, double);
  _getFromIntegrationPoint(gammaCumulate, gammaCumulate, double);
  _getFromIntegrationPoint(yieldStress, yieldStress, double);
  _getFromIntegrationPoint(T, T, double);
  _getFromIntegrationPoint(pressure, pressure, double);
  _getFromIntegrationPoint(internalEnergy, internalEnergy, double);
  _getScalarFromIntegrationPointTensor2(Strain, Strain);
  _getScalarFromIntegrationPointTensor2(StrainInc, StrainInc);
  _getScalarFromIntegrationPointTensor2(PlasticStrain, PlasticStrain);
  _getScalarFromIntegrationPointTensor2(PlasticStrainInc, PlasticStrainInc);
  //_getScalarFromIntegrationPointTensor2(DeviatoricStress, DeviatoricStress);
  _getScalarFromIntegrationPointTensor2(Stress, Stress);

  // Hand made special value for the von Mises stress
  if (field == Field::vonMises)
  {
    Element *pel;
    long j, pt;
    double value = 0.0;
    for (j = 0; j < elements.size(); j++)
    {
      pel = elements(j);
      long loc = pel->nodes.IAppN(_listIndex);
      for (pt = 0; pt < pel->getNumberOfIntegrationPoints(); pt++)
      {
        value += pel->_elementData->nodes[loc].integrationPointsToNode(pt) * pel->integrationPoints(pt)->Stress.getMisesEquivalent();
        // tensor += pel->_elementData->nodes[loc].integrationPointsToNode(pt) * pel->integrationPoints(pt)->Stress;
      }
    }
    return value / elements.size();
  }

  Field fakeField;
  printf("Node::fieldScalar\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return 0.0;
}

/*
@LABEL:Node::fieldVec3D(short field)
@SHORT:Get back a nodalField Vec3D.
@RETURN:Vec3D
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
Vec3D Node::fieldVec3D(short field)
//-----------------------------------------------------------------------------
{
  // Nodal values
  _getFromNodal(coords, coords);
  //_getFromNodal(coords0, initialCoordinates);
  //_getFromNodal(normal, normal);
  _getFromNodal(disp, disp);

  // NodalField values
  _getFromNodal(dispInc, field0->u);
  // _getFromNodal(flux, field0->flux);
  // _getFromNodal(force, field0->force);
  _getFromNodal(speed, field0->speed);
  _getFromNodal(speedInc, field0->acceleration);

  // Integration point field

  Field fakeField;
  printf("Node::fieldVec3D\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return Vec3D();
}

/*
@LABEL:Node::fieldSymTensor2(short field)
@SHORT:Get back a nodalField SymTensor2.
@RETURN:SymTensor2
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Node::fieldSymTensor2(short field)
//-----------------------------------------------------------------------------
{
  // Integration point field
  _getFromIntegrationPoint(Strain, Strain, SymTensor2);
  _getFromIntegrationPoint(StrainInc, StrainInc, SymTensor2);
  _getFromIntegrationPoint(PlasticStrain, PlasticStrain, SymTensor2);
  _getFromIntegrationPoint(PlasticStrainInc, PlasticStrainInc, SymTensor2);
  //_getFromIntegrationPoint(DeviatoricStress, DeviatoricStress, SymTensor2);
  _getFromIntegrationPoint(Stress, Stress, SymTensor2);

  Field fakeField;
  printf("Node::fieldSymTensor2\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return SymTensor2();
}

/*
@LABEL:Node::fieldTensor2(short field)
@SHORT:Get back a nodalField Tensor2.
@RETURN:Tensor2
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Node::fieldTensor2(short field)
//-----------------------------------------------------------------------------
{
  // Integration point field
  //  _getFromIntegrationPoint(Strain, Strain, Tensor2);

  return Tensor2();
}

/*
//-----------------------------------------------------------------------------
void Node::addElement (Element * el)
//-----------------------------------------------------------------------------
{
  elements << el;
}


//-----------------------------------------------------------------------------
bool Node::attachNodeMotion (NodeMotion * _motion)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert (motion == NULL);
#endif
  motion = _motion;
  return (Success);
}

//-----------------------------------------------------------------------------
bool Node::detachNodeMotion ()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert (motion != NULL);
#endif

  // detachement de l'objet
  motion = NULL;
  return (Success);
}

//-----------------------------------------------------------------------------
bool Node::deleteNodeMotion ()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert (motion != NULL);
#endif

  // destruction de l'objet
  delete motion;

  // detachement de l'objet
  motion = NULL;
  return (Success);
}


//-----------------------------------------------------------------------------
void Node::toFile (FILE * pfile)
//-----------------------------------------------------------------------------
{
  fprintf (pfile, "%8ld  %8.5E  %8.5E  %8.5E \n", number, coords (0),
     coords (1), coords (2));
}

//-----------------------------------------------------------------------------
void Node::toFileBound (FILE * pfile)
//-----------------------------------------------------------------------------
{
  if (field0->speed.dot () != 0.0)
    {
      fprintf (pfile, "%8ld  %8.3E %8.3E %8.3E\n",
         number,
         field0->speed (0), field0->speed (1),
         field0->speed (2));
    }
}

#define _getScalarfieldscalar(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component>1) fatalError("Node::fieldScalar::get","No sense for component >1 for a scalar quantity");\
      return field0-> VAR;\
    }
#define _getScalarNodalFieldVec3D(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component==0)\
      return field0-> VAR.getNorm();\
      if (component>3) fatalError("Node::fieldScalar::get","No sense for component >3 for a vectorProduct quantity");\
      return field0-> VAR(component-1);\
    }
#define getNodalFieldVec3DLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      return field0-> VAR;\
    }
#define getNodalVectorialLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      return VAR;\
    }
#define getNodalScalarValueLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component>1) fatalError("Node::fieldScalar::get","No sense for component >1 for a scalar quantity");\
      return  VAR;\
    }
#define getNodalVectorialValueLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component==0)\
      return  VAR.getNorm();\
      if (component>3) fatalError("Node::fieldScalar::get","No sense for component >3 for a vectorProduct quantity");\
      return  VAR(component-1);\
    }

//-----------------------------------------------------------------------------
double Node::fieldScalar(String field, long component)
//-----------------------------------------------------------------------------
{
  // nodalfield values
  _getScalarfieldscalar(density,density);
  _getScalarfieldscalar(densityInc,densityInc);
  _getScalarfieldscalar(energy,e);
  _getScalarfieldscalar(energyInc,de);
  _getScalarNodalFieldVec3D(speed,speed);
  _getScalarNodalFieldVec3D(acceleration,acceleration);
  _getScalarNodalFieldVec3D(force,force);
  _getScalarfieldscalar(T,T);
  _getScalarNodalFieldVec3D(flux,flux);
  _getScalarNodalFieldVec3D(disp,disp);
  _getScalarNodalFieldVec3D(disp,disp);

  // nodal values
  getNodalVectorialValueLocal(coords,coords);
  getNodalVectorialValueLocal(normal,normal);
  getNodalScalarValueLocal(mass,mass);
  getNodalScalarValueLocal(temperatureInit,T0);

  if (field=="stress") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_Sig_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    if (component==0)
      return V1.getMisesEquivalent();
    else
      return V1(((component - (component % 10))/10)-1,(component % 10)-1);
  }

  if (field=="strain") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_Eps_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    if (component==0)
      return V1.getJ2();
    else
      return V1(((component - (component % 10))/10)-1,(component % 10)-1);
  }

  if (field=="strainInc") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_dEps_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    if (component==0)
      return V1.getJ2();
    else
      return V1(((component - (component % 10))/10)-1,(component % 10)-1);
  }

  if (field=="plasticStrain") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_EpsPlas_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    if (component==0)
      return V1.getJ2();
    else
      return V1(((component - (component % 10))/10)-1,(component % 10)-1);
  }

  fatalError("fieldScalar","undefined field %s\n",field.chars());
  return 0;
}

//-----------------------------------------------------------------------------
Vec3D Node::getNodalVector(String field)
//-----------------------------------------------------------------------------
{
  // nodalfield values
  getNodalFieldVec3DLocal(speed,speed);
  getNodalFieldVec3DLocal(acceleration,acceleration);
  getNodalFieldVec3DLocal(force,force);
  getNodalFieldVec3DLocal(flux,flux);
  getNodalFieldVec3DLocal(disp,disp);
  getNodalFieldVec3DLocal(disp,disp);

  // nodal values
  getNodalVectorialLocal(coords,coords);
  getNodalVectorialLocal(normal,normal);

  fatalError("getNodalVector","undefined field %s\n",field.chars());
  Vec3D v;
  return v;
}

//-----------------------------------------------------------------------------
Tensor2 Node::fieldTensor2(String field)
//-----------------------------------------------------------------------------
{

  if (field=="stress") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_Sig_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    return V1;
  }

  if (field=="strain") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_Eps_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    return V1;
  }

  if (field=="strainInc") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_dEps_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    return V1;
  }

  if (field=="plasticStrain") {
    Tensor2 V,V1;
    long i;
    V1=0;
    for (i=0;i<elements.size();i++) {
      elements(i)->get_EpsPlas_atNode(V,elements(i)->getLocalNumber(this));
      V1+=V;
    }
    V1=V1/elements.size();
    return V1;
  }

  fatalError("getValueAtNode","undefined field %s\n",field.chars());
  Tensor2 V1;
  return V1;
}
*/
