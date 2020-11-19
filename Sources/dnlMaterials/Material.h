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
  \file Node.h
  \brief Declaration file for the Node class

  This file is the declaration file for the Node class.

  \ingroup dnlMaterials
*/

#ifndef __dnlMaterials_Material_h__
#define __dnlMaterials_Material_h__

#include <dnlMaths.h>

class ElementSet;
class HardeningLaw;

/*!
  \file Material.h
  \brief fichier .h de definition de la classe generique des materiaux
  \ingroup femLibrary

  Ce fichier sert à la definition la classe generique des materiaux.

*/

/*!
  \brief Classe virtuelle de gestion des materiaux
  \ingroup femLibrary

  Classe virtuelle de gestion des materiaux pour DynELA. Cette classe regroupe les parametres de base communs à tous les types de materiaux sur DynELA, comme le module de Young, le coefficient de Poisson, ou la densite d'un materiau. Pour pouvoir utiliser un materiau sur DynELA, on utilise alors une des classes derivees de celle-ci, ce qui permet alors de definir un comportement elastique, elasto-plastique, ou tout autre type de comportement.

*/
class Material
{
  friend class DynELA;

public:
#ifndef SWIG
  enum
  {
    planeStrain,
    planeStress,
    axisymetric,
    threedimensional
  };
#endif

protected:
  HardeningLaw *_hardening = NULL; // Hardening law of the current material
  List<ElementSet *> _elementsSet; // Liste d'elements qui referencent ce materiau
  Tensor4 _hookeTensor;            // Fourth order Hooke tensor
  Vec3D _color;                    // Associated _color of the material for the post-processor

public:
  String name = "_noname_";         // Given _name of the material
  double youngModulus = 0.0;        // Young modulus of the material  \f$E\f$
  double poissonRatio = 0.0;        // Poisson ratio of the material  \f$\nu\f$
  double density = 0.0;             // Material density of the material \f$\rho\f$
  double heatCapacity = 0.0;        // Specific Heat capacity of the material \f$C_{p}\f$
  double dilatation = 0.0;          // Dilatation coefficient of the material \f$\alpha\f$
  double initialTemperature = 0.0;  // Reference temperature of the material  \f$T_{0}\f$
  double thermalConductivity = 0.0; // Thermal thermalConductivity of the material \f$\lambda}\f$
  double taylorQuinney = 0.0;       // Taylor-Quinney coefficient \f$\eta\f$ defining the amount of plastic work converted into heat energy

public:
  Material(char *newName = NULL);
  Material(const Material &X);
  virtual ~Material();

  bool checkValues();
  //  bool isLinear();
  bool isYield();
  double getBulkModulus();
  double getLambda();
  double getShearModulus();
  double getTwoShearModulus();
  double getHeatFraction();
  HardeningLaw *getHardeningLaw() const;
  Matrix getHookeStiffnessMatrix(short type);
  Tensor4 &hookeTensor();
  Vec3D &color();
  void computeHookeTensor();
  void setColor(double, double, double);
  void setHardeningLaw(HardeningLaw *hardeningLaw);

  // acces aux ecrouissages
  //

  /*  double getYieldStress(IntegrationPoint *pfield);
  double getDerYieldStress(IntegrationPoint *pfield);
  double getIsotropicYieldStress(Element *, double shift = 0.0);
  double getIsotropicYieldHardeningLaw (Element *, double shift = 0.0);

  virtual String convertToDynELASourceFile(String _name);
  virtual void print(ostream &os) const;
  friend ostream &operator<<(ostream &os, Material &mat);
  void toFile(FILE *pfile);
 */
};

// Returns the first Lame coefficient \f$ \mu \f$
/*!
This method computes and returns the value of the first Lame coefficient \f$ \mu \f$ given by the following expression
\f[ \mu=\frac{E}{2(1+\poissonRatio)}, \f]
where \f$ E \f$ and \f$ \poissonRatio \f$ are the Young modulus and the Poisson ration respectively.
\returns The value of the first lame coefficient \f$ \mu \f$
*/
//-----------------------------------------------------------------------------
inline double Material::getTwoShearModulus()
//-----------------------------------------------------------------------------
{
  return youngModulus / (1.0 + poissonRatio);
}

// Returns the shear modulus
/*!
This method computes and returns the value of the shear modulus \f$ G \f$ given by the following expression
\f[ G=\frac{E}{2(1+\poissonRatio)}, \f]
where \f$ E \f$ and \f$ \poissonRatio \f$ are the Young modulus and the Poisson ration respectively.
\returns The value of the shear modulus \f$ G \f$
*/
//-----------------------------------------------------------------------------
inline double Material::getShearModulus()
//-----------------------------------------------------------------------------
{
  return youngModulus / (2.0 * (1.0 + poissonRatio));
}

// Returns the second Lame coefficient \f$ \lambda \f$
/*!
This method computes and returns the value of the second Lame coefficient \f$ \lambda \f$ given by the following expression
\f[ \lambda=\frac{\poissonRatio E}{(1+\poissonRatio)(1-2\poissonRatio)}, \f]
where \f$ E \f$ and \f$ \poissonRatio \f$ are the Young modulus and the Poisson ration respectively.
\returns The value of the second lame coefficient \f$ \lambda \f$
*/
//-----------------------------------------------------------------------------
inline double Material::getLambda()
//-----------------------------------------------------------------------------
{
  return poissonRatio * youngModulus / ((1.0 + poissonRatio) * (1.0 - 2.0 * poissonRatio));
}

// Returns the Bulk modulus \f$K\f$ of the material
/*!
This method computes and returns the value of the Bulk modulus \f$ K \f$ given by the following expression
\f[ K=\frac{1}{3}\left(3\lambda+2G\right)=\frac{E}{3(1-2\poissonRatio)}, \f]
where \f$ E \f$ and \f$ \poissonRatio \f$ are the Young modulus and the Poisson ration respectively.
\returns The value of the Bulk modulus \f$ K \f$
*/
//-----------------------------------------------------------------------------
inline double Material::getBulkModulus()
//-----------------------------------------------------------------------------
{
  return youngModulus / (3.0 * (1.0 - 2.0 * poissonRatio));
}

// Returns the Heat Fraction of the material
/*!
This method computes and returns the value of the Heat Fraction given by the following expression
\f[ Hf=\frac{\eta}{\rho*C_p} \f]
\returns The value of the Heat Fraction
*/
//-----------------------------------------------------------------------------
inline double Material::getHeatFraction()
//-----------------------------------------------------------------------------
{
  return taylorQuinney / (density * heatCapacity);
}

//-----------------------------------------------------------------------------
inline Vec3D &Material::color()
//-----------------------------------------------------------------------------
{
  return _color;
}

//-----------------------------------------------------------------------------
inline Tensor4 &Material::hookeTensor()
//-----------------------------------------------------------------------------
{
  return _hookeTensor;
}

/* //-----------------------------------------------------------------------------
inline bool Material::isLinear()
//-----------------------------------------------------------------------------
{
  return _isLinear;
}
 */
#endif
