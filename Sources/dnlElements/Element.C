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
  \file Element.C
  Definition file for the Element class

  This file is the definition file for the Element class.

  \ingroup dnlElements
*/

#include <Element.h>
#include <Node.h>
#include <Field.h>

//-----------------------------------------------------------------------------
Element::Element(long elementNumber)
//-----------------------------------------------------------------------------
{
  // affectation du numero d'element
  //_listIndex = elementNumber;
  number = elementNumber;

  // affectation par defaut NULL sur les materiaux
  material = NULL;

  // affectation par defaut NULL sur les ref
  _integrationPoint = NULL;

  // affectation par defaut NULL sur les ref
  // underIntegrationPoint = NULL;

  // affectation par defaut edges
  //_elementData = NULL;
}

//-----------------------------------------------------------------------------
Element::Element(const Element &element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie de l'element" << element.number << std::endl;
}

//-----------------------------------------------------------------------------
Element::~Element()
//-----------------------------------------------------------------------------
{
  nodes.flush();
}

//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Element &element)
//-----------------------------------------------------------------------------
{
  element.write(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Element &element)
//-----------------------------------------------------------------------------
{
  element.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void Element::write(std::ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < integrationPoints.getSize(); i++)
  {
    integrationPoints(i)->write(pfile);
  }
}

//-----------------------------------------------------------------------------
Element &Element::read(std::ifstream &pfile)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < integrationPoints.getSize(); i++)
  {
    pfile >> *(integrationPoints(i));
  }

  return *this;
}

//-----------------------------------------------------------------------------
bool Element::operator==(const Element &element) const
//-----------------------------------------------------------------------------
{
  if (_listIndex != element._listIndex)
    return (false);
  if (nodes != element.nodes)
    return (false);
  return (true);
}

//-----------------------------------------------------------------------------
bool Element::operator!=(const Element &element) const
//-----------------------------------------------------------------------------
{
  return !(*this == element);
}

//-----------------------------------------------------------------------------
bool Element::operator>(const Element &element) const
//-----------------------------------------------------------------------------
{
  return (_listIndex > element._listIndex);
}

//-----------------------------------------------------------------------------
bool Element::operator<(const Element &element) const
//-----------------------------------------------------------------------------
{
  return (_listIndex < element._listIndex);
}

//-----------------------------------------------------------------------------
bool compareElementsNumber(Element *element1, Element *element2)
//-----------------------------------------------------------------------------
{
  return (element1->number > element2->number); // comparaison
}

//-----------------------------------------------------------------------------
long substractElementsNumber(Element *element1, const long number)
//-----------------------------------------------------------------------------
{
  return (element1->number - number); // comparaison
}

//-----------------------------------------------------------------------------
void Element::addNode(Node *nd)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  // verif de ce que l'on a passe a la fonction
  if (nd == NULL)
    fatalError("Element::addNode",
               "Tried to insert a non existing node in the element %d\n"
               "the corresponding node is a NULL pointer",
               number);
#endif

  // add the corresponding node in the element
  nodes << nd;
}

//-----------------------------------------------------------------------------
void Element::add(Material *newMaterial)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  // verif de la non nullite du pointeur passe
  assert(newMaterial != NULL);

  if (material != NULL)
  {
    if (newMaterial == material)
      std::cerr << "The two materials are the same\n";
    else
      std::cerr << "The two materials are the differents\n";
    fatalError("Element::attachMaterial",
               "This function have been called more than one time for the element %d.\n"
               "This is not allowed since you tried to attach two materials to the same element\n"
               "You must make a call to Element::detachMaterial before doing this\n",
               number);
  }
#endif

  material = newMaterial;
}

//-----------------------------------------------------------------------------
void Element::add(IntegrationPoint *field, short intPointId)
//-----------------------------------------------------------------------------
{
  // verifier la nature du champ donne en argument
#ifdef VERIF_assert
  if (field == NULL)
    fatalError("Element::add",
               "A NULL pointer has been passed to the function\n"
               "You are not allowed to insert a non existing integration point to the element %d",
               number);
#endif

  // ajout du point d'integration
  integrationPoints << field;

#ifdef VERIF_assert
  assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
  if (integrationPoints.getSize() > _elementData->numberOfIntegrationPoints)
    fatalError("Element::add",
               "integration point %d is out of bounds %d-%d\n", intPointId, 0, _elementData->numberOfIntegrationPoints);
#endif

  field->integrationPointData = &(_elementData->integrationPoint[intPointId]);
}

//-----------------------------------------------------------------------------
void Element::add(UnderIntegrationPoint *field, short intPointId)
//-----------------------------------------------------------------------------
{
  // verifier la nature du champ donne en argument
#ifdef VERIF_assert
  if (field == NULL)
    fatalError("Element::add",
               "A NULL pointer has been passed to the function\n"
               "You are not allowed to insert a non existing integration point to the element %d",
               number);
#endif

  // ajout du point d'integration
  underIntegrationPoints << field;

#ifdef VERIF_assert
  assert(_elementData != NULL);
#endif

#ifdef VERIF_bounds
  if (underIntegrationPoints.getSize() > _elementData->numberOfUnderIntegrationPoints)
    fatalError("Element::add",
               "underIntegration point %d is out of bounds %d-%d\n", intPointId, 0, _elementData->numberOfUnderIntegrationPoints);
#endif

  field->integrationPointData = &(_elementData->underIntegrationPoint[intPointId]);
}

//-----------------------------------------------------------------------------
void Element::createIntegrationPoints()
//-----------------------------------------------------------------------------
{
  for (short intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    IntegrationPoint *pintPt = new IntegrationPoint(getNumberOfDimensions(), getNumberOfNodes());
    add(pintPt, intPoint);
  }

  for (short intPoint = 0; intPoint < _elementData->numberOfUnderIntegrationPoints; intPoint++)
  {
    UnderIntegrationPoint *pintPt = new UnderIntegrationPoint(getNumberOfDimensions(), getNumberOfNodes());
    add(pintPt, intPoint);
  }
}

//-----------------------------------------------------------------------------
void Element::initializeData()
//-----------------------------------------------------------------------------
{
  Node *node;

#ifdef VERIF_assert
  assert(material != NULL);
#endif

  for (int nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    // Get the node
    node = nodes(nodeId);
#ifdef VERIF_assert
    assert(node != NULL);
#endif
  }

  for (int intPoint = 0; intPoint < integrationPoints.getSize(); intPoint++)
  {
    setCurrentIntegrationPoint(intPoint);
    _integrationPoint->temperature = material->initialTemperature;
    _integrationPoint->density = material->density;
  }

  /*   for (int intPoint = 0; intPoint < underIntegrationPoint.getSize(); intPoint++)
  {
    setCurrentUnderIntegrationPoint(intPoint);
    _underIntegrationPoint->temperature = material->initialTemperature;
    _underIntegrationPoint->density = material->density;
  } */
  // Compute Jacobian of the element
  // computeJacobian(true);

  /*   for (int intPoint = 0; intPoint < integrationPoints.getSize(); intPoint++)
  {
    setCurrentIntegrationPoint(intPoint);
    _integrationPoint->detJ0 = _integrationPoint->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      _integrationPoint->detJ0 = _integrationPoint->detJ * _integrationPoint->radius;
    }
  } */
}

//-----------------------------------------------------------------------------
void Element::computeMassMatrix(MatrixDiag &elementMassMatrix)
//-----------------------------------------------------------------------------
{
  double WxdJ;

#ifdef VERIF_maths
  // verifier la taille de la matrice elementMassMatrix
  if ((elementMassMatrix.rows() != _elementData->numberOfNodes) || (elementMassMatrix.columns() != _elementData->numberOfNodes))
  {
    std::cerr << "Error in Element::computeMassMatrix()\nincompatible getSize of matrices elementMassMatrix\n";
    std::cerr << "expected " << _elementData->numberOfNodes << "x" << _elementData->numberOfNodes << std::endl;
    std::cerr << "getting  " << elementMassMatrix.rows() << "x" << elementMassMatrix.columns() << std::endl;
    exit(-1);
  }
#endif

  // Initialize the Element Mass Matrix
  elementMassMatrix = 0.0;

  for (short intPoint = 0; intPoint < integrationPoints.getSize(); intPoint++)
  {
    // Get the current integration point
    setCurrentIntegrationPoint(intPoint);

    // Compute the numerical integration term
    WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      WxdJ *= dnl2PI * _integrationPoint->radius;
    }

    // Computes the Element Mass Matrix
    for (short nodeId = 0; nodeId < _elementData->numberOfNodes; nodeId++)
      elementMassMatrix(nodeId) += material->density * _elementData->integrationPoint[intPoint].shapeFunction(nodeId) * WxdJ;
  }
}

// Calcul du de la vitesse de propagation d'une onde.
//-----------------------------------------------------------------------------
double Element::getElongationWaveSpeed()
//-----------------------------------------------------------------------------
{
  double density = 0.0;
  double poissonRatio = material->poissonRatio;

  // Computes mean density of the element
  for (short intPoint = 0; intPoint < integrationPoints.getSize(); intPoint++)
  {
    // Sum of the densities over all integration points
    density += getIntegrationPoint(intPoint)->density;
  }
  density /= integrationPoints.getSize();

  // Get back the elongational wave speed of the material
  return sqrt((material->youngModulus * (1.0 - poissonRatio)) / (density * (1.0 + poissonRatio) * (1.0 - 2.0 * poissonRatio)));
}

