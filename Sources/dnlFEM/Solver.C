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
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
*/

#include <Solver.h>
#include <dnlKernel.h>
#include <DynELA.h>
#include <Model.h>
/*
#include <DynELA.h>
//#include <Grid.h>
extern DynELA *dynelaData;
 */
/*!
  \class Solver Solver.C
  \brief Class
  \ingroup dnlKernel
*/
//default constructor of the class Solver
/*!
*/
//-----------------------------------------------------------------------------
Solver::Solver(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  // Load default parameters from config file
  dynelaData->settings->getValue("ComputeTimeStepFrequency", _computeTimeStepFrequency);
  dynelaData->settings->getValue("TimeStepSafetyFactor", _timeStepSafetyFactor);
  dynelaData->settings->getValue("TimeStepMethod", _timeStepMethod);
  dynelaData->settings->getValue("ReportFrequency", _reportFrequency);

  /*   timeStep=0.0;
//  timeStepFactor=1;
  currentIncrement=0.0;
  startIncrement=0.0;
  upTime=0.0;

  name="Generic solver";

  frequencyReports=10;

  lastInc=0.0;
 */
}

//copy constructor of the class Solver
/*!
*/
//-----------------------------------------------------------------------------
Solver::Solver(const Solver &X)
//-----------------------------------------------------------------------------
{
}

//destructor of the class Solver
/*!
*/
//-----------------------------------------------------------------------------
Solver::~Solver()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Solver::setTimes(double start_time, double end_time)
//-----------------------------------------------------------------------------
{
  if (start_time > end_time)
    fatalError("Solver::setTimes", "start_time is greater than stop_time\n");

  startTime = start_time;
  endTime = end_time;
}

//-----------------------------------------------------------------------------
void Solver::setModel(Model *newModel)
//-----------------------------------------------------------------------------
{
  model = newModel;
}

// teste que le temps actuel est inferieur à une valeur limite
/*!
Cette methode teste que le temps actuel est tel que:
- le temps actuel est inferieur au temps maximum autorise
- l'currentIncrement actuel est inferieur à l'currentIncrement maximal autorise (si cet currentIncrement maximal est different de zero)
\return true si on est entre les bornes
*/
//-----------------------------------------------------------------------------
bool Solver::timeIsBetweenBounds()
//-----------------------------------------------------------------------------
{
  //  if ((model->currentTime >= startTime) && (model->currentTime < dnlMin(endTime, upTime)))
  if ((model->currentTime >= startTime) && (model->currentTime < endTime))
    return true;

  return false;
}

//-----------------------------------------------------------------------------
bool Solver::timeIsBetweenBounds(double time)
//-----------------------------------------------------------------------------
{
  if ((startTime <= time) && (time < endTime))
    return true;

  return false;
}

//-----------------------------------------------------------------------------
bool Solver::timeAndIncrementsAreBetweenBounds()
//-----------------------------------------------------------------------------
{
  // test sur le temps
  if ((model->currentTime < startTime) && (model->currentTime > endTime))
    return (false);

  // test sur les increments
  if (endIncrement == 0)
    return (true);

  // deuxieme test sur les increments
  if ((currentIncrement >= startIncrement) && (currentIncrement <= endIncrement))
    return (true);

  // par defaut
  return (false);
}

//Permet de definir la valeur du facteur de securite applique dans l'integration explicite
/*!
  Cette methode permet de definir la valeur du coefficient de securite pour le calcul du pas de temps critique dans l'integration numerique explicite.
  \param safetyfactor valeur du facteur de securite
*/
//-----------------------------------------------------------------------------
void Solver::setTimeStepSafetyFactor(double safetyfactor)
//-----------------------------------------------------------------------------
{
  if (safetyfactor <= 0)
    fatalError("Solver::setTimeStepSafetyFactor", "negative or null safety factor value");

  // affect the value
  _timeStepSafetyFactor = safetyfactor;

  if (dynelaData != NULL)
  {
    dynelaData->logFile << "Solver : " << name << " safety factor set to " << _timeStepSafetyFactor << "\n";
  }
}

//-----------------------------------------------------------------------------
void Solver::setTimeStepMethod(short method)
//-----------------------------------------------------------------------------
{
  _timeStepMethod = method;

  if (dynelaData != NULL)
  {
    dynelaData->logFile << "Solver : " << name << " timestep method set to " << (long)_timeStepMethod << "\n";
  }
}

