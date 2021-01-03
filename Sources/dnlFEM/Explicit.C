/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v.4.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2021                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <Explicit.h>
#include <Node.h>
#include <DynELA.h>
#include <Model.h>
#include <NodalField.h>
#include <BoundaryCondition.h>
#include <Material.h>

extern DynELA *dynelaData;

/*
  Default constructor of the Explicit class

  This method is the default constructor of the Explicit class.
  In practice, here we do nothing more than the necessary memory allocations. We define here default values for this explicit solver, for example \f$\rho_b=0.8182\f$ and \f$\gamma_s=0.9\f$.
*/
//-----------------------------------------------------------------------------
Explicit::Explicit(char *newName) : Solver(newName)
//-----------------------------------------------------------------------------
{
  _rho_b = defaultSpectralRadius;

  computeChungHulbertIntegrationParameters();

  /*   name="Explicit";

  // valeur par defaut du rayon spectral
//  _timeStepSafetyFactor = defaultSafetyFactor;

  // calcul des parametres initiaux du schema de Chung-Hulbert

  // par defaut, la frequence de time-step est continue
//  _computeTimeStepFrequency=defaultComputeTimeStepFrequency;

//  _computedTimeStep=0;
//  _computedTimeStepDate=0;
//  _previousTimeStepDate=0;
//  _previousTimeStep=0;

  timeStepMethod = PowerIteration;
 */
}

/*
  Copy constructor of the Explicit class

  \warning Copying is not provided for here, this method generates an execution error.
*/
//-----------------------------------------------------------------------------
Explicit::Explicit(const Explicit &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely Explicit" << &X << std::endl;
  exit(-1);
}

/*
  Default destructor of the Explicit class
*/
//-----------------------------------------------------------------------------
Explicit::~Explicit()
//-----------------------------------------------------------------------------
{
}

/*
  Calculation of the parameters of the Chung-Hulbert integration

  This method computes the values of the Chung-Hulbert integration scheme parameters \f$\alpha_M\f$, \f$\beta\f$ and \f$\gamma\f$.
  The parameter \f$\alpha_M\f$ is defined from the value of \f$\rho_b\f$ by the following relationship: \f[\alpha_M=\frac{2\rho_b-1}{1+\rho_b}\f]
  The parameter \f$\beta\f$ is defined from the value of \f$\rho_b\f$ by the following relationship: \f[\beta=\frac{5-3\rho_b}{(1+\rho_b)^2(2-\rho_b)}\f]
  The parameter \f$\gamma\f$ is defined from the value of \f$\alpha_M\f$ by the following relationship: \f[\gamma=\frac{3}{2}-\alpha_M\f]
  The parameter \f$\Omega_s\f$ is defined from the value of \f$\rho_b\f$ by the following relationship: \f[\Omega_s=\sqrt{\frac{12(1+\rho_b)^3(2-\rho_b)}{10+15\rho_b-\rho_b^2+\rho_b^3-\rho_b^4}}\f]
*/
//-----------------------------------------------------------------------------
void Explicit::computeChungHulbertIntegrationParameters()
//-----------------------------------------------------------------------------
{
  _alphaM = (2.0 * _rho_b - 1.0) / (1.0 + _rho_b);
  _beta = (5.0 - 3.0 * _rho_b) / ((1.0 + _rho_b) * (1.0 + _rho_b) * (2.0 - _rho_b));
  _gamma = 1.5 - _alphaM;
  _omegaS = sqrt((12.0 * pow(1.0 + _rho_b, 3) * (2.0 - _rho_b)) /
                 (10.0 + 15.0 * _rho_b - _rho_b * _rho_b + pow(_rho_b, 3) - pow(_rho_b, 4)));

  // report operation in log file
  if (dynelaData != NULL)
  {
    dynelaData->logFile << "Explicit : " << name << " dissipation factor set to " << _rho_b << "\n";
    dynelaData->logFile << "  alphaM = " << _alphaM << "\n";
    dynelaData->logFile << "  beta   = " << _beta << "\n";
    dynelaData->logFile << "  gamma  = " << _gamma << "\n";
    dynelaData->logFile << "  OmegaS = " << _omegaS << "\n";
  }
}

