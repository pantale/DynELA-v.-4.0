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
\file Element.h
\brief Declaration file for the Element class

This file is the declaration file for the Element class.

\ingroup dnlElements
*/

#ifndef __dnlElements_Element_h__
#define __dnlElements_Element_h__

#include <dnlMaterials.h>
#include <ElementData.h>

class Node;
class Model;

class Element
{
    friend class Node;
    friend class ListIndex<Element *>; // To be able to use ListIndex
    long _listIndex;                   // Local index used for the ListIndex management.
    Vec3D _nodeMin, _nodeMax;          // Bounding box of an element.

protected:
    Matrix _globalToLocal;
    const ElementData *_elementData;
    IntegrationPoint *_integrationPoint;
    UnderIntegrationPoint *_underIntegrationPoint;

public:
    List<IntegrationPoint *> integrationPoints;
    List<UnderIntegrationPoint *> underIntegrationPoints;
    ListIndex<Node *> nodes;
    long number;
    Material *material;
    Matrix stiffnessMatrix;

    enum
    {
        ElGeneric = 0,
        ElQua4N2D,
        ElTri3N2D,
        ElQua4NAx,
        ElHex8N3D,
        ElTet4N3D,
        ElTet10N3D
    };

    //#ifndef SWIG
    enum
    {
        Bidimensional = 0,
        Axisymetric,
        Threedimensional
    };
    //#endif

public:
    Element(long elementNumber = 1);
    Element(const Element &element);
    virtual ~Element();

#ifndef SWIG
    friend std::ofstream &operator<<(std::ofstream &, const Element &);
    friend std::ifstream &operator>>(std::ifstream &, Element &);
    void write(std::ofstream &) const;
    Element &read(std::ifstream &);
#endif
    bool operator==(const Element &) const;
    bool operator!=(const Element &) const;
    bool operator<(const Element &) const;
    bool operator>(const Element &) const;

    bool check();
    //double getDensityAtIntegrationPoint(short time);
    double getElongationWaveSpeed();
    double getIntPointValue(short field, short intPoint);
    IntegrationPoint *getIntegrationPoint(short point);
    UnderIntegrationPoint *getUnderIntegrationPoint(short point);
    long &internalNumber();
    short getFamily() const;
    //    short getLocalIdOfNodeOnEdge(short edge, short node) const;
    short getNumberOfDDL() const;
    short getNumberOfDimensions() const;
    //    short getNumberOfEdges() const;
    short getNumberOfFaces() const;
    short getNumberOfIntegrationPoints() const;
    //    short getNumberOfNeighbourNodes(short i) const;
    short getNumberOfNodes() const;
    //    short getNumberOfNodesOnEdge(short i) const;
    short getNumberOfNodesOnFace(short i) const;
    //    short getNumberOfNodesOnSideFace(short i) const;
    //    short getNumberOfSideFaces() const;
    short getType() const;
    short getVtkType() const;
    String getName() const;
    Vec3D getLocalNodeCoords(short node) const;
    virtual bool checkLevel2() = 0;
    virtual bool computeJacobian(bool reference = false) = 0;
    virtual bool computeUnderJacobian(bool reference = false) = 0;
    virtual double getCharacteristicLength() = 0;
    virtual double getRadiusAtIntegrationPoint() = 0;
    virtual void computeDeformationGradient(Tensor2 &F, short time) = 0;
    virtual void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
    virtual void getdV_atIntPoint(Tensor2 &dv, short time) = 0;
    virtual void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
    virtual void getV_atIntPoint(Vec3D &v, short time) = 0;
    void add(IntegrationPoint *_integrationPoint, short intPointId);
    void add(Material *newMaterial);
    void add(UnderIntegrationPoint *_integrationPoint, short intPointId);
    void addNode(Node *newNode);
    //void computeConstitutiveEquation();
    void computeFinalRotation();
    void computeInternalForces(Vector &F, double timeStep);
    //void computeMassEquation(MatrixDiag &M, Vector &F);
    void computeMassMatrix(MatrixDiag &massMatrix);
    //void computeMomentumEquation(MatrixDiag &M, Vector &F);
    void computePressure();
    //  void computeStateEquationOld();
    void computeStrains();
    //  void computeStrainsOld(double);
    void computeStress(double timeStep);
    void computeStressDirect(double timeStep);
    //  void computeStressOld(double timeStep);
    void createIntegrationPoints();
    void initializeData();
    void setCurrentIntegrationPoint(short point);
    void setCurrentUnderIntegrationPoint(short point);
    double getIntPointValueExtract(short field, short intPoint);
    void computeDensity();
    Node *getNodeOnFace(short face, short node);
    const ElementData *getElementData() const;
    void dumpElementData() const;
    virtual void computeElasticStiffnessMatrix(bool underIntegration = false) = 0;
    virtual double getRadiusAtUnderIntegrationPoint() = 0;

