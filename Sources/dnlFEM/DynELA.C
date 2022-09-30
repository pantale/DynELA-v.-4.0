/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <NodeSet.h>
#include <BoundaryCondition.h>
#include <Model.h>
#include <VtkInterface.h>
#include <Solver.h>

class DynELA;
DynELA *dynelaData = NULL; // initialisation par defaut sur NULL

// Default constructor of the DynELA class
/*
  This method is the default constructor of the DynELA class.
  */
//-----------------------------------------------------------------------------
DynELA::DynELA(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
  {
    // Affects the name
    name = newName;

    // Transfer the name to the model
    model.name = "Model_" + name;
  }

  // Initialize the default element to Unknown element (here is better than in .h file)
  _defaultElement = Element::ElGeneric;

  // Creates a settings and Initialize
  settings = new Settings;

  // Checks if already referenced to global object
  if (dynelaData != NULL)
  {
    fatalError("DynELA::DynELA", "Only One DynELA object is allowed for a Finite Element program");
  }
  // Add global reference to the model
  dynelaData = this;

  // on cree egalement un fichier log pour la lecture des donnees
  logFile.init("DynELA.log");

  // Creates a VTK interface for storing results
  dataFile = new VtkInterface;
  _VTKresultFileName = name;

  // Creates and start a global timer
  cpuTimes.timer("Global")->start();

  // Creates the sub-timers
  cpuTimes.add(new Timer("Global:Solver"));
  cpuTimes.add(new Timer("Solver:TimeStep"));
  cpuTimes.add(new Timer("Solver:Jacobian"));
  cpuTimes.add(new Timer("Solver:InternalForces"));
  cpuTimes.add(new Timer("Solver:ExplicitSolve"));
  cpuTimes.add(new Timer("Solver:Strains"));
  cpuTimes.add(new Timer("Solver:Density"));
  cpuTimes.add(new Timer("Solver:Predictor"));
  cpuTimes.add(new Timer("Solver:Pressure"));
  cpuTimes.add(new Timer("Solver:Stress"));
  cpuTimes.add(new Timer("Solver:FinalRotation"));

  /*  printf("Max Threads %d\n", omp_get_max_threads());
  omp_set_num_threads(1);
  printf("Num Threads %d\n", omp_get_max_threads());
 */
}

// constructeur par recopie de la classe DynELA
/*
  Cette methode est le constructeur par recopie de la classe DynELA. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.
  \date 2002
*/
//-----------------------------------------------------------------------------
DynELA::DynELA(const DynELA &X)
//-----------------------------------------------------------------------------
{
}

// destructeur de la classe DynELA
/*
  \date 2002

*/
//-----------------------------------------------------------------------------
DynELA::~DynELA()
//-----------------------------------------------------------------------------
{
  // on detruit le logFile
  logFile.close();

  // On sauve les logs de CPU
  std::string cpuFileName;
  dynelaData->settings->getValue("CpuFileName", cpuFileName);
  cpuTimes.saveReport(cpuFileName.c_str());
}

// recherche d'un noeud dans la structure en fonction de son numero
/*
  Cette methode recherche un noeud dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. Le noeud est recherche sur la grille courante du modele courant.
  - nodeNumber numero du noeud à rechercher
  Return : pointeur sur le noeud trouve ou NULL en cas d'echec de recherche
  \date 2002

*/
//-----------------------------------------------------------------------------
Node *DynELA::getNodeByNum(long nodeNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (model.nodes.size() > 0)
  {
    if (model.nodes.last()->number == nodeNumber)
      return model.nodes.last();
  }

  // no so search for it
  return model.nodes.search(substractNN, nodeNumber);
}

// recherche d'un element dans la structure en fonction de son numero
/*
  Cette methode recherche un element dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. L'element est recherche sur la grille courante du modele courant.
  - elementNumber numero de l'element à rechercher
  Return : pointeur sur l'element trouve ou NULL en cas d'echec de recherche
  \date 2002

*/
//-----------------------------------------------------------------------------
Element *DynELA::getElementByNum(long elementNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (model.elements.size() > 0)
  {
    if (model.elements.last()->number == elementNumber)
      return model.elements.last();
  }

  // no so search for it
  return model.elements.search(substractElementsNumber, elementNumber);
}

// creation d'un noeud et ajout à la structure
/*
  Cette methode cree un nouveau noeud et l'ajoute à la fois à la liste des noeuds de la structure et à la liste des noeuds de la grille courante du modele courant.
  - nodeNumber numero du nouveau noeud à creer
  - x coordonnee x du noeud à creer
  - y coordonnee y du noeud à creer
  - z coordonnee z du noeud à creer
  \date 2002
*/
//-----------------------------------------------------------------------------
bool DynELA::createNode(long nodeNumber, double xCoord, double yCoord, double zCoord)
//-----------------------------------------------------------------------------
{
  // pointeur sur le nouveau noeud
  Node *newNode = new Node(nodeNumber, xCoord, yCoord, zCoord);

  // l'ajouter à la grille courante du modele courant
  model.add(newNode);

  // add the newNode
  // nodes << newNode;

  // logFile
  char st[80];
  sprintf(st, "%ld [%lf,%lf,%lf]", newNode->number, newNode->coords(0), newNode->coords(1), newNode->coords(2));
  logFile << "Node : " << st << " added to " << name << "\n";

  return true;
}

