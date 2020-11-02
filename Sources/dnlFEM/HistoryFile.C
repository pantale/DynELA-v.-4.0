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
  \file HistoryFile.h
  \brief Declaration file for the HistoryFile class

  This file is the declaration file for the HistoryFile class.

  \ingroup dnlFEM
*/

#include <HistoryFile.h>
#include <Node.h>
#include <Element.h>
#include <NodeSet.h>
#include <ElementSet.h>
#include <Field.h>
#include <DynELA.h>
#include <Model.h>
#include <Solver.h>

//-----------------------------------------------------------------------------
HistoryFileItem::HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFileItem::~HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFileNodeItem::HistoryFileNodeItem() : HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFileNodeItem::~HistoryFileNodeItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double HistoryFileNodeItem::getValue()
//-----------------------------------------------------------------------------
{
  return _node->getNodalValue(_field);
}

//-----------------------------------------------------------------------------
HistoryFileElementItem::HistoryFileElementItem() : HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFileElementItem::~HistoryFileElementItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double HistoryFileElementItem::getValue()
//-----------------------------------------------------------------------------
{
  return _element->getIntPointValue(_field, _intPoint);
}

//-----------------------------------------------------------------------------
HistoryFileGlobalItem::HistoryFileGlobalItem() : HistoryFileItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFileGlobalItem::~HistoryFileGlobalItem()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double HistoryFileGlobalItem::getValue()
//-----------------------------------------------------------------------------
{
  switch (_field)
  {
  case Field::realTimeStep:
    return dynelaData->model.solver->timeStep;
    break;
  case Field::timeStep:
    return dynelaData->model.solver->getTimeStep();
    break;
  case Field::kineticEnergy:
    return dynelaData->model.getTotalKineticEnergy();
    break;
  }

  return 0;
}

//-----------------------------------------------------------------------------
HistoryFile::HistoryFile(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  _pfile = NULL;
  _stopTime = 0.0;
  _startTime = 0.0;
  _nextTime = 0.0;
  _saveTime = 0.0;
}