//-----------------------------------------------------------------------------
void Element::computeInternalForces(Vector &InternalForce, double timeStep)
//-----------------------------------------------------------------------------
{
  long I;
  int i, j;
  double WxdJ;
  // double currentRadius;

  // redim du vecteur InternalForce
  InternalForce.redim(_elementData->numberOfNodes * _elementData->numberOfDimensions);
  InternalForce = 0.0;

  // Boucle sur les points d'integration
  for (short intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPoint);

    // calcul du terme d'integration numerique
    WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
    if (getFamily() == Element::Axisymetric)
    {
      //   currentRadius = getRadiusAtIntegrationPoint();
      WxdJ *= dnl2PI * _integrationPoint->radius;
    }

    // calcul des forces internes
    for (I = 0; I < _elementData->numberOfNodes; I++)
    {
      for (i = 0; i < _elementData->numberOfDimensions; i++)
      {
        for (j = 0; j < _elementData->numberOfDimensions; j++)
        {
          InternalForce(I * _elementData->numberOfDimensions + i) -=
              _integrationPoint->dShapeFunction(I, j) * _integrationPoint->Stress(j, i) * WxdJ;
        }
      }
      if (getFamily() == Element::Axisymetric)
        InternalForce(I * _elementData->numberOfDimensions) -=
            _elementData->integrationPoint[intPoint].shapeFunction(I) * _integrationPoint->Stress(2, 2) / _integrationPoint->radius * WxdJ;
    }
  }
}

//-----------------------------------------------------------------------------
void Element::computeStress(double timeStep)
//-----------------------------------------------------------------------------
{
  bool irun;
  double plasticStrain, plasticStrainRate, yield;
  double fun, dfun;
  double gammaInitial = 1e-8;
  double gammaMin, gammaMax, gamma, dgamma;
  double hard;
  double Snorm0, Snorm, Strial;
  double stressPower;
  double temperature0, temperature;
  double tolNR = 1e-8;
  double plWorkInc;
  int iBissection;
  int iterate;
  int itMax = 250;
  short intPoint;
  SymTensor2 StressOld;
  SymTensor2 DeviatoricStress;
  SymTensor2 Unity;

  // Init unity vector
  Unity.setToUnity();

  HardeningLaw *hardeningLaw = material->getHardeningLaw();

  double TwoG = material->getTwoShearModulus();
  double TwoG32 = dnlSqrt32 * TwoG;

  // Don't know for the moment if the density has to be the initial one of the current one
  double heatFrac = material->taylorQuinney / (material->density * material->heatCapacity);

  // Boucle sur les points d'integration
  for (intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // Select integration point
    setCurrentIntegrationPoint(intPoint);

    // Get back the Stress
    StressOld = _integrationPoint->Stress;
    DeviatoricStress = StressOld.getDeviator();

    // computation of Snorm0
    Snorm0 = DeviatoricStress.getNorm();

    // Trial Deviatoric stress
    DeviatoricStress += TwoG * _integrationPoint->StrainInc.getDeviator();

    // Computation of Snorm
    Snorm = DeviatoricStress.getNorm();

    // Computation of Strial
    Strial = dnlSqrt32 * Snorm;

    // Get back initial temperature
    temperature0 = _integrationPoint->temperature;
    temperature = temperature0;

    // Get back plasticStrain and plasticStrainRate
    plasticStrain = _integrationPoint->plasticStrain;
    plasticStrainRate = _integrationPoint->plasticStrainRate;

    // Initialize value of gamma
    gamma = 0.0;

    // Get back yield stress
    yield = _integrationPoint->yieldStress;
    // yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

    // If the yield is zero, compute the first yield stress thank's to the constitutive law using the initial default value of gamma
    if (yield == 0.0)
    {
      yield = hardeningLaw->getYieldStress(gammaInitial, gammaInitial / timeStep, temperature);
      //    yield = hardeningLaw->getYieldStress(0, 0, temperature);
      _integrationPoint->yieldStress = yield;
    }

    // Initialize the iterate counters
    iterate = 0;
    iBissection = 0;

    // Plasticity criterion test and begin of plastic corrector
    if (Strial > yield)
    {
      // Definition of gamma range
      gammaMin = 0.0;
      // gammaMax = Strial / TwoG32;
      gammaMax = (Strial - yield) / TwoG32;

      // Get back the previous value of gamma
      gamma = _integrationPoint->gamma;

      if (plasticStrain == 0.0)
        gamma = dnlSqrt32 * gammaInitial;

      // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      // initialize the loop
      irun = true;

      // Main loop of the Newton-Raphson procedure
      while (irun)
      {
        // Compute yield stress and hardening parameter
        yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

        // Compute the radial return equation for isotropic case
        fun = Strial - gamma * TwoG32 - yield;

        // Reduce the range of the solution depending the sign of fun
        if (fun < 0.0)
          gammaMax = gamma;
        else
          gammaMin = gamma;

        // Compute the hardening coefficient
        hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);

        // Compute derivative of the radial return equation
        dfun = TwoG32 + dnlSqrt23 * hard;

        // Increment of the gamma parameter
        dgamma = fun / dfun;

        // Increment on the gamma value for Newton-Raphson
        gamma += dgamma;

        // If the solution is outside of the brackets, do a bissection step
        if ((gammaMax - gamma) * (gamma - gammaMin) < 0.0)
        {
          dgamma = 0.5 * (gammaMax - gammaMin);
          gamma = gammaMin + dgamma;
          iBissection += 1;
        }

        // Algorithm converged, end of computations
        if (dnlAbs(dgamma) < tolNR)
        {
          irun = false;
        }
        else
        {
          // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
          plasticStrainRate = dnlSqrt23 * gamma / timeStep;
          plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
          temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

          // Increase the number of iterations
          iterate += 1;
          if (iterate > itMax)
          {
            printf("NO CONVERGENCE in Newton-Raphson\n");
            printf("After %d iterations\n", iterate);
            printf("Time %lf \n", timeStep);
            printf("Precision %lf\n", dnlAbs(fun / yield));
            printf("Strial %lf\n", Strial);
            printf("Gamma0 %lf\n", _integrationPoint->gamma);
            printf("Gamma %lf\n", gamma);
            printf("Gamma M %lf %lf\n", gammaMin, gammaMax);
            printf("DGamma %lf\n", dgamma);
            printf("epsp0 %lf\n", _integrationPoint->plasticStrain + dnlSqrt23 * _integrationPoint->gamma);
            printf("depsp0 %lf\n", dnlSqrt23 * _integrationPoint->gamma / timeStep);
            printf("plasticStrain %lf\n", plasticStrain);
            printf("plasticStrainRate %lf\n", plasticStrainRate);
            printf("temperature %lf\n", temperature);
            printf("old plasticStrain %lf\n", _integrationPoint->plasticStrain);
            printf("old plasticStrainRate %lf\n", _integrationPoint->plasticStrainRate);
            printf("old gamma %lf\n", _integrationPoint->gamma);
            printf("old yieldStress %lf\n", _integrationPoint->yieldStress);
            printf("old temperature %lf\n", _integrationPoint->temperature);
            fatalError("No convergence");
          }
        }
      }

      // Computation of the plastic strain increment
      _integrationPoint->PlasticStrain += gamma * DeviatoricStress / Snorm;

      // Compute the new stress corrector
      DeviatoricStress *= 1.0 - TwoG * gamma / Snorm;

      // Store new plasticStrain and plasticStrainRate
      _integrationPoint->plasticStrain += dnlSqrt23 * gamma;
      _integrationPoint->plasticStrainRate = dnlSqrt23 * gamma / timeStep;

      // Store the value of gamma for next loop
      _integrationPoint->gamma = gamma;
      _integrationPoint->gammaCumulate += gamma;

      // store the new yield stress of the material
      _integrationPoint->yieldStress = yield;
    }

    // Compute the final stress of the element
    _integrationPoint->Stress = DeviatoricStress + _integrationPoint->pressure * Unity;

    // Compute the new specific internal energy
    stressPower = 0.5 * _integrationPoint->StrainInc.doubleProduct(StressOld + _integrationPoint->Stress);
    _integrationPoint->internalEnergy += stressPower / material->density;

    // Get back the gamma value
    if (gamma != 0.0)
    {
      // Compute the plastic Work increment
      plWorkInc = 0.5 * gamma * (DeviatoricStress.getNorm() + Snorm0);

      // New dissipated inelastic specific energy
      _integrationPoint->inelasticEnergy += plWorkInc / material->density;
      _integrationPoint->temperature += heatFrac * plWorkInc;
    }
  }
}