//Permet de definir l'amortissement du schema explicite
/*
  Cette methode permet de definir l'ammortissement numerique introduit dans le schema d'integration numerique explicite en modifiant la valeur du rayon spectral à la frequence de bifurcation. La valeur numerique doit etre comprise dans l'intervalle: \f$\rho_{b}\in[0.0:1.0]\f$. Cette methode fait appel à la methode computeIntegrationParameters() afin de remettre à jour les parametres en fonction du nouveau rayon spectral.
  - val valeur du rayon spectral
*/
//-----------------------------------------------------------------------------
void Explicit::setDissipation(double dissipation)
//-----------------------------------------------------------------------------
{
  if (dissipation < 0.0 || dissipation > 1.0)
    fatalError("Explicit::setDissipation", "dissipation value out of range [0.0, 1.0]");

  // affect the value
  _rho_b = dissipation;

  // recompute parameters
  computeChungHulbertIntegrationParameters();
}

//-----------------------------------------------------------------------------
void Explicit::solve(double solveUpToTime)
//-----------------------------------------------------------------------------
{
  // Verify that the model is existing
  assert(model != NULL);

  // Verify if we need to run the solver
  if (!timeIsBetweenBounds())
    return;

  // Write information to log file
  dynelaData->logFile << "Solve up to " << solveUpToTime << "\n";

  // Get the final time of computation
  _solveUpToTime = solveUpToTime;

  // Compute the Jacobian
  dynelaData->cpuTimes.timer("Jacobian")->start();
  model->computeJacobian(true);
  model->computeUnderJacobian(true);
  dynelaData->cpuTimes.timer("Jacobian")->stop();

  // Compute the Mass Matrix if not already computed
  model->computeMassMatrix();

  // Compute the Time Step enforcing computation
  dynelaData->cpuTimes.timer("TimeStep")->start();
  computeTimeStep(true);
  dynelaData->cpuTimes.timer("TimeStep")->stop();

  // Compute the Strains
  dynelaData->cpuTimes.timer("Strains")->start();
  model->computeStrains();
  dynelaData->cpuTimes.timer("Strains")->stop();

  // Compute pressure increment
  dynelaData->cpuTimes.timer("Pressure")->start();
  model->computePressure();
  dynelaData->cpuTimes.timer("Pressure")->stop();

  // calcul des contraintes au sein de l'element
  dynelaData->cpuTimes.timer("Stress")->start();
  model->computeStress(timeStep);
  dynelaData->cpuTimes.timer("Stress")->stop();

  // Use objectivity
  dynelaData->cpuTimes.timer("FinalRotation")->start();
  model->computeFinalRotation();
  dynelaData->cpuTimes.timer("FinalRotation")->stop();

  // Compute the Internal Forces
  dynelaData->cpuTimes.timer("InternalForces")->start();
  model->computeInternalForces();
  dynelaData->cpuTimes.timer("InternalForces")->stop();

  // Call of time History saves
  model->writeHistoryFiles();

  while (model->currentTime < _solveUpToTime)
  {
    // Update timestep and increment
    updateTimes();

    // Display advancement of the current solution
    if ((currentIncrement % _reportFrequency == 0) || (currentIncrement == 1))
    {
      printf("%s inc=%ld time=%8.4E timeStep=%8.4E\n", model->name.chars(), currentIncrement, model->currentTime, timeStep);

      // write the progress file
      //progressWrite();
    }

    // Predictor phase
    dynelaData->cpuTimes.timer("Predictor")->start();
    computePredictions();
    dynelaData->cpuTimes.timer("Predictor")->stop();

    // Compute the Strains
    dynelaData->cpuTimes.timer("Strains")->start();
    model->computeStrains();
    dynelaData->cpuTimes.timer("Strains")->stop();

    // Compute pressure increment
    dynelaData->cpuTimes.timer("Pressure")->start();
    model->computePressure();
    dynelaData->cpuTimes.timer("Pressure")->stop();

    // calcul des contraintes au sein de l'element
    dynelaData->cpuTimes.timer("Stress")->start();
    model->computeStress(timeStep);
    dynelaData->cpuTimes.timer("Stress")->stop();

    // Use objectivity
    dynelaData->cpuTimes.timer("FinalRotation")->start();
    model->computeFinalRotation();
    dynelaData->cpuTimes.timer("FinalRotation")->stop();

    // Compute the Internal Forces
    dynelaData->cpuTimes.timer("InternalForces")->start();
    model->computeInternalForces();
    dynelaData->cpuTimes.timer("InternalForces")->stop();

    // Solve the step
    dynelaData->cpuTimes.timer("ExplicitSolve")->start();
    explicitSolve();
    dynelaData->cpuTimes.timer("ExplicitSolve")->stop();

    dynelaData->cpuTimes.timer("Density")->start();
    computeDensity();
    dynelaData->cpuTimes.timer("Density")->stop();

    // End step
    endStep();

    if (model->currentTime < _solveUpToTime)
    {
      // Compute the Jacobian
      dynelaData->cpuTimes.timer("Jacobian")->start();
      model->computeJacobian();
      dynelaData->cpuTimes.timer("Jacobian")->stop();

      // calcul du pas de temps critique de la structure
      dynelaData->cpuTimes.timer("TimeStep")->start();
      computeTimeStep();
      dynelaData->cpuTimes.timer("TimeStep")->stop();
    }

    // Write the history files
    //model->writeHistoryFiles();
  }

  printf("%s inc=%ld time=%8.4E timeStep=%8.4E\n", model->name.chars(), currentIncrement, model->currentTime, timeStep);

  /*  bool runStep;

  // first we set the up-time
  upTime=solveUpToTime;

  // verification que l'on doit resoudre
  runStep=timeIsBetweenBounds ();
  if (!runStep) return;

  // calcul preliminaire des matrices internes
  // initialisation du step de calcul
#ifdef computeCpuTimes
  recordTimes.start("Internal_Matrices");
#endif
  model->computeJacobian ();
  // initialisation du step de calcul
#ifdef computeCpuTimes
  recordTimes.stop("Internal_Matrices");
#endif

  // calcul de la matrice de masse
#ifdef computeCpuTimes
  recordTimes.start("Mass_Matrices");
#endif
  model->computeMassMatrix ();
#ifdef computeCpuTimes
  recordTimes.stop("Mass_Matrices");
#endif

  // initial contact

  // calcul des forces internes
#ifdef computeCpuTimes
  recordTimes.start("Internal_Forces");
#endif
  model->computeInternalForces();
#ifdef computeCpuTimes
  recordTimes.stop("Internal_Forces");
#endif

  // calcul des forces d'inertie

  // calcul des forces d'inertie par une mise en rotation de la structure

  // calcul du pas de temps critique de la structure
#ifdef computeCpuTimes
  recordTimes.start("Time_Step");
#endif
  computeTimeStep(true);
#ifdef computeCpuTimes
  recordTimes.stop("Time_Step");
#endif

  // boucle sur le temps
  while(runStep)
    {
      // initialisation du step de calcul
#ifdef computeCpuTimes
      recordTimes.start("Init_Step");
#endif
      beginExplicitStep();
#ifdef computeCpuTimes
      recordTimes.stop("Init_Step");
#endif

      // affichage de l'increment courant tous les 100 increments
#ifndef PRINT_Execution_Solve
      if ((increment % frequencyReports == 0) || (increment == 1))
	{
#endif
	  printf ("%s inc=%ld time=%8.4E timeStep=%8.4E\n", model->name.chars(), increment,
		  model->getCurrentTime(), timeStep);

	  // write the progress file
	  progressWrite();
#ifndef PRINT_Execution_Solve
 	}
#endif

      // prediction des quantites
      // phase dite de prediction correspondant à l'algorithme explicite
#ifdef computeCpuTimes
      recordTimes.start("Predictions");
#endif
      computePredictions ();
#ifdef computeCpuTimes
      recordTimes.stop("Predictions");
#endif


      // detection du contact


      // initialisation de l'iteration de calcul
#ifdef computeCpuTimes
      recordTimes.start("Init_Iteration");
#endif
      initIteration();
#ifdef computeCpuTimes
      recordTimes.stop("Init_Iteration");
#endif

      // resolution implicite ou explicite de l'iteration
#ifdef computeCpuTimes
      recordTimes.start("Explicit_Solve");
#endif
      explicitSolve ();
#ifdef computeCpuTimes
      recordTimes.stop("Explicit_Solve");
#endif

      // fin du step
#ifdef computeCpuTimes
      recordTimes.start("End_Step");
#endif
      endStep();
#ifdef computeCpuTimes
      recordTimes.stop("End_Step");
#endif

      // test de fin de calcul
      if (!timeIsBetweenBounds())
	{
	  runStep=false;
	}

      // si on continue le calcul
      if (runStep)
	{
#ifdef computeCpuTimes
	  recordTimes.start("Internal_Matrices");
#endif
	  model->computeJacobian ();
	  // initialisation du step de calcul
#ifdef computeCpuTimes
	  recordTimes.stop("Internal_Matrices");
#endif

	  // calcul du pas de temps critique de la structure
#ifdef computeCpuTimes
	  recordTimes.start("Time_Step");
#endif
	  computeTimeStep();
#ifdef computeCpuTimes
	  recordTimes.stop("Time_Step");
#endif
	}
    }
 */
  // print the CPU times
  //  recordTimes.report("CPU-TIMES");
}

