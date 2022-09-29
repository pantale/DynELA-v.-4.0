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
  // init du n du noeud
  //_listIndex = n;
  number = n;

  // init des donnees
  mass = 0.;

  // init par defaut des coordonnees
  coordinates.setValue(x, y, z);
  // normal = 0.;

  // init par defaut de deux champs de donnees
  newField = new NodalField;
  currentField = new NodalField;
  // tmp = NULL;

  // init par defaut d'un node Motion
  // motion = NULL;

  boundary = NULL;
}

/*
@LABEL:Node::Node(Node node)
@SHORT:Copy constructor of the Node class.
@RETURN:Node
@ARG:Node&node&Node to copy.
@END
*/
//-----------------------------------------------------------------------------
Node::Node(const Node &node)
//-----------------------------------------------------------------------------
{
  exit(-1);
  std::cout << "Copy of a Node" << std::endl;

  coordinates = node.coordinates;
}

//-----------------------------------------------------------------------------
Node::~Node()
//-----------------------------------------------------------------------------
{
  delete newField;
  delete currentField;
}

//-----------------------------------------------------------------------------
void Node::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "node n=" << number;
  os << " coordinates=(" << coordinates << ")";
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
  pfile << coordinates;
  currentField->write(pfile);
}

//-----------------------------------------------------------------------------
Node &Node::read(std::ifstream &pfile)
//-----------------------------------------------------------------------------
{
  pfile >> coordinates;
  currentField->read(pfile);

  return *this;
}

//-----------------------------------------------------------------------------
long Node::objectSize()
//-----------------------------------------------------------------------------
{
  long sz;
  sz = sizeof(*this);
  sz += currentField->objectSize();
  sz += newField->objectSize();
  return sz;
}

//-----------------------------------------------------------------------------
bool Node::operator==(const Node &node) const
//-----------------------------------------------------------------------------
{
  if (_listIndex != node._listIndex)
    return (false);
  if (coordinates != node.coordinates)
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
@LABEL:Node::swapNodalFields()
@SHORT:Swap the two nodalFields of the node.
This method swaps the two nodalField of the element. So that the currentField becomes the newField and vice versa.
@END
*/
//-----------------------------------------------------------------------------
void Node::swapNodalFields()
//-----------------------------------------------------------------------------
{
  // swap both NodalFields
  NodalField *tmp = newField;
  newField = currentField;
  currentField = tmp;
}

/*
@LABEL:Node::copyNodalFieldToNew()
@SHORT:Copy the content of the nodalField to the newField.
@END
*/
//-----------------------------------------------------------------------------
void Node::copyNodalFieldToNew()
//-----------------------------------------------------------------------------
{
  // newField->density = currentField->density;
  // newField->densityInc = currentField->densityInc;
  // newField->energy = currentField->energy;
  // newField->energyInc = currentField->energyInc;
  // newField->displacement = currentField->displacement;
  newField->displacement = currentField->displacement;
  //  newField->flux = currentField->flux;
  // newField->force = currentField->force;
  newField->speed = currentField->speed;
  newField->acceleration = currentField->acceleration;
}

//-----------------------------------------------------------------------------
bool compareNodesNumber(Node *node1, Node *node2)
//-----------------------------------------------------------------------------
{
  return (node1->number > node2->number); // comparaison
}

//-----------------------------------------------------------------------------
long substractNodesNumber(Node *node1, const long number)
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
    for (elementId = 0; elementId < elements.size(); elementId++)                                                      \
    {                                                                                                                     \
      element = elements(elementId);                                                                                      \
      long loc = element->nodes.IAppN(_listIndex);                                                                        \
      for (intPt = 0; intPt < element->getNumberOfIntegrationPoints(); intPt++)                                           \
      {                                                                                                                   \
        VAR += element->_elementData->nodes[loc].integrationPointsToNode(intPt) * element->integrationPoints(intPt)->VAR; \
      }                                                                                                                   \
    }                                                                                                                     \
    VAR = VAR / elements.size();                                                                                       \
    return VAR;                                                                                                           \
  }