//-----------------------------------------------------------------------------
bool DynELA::createNode(long nodeNumber, Vec3D coords)
//-----------------------------------------------------------------------------
{
  createNode(nodeNumber, coords(0), coords(1), coords(2));

  return true;
}

// renvoie le nombre de noeuds de la structure
/*
  Cette methode renvoie le nombre total de noeuds de la structure

  Return : nombre de noeuds de la structure
*/
//-----------------------------------------------------------------------------
long DynELA::getNodesNumber()
//-----------------------------------------------------------------------------
{
  return model.nodes.size();
}

//-----------------------------------------------------------------------------
long DynELA::getElementsNumber()
//-----------------------------------------------------------------------------
{
  return model.elements.size();
}

//-----------------------------------------------------------------------------
bool DynELA::createElement(long elementNumber, long nodesIndex, ...)
//-----------------------------------------------------------------------------
{
  long nbNodes;
  long nNodes[50];

  // test if the default type is set
  if (_defaultElement == Element::ElGeneric)
  {
    fatalError("DynELA::createElement", "Element type not set\n");
  }

  // nouveau pointeur
  Element *pel = NULL;

  switch (_defaultElement)
  {
  case Element::ElQua4N2D:
    pel = new ElQua4N2D(elementNumber);
    break;
  case Element::ElTri3N2D:
    pel = new ElTri3N2D(elementNumber);
    break;
  case Element::ElQua4NAx:
    pel = new ElQua4NAx(elementNumber);
    break;
  case Element::ElHex8N3D:
    pel = new ElHex8N3D(elementNumber);
    break;
  case Element::ElTet4N3D:
    pel = new ElTet4N3D(elementNumber);
    break;
  case Element::ElTet10N3D:
    pel = new ElTet10N3D(elementNumber);
    break;
  default:
    fatalError("DynELA::createElement(long elementNumber, long nodesIndex, ...)", "Unknown element type\nHave to implement it or wrong Python file ?\n");
  }

  // Creates and assign the integration points
  pel->createIntegrationPoints();

  // Get the number of nodes of the element
  nbNodes = pel->getNumberOfNodes();

  // A place to store the list of arguments
  va_list arguments;

  // Initializing arguments to store all values
  va_start(arguments, nodesIndex);

  // First node from the list
  nNodes[0] = nodesIndex;

  // Loop over the list of nodes
  for (long i = 1; i < nbNodes; i++)
  {
    nNodes[i] = va_arg(arguments, long);
  }

  // Transfer the new element to the model level
  model.create(pel, nNodes);

  // Display the information in the log file
  String str;
  logFile << "Element " << pel->getName() << " : " << str.convert(pel->number) << " [";
  for (long i = 0; i < nbNodes; i++)
  {
    if (i != 0)
      logFile << ", ";
    logFile << str.convert(pel->nodes(i)->number);
  }
  logFile << "] added to " << name << "\n";

  // Cleans up the list
  va_end(arguments);

  return true;
}

//-----------------------------------------------------------------------------
void DynELA::setDefaultElement(short type)
//-----------------------------------------------------------------------------
{
  _defaultElement = type;
}

//-----------------------------------------------------------------------------
void DynELA::add(HistoryFile *newHistoryFile)
//-----------------------------------------------------------------------------
{
  model.add(newHistoryFile);
}

//-----------------------------------------------------------------------------
void DynELA::add(Solver *newSolver)
//-----------------------------------------------------------------------------
{
  model.add(newSolver);
}

// affecte un materiau à un ensemble d'elements
/*
  Cette methode affecte un materiau à un ensemble d'elements de la structure.

  - material materiau à utiliser
  - elementSet ElementSet à utiliser
*/
//-----------------------------------------------------------------------------
void DynELA::add(Material *material, ElementSet *elementSet)
//-----------------------------------------------------------------------------
{
  long i;
  bool already = false;

  // attach all elements of the group
  for (i = 0; i < elementSet->size(); i++)
  {
    elementSet->elements(i)->add(material);

    // logFile
    logFile << "Material " << material->name << " attached to element " << elementSet->elements(i)->number << "\n";
  }

  // attach the group to the material
  material->_elementsSet << elementSet;

  // attach the material to the structure if not already done
  for (i = 0; i < model.materials.size(); i++)
  {
    if (model.materials(i) == material)
    {
      already = true;
      i = model.materials.size();
    }
  }
  if (already == false)
    addMaterial(material);
}