//-----------------------------------------------------------------------------
void Explicit::updateTimes()
//-----------------------------------------------------------------------------
{
  // verification de la projection du temps par rapport au temps final
  // pour ne pas depasser la bonne date
  if (model->currentTime + timeStep > _solveUpToTime)
    timeStep = _solveUpToTime - model->currentTime;

  // mise a jour du temps
  model->nextTime = model->currentTime + timeStep;

  // incrementation du nombre d'increments
  currentIncrement++;
}

//Phase de prediction des deplacements, vitesses et accelerations nodales
/*
  La prediction se fait au niveau des deplacements, vitesses et accelerations nodales à partir des relations suivantes:
  \f[\stackrel{\bullet\bullet}{x}_{n+1}=0\f]
  \f[\stackrel{\bullet}{x}_{n+1}=\stackrel{\bullet}{x}_{n}+\Delta t(1-\gamma)\stackrel{\bullet\bullet}{x}_{n}\f]
  \f[x_{n+1}=x_{n}+\Delta t\stackrel{\bullet}{x}_{n}+\Delta t^{2}\left(\frac{1}{2}-\beta\right)\stackrel{\bullet\bullet}{x}_{n}\f]
  On applique egalement le respect des conditions aux limites imposees au travers de la methode BoundaryCondition::applyConstantOnCurrentFields().
*/
//-----------------------------------------------------------------------------
void Explicit::computePredictions()
//-----------------------------------------------------------------------------
{
  Node *node;

#ifdef PRINT_Execution_Solve
  cout << "Predictions de displacement, speed et acceleration\n";
#endif

  // boucle sur les noeuds du modele
  for (long nodeId = 0; nodeId < model->nodes.getSize(); nodeId++)
  {
    // recuperation du noeud courant
    node = model->nodes(nodeId);

    // verification d'assertion
#ifdef VERIF_assert
    assert(node != NULL);
#endif

    // prediction du deplacement
    node->newField->displacement = timeStep * (node->currentField->speed + (0.5 - _beta) * timeStep * node->currentField->acceleration);
    //node->newField->displacement = node->currentField->displacement + node->newField->displacement;
    /*  node->newField->displacement = node->currentField->acceleration;
    node->newField->displacement *= timeStep * (0.5 - _beta);
    node->newField->displacement += node->currentField->speed;
    node->newField->displacement *= timeStep;
 */

    // prediction de la vitesse
    node->newField->speed = node->currentField->speed + (1.0 - _gamma) * timeStep * node->currentField->acceleration;
    /* node->newField->speed = node->currentField->acceleration;
    node->newField->speed *= timeStep * (1.0 - _gamma);
    node->newField->speed += node->currentField->speed; */

    // prediction de l'acceleration
    node->newField->acceleration = 0.0;

    // application des conditions aux limites imposees
    if (node->boundary != NULL)
      node->boundary->applyConstantOnNewFields(node, model->currentTime, timeStep);

    //  node->newField->displacement = node->currentField->displacement + node->newField->displacement;
  }
}