//-----------------------------------------------------------------------------
void Element::computeStressDirect(double timeStep)
//-----------------------------------------------------------------------------
{
  double plasticStrain, plasticStrainRate, yield;
  double gammaInitial = 1e-8;
  double gamma;
  double hard;
  double Snorm0, Snorm, Strial;
  double stressPower;
  double temperature0, temperature;
  double plWorkInc;
  short intPoint;
  SymTensor2 StressOld;
  SymTensor2 DeviatoricStress;
  SymTensor2 Unity;

  // Init unity vector
  Unity.setToUnity();

  HardeningLaw *hardeningLaw = material->getHardeningLaw();

  double TwoG = material->getTwoShearModulus();
  double TwoG32 = dnlSqrt32 * TwoG;

  // Don't know for the moment if the density has to be the initial one of the current one
  double heatFrac = material->taylorQuinney / (material->density * material->heatCapacity);

  // Boucle sur les points d'integration
  for (intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // Select integration point
    setCurrentIntegrationPoint(intPoint);

    // Get back the Stress
    StressOld = _integrationPoint->Stress;
    DeviatoricStress = StressOld.getDeviator();

    // computation of Snorm0
    Snorm0 = DeviatoricStress.getNorm();

    // Trial Deviatoric stress
    DeviatoricStress += TwoG * _integrationPoint->StrainInc.getDeviator();

    // Computation of Snorm
    Snorm = DeviatoricStress.getNorm();

    // Computation of Strial
    Strial = dnlSqrt32 * Snorm;

    // Get back initial temperature
    temperature0 = _integrationPoint->temperature;
    temperature = temperature0;

    // Get back plasticStrain and plasticStrainRate
    plasticStrain = _integrationPoint->plasticStrain;
    plasticStrainRate = _integrationPoint->plasticStrainRate;

    // Initialize value of gamma
    gamma = 0.0;

    // Get back yield stress
    yield = _integrationPoint->yieldStress;
    // yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

    // If the yield is zero, compute the first yield stress thank's to the constitutive law using the initial default value of gamma
    if (yield == 0.0)
    {
      yield = hardeningLaw->getYieldStress(gammaInitial, gammaInitial / timeStep, temperature);
      //   yield = hardeningLaw->getYieldStress(0, 0, temperature);
      _integrationPoint->yieldStress = yield;
    }

    // // Initialize the iterate counters
    // iterate = 0;
    // iBissection = 0;

    // Plasticity criterion test and begin of plastic corrector
    if (Strial > yield)
    {
      if (plasticStrain > 0.0)
      {
        hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);
      }
      else
      {
        hard = hardeningLaw->getDerivateYieldStress(gammaInitial, plasticStrainRate, temperature, timeStep);
      }

      gamma = (Snorm - dnlSqrt23 * yield) / (TwoG * (1.0 + hard / (1.5 * TwoG)));
      // Definition of gamma range
      //  gammaMin = 0.0;
      // gammaMax = Strial / TwoG32;
      //  gammaMax = (Strial-yield) / TwoG32;

      // Get back the previous value of gamma
      // gamma = _integrationPoint->gamma;

      // if (plasticStrain == 0.0)
      //   gamma = dnlSqrt32 * gammaInitial;

      // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      // plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      // plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      // temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      // // initialize the loop
      // irun = true;

      // // Main loop of the Newton-Raphson procedure
      // while (irun)
      // {
      //   // Compute yield stress and hardening parameter
      //   yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

      //   // Compute the radial return equation for isotropic case
      //   fun = Strial - gamma * TwoG32 - yield;

      //   // Reduce the range of the solution depending the sign of fun
      //   if (fun < 0.0)
      //     gammaMax = gamma;
      //   else
      //     gammaMin = gamma;

      //   // Compute the hardening coefficient
      //   hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);

      //   // Compute derivative of the radial return equation
      //   dfun = TwoG32 + dnlSqrt23 * hard;

      //   // Increment of the gamma parameter
      //   dgamma = fun / dfun;

      //   // Increment on the gamma value for Newton-Raphson
      //   gamma += dgamma;

      //   // If the solution is outside of the brackets, do a bissection step
      //   if ((gammaMax - gamma) * (gamma - gammaMin) < 0.0)
      //   {
      //     dgamma = 0.5 * (gammaMax - gammaMin);
      //     gamma = gammaMin + dgamma;
      //     iBissection += 1;
      //   }

      //   // Algorithm converged, end of computations
      //   if (dnlAbs(dgamma) < tolNR)
      //   {
      //     irun = false;
      //   }
      //   else
      //   {
      //     // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      //     plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      //     plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      //     temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      //     // Increase the number of iterations
      //     iterate += 1;
      //     if (iterate > itMax)
      //     {
      //       printf("NO CONVERGENCE in Newton-Raphson\n");
      //       printf("After %d iterations\n", iterate);
      //       printf("Time %lf \n", timeStep);
      //       printf("Precision %lf\n", dnlAbs(fun / yield));
      //       printf("Strial %lf\n", Strial);
      //       printf("Gamma0 %lf\n", _integrationPoint->gamma);
      //       printf("Gamma %lf\n", gamma);
      //       printf("Gamma M %lf %lf\n", gammaMin, gammaMax);
      //       printf("DGamma %lf\n", dgamma);
      //       printf("epsp0 %lf\n", _integrationPoint->plasticStrain + dnlSqrt23 * _integrationPoint->gamma);
      //       printf("depsp0 %lf\n", dnlSqrt23 * _integrationPoint->gamma / timeStep);
      //       printf("plasticStrain %lf\n", plasticStrain);
      //       printf("plasticStrainRate %lf\n", plasticStrainRate);
      //       printf("temperature %lf\n", temperature);
      //       printf("old plasticStrain %lf\n", _integrationPoint->plasticStrain);
      //       printf("old plasticStrainRate %lf\n", _integrationPoint->plasticStrainRate);
      //       printf("old gamma %lf\n", _integrationPoint->gamma);
      //       printf("old yieldStress %lf\n", _integrationPoint->yieldStress);
      //       printf("old temperature %lf\n", _integrationPoint->temperature);
      //       fatalError("No convergence");
      //     }
      //   }
      // }

      plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

      // Computation of the plastic strain increment
      _integrationPoint->PlasticStrain += gamma * DeviatoricStress / Snorm;

      // Compute the new stress corrector
      DeviatoricStress *= 1.0 - TwoG * gamma / Snorm;

      // Store new plasticStrain and plasticStrainRate
      _integrationPoint->plasticStrain += dnlSqrt23 * gamma;
      _integrationPoint->plasticStrainRate = dnlSqrt23 * gamma / timeStep;

      // Store the value of gamma for next loop
      _integrationPoint->gamma = gamma;
      _integrationPoint->gammaCumulate += gamma;

      // store the new yield stress of the material
      _integrationPoint->yieldStress = yield;
    }

    // Compute the final stress of the element
    _integrationPoint->Stress = DeviatoricStress + _integrationPoint->pressure * Unity;

    // Compute the new specific internal energy
    stressPower = 0.5 * _integrationPoint->StrainInc.doubleProduct(StressOld + _integrationPoint->Stress);
    _integrationPoint->internalEnergy += stressPower / material->density;

    // Get back the gamma value
    if (gamma != 0.0)
    {
      // Compute the plastic Work increment
      plWorkInc = 0.5 * gamma * (DeviatoricStress.getNorm() + Snorm0);

      // New dissipated inelastic specific energy
      _integrationPoint->inelasticEnergy += plWorkInc / material->density;
      _integrationPoint->temperature += heatFrac * plWorkInc;
    }
  }
}