// ajoute un materiau à la structure
/*
  Cette methode ajoute un nouveau materiau à la structure. Le nouveau materiau est initialise et renseigne de maniere externe. Cette methode est uniquement destinee à le stocker au niveau de la stucture. Il est ensuite possible d'y faire reference à partir poissonRatio nom de ce materiau. Cette methode verifie que lorsque l'on ajoute un materiau, si le nom est declare, un autre materiau portant le meme nom n'est pas deje present dans la liste des materiaux. Dans ce cas, une erreur est alors generee.

  - pmat pointeur sur le nouveau materiau à ajouter à la structure
*/
//-----------------------------------------------------------------------------
void DynELA::addMaterial(Material *pmat)
//-----------------------------------------------------------------------------
{
  long i;

  if (pmat->name != "")
  {
    for (i = 0; i < model.materials.size(); i++)
    {
      if (model.materials(i)->name == pmat->name)
      {
        fatalError("DynELA::addMaterial", "Material %s already exist\n", pmat->name.chars());
      }
    }
  }

  // ajout à la liste courante
  model.materials << pmat;

  // logFile
  logFile << "Material " << pmat->name << " added to " << name << "\n";
}

//-----------------------------------------------------------------------------
void DynELA::add(NodeSet *nodeSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  model.add(nodeSet, startNumber, endNumber, increment);
}

//-----------------------------------------------------------------------------
void DynELA::add(ElementSet *elementSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  model.add(elementSet, startNumber, endNumber, increment);
}

// affecte des conditions aux limites initiales à un ensemble de noeuds
/*
  Cette methode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  - boundary condition limite à utiliser
  - nodeSet NodeSet à utiliser
*/
//-----------------------------------------------------------------------------
void DynELA::attachInitialBC(Boundary *boundary, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *boundaryCondition;

  for (long i = 0; i < nodeSet->size(); i++)
  {

    // test if boundary exists already
    if ((boundaryCondition = nodeSet->nodes(i)->boundary) == NULL)
    {
      // no, we have to create it
      boundaryCondition = new BoundaryCondition;
      nodeSet->nodes(i)->boundary = boundaryCondition;
      logFile << "Boundary condition created for node " << nodeSet->nodes(i)->number << "\n";
    }

    // add the boundary condition
    boundaryCondition->addInitial(boundary);

    // logFile
    logFile << "Initial boundary condition attached to node " << nodeSet->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  boundary->nodeSet << nodeSet;
  /* bool tp = 0;
  boundary->typeSet << tp; */
}

// affecte des conditions aux limites constantes à un ensemble de noeuds
/*
  Cette methode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  - boundary condition limite à utiliser
  - nodeSet NodeSet à utiliser
*/
//-----------------------------------------------------------------------------
void DynELA::attachConstantBC(Boundary *boundary, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *boundaryCondition;

  for (long i = 0; i < nodeSet->size(); i++)
  {

    // test if boundary exists already
    if ((boundaryCondition = nodeSet->nodes(i)->boundary) == NULL)
    {
      // no, we have to create it
      boundaryCondition = new BoundaryCondition;
      nodeSet->nodes(i)->boundary = boundaryCondition;
      logFile << "Boundary condition created for node " << nodeSet->nodes(i)->number << "\n";
    }

    // add the boundary condition
    boundaryCondition->addConstant(boundary);

    // logFile
    logFile << "Constant boundary condition attached to node " << nodeSet->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  boundary->nodeSet << nodeSet;
  /* bool tp = 1;
  boundary->typeSet << tp; */
}

//-----------------------------------------------------------------------------
void DynELA::setSaveTimes(double _startSaveTime, double _endSaveTime, double _saveTimeIncrement)
//-----------------------------------------------------------------------------
{
  startSaveTime = _startSaveTime;
  endSaveTime = _endSaveTime;
  saveTimeIncrement = _saveTimeIncrement;
  nextSaveTime = startSaveTime;
}

//-----------------------------------------------------------------------------
void DynELA::writeResultFile()
//-----------------------------------------------------------------------------
{
  // Display estimated end of computation
  displayEstimatedEnd();

  if ((model.currentTime >= nextSaveTime) && (model.currentTime <= endSaveTime))
  {
    // Save file
    writeVTKFile();

    // increase save time
    nextSaveTime += saveTimeIncrement;
  }
}

//-----------------------------------------------------------------------------
void DynELA::displayEstimatedEnd()
//-----------------------------------------------------------------------------
{
  if (dynelaData->cpuTimes.timer("Solver")->getCurrent() >= _nextDisplayTime)
  {
    // Estimate end of computation
    if (model.currentTime > 0)
    {
      double elapsedTime = dynelaData->cpuTimes.timer("Solver")->getCurrent();
      int remainingTime = (elapsedTime - _lastElapsedTime) / (model.currentTime - _lastElapsedComputeTime) * (model.solver->endTime - model.currentTime);
      int remainingHours, remainingMinutes, remainingSeconds;
      int restOfTime;
      int elapsedHours, elapsedMinutes, elapsedSeconds;

      elapsedHours = elapsedTime / 3600;
      restOfTime = int(elapsedTime) % 3600;
      elapsedMinutes = restOfTime / 60;
      elapsedSeconds = restOfTime % 60;
      printf("Current computational time %02d:%02d:%02d\n", elapsedHours, elapsedMinutes, elapsedSeconds);

      remainingHours = remainingTime / 3600;
      restOfTime = int(remainingTime) % 3600;
      remainingMinutes = restOfTime / 60;
      remainingSeconds = restOfTime % 60;
      printf("Estimated end of computation in %02d:%02d:%02d\n", remainingHours, remainingMinutes, remainingSeconds);

      _lastElapsedTime = elapsedTime;
      _lastElapsedComputeTime = model.currentTime;

      // Increase the Next Display Time
      _nextDisplayTime += _displayTimeIncrement;
    }
  }
}

//-----------------------------------------------------------------------------
void DynELA::translate(Vec3D translateVector, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->size(); i++)
      nodeSet->nodes(i)->coords += translateVector;
  else
    for (long i = 0; i < model.nodes.size(); i++)
      model.nodes(i)->coords += translateVector;
}

//-----------------------------------------------------------------------------
void DynELA::scale(double scaleValue, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->size(); i++)
      nodeSet->nodes(i)->coords *= scaleValue;
  else
    for (long i = 0; i < model.nodes.size(); i++)
      model.nodes(i)->coords *= scaleValue;
}

