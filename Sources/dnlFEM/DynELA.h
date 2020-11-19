/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

// TODOCXYFILE

/*!
  \file DynELA.h
  \brief Declaration file for the DynELA class

  This file is the declaration file for the DynELA class.

  \ingroup dnlFEM
*/

#ifndef __dnlFEM_DynELA_h__
#define __dnlFEM_DynELA_h__

#include <omp.h>
#include <dnlKernel.h>
#include <Parallel.h>
#include <Drawing.h>
#include <Model.h>

class DynELA;
class VtkInterface;
class Boundary;
class HistoryFile;
class Solver;
class Material;
class ElementSet;
class NodeSet;

#ifndef SWIG
extern DynELA *dynelaData;
#endif

/*!
  \class DynELA DynELA.h
  \brief Classe de definition et de manipulation des structures elements finis.
  \ingroup dnlFEM
*/
class DynELA
{
  friend class Drawing;      // To allow Drawing class to access private data
  friend class SvgInterface; // To allow SvgInterface class to access private data
  friend class VtkInterface; // To allow VtkInterface class to access private data

private:
  double _displayTimeIncrement = 10;  // Display time increment
  double _lastElapsedComputeTime = 0; // last elapsed compute time
  double _lastElapsedTime = 0;        // Last elapsed time for computing endtime
  double _nextDisplayTime = 10;       // Next display time
  short _defaultElement;              // Current default Element
  short _VTKresultFileIndex = 0;      // Current result file index
  String _VTKresultFileName;          // Current result file name

public:
  double endSaveTime = 0.0;       // Final save time
  double nextSaveTime = 0.0;      // Next save time
  double saveTimeIncrement = 0.0; // Increment of save time
  double startSaveTime = 0.0;     // Start save time
  Drawing drawing;                //
  Model model;                    //
  Parallel parallel;              // Parallel computation
  Settings *settings = NULL;      // Settings
  String name = "_noname_";       // name of the object
  Timers cpuTimes;                // Store the CPU Times
  VtkInterface *dataFile = NULL;  // Interface for results

#ifndef SWIG
  LogFile logFile; // Log file
#endif

public:
  DynELA(char *newName = NULL);
  DynELA(const DynELA &X);
  ~DynELA();

  bool createElement(long elementNumber, long nodesIndex, ...);
  bool createNode(long nodeNumber, double xCoord, double yCoord, double zCoord);
  bool createNode(long nodeNumber, Vec3D coords);
  Element *getElementByNum(long elementNumber);
  long getElementsNumber();
  long getNodesNumber();
  Node *getNodeByNum(long nodeNumber);
  void add(ElementSet *elementSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(HistoryFile *newHistoryFile);
  void add(Material *material, ElementSet *elementSet);
  void add(NodeSet *nodeSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Solver *newSolver);
  void addMaterial(Material *material);
  void attachConstantBC(Boundary *boundary, NodeSet *nodeSet);
  void attachInitialBC(Boundary *boundary, NodeSet *nodeSet);
  void displayEstimatedEnd();
  void getGlobalBox(Vec3D &minPoint, Vec3D &maxPoint);
  void getNodalValuesRange(short field, double &min, double &max);
  void rotate(String set, double angle, NodeSet *nodeSet = NULL);
  void rotate(Vec3D axis, double angle, NodeSet *nodeSet = NULL);
  void scale(double scaleValue, NodeSet *nodeSet = NULL);
  void scale(Vec3D scaleVector, NodeSet *nodeSet = NULL);
  void setDefaultElement(short type);
  void setSaveTimes(double startSaveTime, double endSaveTime, double saveTimeIncrement);
  void solve();
  void translate(Vec3D translateVector, NodeSet *nodeSet = NULL);
  void writeResultFile();
  void writeVTKFile();

  //  void displayOnline();

  // member functions
  /*
  bool checkBinaryVersion (ifstream & pfile, int);
  bool initSolve ();
  double getCurrentTime();
  double getNodalValue(long nod,String field,long component=0);
  double getReadTimeData (ifstream & pfile);
  Element* getElement (long num);
  Element* getElementById (long num);
  long getFirstDataFileNumber();
  long getLastDataFileNumber();
  Material* getMaterial (String name);
  Node* getNode (long num);
  Node* getNodeById (long num);
  Tensor2 getNodalTensor(long nod,String field);
  Vec3D getNodalVector(long nod,String field);
  void addInterface(Interface* pinter);
  void attachBCToNodes(BoundaryCondition* BC, NodeSet* nds);
  void checkBinaryVersionWrite (ofstream & pfile, int);
  void compact();
  void getDataFileBounds(long &min, long& max);
  void getGlobalBox (Vec3D & min, Vec3D & max);
  void mergeModels();
  void readData (ifstream & pfile);
  void readResultFile(long);
  void saveResults();
  void setDefaultElement (Element * pel);
  void setModel(Model* model);
  void setResultFile(String);
  void sortElementsAndNodes();
  void writeData (ofstream & pfile);
   */
};

#endif
