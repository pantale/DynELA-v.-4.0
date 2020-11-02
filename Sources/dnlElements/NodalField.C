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
  \file NodalField.C
  \brief Definition file for the NodalField class

  This file is the definition file for the NodalField class. A NodalField contains all Finite Element data for each node of the structure.

  \ingroup dnlElements
*/

#include <NodalField.h>

//-----------------------------------------------------------------------------
NodalField::NodalField()
//-----------------------------------------------------------------------------
{
  // initialize all values
  resetValues();
}

//-----------------------------------------------------------------------------
NodalField::NodalField(const NodalField &X)
//-----------------------------------------------------------------------------
{
  std::cout << "Copy of a NodalField" << std::endl;
}

//-----------------------------------------------------------------------------
NodalField::~NodalField()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void NodalField::resetValues()
//-----------------------------------------------------------------------------
{
  // initializations
  //density = 0.0;
  //densityInc = 0.0;
  //energy = 0.0;
  //energyInc = 0.0;
  //displacement = 0.0;
  displacement = 0.0;
  //flux = 0.0;
  // force = 0.0;
  speed = 0.0;
  acceleration = 0.0;
}

//-----------------------------------------------------------------------------
void NodalField::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  //os << "density=" << density << ", energy=" << energy << std::endl;
  //  os << "density=" << density << ", energy=" << energy << ", temperature=" << temperature << std::endl;
  os << "acc=" << acceleration << std::endl;
  os << "delta_depl=" << displacement << std::endl;
}

//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const NodalField &field)
//-----------------------------------------------------------------------------
{
  field.print(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const NodalField &intPt)
//-----------------------------------------------------------------------------
{
  intPt.write(os);
  return os;
}

//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, NodalField &intPt)
//-----------------------------------------------------------------------------
{
  intPt.read(is);
  return is;
}

//-----------------------------------------------------------------------------
void NodalField::write(std::ofstream &pfile) const
//-----------------------------------------------------------------------------
{
  // pfile.write((char *)&density, sizeof(double));
  //  pfile.write((char *)&densityInc, sizeof(double));
  //pfile.write((char *)&energy, sizeof(double));
  //pfile.write((char *)&energyInc, sizeof(double));
  //  pfile.write((char *)&temperature, sizeof(double));

  pfile << speed;
  pfile << acceleration;
  //pfile << displacement;
  pfile << displacement;
  // pfile << force;
}

//-----------------------------------------------------------------------------
NodalField &NodalField::read(std::ifstream &pfile)
//-----------------------------------------------------------------------------
{
  //pfile.read((char *)&density, sizeof(double));
  //pfile.read((char *)&densityInc, sizeof(double));
  // pfile.read((char *)&energy, sizeof(double));
  //pfile.read((char *)&energyInc, sizeof(double));
  //  pfile.read((char *)&temperature, sizeof(double));

  pfile >> speed;
  pfile >> acceleration;
  //pfile >> displacement;
  pfile >> displacement;
  // pfile >> force;

  return *this;
}

//-----------------------------------------------------------------------------
long NodalField::objectSize()
//-----------------------------------------------------------------------------
{
  return sizeof(*this);
}
