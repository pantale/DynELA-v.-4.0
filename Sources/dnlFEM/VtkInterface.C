/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <VtkInterface.h>
#include <List.h>
#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <Field.h>
#include <Model.h>

//-----------------------------------------------------------------------------
VtkInterface::VtkInterface(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
  initFields();
}

//-----------------------------------------------------------------------------
VtkInterface::VtkInterface(const VtkInterface &VtkInterface)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
VtkInterface::~VtkInterface()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void VtkInterface::openFile(String fileName)
//-----------------------------------------------------------------------------
{
  if (fileName == "")
  {
    fatalError("VtkInterface::open", "Must specify a log filename in the constructor");
  }

  // put the name
  _fileName = fileName;

  // open the stream
  _stream.open(_fileName.chars(), std::fstream::out);

  if (!_stream.is_open())
  {
    fatalError("VtkInterface::open", "Cannot open _stream for file %s", _fileName.chars());
  }
}

//-----------------------------------------------------------------------------
void VtkInterface::close()
//-----------------------------------------------------------------------------
{
  // close the stream
  _stream.close();
}

//-----------------------------------------------------------------------------
void VtkInterface::headerWrite()
//-----------------------------------------------------------------------------
{
  // Get data from config file
  std::string _name, _version;
  dynelaData->settings->getValue("DynELA", _name);
  dynelaData->settings->getValue("Version", _version);

  _stream << "# vtk DataFile Version 4.0\n";
  _stream << _name + " v." + _version + "\n";
  _stream << "ASCII\n";
  _stream << "DATASET UNSTRUCTURED_GRID\n";

  // Write the current time
  _stream << "FIELD FieldData 1\n";
  _stream << "TIME 1 1 double\n";
  _stream << dynelaData->model.currentTime << "\n";
}

//-----------------------------------------------------------------------------
void VtkInterface::nodesWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->model.nodes.size();
  _stream << "POINTS " << nbNodes << " float\n";

  for (long i = 0; i < nbNodes; i++)
    _stream << dynelaData->model.nodes(i)->coords(0)
            << " " << dynelaData->model.nodes(i)->coords(1) << " "
            << dynelaData->model.nodes(i)->coords(2) << "\n";

  _stream << "\n";
}

//-----------------------------------------------------------------------------
void VtkInterface::elementsWrite()
//-----------------------------------------------------------------------------
{
  long nbElements = dynelaData->model.elements.size();
  long totNodes = 0;
  long nbNodes;
  Element *pElement;

  for (long i = 0; i < nbElements; i++)
    totNodes += dynelaData->model.elements(i)->nodes.size();
  totNodes += nbElements;

  _stream << "CELLS " << nbElements << " " << totNodes << "\n";

  for (long i = 0; i < nbElements; i++)
  {
    pElement = dynelaData->model.elements(i);
    nbNodes = pElement->nodes.size();
    _stream << nbNodes << " ";
    for (int j = 0; j < nbNodes; j++)
      _stream << pElement->nodes(j)->internalNumber() << " ";
    _stream << "\n";
  }

  _stream << "\n";

  _stream << "CELL_TYPES " << nbElements << "\n";
  for (long i = 0; i < nbElements; i++)
    _stream << dynelaData->model.elements(i)->getVtkType() << "\n";

  _stream << "\n";
}

//-----------------------------------------------------------------------------
void VtkInterface::nodesNumbersWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->model.nodes.size();
  _stream << "SCALARS nodesNumbers" << nbNodes << " float\n";

  _stream << "\n";
}

//-----------------------------------------------------------------------------
void VtkInterface::dataWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->model.nodes.size();
  short field;
  Field fields;
  bool lookupWriten = false;

  _stream << "POINT_DATA " << nbNodes << "\n";

  for (int i = 0; i < _outputFields.size(); i++)
  {
    field = _outputFields(i);

    // Scalar field
    if (fields.getType(field) == 0)
    {
      String _name = fields.getVtklabel(field);
      _stream << "SCALARS " << _name << " float\n";
      if (!lookupWriten)
      {
        _stream << "LOOKUP_TABLE default\n";
        //  lookupWriten = true;
      }
      for (long j = 0; j < nbNodes; j++)
        _stream << dynelaData->model.nodes(j)->fieldScalar(field) << "\n";
    }

    // Vector field
    if (fields.getType(field) == 1)
    {
      String _name = fields.getVtklabel(field);
      _stream << "VECTORS " << _name << " float\n";
      for (long j = 0; j < nbNodes; j++)
      {
        Vec3D v = dynelaData->model.nodes(j)->fieldVec3D(field);
        _stream << v(0) << " " << v(1) << " " << v(2) << "\n";
      }
    }

    // Tensor field
    if (fields.getType(field) == 2)
    {
      String _name = fields.getVtklabel(field);
      _stream << "TENSORS " << _name << " float\n";
      for (long j = 0; j < nbNodes; j++)
      {
        SymTensor2 t = dynelaData->model.nodes(j)->fieldSymTensor2(field);
        _stream << t(0, 0) << " " << t(0, 1) << " " << t(0, 2) << " "
                << t(1, 0) << " " << t(1, 1) << " " << t(1, 2) << " "
                << t(2, 0) << " " << t(2, 1) << " " << t(2, 2) << "\n";
      }
    }
  }
  _stream << "\n";
}

//-----------------------------------------------------------------------------
void VtkInterface::write()
//-----------------------------------------------------------------------------
{
  // Header write
  headerWrite();

  // Write the nodes
  nodesWrite();

  // Write the nodes
  elementsWrite();

  // Write the nodes
  dataWrite();
}

//-----------------------------------------------------------------------------
void VtkInterface::initFields()
//-----------------------------------------------------------------------------
{
  Field field;
  std::vector<std::string> fieldList;

  // Read the VtkFields line of config file
  dynelaData->settings->getValue("VtkFields", fieldList);

  // Get all data
  for (short i = 0; i < fieldList.size(); i++)
  {
    String st = fieldList.at(i);
    st.strip();
    short nf = field.getField(st);
    if (nf != -1)
      _outputFields << nf;
  }
}

//-----------------------------------------------------------------------------
short VtkInterface::existField(short field)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < _outputFields.size(); i++)
    if (_outputFields(i) == field)
      return i;
  return -1;
}

//-----------------------------------------------------------------------------
void VtkInterface::addField(short field)
//-----------------------------------------------------------------------------
{
  if (existField(field) == -1)
    _outputFields << field;
}

//-----------------------------------------------------------------------------
void VtkInterface::removeField(short field)
//-----------------------------------------------------------------------------
{
  short ind = existField(field);
  if (ind != -1)
  {
    _outputFields.del(ind);
  }
}

//-----------------------------------------------------------------------------
int VtkInterface::getNumberOfFields()
//-----------------------------------------------------------------------------
{
  return _outputFields.size();
}
