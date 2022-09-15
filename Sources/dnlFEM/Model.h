/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlFEM_Model_h__
#define __dnlFEM_Model_h__

#include <MatrixDiag.h>
#include <Vector.h>

class DynELA;
class Element;
class ElementSet;
class HistoryFile;
class Node;
class NodeSet;
class Solver;
class Material;

//-----------------------------------------------------------------------------
// Class : class Model
//
//  Used to manage a FE Model in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class Model
{
  friend class DynELA;

private:
#ifndef SWIG
  enum
  {
    StressIntNR = 0,
    StressIntDirect
  };
#endif
  bool _massMatrixComputed = false;        // Flag defining that the mass matrix has already been computed
  double _powerIterationFreqMax = 0.0;     // Initial value for the max frequency
  double _powerIterationPrecision = 1e-4;  // Precision of the Power Iteration Agorithm
  int _powerIterationMaxIterations = 1000; // Max number of iterations for the Power Iteration Agorithm
  short _numberOfDimensions = 0;           // Number of dimensions of the model
  Vector _powerIterationEV;
  bool _initSolveDone = false;
  int _stressIntegrationMethod = StressIntNR;

public:
  double currentTime = 0.0;          // Temps actuel du modele
  double nextTime = 0.0;             // Temps actuel du modele + increment de temps
  List <ElementSet *> elementsSets;  // List of the Elements Sets
  List <HistoryFile *> historyFiles; // List of the History Files
  List <Material *> materials;       // Materials list of the structure
  List <NodeSet *> nodesSets;        // List of the Nodes Sets
  ListIndex <Element *> elements;    // List of the Elements
  ListIndex <Node *> nodes;          // List of the Nodes
  MatrixDiag massMatrix;             // Mass matrix
  Solver *solver = NULL;             // solveurs associes au modele
  String name = "Model::_noname_";   // Name of the model
  Vector internalForces;             // Vecteur des forces internes

private:
  bool add(Element *pel);
  bool add(Node *pnd);
  void add(ElementSet *elementSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(HistoryFile *newHistoryFile);
  void add(NodeSet *nodeSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Solver *newSolver);

public:
  // constructeurs
  Model(char *newName = NULL);
  Model(const Model &model);
  ~Model();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  bool checkTopology();
  bool initSolve();
  bool solve(double solveUpToTime = -1.0);
  double computeCourantTimeStep();
  double computePowerIterationTimeStep(bool underIntegration = false);
  double getEndSolveTime();
  double getTotalKineticEnergy();
  double getTotalMass();
  Element *getElementByNum(long elementNumber);
  Node *getNodeByNum(long nodeNumber);
  short getNumberOfDimensions();
  void compactNodesAndElements();
  void computeFinalRotation();
  void computeInternalForces();
  void computeJacobian(bool reference = false);
  void computeUnderJacobian(bool reference = false);
  void computeMassMatrix(bool forceComputation = false);
  void computePressure();
  void computeStrains();
  void computeStress(double timeStep);
  void create(Element *pel, long *listOfNodesNumber);
  void transfertQuantities();
  void writeHistoryFiles();

  /*
  // gestion du temps
  double getTime ();
  void updateTime ();

  // interfaces
  void addInterface(Interface* inter);

  // fonctions de calculs

  void starterWrite (String name);
  double getCurrentTime();
  double getTimeStep();
  void getGlobalBox (Vec3D & min, Vec3D & max);

  // fonctions entree sortie
  void print (ostream & os) const;
  friend ostream & operator << (ostream & os, Model & dom);
  void readData (ifstream & pfile);
  double getReadTimeData (ifstream & pfile);
  void writeData (ofstream & pfile);

  // methodes liees à la lecture des fichiers
  void createNode(long,double,double,double);
  Node* getNodeByNumber(long);
  //void setGrid(Grid* grid);

  // direct implementation
  long getNumberOfDimensions() {return dimension;} // Dimension topologique de la grille
  */
};

//-----------------------------------------------------------------------------
inline short Model::getNumberOfDimensions()
//-----------------------------------------------------------------------------
{
  return _numberOfDimensions;
}

#endif
