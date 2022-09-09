/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlElements_ElementAxi_h__
#define __dnlElements_ElementAxi_h__

#include <ElementPlane.h>

//-----------------------------------------------------------------------------
// Class : ElementAxi
//
// Used to manage Elements in DynELA
//
// This class is included in SWIG
//-----------------------------------------------------------------------------
class ElementAxi : public ElementPlane
{

public:
  ElementAxi(long elementNumber = 1);
  ElementAxi(const ElementAxi &X);
  ~ElementAxi();

  // Interface methods excluded from SWIG
#ifndef SWIG
#endif

  // Interface methods excluded from basic SWIG support
#if !defined(SWIG) || defined(CSWIG)
#endif

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
  double getRadiusAtIntegrationPoint();
  double getRadiusAtUnderIntegrationPoint();
  void computeDeformationGradient(Tensor2 &F, short time);
  bool checkLevel2();
  void getdV_atIntPoint(Tensor2 &dv, short time);

  /*  void getdU_atIntPoint (Tensor2 & du, short time);
  void computeDeformationGradient (Tensor2 & du, short time);
  long numberOfUnderIntegrationPoints () = 0;
  double getCharacteristicLength () = 0;
  double getVolume () = 0;
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
  //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc () = 0;
  void glob2Loc (const Vec3D & point, Vec3D & local) = 0; */
};

#endif
