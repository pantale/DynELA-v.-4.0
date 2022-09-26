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
#include <Model.h>
#include <Element.h>
#include <HistoryFile.h>
#include <Solver.h>
#include <BoundaryCondition.h>

//-----------------------------------------------------------------------------
Model::Model(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  /*   name = "defaultModel";

  // initialisation par defaut
  _numberOfDimensions = 0;

  // attacher a global model
  history_file = NULL;

  _powerIterationFreqMax = 0.0;

  // redimensionner les listes pour gagner du temps dans les lectures
  // on utilise des blocs de 100 allocations pour les noeuds et les elements
  nodes.stackIncrement() = 100;
  elements.stackIncrement() = 100;

  nodes.redim(100);
  elements.redim(100); */
}

//-----------------------------------------------------------------------------
Model::Model(const Model &model)
//-----------------------------------------------------------------------------
{
  /*
  _numberOfDimensions = model._numberOfDimensions;
  cerr << "erreur copie de Model\n";
  exit(-1); */
}

//-----------------------------------------------------------------------------
Model::~Model()
//-----------------------------------------------------------------------------
{
}

// Ajout d'un noeud à la grille courante
/*
  Cette methode ajoute un noeud à la grille courante. Cette methode effectue des verifications de base comme par exemple la presence d'un noeud portant le meme numero que le nouveau noeud dans la liste. En cas de doublon, cette methode stoppe le processus de construction et renvoie une erreur.
  - newNode pointeur sur le nouveau noeud à ajouter à la grille
  Return : true si l'ajout du noeud est correct
*/
//-----------------------------------------------------------------------------
bool Model::add(Node *newNode)
//-----------------------------------------------------------------------------
{
  // search if not already in the list
  if (nodes.dichotomySearch(substractNodesNumber, newNode->number) != NULL)
  {
    fatalError("Model::add", "Node %ld already exists in the node list of this model\n", newNode->number);
  }

  if ((nodes.getSize() >= 1) && (newNode->number < nodes.last()->number))
  {
    // add the node to the grid
    nodes << newNode;

    // qsort of the list if not sorted
    nodes.sort(compareNodesNumber);
  }
  else
  {
    // add the node to the grid
    nodes << newNode;
  }

  // Compact the F**king list of nodes
  nodes.compact();

  // return happy
  return true;
}

//-----------------------------------------------------------------------------
bool Model::add(Element *newElement)
//-----------------------------------------------------------------------------
{
  // search if not already in the list
  if (elements.dichotomySearch(substractElementsNumber, newElement->number) != NULL)
  {
    fatalError("Model::add", "Element %ld already exists in the element list of this Model\n", newElement->number);
  }

  if ((elements.getSize() >= 1) && (newElement->number < elements.last()->number))
  {
    // add the element to the grid
    elements << newElement;

    // qsort of the list if not sorted
    elements.sort(compareElementsNumber);
  }
  else
  {
    // add the element to the grid
    elements << newElement;
  }

  // Compact the F**king list of elements
  elements.compact();

  // return happy
  return true;
}

//-----------------------------------------------------------------------------
void Model::create(Element *newElement, long *listOfNodes)
//-----------------------------------------------------------------------------
{
  Node *pNode;

  // ajout de l'element à la grille et verification de sa non presence
  add(newElement);

  // add the reference to the grid of the element
  // newElement->model = this;

  // construction de l'element et de ses noeuds
  long nbOk = 0;

  for (long j = 0; j < newElement->getNumberOfNodes(); j++)
  {
    if ((pNode = nodes.dichotomySearch(substractNodesNumber, listOfNodes[j])) != NULL)
    {
      // Add the node to the liste of nodes of the new element
      newElement->addNode(pNode);

      // Add the reference of the element to the current node
      pNode->elements << newElement;

      // Delete the node from the candidates
      listOfNodes[j] = -1;

      // Increment number of OK nodes
      nbOk++;
    }
  }

  if (nbOk != newElement->getNumberOfNodes())
  {
    for (long j = 0; j < newElement->getNumberOfNodes(); j++)
    {
      if (listOfNodes[j] != -1)
        std::cout << "Node " << listOfNodes[j] << " doesn't exist in this grid\n";
    }
    fatalError("Model::createElement", "some nodes not exist in this grid");
  }
}