    /*
    void clearIntegrationPoint();
    void detachMaterial();
    void getIntegrationPoint(long);
    void getUnderIntegrationPoint(long);
    long getLocalNumber(Node *);
    bool isNodeinElement(Node *);
    bool isNodeinElement(Vec3D );
    bool isNodeinBoundBox(Node *);
    bool isNodeinBoundBox(Vec3D );
    void toFile(FILE *);
    void computeBoundBox();


    Node *getNodeOnSideFace(short sideFace, short node);
    Node *getNodeOnEdge(short edge, short node);
    Node *getNeighbourNode(short node, short neighbour);

    void computeMassEquation(MatrixDiag &M);
    void computeEnergyEquation(MatrixDiag &M, Vector &F);
    void computeConstitutiveIntegration(MatrixDiag &M, Vector &F);
    void getFe_atIntPoint(Vec3D &force);
    void getEnergy(double &e);
    void getdTemp_atIntPoints(Vec3D &dT);
    void getSigmaAtPoint(Tensor2 &Stress, const Vec3D &point);
    void get_Sig_atNode(Tensor2 &Stress, long node);
    void get_Eps_atNode(Tensor2 &Strain, long node);
    void get_dEps_atNode(Tensor2 &StrainInc, long node);
    void get_EpsPlas_atNode(Tensor2 &Stress, long node);
    void get_evp_atNode(double &alpha, long i);
    virtual long numberOfUnderIntegrationPoints() = 0;
    virtual double getVolume() = 0;
    //bool getIntegrationPointCoords (short, Vec3D & coords, double& weight);
    bool linkIntegrationPointData(short);
    //virtualbool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
    virtual void getU_atIntPoint(Vec3D &u, int t) = 0;
    virtual void getdU_atIntPoint(Tensor2 &du, int t) = 0;
    virtual void computeGlob2Loc() = 0;
    virtual void glob2Loc(const Vec3D &point, Vec3D &local) = 0;
    //virtual //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;*/
};

bool compareElementsNumber(Element *element1, Element *element2);
long substractElementsNumber(Element *element1, const long number);

//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodes() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfNodes;
}

//-----------------------------------------------------------------------------
inline String Element::getName() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->name;
}

//-----------------------------------------------------------------------------
inline short Element::getType() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->type;
}

//-----------------------------------------------------------------------------
inline short Element::getFamily() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->family;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfFaces() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfFaces;
}

//-----------------------------------------------------------------------------
inline const ElementData *Element::getElementData() const
//-----------------------------------------------------------------------------
{
    return _elementData;
}
/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfEdges() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfEdges;
}
 */
/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfSideFaces() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfSideFaces;
}
 */
//-----------------------------------------------------------------------------
inline short Element::getNumberOfDDL() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfDDL;
}

//-----------------------------------------------------------------------------
inline short Element::getVtkType() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->vtkType;
}

//-----------------------------------------------------------------------------
inline short Element::getNumberOfDimensions() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfDimensions;
}

/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnSideFace(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfSideFaces)
        fatalError("Element::getNumberOfNodesOnSideFace",
                   "sideface %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfSideFaces);
#endif
    return _elementData->sideFaces[i].numberOfNodes;
}
 */
//-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnFace(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfFaces)
        fatalError("Element::getNumberOfNodesOnFace",
                   "face %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfFaces);
#endif
    return _elementData->faces[i].numberOfNodes;
}

/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfNodesOnEdge(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfEdges)
        fatalError("Element::getNumberOfNodesOnEdge",
                   "edge %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfEdges);
#endif
    return _elementData->edges[i].numberOfNodes;
}
 */
/* //-----------------------------------------------------------------------------
inline short Element::getNumberOfNeighbourNodes(short i) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
#ifdef VERIF_bounds
    if (i > _elementData->numberOfNodes)
        fatalError("Element::getNumberOfNeighbourNodes",
                   "node %d is out of bounds %d-%d\n", i, 0, _elementData->numberOfNodes);
#endif
    return _elementData->nodes[i].numberOfNeighbourNodes;
}
 */
//-----------------------------------------------------------------------------
inline short Element::getNumberOfIntegrationPoints() const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return _elementData->numberOfIntegrationPoints;
}

/* //-----------------------------------------------------------------------------
inline short Element::getLocalIdOfNodeOnEdge(short edge, short node) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return (_elementData->edges[edge].number[node]);
}
 */
//-----------------------------------------------------------------------------
inline Vec3D Element::getLocalNodeCoords(short node) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
    assert(_elementData != NULL);
#endif
    return (_elementData->nodes[node].localCoords);
}

//Internal number. This method gives a direct access to the internal number of the current node.
/*!
  \return Internal number of the current node.
*/
//-----------------------------------------------------------------------------
inline long &Element::internalNumber()
//-----------------------------------------------------------------------------
{
    return (_listIndex);
}

//-----------------------------------------------------------------------------
inline IntegrationPoint *Element::getIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    return integrationPoints(point);
}

//-----------------------------------------------------------------------------
inline UnderIntegrationPoint *Element::getUnderIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    return underIntegrationPoints(point);
}

//-----------------------------------------------------------------------------
inline void Element::setCurrentIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    _integrationPoint = integrationPoints(point);
}

//-----------------------------------------------------------------------------
inline void Element::setCurrentUnderIntegrationPoint(short point)
//-----------------------------------------------------------------------------
{
    _underIntegrationPoint = underIntegrationPoints(point);
}

/*


struct LateralSurface
{
    long tab[maxNumberOfNodesBySideFace];
    Element *pel;
    short face;
};
bool compareLateralSurfaces(LateralSurface *p1, LateralSurface *p2);

//------------------------------- Inline -----------------------------------
*/
#endif