//Resolution explicite de l'increment
/*
  Cette methode effectue la resolution explicite de l'increment de temps. La methode explicite etant une methode directe, aucune iteration n'est necessaire ici, les quantites peuvent etre calculees directement en utilisante les relations suivantes:
*/
//-----------------------------------------------------------------------------
void Explicit::explicitSolve()
//-----------------------------------------------------------------------------
{
  Node *node;

#ifdef PRINT_Execution_Solve
  cout << "Resolution explicite du pas de temps\n";
#endif

  // phase de calcul du terme d'acceleration global
  //  $GLOBAL$ nd->newField->acceleration = $M^-1$ * (Fext - internalForces)
  // Vector accVector = model->massMatrix.getSolve(model->internalForces);
  model->massMatrix.solve(model->internalForces);

  int numberOfDimensions = model->getNumberOfDimensions();

  // update du champ des accelerations
  for (long nodeId = 0; nodeId < model->nodes.getSize(); nodeId++)
  {
    node = model->nodes(nodeId);

    // mise a jour des accelerations
    for (int dim = 0; dim < numberOfDimensions; dim++)
      node->newField->acceleration(dim) = model->internalForces(nodeId * numberOfDimensions + dim);

    // mise à jour de l'acceleration materielle
    node->newField->acceleration -= _alphaM * node->currentField->acceleration;
    node->newField->acceleration /= (1.0 - _alphaM);

    // mise à jour de la vitesse materielle
    node->newField->speed += _gamma * timeStep * node->newField->acceleration;

    // mise à jour du deplacement
    node->newField->displacement += _beta * dnlSquare(timeStep) * node->newField->acceleration;

    // application des conditions aux limites imposees
    if (node->boundary != NULL)
      node->boundary->applyConstantOnNewFields(node, model->currentTime, timeStep);

    // prise en compte du contact

    // prise en compte des conditions aux limites
    node->displacement += node->newField->displacement;

    // mise à jour de la position des noeuds
    node->coordinates += node->newField->displacement;
  }
}