//-----------------------------------------------------------------------------
void DynELA::scale(Vec3D scaleVector, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->size(); i++)
      for (long c = 0; c < 3; c++)
        nodeSet->nodes(i)->coords(c) *= scaleVector(c);
  else
    for (long i = 0; i < model.nodes.size(); i++)
      for (long c = 0; c < 3; c++)
        model.nodes(i)->coords(c) *= scaleVector(c);
}

//-----------------------------------------------------------------------------
void DynELA::rotate(String set, double angle, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;
  Mat = 0;

  double angleRadians = angle * dnlDegToRad;

  // rotation autour de X
  if (set == "X" || set == "x")
  {
    Mat(0, 0) = 1;
    Mat(1, 1) = cos(angleRadians);
    Mat(1, 2) = sin(angleRadians);
    Mat(2, 1) = -sin(angleRadians);
    Mat(2, 2) = cos(angleRadians);
  }

  // rotation autour de Y
  if (set == "Y" || set == "y")
  {
    Mat(0, 0) = cos(angleRadians);
    Mat(0, 2) = -sin(angleRadians);
    Mat(1, 1) = 1;
    Mat(2, 0) = sin(angleRadians);
    Mat(2, 2) = cos(angleRadians);
  }

  // rotation autour de Z
  if (set == "Z" || set == "z")
  {
    Mat(0, 0) = cos(-angleRadians);
    Mat(0, 1) = sin(-angleRadians);
    Mat(1, 0) = -sin(-angleRadians);
    Mat(1, 1) = cos(-angleRadians);
    Mat(2, 2) = 1;
  }

  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->size(); i++)
      nodeSet->nodes(i)->coords = Mat * nodeSet->nodes(i)->coords;
  else
    for (long i = 0; i < model.nodes.size(); i++)
      model.nodes(i)->coords = Mat * model.nodes(i)->coords;
}

//-----------------------------------------------------------------------------
void DynELA::rotate(Vec3D axis, double angle, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;

  double angleRadians = angle * dnlDegToRad;

  // normalisation de l'axe
  axis.normalize();

  Mat(0, 0) = dnlSquare(axis(0)) + cos(angleRadians) * (-dnlSquare(axis(0)) + 1);
  Mat(0, 1) = axis(0) * axis(1) - axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(0, 2) = axis(0) * axis(2) + axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(1, 0) = axis(0) * axis(1) + axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(1, 1) = dnlSquare(axis(1)) + cos(angleRadians) * (-dnlSquare(axis(1)) + 1);
  Mat(1, 2) = axis(1) * axis(2) - axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 0) = axis(0) * axis(2) - axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(2, 1) = axis(1) * axis(2) + axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 2) = dnlSquare(axis(2)) + cos(angleRadians) * (-dnlSquare(axis(2)) + 1);

  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->size(); i++)
      nodeSet->nodes(i)->coords = Mat * nodeSet->nodes(i)->coords;
  else
    for (long i = 0; i < model.nodes.size(); i++)
      model.nodes(i)->coords = Mat * model.nodes(i)->coords;
}

// calcule les coordonnees mini et maxi de l'ensemble des noeuds d'une structure
/*
  Cette methode calcule l'enveloppe mini et maxi des coordonnees des points d'uns structure complete

  - min coordonnee minimale
  - max coordonnee maximale
*/
//-----------------------------------------------------------------------------
void DynELA::getGlobalBox(Vec3D &minPoint, Vec3D &maxPoint)
//-----------------------------------------------------------------------------
{
  Vec3D coords;
  long i, j;

  // affectation par defaut au commencement
  maxPoint = minPoint = model.nodes(0)->coords;

  // boucle de recherche
  for (i = 1; i < model.nodes.size(); i++)
  {
    coords = model.nodes(i)->coords;

    for (j = 0; j < 3; j++)
    {
      if (coords(j) < minPoint(j))
        minPoint(j) = coords(j);
      if (coords(j) > maxPoint(j))
        maxPoint(j) = coords(j);
    }
  }
}