// Add new nodes to the current NodeSet
/*
  This method adds a set of existing nodes and a NodeSet. The nodes are specified by their identification numbers given in a variation range (start number, end number and increment). The nodes are then searched in the current grid of the current model of the structure.
  \warning Les noeuds ajoutes doivent etre presents dans la grille courante du modele courant.
  - sNumber of start number in the list
  - eNumber of end number in the list
  - inc increment on the numbers (default = 1)
*/
//-----------------------------------------------------------------------------
void Model::add(NodeSet *nodeSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  Node *node;

  if (startNumber != -1)
  {
    if (endNumber == -1)
    {
      if ((node = getNodeByNum(startNumber)) == NULL)
        fatalError("Model::add", "Node %d not exist in current grid and model", startNumber);

      // add the node to the nodeSet
      nodeSet->add(node);

      // logFile
      dynelaData->logFile << "node " << node->number << " added to node set " << nodeSet->name.chars() << "\n";
    }
    else
    {
      for (long i = startNumber; i <= endNumber; i += increment)
      {
        if ((node = getNodeByNum(i)) == NULL)
        {
          fatalError("Model::add", "Node %d not exist in current grid and model", i);
        }

        // add the node
        nodeSet->add(node);

        // logFile
        dynelaData->logFile << "node " << node->number << " added to node set " << nodeSet->name.chars() << "\n";
      }
    }
  }

  // Add the nodeSet to the list if not exists
  if (!nodesSets.contains(nodeSet))
  {
    nodesSets << nodeSet;
    dynelaData->logFile << "NodeSet " << nodeSet->name.chars() << " added to " << dynelaData->name << "\n";
  }
}

// ajoute un ensemble d'elements à un ElementSet
/*
  Cette methode ajoute un ensemble d'elements existant à un ElementSet. Les elements sont specifies par leurs numeros d'identification donne dans un intervalle de variation (numero de debut, numero de fin et increment). Les elements sont alors recherches dans la grille courante du modele courant de la structure.
  \warning Les elements ajoutes doivent etre presents dans la grille courante du modele courant.
  - startNumber numero de depart dans la liste
  - endNumber numero de fin dans la liste
  - increment increment sur les numeros (par defaut = 1)

*/
//-----------------------------------------------------------------------------
void Model::add(ElementSet *elementSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  Element *element;

  if (startNumber != -1)
  {
    if (endNumber == -1)
    {
      if ((element = getElementByNum(startNumber)) == NULL)
        fatalError("Model::add", "Node %d not exist in current grid and model", startNumber);

      // add the node to the elementSet
      elementSet->add(element);

      // logFile
      dynelaData->logFile << "element " << element->number << " added to element set " << elementSet->name.chars() << "\n";
    }
    else
    {
      for (long i = startNumber; i <= endNumber; i += increment)
      {
        if ((element = getElementByNum(i)) == NULL)
        {
          fatalError("Model::add", "Node %d not exist in current grid and model", i);
        }

        // add the node
        elementSet->add(element);

        // logFile
        dynelaData->logFile << "element " << element->number << " added to element set " << elementSet->name.chars() << "\n";
      }
    }
  }

  // Add the elementSet to the list if not exists
  if (!elementsSets.contains(elementSet))
  {
    elementsSets << elementSet;

    // logFile
    dynelaData->logFile << "ElementSet " << elementSet->name.chars() << " added to " << dynelaData->name << "\n";
  }
}

// recherche d'un noeud dans la structure en fonction de son numero
/*
  Cette methode recherche un noeud dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. Le noeud est recherche sur la grille courante du modele courant.
  - nodeNumber numero du noeud à rechercher
  Return : pointeur sur le noeud trouve ou NULL en cas d'echec de recherche
  \date 2002

*/
//-----------------------------------------------------------------------------
Node *Model::getNodeByNum(long nodeNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (nodes.getSize() > 0)
  {
    if (nodes.last()->number == nodeNumber)
      return nodes.last();
  }

  // no so search for it
  return nodes.dichotomySearch(substractNodesNumber, nodeNumber);
}

// recherche d'un element dans la structure en fonction de son numero
/*
  Cette methode recherche un element dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. L'element est recherche sur la grille courante du modele courant.
  - elementNumber numero de l'element à rechercher
  Return : pointeur sur l'element trouve ou NULL en cas d'echec de recherche
  \date 2002

*/
//-----------------------------------------------------------------------------
Element *Model::getElementByNum(long elementNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (elements.getSize() > 0)
  {
    if (elements.last()->number == elementNumber)
      return elements.last();
  }

  // no so search for it
  return elements.dichotomySearch(substractElementsNumber, elementNumber);
}