#define _getScalarFromIntegrationPointTensor2(FIELD, VAR)                                                                    \
  if ((Field::FIELD <= field) && (field <= Field::FIELD##ZZ))                                                                \
  {                                                                                                                          \
    Element *element;                                                                                                        \
    long elementId, intPt;                                                                                                   \
    SymTensor2 tensor;                                                                                                       \
    tensor = 0.0;                                                                                                            \
    for (elementId = 0; elementId < elements.size(); elementId++)                                                         \
    {                                                                                                                        \
      element = elements(elementId);                                                                                         \
      long loc = element->nodes.IAppN(_listIndex);                                                                           \
      for (intPt = 0; intPt < element->getNumberOfIntegrationPoints(); intPt++)                                              \
      {                                                                                                                      \
        tensor += element->_elementData->nodes[loc].integrationPointsToNode(intPt) * element->integrationPoints(intPt)->VAR; \
      }                                                                                                                      \
    }                                                                                                                        \
    tensor = tensor / elements.size();                                                                                    \
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
@LABEL:Node::getNodalValue(short field)
@SHORT:Get back a nodalField value.
@RETURN:double
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
double Node::getNodalValue(short field)
//-----------------------------------------------------------------------------
{
  // Nodal values
  // _getFromNodal(initialTemperature, initialTemperature);
  _getFromNodal(mass, mass);
  _getScalarFromNodalVec3D(nodeCoordinate, coordinates);
  //_getScalarFromNodalVec3D(initialNodeCoordinate, initialCoordinates);
  //_getScalarFromNodalVec3D(normal, normal);
  _getScalarFromNodalVec3D(displacement, displacement);

  // NodalField values
  //_getFromNodal(energy, currentField->energy);
  //_getFromNodal(energyIncrement, currentField->energyInc);
  _getScalarFromNodalVec3D(displacementIncrement, currentField->displacement);
  // _getScalarFromNodalVec3D(flux, currentField->flux);
  // _getScalarFromNodalVec3D(force, currentField->force);
  _getScalarFromNodalVec3D(speed, currentField->speed);
  _getScalarFromNodalVec3D(speedIncrement, currentField->acceleration);

  // Integration point field
  _getFromIntegrationPoint(density, density, double);
  _getFromIntegrationPoint(plasticStrain, plasticStrain, double);
  _getFromIntegrationPoint(plasticStrainRate, plasticStrainRate, double);
  _getFromIntegrationPoint(gamma, gamma, double);
  _getFromIntegrationPoint(gammaCumulate, gammaCumulate, double);
  _getFromIntegrationPoint(yieldStress, yieldStress, double);
  _getFromIntegrationPoint(temperature, temperature, double);
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
  printf("Node::getNodalValue\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return 0.0;
}

/*
@LABEL:Node::getNodalVec3D(short field)
@SHORT:Get back a nodalField Vec3D.
@RETURN:Vec3D
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
Vec3D Node::getNodalVec3D(short field)
//-----------------------------------------------------------------------------
{
  // Nodal values
  _getFromNodal(nodeCoordinate, coordinates);
  //_getFromNodal(initialNodeCoordinate, initialCoordinates);
  //_getFromNodal(normal, normal);
  _getFromNodal(displacement, displacement);

  // NodalField values
  _getFromNodal(displacementIncrement, currentField->displacement);
  // _getFromNodal(flux, currentField->flux);
  // _getFromNodal(force, currentField->force);
  _getFromNodal(speed, currentField->speed);
  _getFromNodal(speedIncrement, currentField->acceleration);

  // Integration point field

  Field fakeField;
  printf("Node::getNodalVec3D\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return Vec3D();
}

/*
@LABEL:Node::getNodalSymTensor(short field)
@SHORT:Get back a nodalField SymTensor2.
@RETURN:SymTensor2
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
SymTensor2 Node::getNodalSymTensor(short field)
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
  printf("Node::getNodalSymTensor\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return SymTensor2();
}

/*
@LABEL:Node::getNodalTensor(short field)
@SHORT:Get back a nodalField Tensor2.
@RETURN:Tensor2
@ARG:short&field&Field to extract (see NodalField for informations).
@END
*/
//-----------------------------------------------------------------------------
Tensor2 Node::getNodalTensor(short field)
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
  fprintf (pfile, "%8ld  %8.5E  %8.5E  %8.5E \n", number, coordinates (0),
     coordinates (1), coordinates (2));
}

//-----------------------------------------------------------------------------
void Node::toFileBound (FILE * pfile)
//-----------------------------------------------------------------------------
{
  if (currentField->speed.dot () != 0.0)
    {
      fprintf (pfile, "%8ld  %8.3E %8.3E %8.3E\n",
         number,
         currentField->speed (0), currentField->speed (1),
         currentField->speed (2));
    }
}

#define _getScalarNodalFieldScalar(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component>1) fatalError("Node::getNodalValue::get","No sense for component >1 for a scalar quantity");\
      return currentField-> VAR;\
    }
#define _getScalarNodalFieldVec3D(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component==0)\
      return currentField-> VAR.getNorm();\
      if (component>3) fatalError("Node::getNodalValue::get","No sense for component >3 for a vectorProduct quantity");\
      return currentField-> VAR(component-1);\
    }
#define getNodalFieldVec3DLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      return currentField-> VAR;\
    }
#define getNodalVectorialLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      return VAR;\
    }
#define getNodalScalarValueLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component>1) fatalError("Node::getNodalValue::get","No sense for component >1 for a scalar quantity");\
      return  VAR;\
    }
#define getNodalVectorialValueLocal(FIELD,VAR)  if (field==#FIELD) \
    {\
      if (component==0)\
      return  VAR.getNorm();\
      if (component>3) fatalError("Node::getNodalValue::get","No sense for component >3 for a vectorProduct quantity");\
      return  VAR(component-1);\
    }

//-----------------------------------------------------------------------------
double Node::getNodalValue(String field, long component)
//-----------------------------------------------------------------------------
{
  // nodalfield values
  _getScalarNodalFieldScalar(density,density);
  _getScalarNodalFieldScalar(densityInc,densityInc);
  _getScalarNodalFieldScalar(energy,e);
  _getScalarNodalFieldScalar(energyInc,de);
  _getScalarNodalFieldVec3D(speed,speed);
  _getScalarNodalFieldVec3D(acceleration,acceleration);
  _getScalarNodalFieldVec3D(force,force);
  _getScalarNodalFieldScalar(temperature,T);
  _getScalarNodalFieldVec3D(flux,flux);
  _getScalarNodalFieldVec3D(displacement,displacement);
  _getScalarNodalFieldVec3D(displacement,displacement);

  // nodal values
  getNodalVectorialValueLocal(coordinates,coordinates);
  getNodalVectorialValueLocal(normal,normal);
  getNodalScalarValueLocal(mass,mass);
  getNodalScalarValueLocal(temperatureInit,initialTemperature);

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

  fatalError("getNodalValue","undefined field %s\n",field.chars());
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
  getNodalFieldVec3DLocal(displacement,displacement);
  getNodalFieldVec3DLocal(displacement,displacement);

  // nodal values
  getNodalVectorialLocal(coordinates,coordinates);
  getNodalVectorialLocal(normal,normal);

  fatalError("getNodalVector","undefined field %s\n",field.chars());
  Vec3D v;
  return v;
}

//-----------------------------------------------------------------------------
Tensor2 Node::getNodalTensor(String field)
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