//-----------------------------------------------------------------------------
void Explicit::endStep()
//-----------------------------------------------------------------------------
{
  // update du time
  model->currentTime += timeStep;

  // sauvegarde des history file
  model->writeHistoryFiles();

  dynelaData->writeResultFile();

  // swap des valeurs nodales
  model->transfertQuantities();
}

//-----------------------------------------------------------------------------
void Explicit::computeDensity()
//-----------------------------------------------------------------------------
{
  for (long elementId = 0; elementId < model->elements.getSize(); elementId++)
  {
    model->elements(elementId)->computeDensity();
  }
}

//Renvoie le parametre \f$\alpha_M\f$ de l'integration de Chung-Hulbert
/*
  Cette methode renvoie la valeur du parametre \f$\alpha_M\f$ pour le schema d'integration de Chung-Hulbert.
  \see computeIntegrationParameters()
  Return : valeur numerique de \f$\alpha_M\f$


//-----------------------------------------------------------------------------
double Explicit::getAlphaM()
//-----------------------------------------------------------------------------
{
  return alphaM;
}

//Renvoie le parametre \f$\beta\f$ de l'integration de Chung-Hulbert
/*
  Cette methode renvoie la valeur du parametre \f$\beta\f$ pour le schema d'integration de Chung-Hulbert.
  \see computeIntegrationParameters()
  Return : valeur numerique de \f$\beta\f$


//-----------------------------------------------------------------------------
double Explicit::getBeta()
//-----------------------------------------------------------------------------
{
  return beta;
}

//Renvoie le parametre \f$\_\f$ de l'integration de Chung-Hulbert
/*
  Cette methode renvoie la valeur du parametre \f$\gamma\f$ pour le schema d'integration de Chung-Hulbert.
  \see computeIntegrationParameters()
  Return : valeur numerique de \f$\gamma\f$


//-----------------------------------------------------------------------------
double Explicit::getGamma()
//-----------------------------------------------------------------------------
{
  return gamma;
}

//-----------------------------------------------------------------------------
void Explicit::initIteration ()
//-----------------------------------------------------------------------------
{
  // calcul des forces internes
  model->computeInternalForces();

  // calcul des forces externes
}
*/