//Permet de definit la frequence (en nombre d'iterations) avec laquelle on refait le calcul du pas de temps critique
/*!
  Cette methode permet de definir la frequence (en nombre d'iterations) avec laquelle on refait le calcul du pas de temps critique. Ce calcul du pas de temps critique evolue faiblement d'un currentIncrement à l'autre, et on peut donc au travers de ce parametre definir la frequence à laquelle ce calcul est effectue.
  \param frequency de la frequence de calcul du pas de temps critique
*/
//-----------------------------------------------------------------------------
void Solver::setComputeTimeStepFrequency(int frequency)
//-----------------------------------------------------------------------------
{
  _computeTimeStepFrequency = frequency;

  if (dynelaData != NULL)
  {
    dynelaData->logFile << "Solver : " << name << " timestep frequency set to " << _computeTimeStepFrequency << "\n";
  }
}

//-----------------------------------------------------------------------------
void Solver::setIncrements(long start, long stop)
//-----------------------------------------------------------------------------
{
  startIncrement = start;
  endIncrement = stop;
}

//Calcul du time step de minimal du modele
/*!
  Cette methode calcule le time step minimal du modele en fonction de la grille courante. Cette methode fait appel à la methode Model::computePowerIterationTimeStep() pour l'evaluation numerique de la valeur du time step minimal. La relation utilisee pour ce calcul est donc donnee par l'une des equations ci-dessous selon la valeur definie par la methode setTimeStepMethod():
Si methode pulsation maxi
  \f[ \Delta t = \gamma_s \frac{\Omega_s}{f_{max}} \f]
Si critere de Courant:
  \f[ \Delta t = factor * \Delta t_{crit} \f]

  La valeur du timeStep n'est recalculee que si l'une des conditions ci-dessous est vraie:
  Le parametre forceComputation est true
  Le numero d'currentIncrement est un multiple entier de la valeur de _computeTimeStepFrequency
  La valeur de l'ancien currentIncrement de temps à ete modifiee par une routine externe
*/
//-----------------------------------------------------------------------------
void Solver::computeTimeStep(bool forceComputation)
//-----------------------------------------------------------------------------
{
  // teste si le numero d'increment correspond a la frequence de recalcul
  if (forceComputation || (currentIncrement % _computeTimeStepFrequency == 0))
  {
    double maximumFrequency = 0.0;
    switch (_timeStepMethod)
    {
    case Courant:
    {
      maximumFrequency = 2.0 / model->computeCourantTimeStep();
    }
    break;
    case PowerIteration:
    {
      maximumFrequency = model->computePowerIterationTimeStep(false);
    }
    break;
    case PowerIterationUnder:
    {
      maximumFrequency = model->computePowerIterationTimeStep(true);
    }
    break;
    default:
      fatalError("Solver::computeTimeStep", "Unknown method\n");
    }
    //std::cout << "FREQ =" << maximumFrequency << "\n";

    // Compute timeStep for the integration
    timeStep = _timeStepSafetyFactor * _omegaS / maximumFrequency;

    /*  // Remember previous values
    _previousTimeStepDate = _computedTimeStepDate;
    _previousTimeStep = _computedTimeStep;

    // set actual date
    _computedTimeStepDate = model->currentTime;
    _computedTimeStep = timeStep; */
  }

  // interpolation lineaire du timeStep si possible
  /*   else
  {
    if (_computedTimeStepDate != _previousTimeStepDate)
      timeStep = dnlLinearInterp(_previousTimeStep, _computedTimeStep, (model->currentTime - _previousTimeStepDate) / (_computedTimeStepDate - _previousTimeStepDate));
  }
 */
}

//-----------------------------------------------------------------------------
void Solver::initialize()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double Solver::getTimeStep()
//-----------------------------------------------------------------------------
{
  //return _computedTimeStep;
  return timeStep;
}

/*

//-----------------------------------------------------------------------------
void Solver::progressWrite()
//-----------------------------------------------------------------------------
{
  String st;
  double interval;

  interval=theclock.getInterval();

  if (interval<10) frequencyReports*=10;

  st = "." + parsedFileName.before(sourceFileExtension) + progressFileExtension;
  FILE *fprogress = fopen (st.chars (), "w");
  fprintf (fprogress, "%ld %10.7E %10.7E %10.7E %10.7E %ld\n",
	   currentIncrement,
	   model->currentTime,
	   timeStep,
	   endTime,
	   interval,
	   currentIncrement-lastInc);
  lastInc = currentIncrement;

  fclose (fprogress);
}
*/