//-----------------------------------------------------------------------------
bool Model::checkTopology()
//-----------------------------------------------------------------------------
{
  // Verify coherence of the elements
  short firstElementFamily = elements(0)->getFamily();

  for (long i = 1; i < elements.getSize(); i++)
    if (firstElementFamily != elements(i)->getFamily())
    {
      std::cout << "Passing from ";
      if (firstElementFamily == Element::Bidimensional)
        std::cout << "2D";
      if (firstElementFamily == Element::Axisymetric)
        std::cout << "2D Axi";
      if (firstElementFamily == Element::Threedimensional)
        std::cout << "3D";
      std::cout << " to ";
      if (elements(i)->getFamily() == Element::Bidimensional)
        std::cout << "2D\n";
      if (elements(i)->getFamily() == Element::Axisymetric)
        std::cout << "2D Axi\n";
      if (elements(i)->getFamily() == Element::Threedimensional)
        std::cout << "3D\n";
      fatalError("Grid topology verification\n",
                 "Mixed Topology found in element %d \n", i);
      return false;
    }

  return true;
}

//-----------------------------------------------------------------------------
bool Model::initSolve()
//-----------------------------------------------------------------------------
{
  // If the init solve has already beeen done, return
  if (_initSolveDone)
  {
    std::cout << "Already Done Model::iniSolve !\n";
    return true;
  }
  // Write to log file
  dynelaData->logFile.separatorWrite("DynELA Solver Initialization phase");

  // Log initialization of the model
  dynelaData->logFile << "\nInitializing model : " << name << "\n";

  // If the list of element is void, nothing to do and return false
  if (elements.getSize() == 0)
    return false;

  // Set the dimension of the model
  _numberOfDimensions = elements(0)->getNumberOfDimensions();
  dynelaData->logFile << "Grid topology set to " << (elements(0)->getFamily() == Element::Bidimensional ? "2D" : elements(0)->getFamily() == Element::Axisymetric ? "2D Axi"
                                                                                                                                                                  : "3D")
                      << "\n";

  // Check the topology of the model
  if (checkTopology())
    dynelaData->logFile << "Topology of the Model is OK\n";

  // verification des elements par methode interne
  dynelaData->logFile << "Verification of elements ... ";
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    elements(elementId)->check();
  }
  dynelaData->logFile << "Ok\n";

  // Write informations in the log file
  dynelaData->logFile << "Model contains " << elements.getSize() << " elements and " << nodes.getSize() << " nodes\n";

  // Initialize data
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    // recuperation de l'element
    Element *pel = elements(elementId);
#ifdef VERIF_assert
    assert(pel != NULL);
#endif

    // Initialize data for the element
    pel->initializeData();
  }
  dynelaData->logFile << elements.getSize() << " elements have been initialized\n";

  // Saving the initial coordinates
  /*
  dynelaData->logFile << "Saving initial coordinates ... ";
  for (int nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    nodes(nodeId)->initialCoordinates = nodes(nodeId)->coordinates;
  }
  dynelaData->logFile << "ok\n";
 */

  // application des conditions initiales
  dynelaData->logFile << "Applying initial conditions ... ";
  for (int nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    if (nodes(nodeId)->boundary != NULL)
      nodes(nodeId)->boundary->applyInitial(nodes(nodeId), 0, 0);
  }
  dynelaData->logFile << "Ok\n";

  // Compact nodes and elements list
  compactNodesAndElements();
  /*

 // verification des interfaces
  for (i = 0; i < interfaces.getSize(); i++)
  {
    dynelaData->logFile << "Interface " << i << " verification ...\n";
    interfaces(i)->Init();
    interfaces(i)->check();
  }
*/

  // Get last time for all the solvers
  currentTime = solver->startTime;

  /*   double finalTimeOfPreviousSolver = 0.0;
  if (solvers.getSize() > 0)
  {
    // get the init time of first solver
    currentTime = solvers(0)->startTime;
    // get the end time of first solver
    finalTimeOfPreviousSolver = solvers(0)->endTime;
  }
 */

  // Reverse link of the solver to this model
  solver->setModel(this);

  // init the solver
  dynelaData->logFile << "Solver initialization ...\n";
  solver->initialize();

  // Loop over all the solvers of the Model
  /*   for (int solverId = 0; solverId < solvers.getSize(); solverId++)
  {
    // get the solver
    solver = solvers(solverId);

    // Reverse link of the solver to this model
    solver->setModel(this);

    // init the solver
    dynelaData->logFile << "Solver " << solverId << " initialization ...\n";
    solver->initialize();

    // If there is at least two solvers
    if (solverId > 0)
    {
      if (finalTimeOfPreviousSolver != solver->startTime)
      {
        fatalError("Solvers chain error found", "Solver %d has %lf start-time while previous solver end at %lf",
                   solverId, solver->startTime, finalTimeOfPreviousSolver);
      }
      finalTimeOfPreviousSolver = solver->endTime;
    }
  }

  // ajout des motions Lagrangienes pour tous les noeuds non definis
  set = 0;
  //  for (i=0; i<grids.getSize();i++)
  for (j = 0; j < nodes.getSize(); j++)
  {
    if (nodes(j)->motion == NULL)
    {
      lagMotion *lag = new lagMotion;
      lag->attachNode(nodes(j));
      nodes(j)->attachNodeMotion(lag);
      set++;
    }
  }
  cout << set << " Lagrangian nodes created\n";


   for (long elementId=0;elementId<elements.getSize();elementId++)
 {
    elements(elementId)->computeJacobian(true);
  }

 */

  // Dispatch elements to cores
  dynelaData->parallel.dispatchElements(elements);

  // Remember that the initSolve has been done
  _initSolveDone = true;

  return (true);
}