/* //-----------------------------------------------------------------------------
void Element::computeStressDirect(double timeStep)
//-----------------------------------------------------------------------------
{
  double plasticStrain, plasticStrainRate, yield;
  double gammaInitial = 1e-8;
  double gamma;
  double hard;
  double Snorm0, Snorm, Strial;
  double stressPower;
  double temperature0, temperature;
  double plWorkInc;
  short intPoint;
  SymTensor2 StressOld;
  SymTensor2 DeviatoricStress;
  SymTensor2 Unity;

  // Init unity vector
  Unity.setToUnity();

  HardeningLaw *hardeningLaw = material->getHardeningLaw();

  double TwoG = material->getTwoShearModulus();
  double TwoG32 = dnlSqrt32 * TwoG;

  // Boucle sur les points d'integration
  for (intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // Select integration point
    setCurrentIntegrationPoint(intPoint);

    // Compute the heat fraction
    double heatFrac = material->taylorQuinney / (_integrationPoint->density * material->heatCapacity);

    // Get back the Stress
    StressOld = _integrationPoint->Stress;
    DeviatoricStress = StressOld.getDeviator();

    // computation of Snorm0
    Snorm0 = DeviatoricStress.getNorm();

    // Trial Deviatoric stress
    DeviatoricStress += TwoG * _integrationPoint->StrainInc.getDeviator();

    // Computation of Snorm
    Snorm = DeviatoricStress.getNorm();

    // Computation of Strial
    Strial = dnlSqrt32 * Snorm;

    // Get back initial temperature
    temperature0 = _integrationPoint->temperature;
    temperature = temperature0;

    // Get back plasticStrain and plasticStrainRate
    plasticStrain = _integrationPoint->plasticStrain;
    plasticStrainRate = _integrationPoint->plasticStrainRate;

    // Initialize value of gamma
    gamma = 0.0;

    // Get back yield stress
    // yield = _integrationPoint->yieldStress;
    yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

    // If the yield is zero, compute the first yield stress thank's to the constitutive law using the initial default value of gamma
    //    if (yield == 0.0)
    // {
    //   //yield = hardeningLaw->getYieldStress(gammaInitial, gammaInitial / timeStep, temperature);
    //   yield = hardeningLaw->getYieldStress(0, 0, temperature);
    //   _integrationPoint->yieldStress = yield;
    // }

    // // Initialize the iterate counters
    // iterate = 0;
    // iBissection = 0;

    // Plasticity criterion test and begin of plastic corrector
    if (Strial > yield)
    {
      if (plasticStrain > 0.0)
      {
        hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);
      }
      else
      {
        hard = hardeningLaw->getDerivateYieldStress(gammaInitial, plasticStrainRate, temperature, timeStep);
      }

      gamma = (Snorm - dnlSqrt23 * yield) / (TwoG * (1.0 + hard / (1.5 * TwoG)));
      // Definition of gamma range
      //  gammaMin = 0.0;
      // gammaMax = Strial / TwoG32;
      //  gammaMax = (Strial-yield) / TwoG32;

      // Get back the previous value of gamma
      // gamma = _integrationPoint->gamma;

      // if (plasticStrain == 0.0)
      //   gamma = dnlSqrt32 * gammaInitial;

      // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      // plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      // plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      // temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      // // initialize the loop
      // irun = true;

      // // Main loop of the Newton-Raphson procedure
      // while (irun)
      // {
      //   // Compute yield stress and hardening parameter
      //   yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

      //   // Compute the radial return equation for isotropic case
      //   fun = Strial - gamma * TwoG32 - yield;

      //   // Reduce the range of the solution depending the sign of fun
      //   if (fun < 0.0)
      //     gammaMax = gamma;
      //   else
      //     gammaMin = gamma;

      //   // Compute the hardening coefficient
      //   hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);

      //   // Compute derivative of the radial return equation
      //   dfun = TwoG32 + dnlSqrt23 * hard;

      //   // Increment of the gamma parameter
      //   dgamma = fun / dfun;

      //   // Increment on the gamma value for Newton-Raphson
      //   gamma += dgamma;

      //   // If the solution is outside of the brackets, do a bissection step
      //   if ((gammaMax - gamma) * (gamma - gammaMin) < 0.0)
      //   {
      //     dgamma = 0.5 * (gammaMax - gammaMin);
      //     gamma = gammaMin + dgamma;
      //     iBissection += 1;
      //   }

      //   // Algorithm converged, end of computations
      //   if (dnlAbs(dgamma) < tolNR)
      //   {
      //     irun = false;
      //   }
      //   else
      //   {
      //     // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      //     plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      //     plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      //     temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      //     // Increase the number of iterations
      //     iterate += 1;
      //     if (iterate > itMax)
      //     {
      //       printf("NO CONVERGENCE in Newton-Raphson\n");
      //       printf("After %d iterations\n", iterate);
      //       printf("Time %lf \n", timeStep);
      //       printf("Precision %lf\n", dnlAbs(fun / yield));
      //       printf("Strial %lf\n", Strial);
      //       printf("Gamma0 %lf\n", _integrationPoint->gamma);
      //       printf("Gamma %lf\n", gamma);
      //       printf("Gamma M %lf %lf\n", gammaMin, gammaMax);
      //       printf("DGamma %lf\n", dgamma);
      //       printf("epsp0 %lf\n", _integrationPoint->plasticStrain + dnlSqrt23 * _integrationPoint->gamma);
      //       printf("depsp0 %lf\n", dnlSqrt23 * _integrationPoint->gamma / timeStep);
      //       printf("plasticStrain %lf\n", plasticStrain);
      //       printf("plasticStrainRate %lf\n", plasticStrainRate);
      //       printf("temperature %lf\n", temperature);
      //       printf("old plasticStrain %lf\n", _integrationPoint->plasticStrain);
      //       printf("old plasticStrainRate %lf\n", _integrationPoint->plasticStrainRate);
      //       printf("old gamma %lf\n", _integrationPoint->gamma);
      //       printf("old yieldStress %lf\n", _integrationPoint->yieldStress);
      //       printf("old temperature %lf\n", _integrationPoint->temperature);
      //       fatalError("No convergence");
      //     }
      //   }
      // }

      plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      temperature = temperature0 + 0.5 * gamma * heatFrac * (dnlSqrt23 * yield + Snorm0);

      yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

      // Computation of the plastic strain increment
      _integrationPoint->PlasticStrain += gamma * DeviatoricStress / Snorm;

      // Compute the new stress corrector
      DeviatoricStress *= 1.0 - TwoG * gamma / Snorm;

      // Store new plasticStrain and plasticStrainRate
      _integrationPoint->plasticStrain += dnlSqrt23 * gamma;
      _integrationPoint->plasticStrainRate = dnlSqrt23 * gamma / timeStep;

      // Store the value of gamma for next loop
      _integrationPoint->gamma = gamma;
      _integrationPoint->gammaCumulate += gamma;

      // store the new yield stress of the material
      _integrationPoint->yieldStress = yield;
    }

    // Compute the final stress of the element
    _integrationPoint->Stress = DeviatoricStress + _integrationPoint->pressure * Unity;

    // Compute the new specific internal energy
    stressPower = 0.5 * _integrationPoint->StrainInc.doubleProduct(StressOld + _integrationPoint->Stress);
    _integrationPoint->internalEnergy += stressPower / material->density;

    // Get back the gamma value
    if (gamma != 0.0)
    {
      // Compute the plastic Work increment
      plWorkInc = 0.5 * gamma * (DeviatoricStress.getNorm() + Snorm0);

      // New dissipated inelastic specific energy
      _integrationPoint->inelasticEnergy += plWorkInc / material->density;
      _integrationPoint->temperature += heatFrac * plWorkInc;
    }
  }
} */

//-----------------------------------------------------------------------------
bool Element::check()
//-----------------------------------------------------------------------------
{
  // check the number of nodes of the element
  if (nodes.getSize() != _elementData->numberOfNodes)
    fatalError("Element::check",
               "Element %d has only %d nodes while this must have %d nodes\n",
               number, nodes.getSize(), _elementData->numberOfNodes);

  // check for the material
  if (material == NULL)
    fatalError("Element::check",
               "Element %d has no material attached to it\n"
               "this is a fault, you must attach a material definition to all the elements of the structure\n");

  // level 2 check of the element
  checkLevel2();

  return true;
}

//-----------------------------------------------------------------------------
void Element::computeStrains()
//-----------------------------------------------------------------------------
{
  Tensor2 F;

  for (short intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // Get back the current integration point
    setCurrentIntegrationPoint(intPointId);

    // Computation of the Gradient of deformation
    computeDeformationGradient(F, 0);

    // Polar decomposition
    F.polarDecomposeCuppenLnU(_integrationPoint->StrainInc, _integrationPoint->R);

    // Compute the total strain tensor
    _integrationPoint->Strain += _integrationPoint->StrainInc;
  }
}

//-----------------------------------------------------------------------------
void Element::computePressure()
//-----------------------------------------------------------------------------
{
  int intPointId;
  double K = material->getBulkModulus();
  double pressureIncrement = 0.0;

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    pressureIncrement += getIntegrationPoint(intPointId)->StrainInc.getTrace();
  }

  pressureIncrement /= getNumberOfIntegrationPoints();

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    getIntegrationPoint(intPointId)->pressure = getIntegrationPoint(intPointId)->Stress.getThirdTrace() + K * pressureIncrement;
  }
}

//-----------------------------------------------------------------------------
void Element::computeFinalRotation()
//-----------------------------------------------------------------------------
{
  int intPointId;

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // Get back the current integration point
    setCurrentIntegrationPoint(intPointId);

    // Apply the Final Rotation for Objectivity of the Constitutive Law
    _integrationPoint->Stress = _integrationPoint->Stress.productByRxRT(_integrationPoint->R);
    _integrationPoint->Strain = _integrationPoint->Strain.productByRxRT(_integrationPoint->R);
    _integrationPoint->PlasticStrain = _integrationPoint->PlasticStrain.productByRxRT(_integrationPoint->R);
  }
}

#define _getFromIntegrationPoint(FIELD, VAR)     \
  if (field == Field::FIELD)                     \
  {                                              \
    return getIntegrationPoint(intPoint)->FIELD; \
  }

