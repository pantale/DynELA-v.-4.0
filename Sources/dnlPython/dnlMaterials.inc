/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%{
  #include "ElasticLaw.h"
  #include "ElastoplasticLaw.h"
  #include "HardeningLaw.h"
  #include "IntegrationPoint.h"
  #include "JohnsonCookLaw.h"
  #include "ANN2SigLaw.h"
  #include "ANNFortranLaw.h"
  #include "Material.h"
  #include "TabularLaw.h"
%}

%include "IntegrationPoint.h"

%include "Material.h"
%include "HardeningLaw.h"

%include "ElasticLaw.h"
%include "ElastoplasticLaw.h"
%include "JohnsonCookLaw.h"
%include "ANN2SigLaw.h"
%include "ANNFortranLaw.h"
%include "TabularLaw.h"