//-----------------------------------------------------------------------------
void Model::compactNodesAndElements()
//-----------------------------------------------------------------------------
{
  // Compact nodes list
  dynelaData->logFile << "Compact nodes list ...\n";
  nodes.compact();

  // Compact elements list
  dynelaData->logFile << "Compact elements list ...\n";
  elements.compact();
}

//-----------------------------------------------------------------------------
void Model::computeMassMatrix(bool forceComputation)
//-----------------------------------------------------------------------------
{
  // If already computed and not forced to be recomputed, then return
  if (_massMatrixComputed && !forceComputation)
    return;

  // Just to check but have to be removed !!!
  if (_massMatrixComputed)
    fatalError("Model::computeMassMatrix(bool forceComputation) called twice");

  // local variables
  long globalNodeNumber;
  MatrixDiag elementMassMatrix;
  Element *element;

  // Compute size of the Mass matrix ie, the numer of dimensions times number of nodes
  long numberOfDDL = _numberOfDimensions * nodes.getSize();
  massMatrix.redim(numberOfDDL);

  // Initialize the Mass matrix
  massMatrix = 0.0;

  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    // Get the current element
    element = elements(elementId);

    // Number of nodes of the element
    int numberOfNodes = element->getNumberOfNodes();

    // Redim the local Mass matrix
    elementMassMatrix.redim(numberOfNodes);

    // Computes the local mass matrix of the element
    element->computeMassMatrix(elementMassMatrix);

    // Assembly phase for the global mass matrix
    for (short nodeId = 0; nodeId < numberOfNodes; nodeId++)
    {
      // Get the real node number
      globalNodeNumber = element->nodes(nodeId)->internalNumber();

      // Assembly of the mass matrix
      for (short dim = 0; dim < _numberOfDimensions; dim++)
        massMatrix(globalNodeNumber * _numberOfDimensions + dim) += elementMassMatrix(nodeId);
    }
  }

  // Redistribution of nodal masses to nodes
  for (long nodeId = 0; nodeId < nodes.getSize(); nodeId++)
    nodes(nodeId)->mass = massMatrix(nodes(nodeId)->internalNumber() * _numberOfDimensions);

  // Mass matrix has been computed, remember it !
  _massMatrixComputed = true;
}

//-----------------------------------------------------------------------------
double Model::getTotalMass()
//-----------------------------------------------------------------------------
{
  // Initialize the total mass
  double totalMass = 0.0;

  // Loop over all nodes of the model and sum of nodal masses
  for (long nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    totalMass += nodes(nodeId)->mass;
  }

  // return the value
  return (totalMass);
}

//-----------------------------------------------------------------------------
double Model::getTotalKineticEnergy()
//-----------------------------------------------------------------------------
{
  // Initialize the kinetic energy
  double kineticEnergy = 0.0;

  // Loop over all nodes of the model and sum of kinetic energies of nodes
  for (long nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    kineticEnergy += (nodes(nodeId)->mass * nodes(nodeId)->currentField->speed.innerProduct()) / 2.0;
  }

  // return the value
  return (kineticEnergy);
}