#define _getScalarFromIntegrationPointTensor2(FIELD, VAR)     \
  if ((Field::FIELD <= field) && (field <= Field::FIELD##ZZ)) \
  {                                                           \
    if (field == Field::FIELD)                                \
      return getIntegrationPoint(intPoint)->FIELD.getNorm();  \
    if (field == Field::FIELD##XX)                            \
      return getIntegrationPoint(intPoint)->FIELD(0, 0);      \
    if (field == Field::FIELD##XY)                            \
      return getIntegrationPoint(intPoint)->FIELD(0, 1);      \
    if (field == Field::FIELD##XZ)                            \
      return getIntegrationPoint(intPoint)->FIELD(0, 2);      \
    if (field == Field::FIELD##YX)                            \
      return getIntegrationPoint(intPoint)->FIELD(1, 0);      \
    if (field == Field::FIELD##YY)                            \
      return getIntegrationPoint(intPoint)->FIELD(1, 1);      \
    if (field == Field::FIELD##YZ)                            \
      return getIntegrationPoint(intPoint)->FIELD(1, 2);      \
    if (field == Field::FIELD##ZX)                            \
      return getIntegrationPoint(intPoint)->FIELD(2, 0);      \
    if (field == Field::FIELD##ZY)                            \
      return getIntegrationPoint(intPoint)->FIELD(2, 1);      \
    if (field == Field::FIELD##ZZ)                            \
      return getIntegrationPoint(intPoint)->FIELD(2, 2);      \
  }

//-----------------------------------------------------------------------------
double Element::getIntPointValueExtract(short field, short intPoint)
//-----------------------------------------------------------------------------
{
  _getFromIntegrationPoint(plasticStrain, plasticStrain);
  _getFromIntegrationPoint(plasticStrainRate, plasticStrainRate);
  _getFromIntegrationPoint(gamma, gamma);
  _getFromIntegrationPoint(gammaCumulate, gammaCumulate);
  _getFromIntegrationPoint(yieldStress, yieldStress);
  _getFromIntegrationPoint(temperature, temperature);
  _getFromIntegrationPoint(density, density);
  _getFromIntegrationPoint(pressure, pressure);
  _getFromIntegrationPoint(internalEnergy, internalEnergy);
  _getScalarFromIntegrationPointTensor2(Strain, Strain);
  _getScalarFromIntegrationPointTensor2(StrainInc, StrainInc);
  _getScalarFromIntegrationPointTensor2(PlasticStrain, PlasticStrain);
  _getScalarFromIntegrationPointTensor2(PlasticStrainInc, PlasticStrainInc);
  _getScalarFromIntegrationPointTensor2(Stress, Stress);

  if (field == Field::vonMises)
  {
    return getIntegrationPoint(intPoint)->Stress.getMisesEquivalent();
  }

  Field fakeField;
  printf("Element::getIntPointValue\nUnknown field %s\n", fakeField.getVtklabel(field).chars());
  return 0.0;
}

//-----------------------------------------------------------------------------
double Element::getIntPointValue(short field, short _intPoint)
//-----------------------------------------------------------------------------
{
  double value = 0.0;

  // If no point is defined return the value of the point
  if (_intPoint >= 0)
    return getIntPointValueExtract(field, _intPoint);

  // If no point defined, return the mean value on all integration points
  for (short intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    value += getIntPointValueExtract(field, intPoint);
  }

  return value / _elementData->numberOfIntegrationPoints;
}

//-----------------------------------------------------------------------------
void Element::computeDensity()
//-----------------------------------------------------------------------------
{
  for (short intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPoint);

    _integrationPoint->density = material->density * _integrationPoint->detJ0 / _integrationPoint->detJ;

    if (getFamily() == Element::Axisymetric)
      _integrationPoint->density /= _integrationPoint->radius;
  }
}

//-----------------------------------------------------------------------------
Node *Element::getNodeOnFace(short face, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(_elementData != NULL);
#endif
  return nodes(_elementData->faces[face].number[node]);
}

//-----------------------------------------------------------------------------
void Element::dumpElementData() const
//-----------------------------------------------------------------------------
{
  std::cout << "Dump Element\n";
  std::cout << "name : " << _elementData->name << "\n";
  std::cout << "type : " << _elementData->type << "\n";
  std::cout << "family : " << _elementData->family << "\n";
  std::cout << "numberOfDimensions : " << _elementData->numberOfDimensions << "\n";
  std::cout << "numberOfDDL : " << _elementData->numberOfDDL << "\n";
  std::cout << "vtkType : " << _elementData->vtkType << "\n";
  std::cout << "numberOfNodes : " << _elementData->numberOfNodes << "\n";
  std::cout << "numberOfIntegrationPoints : " << _elementData->numberOfIntegrationPoints << "\n";
  std::cout << "numberOfFaces : " << _elementData->numberOfFaces << "\n";

  for (int i = 0; i < _elementData->numberOfNodes; i++)
  {
    std::cout << " - node " << i << "\n";
    std::cout << "localCoords : " << _elementData->nodes[i].localCoords << "\n";
    std::cout << "numberOfNeighbourNodes : " << _elementData->nodes[i].numberOfNeighbourNodes << "\n";
    std::cout << "neighbour : [";
    for (int k = 0; k < _elementData->nodes[i].numberOfNeighbourNodes; k++)
    {
      if (k != 0)
        std::cout << ", ";
      std::cout << _elementData->nodes[i].neighbour[k];
    }
    std::cout << "]\n";
    std::cout << "integrationPointsToNode : " << _elementData->nodes[i].integrationPointsToNode << "\n";
  }

  for (int i = 0; i < _elementData->numberOfIntegrationPoints; i++)
  {
    std::cout << " - int Point " << i << "\n";
    std::cout << "coords : " << _elementData->integrationPoint[i].coords << "\n";
    std::cout << "weight : " << _elementData->integrationPoint[i].weight << "\n";
    std::cout << "shapeFunction : " << _elementData->integrationPoint[i].shapeFunction << "\n";
    std::cout << "derShapeFunction : " << _elementData->integrationPoint[i].derShapeFunction << "\n";
  }

  for (int i = 0; i < _elementData->numberOfFaces; i++)
  {
    std::cout << " - Face " << i << "\n";
    std::cout << "numberOfNodes : " << _elementData->faces[i].numberOfNodes << "\n";
    std::cout << "number : [";
    for (int k = 0; k < _elementData->faces[i].numberOfNodes; k++)
    {
      if (k != 0)
        std::cout << ", ";
      std::cout << _elementData->faces[i].number[k];
    }
    std::cout << "]\n";
  }

  std::cout << "CheckElementData : " << _elementData->CheckElementData << "\n";
}

// --------------------------- ADDED AFTER IS TO BE DELETED

/*
//-----------------------------------------------------------------------------
void Element::computeStrainsOld(double timeStep)
//-----------------------------------------------------------------------------
{
  Tensor2 F;

  for (long intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPointId);

    // calcul de F
    computeDeformationGradient(F, 1);

    // decomposition polaire et recup de U et R
    F.polarDecomposeLnU(_integrationPoint->StrainInc, _integrationPoint->R);

    // calcul de Strain
    _integrationPoint->Strain += _integrationPoint->StrainInc;

    // calcul de epsilon Equivalent total
    //      _integrationPoint->EpsEqv = _integrationPoint->Strain.getJ2 ();
  }
}

//-----------------------------------------------------------------------------
void Element::computeConstitutiveEquation()
//-----------------------------------------------------------------------------
{
  long intPointId;
  double TwoG = material->getTwoShearModulus();

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPointId);

    //DELETE Snorm0   _integrationPoint->Snorm0 = DeviatoricStress.getNorm();

    //DELETE DeviatoricStress += TwoG * _integrationPoint->StrainInc.getDeviator();
  }
}

//-----------------------------------------------------------------------------
void Element::computeStateEquationOld()
//-----------------------------------------------------------------------------
{
  long intPointId;
  double K = material->getBulkModulus();
  double meanPressureIncrement = 0.0;

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPointId);

    meanPressureIncrement += _integrationPoint->StrainInc.getTrace();
  }

  meanPressureIncrement /= getNumberOfIntegrationPoints();

  for (intPointId = 0; intPointId < getNumberOfIntegrationPoints(); intPointId++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPointId);

    _integrationPoint->pressure += K * meanPressureIncrement;
  }
}

//-----------------------------------------------------------------------------
void Element::computeStressOld(double timeStep)
//-----------------------------------------------------------------------------
{
  bool irun;
  double plasticStrain, plasticStrainRate, yield;
  double fun, dfun;
  double gammaInitial = 1e-8;
  double gammaMin, gammaMax, gamma, dgamma;
  double hard;
  double K, TwoG, TwoG32;
  double meanPressureIncrease = 0.0;
  double Snorm0, Snorm, Strial;
  double stressPower;
  double temperature0, temperature;
  double tolNR = 1e-8;
  double xcor;
  double plWorkInc;
  int iBissection;
  int iterate;
  int itMax = 250;
  short intPoint;
  SymTensor2 StressOld;
  SymTensor2 DeviatoricStress;

  SymTensor2 Unity;
  Tensor2 F;

  // Init unity vector
  Unity.setToUnity();
  HardeningLaw *hardeningLaw = material->getHardeningLaw();

  K = material->getBulkModulus();
  TwoG = material->getTwoShearModulus();
  TwoG32 = dnlSqrt32 * TwoG;

  // Boucle sur les points d'integration
  for (intPoint = 0; intPoint < _elementData->numberOfIntegrationPoints; intPoint++)
  {
    // Select integration point
    setCurrentIntegrationPoint(intPoint);

    // Compute gradient of deformation at the Begining
    // computeDeformationGradient(F, 0);

    // Polar decomposition of the gradient of deformation
    // F.polarDecomposeLnU(_integrationPoint->StrainInc, _integrationPoint->R);

    // Cumulate the strain on the current integration point
    //_integrationPoint->Strain += _integrationPoint->StrainInc;

    // Compute the increment of meanPressureIncrease
    //meanPressureIncrease += _integrationPoint->StrainInc.getTrace();

    // computation of Snorm0
    // Snorm0 = DeviatoricStress.getNorm();
    // _integrationPoint->Snorm0 = Snorm0;

    // Trial Deviatoric stress
    //  DeviatoricStress += TwoG * _integrationPoint->StrainInc.getDeviator();

    // Computation of Snorm
    Snorm = DeviatoricStress.getNorm();

    // Computation of Strial
    Strial = dnlSqrt32 * Snorm;

    // Get back initial temperature
    temperature0 = _integrationPoint->temperature;
    temperature = temperature0;

    // Get back plasticStrain and plasticStrainRate
    plasticStrain = _integrationPoint->plasticStrain;
    plasticStrainRate = _integrationPoint->plasticStrainRate;

    // Initialize value of gamma
    gamma = 0.0;

    // Get back yield stress
    yield = _integrationPoint->yieldStress;

    // If the yield is zero, compute the first yield stress thank's to the constitutive law using the initial default value of gamma
    if (yield == 0.0)
    {
      yield = hardeningLaw->getYieldStress(gammaInitial, gammaInitial / timeStep, temperature);
    }

    // Initialize the iterate counters
    iterate = 0;
    iBissection = 0;

    // Plasticity criterion test and begin of plastic corrector
    if (Strial > yield)
    {
      // Definition of gamma range
      gammaMin = 0.0;
      gammaMax = Strial / (TwoG32);

      // Get back the previous value of gamma
      gamma = _integrationPoint->gamma;

      if (plasticStrain == 0.0)
        gamma = dnlSqrt32 * gammaInitial;

      // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
      plasticStrainRate = dnlSqrt23 * gamma / timeStep;
      plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
      temperature = temperature0 + 0.5 * gamma * material->getHeatFraction() * (dnlSqrt23 * yield + Snorm0);

      // initialize the loop
      irun = true;

      // Main loop of the Newton-Raphson procedure
      while (irun)
      {
        // Compute yield stress and hardening parameter
        yield = hardeningLaw->getYieldStress(plasticStrain, plasticStrainRate, temperature);

        // Compute the radial return equation for isotropic case
        fun = Strial - gamma * TwoG32 - yield;

        // Reduce the range of the solution depending the sign of fun
        if (fun < 0.0)
          gammaMax = gamma;
        else
          gammaMin = gamma;

        // Compute the hardening coefficient
        hard = hardeningLaw->getDerivateYieldStress(plasticStrain, plasticStrainRate, temperature, timeStep);

        // Compute derivative of the radial return equation
        dfun = TwoG32 + dnlSqrt23 * hard;

        // Increment of the gamma parameter
        dgamma = fun / dfun;

        // Increment on the gamma value for Newton-Raphson
        gamma += dgamma;

        // If the solution is outside of the brackets, do a bissection step
        if ((gammaMax - gamma) * (gamma - gammaMin) < 0.0)
        {
          dgamma = 0.5 * (gammaMax - gammaMin);
          gamma = gammaMin + dgamma;
          iBissection += 1;
        }

        // Algorithm converged, end of computations
        if (dnlAbs(dgamma) < tolNR)
          irun = false;

        // Update the values of plasticStrain, plasticStrainRate and temperature for next loop
        plasticStrainRate = dnlSqrt23 * gamma / timeStep;
        plasticStrain = _integrationPoint->plasticStrain + dnlSqrt23 * gamma;
        temperature = temperature0 + 0.5 * gamma * material->getHeatFraction() * (dnlSqrt23 * yield + Snorm0);

        // Increase the number of iterations
        iterate += 1;
        if (iterate > itMax)
        {
          printf("NO CONVERGENCE in Newton-Raphson\n");
          printf("After %d iterations\n", iterate);
          printf("Time %lf \n", timeStep);
          printf("Precision %lf\n", dnlAbs(fun / yield));
          printf("Strial %lf\n", Strial);
          printf("Gamma0 %lf\n", _integrationPoint->gamma);
          printf("Gamma %lf\n", gamma);
          printf("Gamma M %lf %lf\n", gammaMin, gammaMax);
          printf("DGamma %lf\n", dgamma);
          printf("epsp0 %lf\n", _integrationPoint->plasticStrain + dnlSqrt23 * _integrationPoint->gamma);
          printf("depsp0 %lf\n", dnlSqrt23 * _integrationPoint->gamma / timeStep);
          printf("plasticStrain %lf\n", plasticStrain);
          printf("plasticStrainRate %lf\n", plasticStrainRate);
          printf("temperature %lf\n", temperature);
          printf("old sdv1 %lf\n", _integrationPoint->plasticStrain);
          printf("old sdv2 %lf\n", _integrationPoint->plasticStrainRate);
          printf("old sdv3 %lf\n", _integrationPoint->gamma);
          printf("old sdv4 %lf\n", _integrationPoint->yieldStress);
          printf("old sdv5 %lf\n", _integrationPoint->temperature);
          fatalError("No convergence");
        }
      }

      // Compute the new stress corrector
      xcor = (1.0 - TwoG * gamma / Snorm);
      DeviatoricStress *= xcor;

      // End of plastic corrector algorithm
    }

    // Store new plasticStrain and plasticStrainRate
    _integrationPoint->plasticStrain = plasticStrain;
    _integrationPoint->plasticStrainRate = plasticStrainRate;

    // Store the value of gamma for next loop
    _integrationPoint->gamma = gamma;

    // store the new yield stress of the material
    _integrationPoint->yieldStress = yield;

    // Get back the old stress Tensor
    StressOld = _integrationPoint->Stress;

    // Compute the final pressure of the element
    //_integrationPoint->pressure += K * meanPressureIncrease;

    // Apply the Final Rotation for Objectivity of the Constitutive Law
    DeviatoricStress = DeviatoricStress.productByRxRT(_integrationPoint->R);
    _integrationPoint->Strain = _integrationPoint->Strain.productByRxRT(_integrationPoint->R);
    _integrationPoint->PlasticStrain = _integrationPoint->PlasticStrain.productByRxRT(_integrationPoint->R);

    // Compute the final stress of the element
    _integrationPoint->Stress = DeviatoricStress + _integrationPoint->pressure * Unity;

    // Compute the new specific internal energy
    stressPower = 0.5 * _integrationPoint->StrainInc.doubleProduct(StressOld + _integrationPoint->Stress);
    _integrationPoint->internalEnergy += stressPower / material->density;

    // Get back the gamma value
    gamma = _integrationPoint->gamma;
    if (gamma != 0.0)
    {
      // Compute the plastic Work increment
      //DELETE Snorm0    plWorkInc = 0.5 * gamma * (DeviatoricStress.getNorm() + _integrationPoint->Snorm0);

      // New dissipated inelastic specific energy
      _integrationPoint->inelasticEnergy += plWorkInc / material->density;
      _integrationPoint->temperature += material->getHeatFraction() * plWorkInc;
    }
  }
}

//-----------------------------------------------------------------------------
void Element::computeMassEquation(MatrixDiag &M, Vector &F)
//-----------------------------------------------------------------------------
{
  Vector density(_elementData->numberOfNodes);
  Tensor2 dv;
  double WxdJ;

#ifdef VERIF_maths
  // verifier la taille de la matrice M
  if ((M.rows() != _elementData->numberOfNodes) || (M.columns() != _elementData->numberOfNodes))
  {
    std::cerr << "Error in computeMassEquation()\nincompatible getSize of matrices M\n";
    std::cerr << "expected " << _elementData->numberOfNodes << "x" << _elementData->numberOfNodes << std::endl;
    std::cerr << "getting  " << M.rows() << "x" << M.columns() << std::endl;
    exit(-1);
  }

  // verifier la taille du vecteur F
  if (F.getSize() != _elementData->numberOfNodes)
  {
    std::cerr << "Error in computeMassEquation()\nincompatible getSize of vector F\n";
    std::cerr << "expected " << _elementData->numberOfNodes << std::endl;
    std::cerr << "getting  " << F.getSize() << std::endl;
    exit(-1);
  }
#endif

  // initialiser M, F
  M = 0.0;
  F = 0.0;

  // chargement des densites nodales predites
  // for (int nodeId = 0; nodeId < _elementData->numberOfNodes; nodeId++)
  //   density(nodeId) = nodes(nodeId)->currentField->density;

  for (int intPoint = 0; intPoint < getNumberOfIntegrationPoints(); intPoint++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPoint);

    // calcul du gradient de vitesses au point (Version 5 vitesse a la fin de l'increment)
    getdV_atIntPoint(dv, 1);

    // calcul du terme d'integration numerique
    WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
    if (getFamily() == Element::Axisymetric)
    {
    //  double currentRadius;
      //currentRadius = _integrationPoint->radius;
      WxdJ *= dnl2PI *_integrationPoint->radius;
    }

    // calcul de la matrice M consistante
    for (int nodeId = 0; nodeId < _elementData->numberOfNodes; nodeId++)
      M(nodeId) += _elementData->integrationPoint[intPoint].shapeFunction(nodeId) * WxdJ;

    // calcul des vecteurs de densites
    for (int nodeId = 0; nodeId < _elementData->numberOfNodes; nodeId++)
    {
      for (int I = 0; I < _elementData->numberOfNodes; I++)
      {
        F(nodeId) -= _elementData->integrationPoint[intPoint].shapeFunction(nodeId) * _elementData->integrationPoint[intPoint].shapeFunction(I) * dv.getTrace() * density(I) * WxdJ;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void Element::computeMomentumEquation(MatrixDiag &M, Vector &F)
//-----------------------------------------------------------------------------
{
  long intPoint;
  long i, j, I;
  double density, WxdJ;//, currentRadius;

#ifdef VERIF_maths
  // verifier la taille de la matrice M
  if ((M.rows() != _elementData->numberOfNodes * getNumberOfDimensions()) || (M.columns() != _elementData->numberOfNodes * getNumberOfDimensions()))
  {
    std::cerr << "Error in computeMomentumEquation()\nincompatible getSize of matrices M\n";
    std::cerr << "expected " << getNumberOfDimensions() * _elementData->numberOfNodes << "x" << getNumberOfDimensions() * _elementData->numberOfNodes << std::endl;
    std::cerr << "getting  " << M.rows() << "x" << M.columns() << std::endl;
    exit(-1);
  }

  // verifier la taille du vecteur F
  if (F.getSize() != _elementData->numberOfNodes * getNumberOfDimensions())
  {
    std::cerr << "Error in computeMomentumEquation()\nincompatible getSize of vector F\n";
    std::cerr << "expected " << _elementData->numberOfNodes * getNumberOfDimensions() << std::endl;
    std::cerr << "getting  " << F.getSize() << std::endl;
    exit(-1);
  }
#endif

  // initialiser M, F
  M = 0.0;
  F = 0.0;

  for (intPoint = 0; intPoint < getNumberOfIntegrationPoints(); intPoint++)
  {
    // recuperation du point d'integration
    setCurrentIntegrationPoint(intPoint);

    // calcul de la masse volumique au point
    density = _integrationPoint->density;
    // getDens_atIntPoint(density);

    // calcul du terme d'integration numerique
    WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
    if (getFamily() == Element::Axisymetric)
    {
     // currentRadius = _integrationPoint->radius;
      WxdJ *= dnl2PI * _integrationPoint->radius;
    }

    // calcul de M consistent et de la contribution en masse
    for (I = 0; I < _elementData->numberOfNodes; I++)
      for (i = 0; i < getNumberOfDimensions(); i++)
        M(I * getNumberOfDimensions() + i) +=
            _elementData->integrationPoint[intPoint].shapeFunction(I) * density * WxdJ;

    // calcul des forces internes
    for (I = 0; I < _elementData->numberOfNodes; I++)
    {
      for (i = 0; i < getNumberOfDimensions(); i++)
      {
        for (j = 0; j < getNumberOfDimensions(); j++)
        {
          F(I * getNumberOfDimensions() + i) -=
              _integrationPoint->dShapeFunction(I, j) * _integrationPoint->Stress(j, i) * WxdJ;
        }
      }
    }

    //contribution axisymetrique
    if (getFamily() == Element::Axisymetric)
    {
      // currentRadius deja calcule
      //currentRadius = _integrationPoint->radius;

      for (I = 0; I < _elementData->numberOfNodes; I++)
      {
        F(I * getNumberOfDimensions()) -=
            (_elementData->integrationPoint[intPoint].shapeFunction(I) * _integrationPoint->Stress(2, 2) / _integrationPoint->radius) * WxdJ;
      }
    }
  }
}


//Ajout d'un point d'integration à l'element courant


//-----------------------------------------------------------------------------
void Element::clearIntegrationPoint ()
//-----------------------------------------------------------------------------
{
  // vider la liste
  integrationPoints.flush ();
}


//-----------------------------------------------------------------------------
void Element::detachMaterial ()
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  if (material == NULL)
    fatalError ("Element::detachMaterial",
    "This function have been called while there is no material attached to this element.\n"
    "This is not allowed since you tried to detach a non existing material to the element\n"
    "You must make a call to Element::attachMaterial before doing this\n");
#endif

  material = NULL;

//  return(Success);
}

//Positionne le point d'integration courant

//-----------------------------------------------------------------------------
void Element::getIntegrationPoint (long pt)
//-----------------------------------------------------------------------------
{
  // pointage sur le point d'integration
  ref = integrationPoints (pt);

#ifdef VERIF_assert
  if (ref == NULL)
    {
      fatalError ("Element::getIntegrationPoint",
      "Integration point [%d] doesn't exist for the element %d",
      pt, _listIndex);
    }
#endif
}

//Positionne le point de sous-integration courant

//-----------------------------------------------------------------------------
void Element::getUnderIntegrationPoint (long pt)
//-----------------------------------------------------------------------------
{
  // pointage sur le point d'integration
  underIntegrationPoint = underIntegrationPoints (pt);

#ifdef VERIF_assert
  if (underIntegrationPoint == NULL)
    {
      fatalError ("Element::getIntegrationPoint",
      "Integration point [%d] doesn't exist for the element %d",
      pt, _listIndex);
    }
#endif
}

//-----------------------------------------------------------------------------
long Element::getLocalNumber (Node * nd)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < nodes.getSize (); i++)
    {
      if (nodes (i) == nd)
  return i;
    }
  cerr << "indice " << nd->number << " not found";
  return 0;
}



//-----------------------------------------------------------------------------
double Element::getThermalSpeed()
//-----------------------------------------------------------------------------
{
  double	heatCapacity=material->heatCoeff();
  double	K=material->conductivity();

  // calcul de la densite moyenne de l'element
  double	density=0.;
  for (long i=0;i<_elementData->numberOfNodes;i++) density+=nodes(i)->currentField->density;
  density/=_elementData->numberOfNodes;

  // vitesse du son dans le materiau
  return ((density*heatCapacity)/(2.*K));
//  return sqrt((E*(1-poissonRatio))/(density*(1+poissonRatio)*(1-2*poissonRatio)));
}

//Phase d'integration de la loi de conservation de la masse


//Calcul de la matrice de masse d'un element

//-----------------------------------------------------------------------------
void Element::computeMassEquation (MatrixDiag & M)
//-----------------------------------------------------------------------------
{
  long i, pt;
  double WxdJ;

  // redim de la matrice
  M.redim(_elementData->numberOfNodes, _elementData->numberOfNodes);

  for (pt = 0; pt < getNumberOfIntegrationPoints (); pt++)
    {
      // recuperation du point d'integration
   WARNING      getIntegrationPoint (pt);

      // calcul du terme d'integration numerique
      WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
      if (getFamily() == Element::Axisymetric)
  {
    //double currentRadius;
    //currentRadius=_integrationPoint->radius;
    WxdJ *= dnl2PI * _integrationPoint->radius;
  }

      // calcul de la matrice M consistante
      for (i = 0; i < _elementData->numberOfNodes; i++)
  M (i) += (_elementData->integrationPoint[pt].shapeFunction (i)) * WxdJ;
    }
}

//Phase d'integration de la loi de conservation de la quantite de mouvement



//Phase d'integration de la loi de comportement


//Phase d'integration de la loi d'etat



//Phase d'integration de la loi de conservation de l'energie

//-----------------------------------------------------------------------------
void Element::computeEnergyEquation (MatrixDiag & M, Vector & F)
//-----------------------------------------------------------------------------
{
  Vector e (_elementData->numberOfNodes);		// vecteur des densites aux noeuds
  Vec3D dT;
//  Vec3D       c; // vitesses convectives
  Tensor2 Stress;
  Tensor2 dv;			// matrice gradient des vitesses
  Tensor2 dvs;			// matrice gradient des vitesses
  long pt;
  long i, k;
  double  WxdJ, density;
  double SigklVkl, lambda;

#ifdef VERIF_maths
  // verifier la taille de la matrice M
  if ((M.rows () != _elementData->numberOfNodes) || (M.columns () != _elementData->numberOfNodes))
    {
      cerr <<
  "Error in computeEnergyEquation()\nincompatible getSize of matrices M\n";
      cerr << "expected " << _elementData->numberOfNodes << "x" << _elementData->numberOfNodes << std::endl;
      cerr << "getting  " << M.rows () << "x" << M.columns () << std::endl;
      exit (-1);
    }

  // verifier la taille du vecteur F
  if (F.getSize () != _elementData->numberOfNodes)
    {
      cerr <<
  "Error in computeEnergyEquation()\nincompatible getSize of vector F\n";
      cerr << "expected " << _elementData->numberOfNodes << std::endl;
      cerr << "getting  " << F.getSize () << std::endl;
      exit (-1);
    }
#endif

  // initialiser M, F
  M = 0.;
  F = 0.;
//  mass=0.;

  // chargement des energies nodales
  for (i = 0; i < _elementData->numberOfNodes; i++)
    e (i) = nodes (i)->newField->e;
  // cout << "e="<<e<<std::endl;

  // calcul de la conductivite thermique du materiau
  lambda = material->conductivity ();

  for (pt = 0; pt < getNumberOfIntegrationPoints (); pt++)
    {

      // recuperation du point d'integration
   WARNING      getIntegrationPoint (pt);

      // calcul de la vitesse convective au point
//    getConvVel(c);
      // cout << "c="<<c<<std::endl;

      // calcul de la densite au point
      getDens_atIntPoint (density);
      // cout << "density="<<density<<std::endl;

      // calcul de la temperature au point
      getdTemp_atIntPoints (dT);

      // calcul du tenseur des contraintes au point
      Stress = integrationPoints (pt)->Stress;
      // cout << "Stress="<<Stress<<std::endl;

      // calcul du gradient de vitesses au point
      // getdV(dv);
//    getdV_atIntPoint(dv,1);
//    dv.getSymetricPart(dvs);
      // cout << "dv="<<dv<<std::endl;
      // cout << "dvs="<<dvs<<std::endl;

      // calcul de SigklVkl
      SigklVkl = Stress.doubleProduct (integrationPoints (pt)->PlasticStrainInc);

      // calcul du terme d'integration numerique
      WxdJ = _integrationPoint->integrationPointData->weight * _integrationPoint->detJ;
      if (getFamily() == Element::Axisymetric)
  {
    //double currentRadius;
    //currentRadius=_integrationPoint->radius;
    WxdJ *= dnl2PI * _integrationPoint->radius;
  }

      // calcul de l'increment de masse pour le lumping
      //  mass+=density*WxdJ;

      // calcul de M consistent et de la contribution en masse
      for (i = 0; i < _elementData->numberOfNodes; i++)
  M (i) += density * (_elementData->integrationPoint[pt].shapeFunction (i)) * WxdJ;

      // calcul des vecteurs de densites
      for (i = 0; i < _elementData->numberOfNodes; i++)
  {


    // contribution de la partie convective
    //        F(i)-=density*_integrationPoint->N(i)*tmpConv*WxdJ;

    // contribution de la deformation
    F (i) += _elementData->integrationPoint[pt].shapeFunction (i) * SigklVkl * WxdJ;

    // partie conductive
    for (k = 0; k < getNumberOfDimensions(); k++)
      F (i) -= lambda * _integrationPoint->dShapeFunction (i, k) * dT (k) * WxdJ;
  }
    }

  // lumping de la matrice de masse
//   alpha=mass/M.getTrace();
//   for (i=0;i<_elementData->numberOfNodes;i++) M(i)*=alpha;
}

//Phase de calcul des deformations


//Calcul du gradient de temperature sur un point d'integration

//-----------------------------------------------------------------------------
void Element::getdTemp_atIntPoints (Vec3D & dT)
//-----------------------------------------------------------------------------
{
  long i, k;

  dT = 0.;
  for (i = 0; i < getNumberOfDimensions(); i++)
    for (k = 0; k < _elementData->numberOfNodes; k++)
      dT (i) += _integrationPoint->dShapeFunction (k, i) * nodes (k)->currentField->T;
}

//Recuperation de la densite sur un point d'integration


//Recuperation du vecteur des efforts externes sur un point d'integration

//-----------------------------------------------------------------------------
void Element::getFe_atIntPoint (Vec3D & force)
//-----------------------------------------------------------------------------
{
  long i, j;

  force = 0.;
   for (i = 0; i < _elementData->numberOfNodes; i++)
     for (j = 0; j < getNumberOfDimensions(); j++)
       force (j) += _integrationPoint->integrationPointData->shapeFunction (i) * nodes (i)->newField->force (j);
}

//-----------------------------------------------------------------------------
void Element::getEnergy (double & e)
//-----------------------------------------------------------------------------
{
  long i;

  e = 0.;
   for (i = 0; i < _elementData->numberOfNodes; i++)
     e += _integrationPoint->integrationPointData->shapeFunction (i) * nodes (i)->newField->e;
}

//-----------------------------------------------------------------------------
void Element::getSigmaAtPoint (Tensor2 & Stress, const Vec3D & point)
//-----------------------------------------------------------------------------
{
  long i;
  Vec3D loc;
  Vector N (getNumberOfIntegrationPoints ());

  // calcul global local
  glob2Loc (point, loc);

  // calcul des fonctions d'interpolation
//  getIntgtoNodes (N, loc);
  fatalError("Element::getSigmaAtPoint","Not implemented\n");

  // initialisation
  Stress = 0.;

  // interpolation du tenseur des contraintes
  for (i = 0; i < getNumberOfIntegrationPoints (); i++)
    Stress += N (i) * integrationPoints (i)->Stress;
}

// definition de l'algorithme de calcul des valeurs nodales en fonction des valeurs stockees au point d'integration
#define get_IntegrationValue_atNode(VAR,NODE) \
{ \
  Node *pnd = NODE; \
  Element *pel; \
  long j, pt, nb = 0; \
  VAR = 0.; \
  for (j = 0; j < pnd->elements.getSize (); j++) \
    { \
      pel = pnd->elements (j); \
      long loc = pel->nodes.IAppN (pnd->_listIndex); \
      for ( pt = 0; pt < pel->getNumberOfIntegrationPoints (); pt++) \
  { \
    VAR += pel->_elementData->nodes[loc].integrationPointsToNode(pt) * pel->integrationPoints (pt)->VAR; \
  } \
      nb++; \
    } \
  VAR = VAR / nb; \
}

//-----------------------------------------------------------------------------
void Element::get_Sig_atNode (Tensor2 & Stress, long i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(Stress, nodes (i));
}

//-----------------------------------------------------------------------------
void Element::get_Eps_atNode (Tensor2 & Strain, long i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(Strain, nodes (i));
}

//-----------------------------------------------------------------------------
void Element::get_dEps_atNode (Tensor2 & StrainInc, long i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(StrainInc, nodes (i));
}

//-----------------------------------------------------------------------------
void Element::get_EpsPlas_atNode (Tensor2 & PlasticStrain, long i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(PlasticStrain, nodes (i));
}

//-----------------------------------------------------------------------------
void Element::get_evp_atNode (double & plasticStrain, long i)
//-----------------------------------------------------------------------------
{
  get_IntegrationValue_atNode(plasticStrain, nodes (i));
}

//-----------------------------------------------------------------------------
void Element::computeBoundBox ()
//-----------------------------------------------------------------------------
{
  long i, j;
  Node *pnd;

  // initialisation du vecteur
  _nodeMin = _nodeMax = nodes (0)->coordinates;

  for (i = 1; i < nodes.getSize (); i++)
    {
      pnd = nodes (i);
      for (j = 0; j < 3; j++)
  {
    if (pnd->coordinates (j) < _nodeMin (j))
      _nodeMin (j) = pnd->coordinates (j);
    if (pnd->coordinates (j) > _nodeMax (j))
      _nodeMax (j) = pnd->coordinates (j);
  }
    }
}

//-----------------------------------------------------------------------------
bool Element::isNodeinBoundBox (Vec3D coordinates)
//-----------------------------------------------------------------------------
{
  if ((coordinates (0) < _nodeMin (0)) || (coordinates (0) > _nodeMax (0)))
    return (false);
  if ((coordinates (1) < _nodeMin (1)) || (coordinates (1) > _nodeMax (1)))
    return (false);
  if ((coordinates (2) < _nodeMin (2)) || (coordinates (2) > _nodeMax (2)))
    return (false);
  return (true);
}

//-----------------------------------------------------------------------------
bool Element::isNodeinBoundBox (Node * pnd)
//-----------------------------------------------------------------------------
{
  return isNodeinBoundBox (pnd->coordinates);
}

//-----------------------------------------------------------------------------
bool Element::isNodeinElement (Node * pnd)
//-----------------------------------------------------------------------------
{
  return isNodeinElement (pnd->coordinates);
}

//-----------------------------------------------------------------------------
bool Element::isNodeinElement (Vec3D coordinates)
//-----------------------------------------------------------------------------
{
  // test rapide
  if (isNodeinBoundBox (coordinates) == false)
    return (false);

  Vec3D loc;
  Vector shapes(getNumberOfNodes());

  // calcul du passage global en local
  glob2Loc (coordinates, loc);

  // calcul des fonctions d'interpolation au point
  getShapeFunctionAtPoint (shapes, loc);

  // recherche des bornes
  for (long i = 0; i < getNumberOfNodes(); i++)
    {

      // recherche avec une tolerance 1e-7
      if ((shapes (i) < -0.0000001) || (shapes (i) > 1.0000001))
  return (false);
    }

  // ok, le noeud est dedans
  return (true);
}

//-----------------------------------------------------------------------------
void Element::toFile (FILE * pfile)
//-----------------------------------------------------------------------------
{
  fprintf (pfile, "%8ld  %s  ", number, getName ().chars());
  for (long i = 0; i < _elementData->numberOfNodes; i++)
    {
      fprintf (pfile, "%6ld ", nodes (i)->number);
    }
  fprintf (pfile, "\n");
}


//-----------------------------------------------------------------------------
Node* Element::getNodeOnEdge(short edge, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(_elementData!=NULL);
#endif
  return nodes(_elementData->edges[edge].number[node]);
}

//-----------------------------------------------------------------------------
Node* Element::getNodeOnSideFace (short sideface, short node)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(_elementData!=NULL);
#endif
  return nodes(_elementData->sideFaces[sideface].number[node]);
}



//-----------------------------------------------------------------------------
Node* Element::getNeighbourNode (short node, short neighbour)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(_elementData!=NULL);
#endif
  return nodes(_elementData->nodes[node].neighbour[neighbour]);
}

//-----------------------------------------------------------------------------
//bool Element::getIntegrationPointCoords (short i, Vec3D & coordinates, double & weight)
bool Element::linkIntegrationPointData (short i)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert(_elementData!=NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfIntegrationPoints)
      fatalError("Element::getIntegrPointCoords",
     "integration point %d is out of bounds %d-%d\n",i,0,_elementData->numberOfIntegrationPoints);
#endif

  integrationPointData = _elementData->integrationPoint[i];

    return true;
}

//-----------------------------------------------------------------------------
bool compareLateralSurfaces(LateralSurface* p1, LateralSurface* p2)
//-----------------------------------------------------------------------------
{
  short i;

  // test if elements are from the same model
  if (p1->pel->model != p2->pel->model) return (p1->pel->model > p2->pel->model);

  for (i=0; i<maxNumberOfNodesBySideFace-1;i++)
    {
      if (p1->tab[i] < p2->tab[i]) return 0;
      if (p1->tab[i] > p2->tab[i]) return 1;
    }
  return (p1->tab[i] > p2->tab[i]);
}
*/
