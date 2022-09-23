/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

#ifndef __dnlMaths_Eigen3x3_h__
#define __dnlMaths_Eigen3x3_h__

#include <dnlKernel.h>

int dsyevq3(double A[3][3], double Q[3][3], double w[3]); // QL with implicit shifts
int dsyevj3(double A[3][3], double Q[3][3], double w[3]); // Jacobi
int dsyevd3(double A[3][3], double Q[3][3], double w[3]); // Cuppen

#endif