// Calcul du time step de minimal de la grille (Courant)
/*
  Cette methode calcule le time step minimal de la grille à partir de la definition de la geometrie des elements et de la vitesse de propagation du son dans les elements de la structure. Cette relation est basee sur le critere de stabilite de Courant.
  La relation utilisee pour ce calcul est donnee par:
  \f[ \Delta t = \Delta t_{crit} \f] avec \f[ \Delta t_{crit}=\frac{l}{C_s} \f] calcule pour tous les elements de la grille dans laquelle \f$ l \f$ est la longueur caracteristique de l'element calculee par Element::getCharacteristicLength() et \f$ C_s \f$ est la vitesse de propagation d'une onde longitudinale dans l'element calculee par la methode Element::getElongationWaveSpeed().
  Return : valeur numerique de la valeur du timeStep critique de la grille
*/
//-----------------------------------------------------------------------------
double Model::computeCourantTimeStep()
//-----------------------------------------------------------------------------
{
  double characteristicLength;
  double criticalTimeStep;
  double elongationWaveSpeed;
  double timeStep;

  // initialisation de la longueur caracteristique de l'element 0
  characteristicLength = elements(0)->getCharacteristicLength();

  // vitesse du son dans l'element 0
  elongationWaveSpeed = elements(0)->getElongationWaveSpeed();

  // valeur critique du timeStep step
  criticalTimeStep = characteristicLength / elongationWaveSpeed;

  for (long elementId = 1; elementId < elements.getSize(); elementId++)
  {
    // longueur caracteristique de l'element
    characteristicLength = elements(elementId)->getCharacteristicLength();

    // vitesse du son dans l'element
    elongationWaveSpeed = elements(elementId)->getElongationWaveSpeed();

    // valeur critique
    timeStep = characteristicLength / elongationWaveSpeed;

    // minimum des valeurs
    if (timeStep < criticalTimeStep)
      criticalTimeStep = timeStep;
  }
  return criticalTimeStep;
}

//-----------------------------------------------------------------------------
void Model::add(Solver *newSolver)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(newSolver != NULL);
#endif

  // Add solver to the model
  // solvers << newSolver;
  solver = newSolver;

  // logFile
  dynelaData->logFile << "Solver " << newSolver->name << " added to model " << name << "\n";
}

//-----------------------------------------------------------------------------
double Model::getEndSolveTime()
//-----------------------------------------------------------------------------
{
  // return solvers.last()->endTime;
  return solver->endTime;
}

//-----------------------------------------------------------------------------
bool Model::solve(double solveUpToTime)
//-----------------------------------------------------------------------------
{
  // If solveUpToTime is not defined, solve to the end in one step
  if (solveUpToTime < 0)
  {
    // end time is the one of the last solver
    // solveUpToTime = solvers.last()->endTime;
    solveUpToTime = solver->endTime;
  }

  // Print message
  printf("Run solver to %lf s - %4.1f %%\n", solveUpToTime, 100 * (currentTime / solver->endTime));

  // check to see if time is in bounds
  if (currentTime >= solveUpToTime)
  {
    dynelaData->logFile << "Current time greater than up time " << currentTime << " > " << solveUpToTime << "\n";

    // End of computation and return
    return false;
  }

  // Log the Solver into log file
  dynelaData->logFile << "Run solver: \n";

  // Yeah, run the solver !!
  solver->solve(solveUpToTime);

  return true;
}

// Calcule le determinant du Jacobien de tous les elements de la grille
/*
  Cette methode calcule le Jacobien de tous les elements de la grille.
*/
/*//-----------------------------------------------------------------------------
void Model::computeJacobian()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "Compute Jacobian\n";
#endif

  Element *pel = elements.first();
  while ((pel = elements.currentUp()) != NULL)
  {
    if (pel->computeJacobian() == false)
    {
      std::cerr << "Emergency save of the last result\n";
      std::cerr << "Program aborted\n";
      dynelaData->writeVTKFile();
      exit(-1);
    }
  }
}*/

//-----------------------------------------------------------------------------
void Model::computeJacobian(bool reference)
//-----------------------------------------------------------------------------
{
#pragma omp parallel
  {
    ElementsChunk *chunk = dynelaData->parallel.getElementsOfCurrentCore();

    Element *pel = chunk->elements.initLoop();
    while ((pel = chunk->elements.currentUp()) != NULL)
    {
      if (pel->computeJacobian(reference) == false)
      {
        std::cerr << "Emergency save of the last result\n";
        std::cerr << "Program aborted\n";
        dynelaData->writeVTKFile();
        exit(-1);
      }
    }
    chunk->elements.endLoop();
  }
}

//-----------------------------------------------------------------------------
void Model::computeUnderJacobian(bool reference)
//-----------------------------------------------------------------------------
{
#pragma omp parallel
  {
    ElementsChunk *chunk = dynelaData->parallel.getElementsOfCurrentCore();

    Element *pel = chunk->elements.initLoop();
    while ((pel = chunk->elements.currentUp()) != NULL)
    {
      if (pel->computeUnderJacobian(reference) == false)
      {
        std::cerr << "Emergency save of the last result\n";
        std::cerr << "Program aborted\n";
        dynelaData->writeVTKFile();
        exit(-1);
      }
    }
    chunk->elements.endLoop();
  }
}

//-----------------------------------------------------------------------------
void Model::computeStrains()
//-----------------------------------------------------------------------------
{
#pragma omp parallel
  {
    ElementsChunk *chunk = dynelaData->parallel.getElementsOfCurrentCore();

    Element *pel = chunk->elements.initLoop();
    while ((pel = chunk->elements.currentUp()) != NULL)
    {
      pel->computeStrains();
    }
    chunk->elements.endLoop();
  }
}