//-----------------------------------------------------------------------------
HistoryFile::HistoryFile(const HistoryFile &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HistoryFile::~HistoryFile()
//-----------------------------------------------------------------------------
{
  close();
}

//-----------------------------------------------------------------------------
void HistoryFile::setSaveTime(double saveTime)
//-----------------------------------------------------------------------------
{
  _startTime = 0.0;
  _nextTime = 0.0;
  _saveTime = saveTime;
  _stopTime = -1.0;
}

//-----------------------------------------------------------------------------
void HistoryFile::setSaveTime(double startTime, double stopTime, double saveTime)
//-----------------------------------------------------------------------------
{
  _startTime = startTime;
  _nextTime = startTime;
  _saveTime = saveTime;
  _stopTime = stopTime;
}

//-----------------------------------------------------------------------------
double HistoryFile::getSaveTime()
//-----------------------------------------------------------------------------
{
  return _saveTime;
}

//-----------------------------------------------------------------------------
double HistoryFile::getStartTime()
//-----------------------------------------------------------------------------
{
  return _startTime;
}

//-----------------------------------------------------------------------------
double HistoryFile::getStopTime()
//-----------------------------------------------------------------------------
{
  return _stopTime;
}

//-----------------------------------------------------------------------------
void HistoryFile::setFileName(String fileName)
//-----------------------------------------------------------------------------
{
  _fileName = fileName;
}

//-----------------------------------------------------------------------------
String HistoryFile::getFileName()
//-----------------------------------------------------------------------------
{
  return _fileName;
}

//-----------------------------------------------------------------------------
void HistoryFile::add(NodeSet *nodeSet, short field)
//-----------------------------------------------------------------------------
{
  String baseName;
  String _tmpStr;
  Field fields;
  baseName = fields.getVtklabel(field);

  for (long nodeId = 0; nodeId < nodeSet->nodes.getSize(); nodeId++)
  {
    HistoryFileNodeItem *newItem = new HistoryFileNodeItem;

    // affect the name
    if (nodeSet->nodes.getSize() > 1)
    {
      _tmpStr.convert(nodeSet->nodes(nodeId)->number);
      _tmpStr = baseName + "_n" + _tmpStr;
      newItem->_name = _tmpStr;
    }
    else
    {
      newItem->_name = baseName;
    }

    // affect the node
    newItem->_node = nodeSet->nodes(nodeId);

    // affect the field
    newItem->_field = field;

    // Add the new item to the list
    _items << newItem;
  }
}

//-----------------------------------------------------------------------------
void HistoryFile::add(ElementSet *elementSet, short intPt, short field)
//-----------------------------------------------------------------------------
{
  String baseName;
  String _tmpStr;
  Field fields;

  if (intPt > 0)
  {
    _tmpStr.convert(intPt);
    baseName = fields.getVtklabel(field) + '_' + _tmpStr;
  }
  else
  {
    baseName = fields.getVtklabel(field);
  }

  for (long elementId = 0; elementId < elementSet->elements.getSize(); elementId++)
  {
    HistoryFileElementItem *newItem = new HistoryFileElementItem;

    // affect the name
    if (elementSet->elements.getSize() > 1)
    {
      _tmpStr.convert(elementSet->elements(elementId)->number);
      _tmpStr = baseName + "_e" + _tmpStr;
      newItem->_name = _tmpStr;
    }
    else
    {
      newItem->_name = baseName;
    }

    // affect the element
    newItem->_element = elementSet->elements(elementId);

    // affect the field
    newItem->_field = field;

    // affect the point
    newItem->_intPoint = intPt - 1;

    // Add the new item to the list
    _items << newItem;
  }
}

//-----------------------------------------------------------------------------
void HistoryFile::add(short field)
//-----------------------------------------------------------------------------
{
  Field fields;
  String baseName = fields.getVtklabel(field);

  HistoryFileGlobalItem *newItem = new HistoryFileGlobalItem;

  // affect the name
  newItem->_name = baseName;

  // affect the field
  newItem->_field = field;

  // Add the new item to the list
  _items << newItem;
}

//-----------------------------------------------------------------------------
void HistoryFile::save(double currentTime)
//-----------------------------------------------------------------------------
{
  // if end of save time
  /*   if ((_stopTime > 0.0) && (currentTime > _stopTime))
    return;
 */
  // If file has not been initialized
  if (_pfile == NULL)
  {
    open();
    headerWrite();
  }

  // If it is not time to save a new history
  if ((currentTime > _startTime) && (currentTime < _nextTime))
    return;

  // Compute next save time
  _nextTime += _saveTime;

  // Writes current time to file
  fprintf(_pfile, "%10.7E ", currentTime);

  // Writes data to file
  for (long itemToWrite = 0; itemToWrite < _items.getSize(); itemToWrite++)
  {
    fprintf(_pfile, "%10.7E ", _items(itemToWrite)->getValue());
  }
  fprintf(_pfile, "\n");

  fflush(_pfile);
}

//-----------------------------------------------------------------------------
void HistoryFile::open()
//-----------------------------------------------------------------------------
{
  if (_fileName == "")
    fatalError("HistoryFile::open", "No file name specified for object : %s\n", name.chars());
  if (_pfile != NULL)
    internalFatalError("HistoryFile::open", "File already open as %s\n", _fileName.chars());
  _pfile = fopen(_fileName.chars(), "w");
}

//-----------------------------------------------------------------------------
void HistoryFile::close()
//-----------------------------------------------------------------------------
{
  if (_pfile != NULL)
    fclose(_pfile);
  _pfile = NULL;
}

//-----------------------------------------------------------------------------
void HistoryFile::headerWrite()
//-----------------------------------------------------------------------------
{
  if (_pfile == NULL)
    internalFatalError("HistoryFile::headerWrite", "File not open\n");
  fprintf(_pfile, "#DynELA_plot history file\n");
  fprintf(_pfile, "#plotted :");
  for (long i = 0; i < _items.getSize(); i++)
  {
    fprintf(_pfile, "%s ", _items(i)->_name.chars());
  }
  fprintf(_pfile, "\n");
}
