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

// begin date : 14/03/1997

/*
  Class HardeningLaw definition
*/

/*!
  \file HardeningLaw.h
  \brief fichier .h de definition de la classe generique des materiaux
  \ingroup femLibrary

  Ce fichier sert à la definition la classe generique des materiaux.

*/

#include <HardeningLaw.h>

//-----------------------------------------------------------------------------
HardeningLaw::HardeningLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HardeningLaw::HardeningLaw(const HardeningLaw &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
HardeningLaw::~HardeningLaw()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void HardeningLaw::add(Material *material)
//-----------------------------------------------------------------------------
{
  _material = material;
}

//-----------------------------------------------------------------------------
int HardeningLaw::getType()
//-----------------------------------------------------------------------------
{
  return _lawType;
}

//-----------------------------------------------------------------------------
bool HardeningLaw::isYield()
//-----------------------------------------------------------------------------
{
  return _yieldLaw;
}

//-----------------------------------------------------------------------------
String HardeningLaw::getName()
//-----------------------------------------------------------------------------
{
  return _lawName;
}

/* //-----------------------------------------------------------------------------
void HardeningLaw::plotToFile(String name, double epsMax)
//-----------------------------------------------------------------------------
{
  FILE* pfile;

  pfile=fopen(name.chars(),"w");
  if (pfile==NULL)
    {
      CreateException(FileNotOpen exception,"toto");
      exception.print();
      cerr << "Unable to open file "<<name<<"\n";
      exit(-1);
    }

  plot(pfile,epsMax);

  fclose(pfile);
}

//-----------------------------------------------------------------------------
String HardeningLaw::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
//   String str;
//   char sstr[1000];
//   str="";

// //   sprintf(sstr,"%s.setHeatCoefficient(%12.7E);\n",name.chars(),heatCoeff());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setDilatation(%12.7E);\n",name.chars(),dilat());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setInitTemperature(%12.7E);\n",name.chars(),initTemp());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setConductivity(%12.7E);\n",name.chars(),conductivity());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setYoung(%12.7E);\n",name.chars(),young());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setPoisson(%12.7E);\n",name.chars(),poissonRatio());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setDensity(%12.7E);\n",name.chars(),density());
// //   str+=sstr;
// //   sprintf(sstr,"%s.setColor(%12.7E, %12.7E, %12.7E);\n",name.chars(),color(0),color(1),color(2));
// //   str+=sstr;

   return "";
}
 */