/* //-----------------------------------------------------------------------------
void Model::computeStrains()
//-----------------------------------------------------------------------------
{
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    elements(elementId)->computeStrains();
  }
}
 */
//-----------------------------------------------------------------------------
void Model::computePressure()
//-----------------------------------------------------------------------------
{
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    elements(elementId)->computePressure();
  }
}

//-----------------------------------------------------------------------------
void Model::computeStress(double timeStep)
//-----------------------------------------------------------------------------
{
  if (_stressIntegrationMethod == StressIntNR)
    for (long elementId = 0; elementId < elements.getSize(); elementId++)
    {
      elements(elementId)->computeStress(timeStep);
    }

  if (_stressIntegrationMethod == StressIntDirect)
    for (long elementId = 0; elementId < elements.getSize(); elementId++)
    {
      elements(elementId)->computeStressDirect(timeStep);
    }
}

//-----------------------------------------------------------------------------
void Model::computeFinalRotation()
//-----------------------------------------------------------------------------
{
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    elements(elementId)->computeFinalRotation();
  }
}

//-----------------------------------------------------------------------------
void Model::computeInternalForces()
//-----------------------------------------------------------------------------
{
  Vector elementInternalForces;
  long glob;

  long numberOfDDL = _numberOfDimensions * nodes.getSize();

#ifdef PRINT_Execution_Solve
  cout << "Compute Internal Forces\n";
#endif

  // RAZ of internal forces vector
  internalForces.redim(numberOfDDL);
  internalForces = 0.0;

  // calcul des forces internes
  for (long elementId = 0; elementId < elements.getSize(); elementId++)
  {
    // calcul des forces internes de l'element
    elements(elementId)->computeInternalForces(elementInternalForces, solver->timeStep);

    // assemblage des forces internes
    for (long nodeId = 0; nodeId < elements(elementId)->nodes.getSize(); nodeId++)
    {
      // recuperation du numero global
      glob = (elements(elementId)->nodes(nodeId)->internalNumber()) * _numberOfDimensions;

      for (int dim = 0; dim < _numberOfDimensions; dim++)
      {
        // assemblage de F
        internalForces(glob + dim) += elementInternalForces(nodeId * _numberOfDimensions + dim);
      }
    }
  }
}

//-----------------------------------------------------------------------------
void Model::transfertQuantities()
//-----------------------------------------------------------------------------
{

#ifdef PRINT_Execution_Solve
  cout << "Quantities transfert\n";
#endif

  for (long nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    nodes(nodeId)->swapNodalFields();
  }
}

//-----------------------------------------------------------------------------
void Model::writeHistoryFiles()
//-----------------------------------------------------------------------------
{
  for (short historyId = 0; historyId < historyFiles.getSize(); historyId++)
  {
    historyFiles(historyId)->save(currentTime);
  }
}

//-----------------------------------------------------------------------------
void Model::add(HistoryFile *newHistoryFile)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(newHistoryFile != NULL);
#endif

  // ajouter l'history file au modele courant
  historyFiles << newHistoryFile;

  // logFile
  dynelaData->logFile << "HistoryFile " << newHistoryFile->name << " linked to current model\n";
}