//-----------------------------------------------------------------------------
void DynELA::writeVTKFile()
//-----------------------------------------------------------------------------
{
  String fileName;
  String number;
  number.convert(_VTKresultFileIndex, 3);
  fileName = _VTKresultFileName + number + ".vtk";

  // Initialize the vtk data file
  dataFile->openFile(fileName);

  // Write the vtk data file
  dataFile->write();

  // Close the vtk data file
  dataFile->close();

  logFile << "Result file: " << fileName << " written at time " << model.currentTime << " s\n";
  std::cout << "Write VTK result file: " << fileName << " at time " << model.currentTime << " s\n";

  // increment the index
  _VTKresultFileIndex++;
}

//-----------------------------------------------------------------------------
bool DynELA::initSolve()
//-----------------------------------------------------------------------------
{
  // Run the init solve of this model
  model.initSolve();

  return true;
}

// lancement du solveur general
/*
  Cette methode lance la procedure de solveur general de la structure. Elle prend en compte tous les types de solveurs possible et gere aussi bien la resolution mono-modele que la resolution multi-modele. C'est le point d'entree de tout solveur.
*/
//-----------------------------------------------------------------------------
void DynELA::solve()
//-----------------------------------------------------------------------------
{
  // Get default value for displaying progress time
  dynelaData->settings->getValue("DisplayProgress", _displayTimeIncrement);
  dynelaData->settings->getValue("DisplayProgress", _nextDisplayTime);

  // Display start of solve phase
  std::cout << "\nProcessing DynELA ...\n";

  // Run the init solve of this model
  initSolve();

  // Get the end time of the structure and display it
  double endOfComputationTime = model.getEndSolveTime();
  logFile << "Set final computation time to: " << endOfComputationTime << " s\n";

  // Save initial configuration
  writeResultFile();

  // Write start on computation into log file
  logFile.separatorWrite("DynELA Solver phase");

  // Start Solver timer
  cpuTimes.timer("Solver")->start();

  // Run the Explicit Solver
  bool solvedIsOK = model.solve(endOfComputationTime);

  // Stop the timer for the solver
  cpuTimes.timer("Solver")->stop();

  // Test if solve was Ok or not
  if (solvedIsOK == false)
  {
    // Emergency write of a result file
    writeVTKFile();
    fatalError("Solver Error", "Unable to solve problem upto time = %10.3E\n", nextSaveTime);
  }

  // Write the final result file
  writeVTKFile();

  // Stop all time logs
  cpuTimes.stop();
}