//-----------------------------------------------------------------------------
double Model::computePowerIterationTimeStep(bool underIntegration)
//-----------------------------------------------------------------------------
{
  bool ok = false;
  double convergence;
  double fmax = 0.0;
  Element *pel;
  long iteration = 0;
  long localIndices[maxNumberOfNodes];
  Vector localValues;
  Vector powerIterationEV0;

  // matrices globales
  long numberOfDDL = _numberOfDimensions * nodes.getSize();

  pel = elements.initLoop();
  while ((pel = elements.currentUp()) != NULL)
  {
    // recuperation de l'element
    pel->computeElasticStiffnessMatrix(underIntegration);
  }
  elements.endLoop();

  // initialisation du vecteur si besoin
  if ((_powerIterationFreqMax == 0) || (_powerIterationEV.getSize() != numberOfDDL))
  {
    _powerIterationEV.redim(numberOfDDL);
    _powerIterationEV(0) = 1.0;
    for (long i = 1; i < numberOfDDL; i++)
      _powerIterationEV(i) = _powerIterationEV(i - 1) - 2. / (numberOfDDL - 1);
  }

  while (!ok)
  {
    iteration++;
    powerIterationEV0 = _powerIterationEV;
    pel = elements.initLoop();
    while ((pel = elements.currentUp()) != NULL)
    {
      localValues.redim(pel->stiffnessMatrix.rows());
      localValues = 0.;
      for (short I = 0; I < pel->nodes.getSize(); I++)
        localIndices[I] = pel->nodes(I)->internalNumber();
      localValues.scatterFrom(powerIterationEV0, localIndices, _numberOfDimensions);
      localValues = pel->stiffnessMatrix * localValues;
      _powerIterationEV.gatherFrom(localValues, localIndices, _numberOfDimensions);
    }
    elements.endLoop();

    massMatrix.divideBy(_powerIterationEV);
    fmax = _powerIterationEV.maxAbsoluteValue();
    _powerIterationEV /= fmax;
    convergence = dnlAbs(fmax - _powerIterationFreqMax) / fmax;

    // verifier si on a converge
    if (convergence < _powerIterationPrecision)
      ok = true;

    _powerIterationFreqMax = fmax;

    if (iteration > _powerIterationMaxIterations)
    {
      fatalError("Model::computePowerIterationTimeStep", "power iteration method not converged %10.4E\n", convergence);
    }
  }

  return sqrt(fmax);
}
/*
//-----------------------------------------------------------------------------
void Model::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  //    os << "numDoF n="<<NumDOFS<<endl;
  os << "Nodes list" << endl;
  //  (nodes).print (os);
  os << "Elements list\n";
  //  (elements).print (os);
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Model &model)
//-----------------------------------------------------------------------------
{
  model.print(os);
  return os;
}

//-----------------------------------------------------------------------------
double Model::getReadTimeData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  double tim;

  if (dynelaData->checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&tim, sizeof(double));

  return tim;
}

//-----------------------------------------------------------------------------
void Model::readData(ifstream &pfile)
//-----------------------------------------------------------------------------
{
  long i;

  if (dynelaData->checkBinaryVersion(pfile, 0) != Ok)
    fatalError("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read((char *)&currentTime, sizeof(double));

  for (i = 0; i < nodes.getSize(); i++)
  {
    pfile >> *(nodes(i));
  }

  if (dynelaData->checkBinaryVersion(pfile, 1) != Ok)
    fatalError("nodal datas", "Read error");

  for (i = 0; i < elements.getSize(); i++)
  {
    pfile >> *(elements(i));
  }

  if (dynelaData->checkBinaryVersion(pfile, 1) != Ok)
    fatalError("element datas", "Read error");
}

//-----------------------------------------------------------------------------
void Model::writeData(ofstream &pfile)
//-----------------------------------------------------------------------------
{
  long i;

  dynelaData->checkBinaryVersionWrite(pfile, 0);

  // save current time
  pfile.write((char *)&currentTime , sizeof(double));

  // save the nodal values
  for (i = 0; i < nodes.getSize(); i++)
  {
    nodes(i)->write(pfile);
  }

  dynelaData->checkBinaryVersionWrite(pfile, 1);

  // save the element values
  for (i = 0; i < elements.getSize(); i++)
  {
    elements(i)->write(pfile);
  }
  dynelaData->checkBinaryVersionWrite(pfile, 1);
}

//-----------------------------------------------------------------------------
void Model::getGlobalBox(Vec3D &min, Vec3D &max)
//-----------------------------------------------------------------------------
{
  Vec3D coordinates;
  long i, j;
  // affectation par defaut au commencement
  max = min = nodes(0)->coordinates;

  // boucle de recherche
  for (i = 1; i < nodes.getSize(); i++)
  {
    coordinates = nodes(i)->coordinates;

    for (j = 0; j < 3; j++)
    {
      if (coordinates(j) < min(j))
        min(j) = coordinates(j);
      if (coordinates(j) > max(j))
        max(j) = coordinates(j);
    }
  }
}

//-----------------------------------------------------------------------------
void Model::createNode(long num, double x, double y, double z)
//-----------------------------------------------------------------------------
{
  Node *pnd;

  if ((pnd = (nodes).AppN(num)) == NULL)
  {
    // creer un nouveau noeud
    pnd = new Node(num);

    // l'ajouter au modele
    nodes << pnd;
  }
  else
  {
    //      cerr << "Warning: node " << num << " redefined line " << scanned_line <<
    //	endl;
  }

  // chargement des coordonnees
  pnd->coordinates(0) = x;
  pnd->coordinates(1) = y;
  pnd->coordinates(2) = z;

  //  fprintf (out_file, "node %ld %f,%f,%f", num, x, y, z);

  // ajout des references online
  //   if (online_node_set == true)
  //     {
  //       selection.nodesSet (selection.nodesSet.getSize () - 1)->Add (pnd);
  //       fprintf (out_file, " of set %s",
  // 	       selection.nodesSet (selection.nodesSet.getSize () - 1)->nom.chars ());
  //     }

  // affichage du defilement
  //   if (num % 1000 == 0)
  //     {
  //       printf (".");
  //       fflush (stdout);
  //     }

  //   fprintf (out_file, "\n");
}

//-----------------------------------------------------------------------------
Node *Model::getNodeByNumber(long num)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < nodes.getSize(); i++)
  {
    if (nodes(i)->number == num)
      return nodes(i);
  }

  return NULL;
}

//-----------------------------------------------------------------------------
double Model::getCurrentTime()
//-----------------------------------------------------------------------------
{
  return currentTime;
}

//-----------------------------------------------------------------------------
void Model::addInterface(Interface *inter)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(inter != NULL);
#endif

  interfaces << inter;
}

//-----------------------------------------------------------------------------
void Model::starterWrite(String name)
//-----------------------------------------------------------------------------
{
  /*  String output_filename;
  FILE *pfile;
  long i;

  output_filename = name;
  pfile = fopen (output_filename.chars (), "w");

  // nodes write
  fprintf (pfile, "\n                 NODES DEFINITION\n");
  fprintf (pfile, "------------------------------------------------\n");
  fprintf (pfile, "  NODE               NODE COORDINATES\n");
  fprintf (pfile, " NUMBER        X            Y            Z\n");
  for (i = 0; i < nodes.getSize (); i++)
    {
      nodes (i)->toFile (pfile);
    }

  fprintf (pfile, "\n                 ELEMENTS DEFINITION\n");
  fprintf (pfile, "------------------------------------------------\n");
  fprintf (pfile, " ELEMENT   ELEMENT           LIST OF NODES\n");
  fprintf (pfile, " NUMBER     NAME   \n");
  for (i = 0; i < elements.getSize (); i++)
    {
      elements (i)->toFile (pfile);
    }

  fprintf (pfile, "\n                 MATERIALS DEFINITION\n");
  fprintf (pfile, "------------------------------------------------\n");
  for (i = 0; i < materials.getSize (); i++)
    {
      materials (i)->toFile (pfile);
    }

  fprintf (pfile, "\n                                 BOUNDARY CONDITIONS\n");
  fprintf (pfile,
     "------------------------------------------------------------------------------------\n");
  fprintf (pfile,
     "  NODE              MATERIAL SPEEDS                       GRID SPEEDS\n");
  fprintf (pfile,
     " NUMBER        X         Y         Z          X         Y         Z\n");
  for (i = 0; i < nodes.getSize (); i++)
    {
      nodes (i)->toFileBound (pfile);
    }
  fprintf (pfile, "\n                 CONTACT SURFACES\n");
  fprintf (pfile, "------------------------------------------------\n");
  for (i = 0; i < interfaces.getSize (); i++)
    {
      interfaces (i)->toFile (pfile);
    }

  fprintf (pfile, "\n                 TIMES\n");
  fprintf (pfile, "------------------------------------------------\n");
  times.toFile (pfile);

  fclose (pfile);

  cout << "End of write ...\n";*/
//}

// Crée un element et l'ajoute à la grille courante
/*
  Cette methode cree un element et l'ajoute à la grille courante.

  Elle cree les noeuds de l'element en verifiant que ces noeuds sont bien presents sur la grille courante. Dans le cas contraire, une erreur est generee. Le numero de l'element doit avoir ete cree avec l'element. Cette methode ajoute alors les noeuds en fonction des noeuds presents sur la grille.

  - pel pointeur sur le nouvel element que l'on vient de generer.
  - nNodes liste des numeros de noeuds qui composent l'element


//-----------------------------------------------------------------------------
void Model::createElement(Element *pel, long *nNodes)
//-----------------------------------------------------------------------------
{
  Node *pnd;

  // ajout de l'element à la grille et verification de sa non presence
  add(pel);

  // add the reference to the grid of the element
  //  pel->grid=this;
  pel->model = this;

  // construction de l'element et de ses noeuds
  long nbOk = 0;

  for (long j = 0; j < pel->getNumberOfNodes(); j++)
  {
    if ((pnd = nodes.dichotomySearch(substractNodesNumber, nNodes[j])) != NULL)
    {
      pel->addNode(pnd);
      nNodes[j] = -1;
      nbOk++;
    }
  }

  if (nbOk != pel->getNumberOfNodes())
  {
    for (long j = 0; j < pel->getNumberOfNodes(); j++)
    {
      if (nNodes[j] != -1)
        cout << "Node " << nNodes[j] << " doesn't exist in this grid\n";
    }
    fatalError("Model::createElement", "some nodes not exist in this grid");
  }
}
*/