//-----------------------------------------------------------------------------
void DynELA::getNodalValuesRange(short field, double &min, double &max)
//-----------------------------------------------------------------------------
{
  Field fields;

  if (fields.getType(field) != 0)
  {
    fatalError("DynELA::getNodalValuesRange", "field must be scalar");
  }

  Node *pnd = model.nodes.initLoop();
  double val;
  min = max = pnd->fieldScalar(field);
  while ((pnd = model.nodes.currentUp()) != NULL)
  {
    val = pnd->fieldScalar(field);
    if (val < min)
      min = val;
    if (val > max)
      max = val;
  }
  model.nodes.endLoop();
}
/*

//Initialise les structures memoire pour la resolution
/*
  Cette methode est utilisee pour initialiser les structures memoire apres lecture des donnees dans le fichier source. C'est la phase de preprocessing de la structure. Un ensemble de verifications sont effectuees dans cette methode.

  Return : true si la methode n'a genere aucune erreur


//-----------------------------------------------------------------------------
bool DynELA::initSolve ()
//-----------------------------------------------------------------------------
{
  long i;

  cout << "\nPreProcessing of the structure ...\n";

  // log file
  logFile->separatorWrite();
  logFile << "\nInitialisation of solver phase ........\n\nStructure architecture\n";

  // log file
  logFile << elements.size()<<" elements\n"<<nodes.size()<<" nodes\n";

  // initialisation du temps
  currentTime=0;
  // initialisation des modeles
  for (i=0; i<models.size();i++)
    {
      logFile << "\nVerification of model "<<i<<" ...\n";
      if (models(i)->initSolve()==false)
  {
    // model has to be deleted
    logFile << "Void Model "<<i<< " has been deleted ...\n";
    delete models(i);
    models.del(i);
    i--;
  }
    }
  logFile << "\nPhysic contains "<<i<< " model"<<(i>1 ? "s":"")<<" ...\n\n";

  // si pas de modeles, alors pas de physique

  // si pas de dommaines, alors pas de structure
  if (models.size()==0) return (false);

  // verification de la coherence des materiaux
  for (i = 0; i < model.materials.size (); i++)
    {
      logFile << "Verification of material "<<i<<" named "<<model.materials (i)->name<<" ...\n";
      model.materials (i)->checkValues ();
    }

  // calcul des materiaux
  for (i = 0; i < model.materials.size (); i++)
    {
      logFile << "Initialisation of material "<<i<<" named "<<model.materials (i)->name<<" ...\n";
      model.materials (i)->computeHookeTensor ();
    }

  logFile << "\nEnd of initialisation of solver phase ........\n";

  // preparation des save times
  if (startSaveTime == 0) startSaveTime=saveTimeIncrement;
  nextSaveTime = startSaveTime;

  nodes.sort();
  elements.sort();

  return (true);
}

//recherche si un materiau existe actuellement
/*
  Cette methode est utilisee pour recuperer un materiau donne à partir de son nom dans une structure. Elle cherche un materiau donne en fonction de son nom et renvoie un pointeur sur ce materiau (s'il existe) ou NULL dans le cas oe le materiau n'a pu etre trouve dans la liste des materiaux de la structure.

  - name nom du materiau à recuperer
  Return : un pointeur sur ce materiau (s'il existe) ou NULL dans le cas oe le materiau n'a pu etre trouve dans la liste des materiaux de la structure.


//-----------------------------------------------------------------------------
Material * DynELA::getMaterial (String name)
//-----------------------------------------------------------------------------
{
  // balayage de la liste des materiaux
  for (long i = 0; i < model.materials.size (); i++)
    {
      // materiau trouve
      if (model.materials (i)->name == name)
  return model.materials (i);
    }

  // materiau non trouve
  return NULL;
}

//affecte des conditions aux limites à un ensemble de noeuds
/*
  Cette methode affecte des conditions aux limites à un ensemble de noeuds de la structure.

  - BC condition limite à utiliser
  - nds NodeSet à utiliser


//-----------------------------------------------------------------------------
void DynELA::attachBCToNodes(BoundaryCondition* BC, NodeSet* nds)
//-----------------------------------------------------------------------------
{
  for (long i=0;i<nds->size();i++){
    nds->nodes(i)->boundary=BC;

  // logFile
   logFile << "A boundary condition has been attached to node "<<nds->nodes(i)->number<<"\n";
  }
}

//ajoute un solveur à la structure
/*
  Cette methode ajoute un solveur à la structure. Le solveur est ajoute à la liste des solveurs du modele courant de la structure.

  - solver solveur à utiliser






//renvoie le nombre d'elements de la structure
/*
  Cette methode renvoie le nombre total d'elements de la structure

  Return : nombre d'elements de la structure



//renvoie le ieme noeud de la structure
/*
  Cette methode renvoie le ieme noeud de la structure

  - i indice du noeud
  Return : ieme noeud de la structure


//-----------------------------------------------------------------------------
Node* DynELA::getNode(long i)
//-----------------------------------------------------------------------------
{
  return nodes(i);
}

//renvoie le ieme element de la structure
/*
  Cette methode renvoie le renvoie le ieme element de la structure

  - i indice de l'element
  Return : ieme element de la structure


//-----------------------------------------------------------------------------
Element* DynELA::getElement(long i)
//-----------------------------------------------------------------------------
{
  return elements(i);
}

//renvoie un noeud en fonction de son Id
/*
  Cette methode renvoie un noeud en fonction de son Id

  - i Id du noeud
  Return : ieme noeud de la structure


//-----------------------------------------------------------------------------
Node* DynELA::getNodeById(long i)
//-----------------------------------------------------------------------------
{
  return nodes.AppN(i);
}

//renvoie un element en fonction de son Id
/*
  Cette methode renvoie un element en fonction de son Id

  - i Id de l'element
  Return : ieme noeud de la structure


//-----------------------------------------------------------------------------
Element* DynELA::getElementById(long i)
//-----------------------------------------------------------------------------
{
  return elements.AppN(i);
}

//selectionne un modele dans la structure
/*
  Cette methode permet de creer un nouveau modele dans la structure ou de selectionner un autre modele pour le modele courant de la structure.

  - model pointeur sur le modele à selectionner


//-----------------------------------------------------------------------------
void DynELA::setModel(Model* model)
//-----------------------------------------------------------------------------
{
//  long ind;
  // si le modele existe deja, on n'a pas à le creer
  if (!models.contains(model))
    {
      models << model;
    }

  // change the current index
  models(models.index(model));
  logFile << "DynELA: "<<name << " model: "<<models.current()->name<<" selected\n";
}


//affiche pendant la lecture des donnees un etat d'avancement sur la console
/*
  Cette methode affiche pendant la lecture des donnees un etat d'avancement sur la console du remplissage memoire concernant les nombres de noeuds, elements et modeles de la structure complete.



//-----------------------------------------------------------------------------
void DynELA::displayOnline()
//-----------------------------------------------------------------------------
{
//  long doms=0;

  // memoire du modele
//  long indDom=models.index(models.current());

//  for (long i=0;i<models.size();i++) doms+=physics(i)->models.size();

  printf("\rRead %ld model%s %ld node%s %ld element%s %ld material%s",
   models.size(),(models.size() > 1 ? "s" : ""),
   nodes.size(),(nodes.size()>1 ? "s" : ""),
   elements.size(),(elements.size()>1 ? "s" : ""),
   model.materials.size(),(model.materials.size()>1 ? "s" : ""));

  // reset currentModel
 // physics(indPhy);

  fflush(stdout);
}

//-----------------------------------------------------------------------------
void DynELA::saveResults()
//-----------------------------------------------------------------------------
{
  if ((currentTime>=nextSaveTime) && (currentTime!=previousSaveTime)) {

    // transfer the datas
    resultFile->transfert();

    // increment saveTimes
    previousSaveTime=currentTime;
    nextSaveTime+=saveTimeIncrement;
  }
}


//-----------------------------------------------------------------------------
double DynELA::getReadTimeData (ifstream & pfile)
//-----------------------------------------------------------------------------
{
  double tim;

  if (checkBinaryVersion (pfile, 0) != Ok)
    fatalError ("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read ((char *) &tim, sizeof (double));

  return tim;
}

//-----------------------------------------------------------------------------
void DynELA::readData (ifstream & pfile)
//-----------------------------------------------------------------------------
{
  long i;
  //  long currentModel;

  if (checkBinaryVersion (pfile, 0) != Ok)
    fatalError ("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read ((char *) &currentTime, sizeof (double));

  // load current model number
  //  pfile.read ((char *) &currentModel, sizeof (long));
  //  models(currentModel);

  // load the models
  for (i = 0; i < models.size (); i++)
    {
      models(i)->readData(pfile);
      if (checkBinaryVersion (pfile, 1) != Ok)
  fatalError ("model datas", "Read error");
    }
}

//-----------------------------------------------------------------------------
void DynELA::writeData (ofstream & pfile)
//-----------------------------------------------------------------------------
{
  long i;
  // long currentPhysic;

  checkBinaryVersionWrite (pfile, 0);

  // save current time
  pfile.write ((char *) &currentTime, sizeof (double));

  // load the models
  for (i = 0; i < models.size (); i++)
    {
      models(i)->writeData(pfile);
      checkBinaryVersionWrite (pfile, 1);
    }
}

//-----------------------------------------------------------------------------
bool DynELA::checkBinaryVersion (ifstream & pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  if (top == 0)
    {
      pfile >> version;
      pfile >> check;

      if (check (0) != versionCheckDefine)
  {
    cerr << "File recorded by DynELA v." << (int) version (0) << "." <<
      (int) version (1) << "." << (int) version (2) << "-" << (int)
      check (1) << endl;
    cerr << endl << "This recording format is not compatible with ";
    cerr << "DynELA v." << VERSION << "." << RELEASE << "." <<
      UNDER_RELEASE << "-" << ALPHA_VERSION << endl;
    cerr << "\nYou MUST delete previous results and rerun the solver\n";
    return Fail;
  }
      return Ok;
    }
  else
    {
      pfile >> check;
      if (check (0) != separatorCheckDefine)
  {
    cerr << "\n May be file is trucated\n";
    return Fail;
  }
      return Ok;
    }
}

//-----------------------------------------------------------------------------
void DynELA::checkBinaryVersionWrite (ofstream & pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  version (0) = (double) VERSION;
  version (1) = (double) RELEASE;
  version (2) = (double) UNDER_RELEASE;
  check (0) = versionCheckDefine;
  check (1) = ALPHA_VERSION;

  if (top == 0)
    {
      check (0) = versionCheckDefine;
      pfile << version;
      pfile << check;
    }
  else
    {
      check (0) = separatorCheckDefine;
      pfile << check;
    }
}


//-----------------------------------------------------------------------------
void DynELA::sortElementsAndNodes()
//-----------------------------------------------------------------------------
{
  nodes.compact();
  elements.compact();
}

//-----------------------------------------------------------------------------
void DynELA::setDefaultElement (Element * pel)
//-----------------------------------------------------------------------------
{
  _defaultElement=pel;
}

//-----------------------------------------------------------------------------
void DynELA::addInterface (Interface* pinter)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert (pinter!=NULL);
#endif

  models.current()->addInterface(pinter);
  logFile << "Interface : " << pinter->name << " added to "<<name<<"\n";
}


//-----------------------------------------------------------------------------
void DynELA::compact()
//-----------------------------------------------------------------------------
{
  long i,j,k,l;
  Model *pmodel;
//  Physic* pphysic;
//  Grid* pgrid;
  long elementNum,nodeNum;

//  for (i=0;i<physics.size();i++)
//    {
//      pphysic=physics(i);
      for (j=0;j<models.size();j++)
  {
    pmodel=models(j);
//	  for(k=0;k<pmodel.grids.size();k++)
//	    {
        elementNum=1;
        nodeNum=1;
//	      pgrid=pmodel.grids(k);
        for (l=0;l<pmodel.nodes.size();l++)
    {
      pmodel.nodes(l)->number=nodeNum++;
    }
        for (l=0;l<pmodel.elements.size();l++)
    {
      pmodel.elements(l)->number=elementNum++;
    }
      }
  //}
   // }
}

//-----------------------------------------------------------------------------
void DynELA::readResultFile(long num)
//-----------------------------------------------------------------------------
{
  // get the old mode
  char oldMode=resultFile->getMode();

  // set mode to read mode
  resultFile->setMode(Read);

  // set the increment
  resultFile->incrementalFile()=true;
  resultFile->startCurrentSave(num);

  // get the transfert
  resultFile->transfert();

  // rest the mode
  resultFile->setMode(oldMode);

  // logFile
  logFile << "Result datafile "<<resultFile->getFileName() << " loaded\n";
}

//-----------------------------------------------------------------------------
double DynELA::getCurrentTime()
//-----------------------------------------------------------------------------
{
  return currentTime;
}

//-----------------------------------------------------------------------------
void DynELA::getDataFileBounds(long &min, long& max)
//-----------------------------------------------------------------------------
{
  dirent *file;
  DIR* direct;
  direct= opendir(".");
  String sub;
  bool v0;
  long val;
  String filename=resultFile->getFileName();

  // skip file extension if exists
  if (filename.contains(sourceFileExtension)) filename=filename.before(sourceFileExtension);

  v0=false;
  bool ok=false;

  if ( !direct ) {
    fatalError("readDirectory","can't read directory");
  }
  while ( (file = readdir(direct)) ) {
    String name(file->d_name);
    if ((name.contains(filename)) && (name.contains(resultFileExtension))) {
      sub=name.before(resultFileExtension);
      sub=sub.after(filename);
      sub=sub.after(".");
      val=sub.getInt();
      if (!v0) {min=max=val;v0=true;}
      if (val<min) min=val;
      if (val>max) max=val;
      ok=true;
    }
  }
  if (!ok) min=max=0;
}

//-----------------------------------------------------------------------------
long DynELA::getLastDataFileNumber()
//-----------------------------------------------------------------------------
{
  long min,max;
  getDataFileBounds(min, max);
  return max;
}

//-----------------------------------------------------------------------------
long DynELA::getFirstDataFileNumber()
//-----------------------------------------------------------------------------
{
  long min,max;
  getDataFileBounds(min, max);
  return min;
}

//-----------------------------------------------------------------------------
double DynELA::fieldScalar(long nod,String field,long component)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->fieldScalar( field, component);
}

//-----------------------------------------------------------------------------
Vec3D DynELA::getNodalVector(long nod,String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalVector( field);
}

//-----------------------------------------------------------------------------
Tensor2 DynELA::fieldTensor2(long nod,String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->fieldTensor2( field);
}

//-----------------------------------------------------------------------------
void DynELA::setResultFile(String file)
//-----------------------------------------------------------------------------
{
  resultFile->getFileName()=file;
}

//-----------------------------------------------------------------------------
void DynELA::mergeModels()
//-----------------------------------------------------------------------------
{
  // if only one grid;
  if (models.size()==1) return;

  long nnum=1,elnum=1;
  long i,j;
  Model* pmodeld, *pmodelo;

  pmodeld=models(0);

  for (j=0;j<pmodeld->elements.size();j++)
    {
      pmodeld->elements(j)->number=elnum++;
    }
  for (j=0;j<pmodeld->nodes.size();j++)
    {
      pmodeld->nodes(j)->number=nnum++;
    }

  for (i=1; i<models.size();i++)
    {
      pmodelo=models(i);

      // copy elements
      for (j=0;j<pmodelo->elements.size();j++)
  {
    pmodeld->elements << pmodelo->elements(j);
    pmodelo->elements(j)->number=elnum++;
  }

      // copy nodes
      for (j=0;j<pmodelo->nodes.size();j++)
  {
    pmodeld->nodes << pmodelo->nodes(j);
    pmodelo->nodes(j)->number=nnum++;
  }
    }

  models.del(1,models.size()-1);
}
*/

//-----------------------------------------------------------------------------
void DynELA::setStressComputation(char *method)
//-----------------------------------------------------------------------------
{
  String meth = method;
  bool ok = false;
  if (meth == "Direct")
  {
    model._stressIntegrationMethod = model.StressIntDirect;
    // logFile
    logFile << "Stress computation method set to : Direct \n";
    ok = true;
  }
  if (meth == "NewtonRaphson")
  {
    model._stressIntegrationMethod = model.StressIntNR;
    // logFile
    logFile << "Stress computation method set to : Newton-Raphson \n";
    ok = true;
  }
  if (!ok)
  {
    printf("Unknown setStressComputation method : %s\n", method);
    exit(-1);
  }
}
