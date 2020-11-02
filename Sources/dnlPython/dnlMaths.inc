/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-inc-file
//@!BEGIN = PRIVATE

%{
  #include "DiscreteFunction.h"
  #include "DiscreteFunctionSet.h"
  #include "Function.h"
  #include "Matrices.h"
  #include "Matrix.h"
  #include "MatrixDiag.h"
  #include "PolynomialFunction.h"
  #include "RampFunction.h"
  #include "SinusFunction.h"
  #include "SymTensor2.h"
  #include "Tensor2.h"
  #include "Tensor3.h"
  #include "Tensor4.h"
  #include "Vec3D.h"
  #include "Vector.h"
  #include "ColorMap.h"
%}

%include "Vec3D.h"
%extend Vec3D
{
  Vec3D __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[256];
    sprintf(temp,"[ %g, %g, %g ]", (*self)(0),(*self)(1),(*self)(2));
    return &temp[0];
  }
  double __getitem__(int i)
  {
    return (*self)(i);
  }
  void __setitem__(int i, double a)
  {
    (*self)(i) = a;
  }
}

%include "Vector.h"
%extend Vector
{
  Vector __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[256];
    if (self->getSize() >= 5)
    {
      sprintf(temp,"[ %g, %g, %g, ..., %g ]", (*self)(0),(*self)(1),(*self)(2),(*self)(self->getSize()-1));
    }
    if (self->getSize() == 4) sprintf(temp,"[ %g, %g, %g, %g ]", (*self)(0),(*self)(1),(*self)(2),(*self)(3));
    if (self->getSize() == 3) sprintf(temp,"[ %g, %g, %g ]", (*self)(0),(*self)(1),(*self)(2));
    if (self->getSize() == 2) sprintf(temp,"[ %g, %g ]", (*self)(0),(*self)(1));
    if (self->getSize() == 1) sprintf(temp,"[ %g ]", (*self)(0));
    if (self->getSize() == 0) sprintf(temp,"[]");
    return &temp[0];
  }
  double __getitem__(int i)
  {
    return (*self)(i);
  }
  void __setitem__(int i, double a)
  {
    (*self)(i) = a;
  }
}

%include "Tensor2.h"
%extend Tensor2
{
  Tensor2 __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[512];
    sprintf(temp,"[[%g, %g, %g],\n [%g, %g, %g],\n [%g, %g, %g]]", (*self)(0,0),(*self)(0,1),(*self)(0,2),(*self)(1,0),(*self)(1,1),(*self)(1,2),(*self)(2,0),(*self)(2,1),(*self)(2,2));
    return &temp[0];
  }
  double getitem(int i, int j)
  {
    return (*self)(i,j);
  }
  void setitem(int i, int j, double a)
  {
    (*self)(i,j) = a;
  }
  %pythoncode 
  {
    def __getitem__(self, idx): return self.getitem(idx[0],idx[1])
    def __setitem__(self, idx, v): return self.setitem(idx[0],idx[1], v)
  }
}

%include "Tensor3.h"
%extend Tensor3
{
  Tensor3 __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[256];
    sprintf(temp,"Tensor3 is too complex to be displayed");
    return &temp[0];
  }
  double getitem(int i, int j, int k)
  {
    return (*self)(i,j,k);
  }
  void setitem(int i, int j, int k, double a)
  {
    (*self)(i,j,k) = a;
  }
  %pythoncode 
  {
    def __getitem__(self, idx): return self.getitem(idx[0],idx[1],idx[2])
    def __setitem__(self, idx, v): return self.setitem(idx[0],idx[1],idx[2], v)
  }
}

%include "Tensor4.h"
%extend Tensor4
{
  Tensor4 __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[256];
    sprintf(temp,"Tensor4 is too complex to be displayed");
    return &temp[0];
  }
  double getitem(int i, int j, int k, int l)
  {
    return (*self)(i,j,k,l);
  }
  void setitem(int i, int j, int k, int l, double a)
  {
    (*self)(i,j,k,l) = a;
  }
  %pythoncode 
  {
    def __getitem__(self, idx): return self.getitem(idx[0],idx[1],idx[2],idx[3])
    def __setitem__(self, idx, v): return self.setitem(idx[0],idx[1],idx[2],idx[3], v)
  }
}

%include "SymTensor2.h"
%extend SymTensor2
{
  SymTensor2 __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[512];
    sprintf(temp,"[[%g, %g, %g],\n [%g, %g, %g],\n [%g, %g, %g]]", (*self)(0,0),(*self)(0,1),(*self)(0,2),(*self)(1,0),(*self)(1,1),(*self)(1,2),(*self)(2,0),(*self)(2,1),(*self)(2,2));
    return &temp[0];
  }
  double getitem(int i, int j)
  {
    return (*self)(i,j);
  }
  void setitem(int i, int j, double a)
  {
    (*self)(i,j) = a;
  }
  %pythoncode 
  {
    def __getitem__(self, idx): return self.getitem(idx[0],idx[1])
    def __setitem__(self, idx, v): return self.setitem(idx[0],idx[1], v)
  }
}

%include "Matrices.h"

%include "Matrix.h"
%extend Matrix
{
  Matrix __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[1024];
    if (self->rows() == 3 && self->columns()==3)
    {
      sprintf(temp,"[[%g, %g, %g],\n [%g, %g, %g],\n [%g, %g, %g]]", (*self)(0,0),(*self)(0,1),(*self)(0,2),(*self)(1,0),(*self)(1,1),(*self)(1,2),(*self)(2,0),(*self)(2,1),(*self)(2,2));
    }
    else if (self->rows() == 2 && self->columns()==2)
    {
      sprintf(temp,"[[%g, %g],\n [%g, %g]]", (*self)(0,0),(*self)(0,1),(*self)(1,0),(*self)(1,1));
    }
    else if (self->rows() == 4 && self->columns()==4)
    {
      sprintf(temp,"[[%g, %g, %g, %g],\n [%g, %g, %g, %g],\n [%g, %g, %g, %g],\n [%g, %g, %g, %g]]]", 
        (*self)(0,0),(*self)(0,1),(*self)(0,2),(*self)(0,3),
        (*self)(1,0),(*self)(1,1),(*self)(1,2),(*self)(1,3),
        (*self)(2,0),(*self)(2,1),(*self)(2,2),(*self)(2,3),
        (*self)(3,0),(*self)(3,1),(*self)(3,2),(*self)(3,3));
    }
    else 
    {
      sprintf(temp,"Matrix of size %ldx%ld is too complex to be displayed",self->rows(),self->columns());
    }
    return &temp[0];
  }
  double getitem(int i, int j)
  {
    return (*self)(i,j);
  }
  void setitem(int i, int j, double a)
  {
    (*self)(i,j) = a;
  }
  %pythoncode 
  {
    def __getitem__(self, idx): return self.getitem(idx[0],idx[1])
    def __setitem__(self, idx, v): return self.setitem(idx[0],idx[1], v)
  }
}

%include "MatrixDiag.h"
%extend MatrixDiag
{
  MatrixDiag __rmul__(double d) {
    return (*self)*d;
  }
  char *__str__()
  {
    static char temp[1024];
    if (self->rows() == 3)
    {
      sprintf(temp,"[[%g, 0.0, 0.0],\n [0.0, %g, 0.0],\n [0.0, 0.0, %g]]", (*self)(0),(*self)(1),(*self)(2));
    }
    else if (self->rows() == 2)
    {
      sprintf(temp,"[[%g, 0.0],\n [0.0, %g]]", (*self)(0),(*self)(1));
    }
    else if (self->rows() == 4)
    {
      sprintf(temp,"[[%g, 0.0, 0.0, 0.0],\n [0.0, %g, 0.0, 0.0],\n [0.0, 0.0, %g, 0.0],\n [0.0, 0.0, 0.0, %g]]]",(*self)(0),(*self)(1),(*self)(2),(*self)(3));
    }
    else 
    {
      sprintf(temp,"MatrixDiag of size %ldx%ld is too complex to be displayed",self->rows(),self->columns());
    }
    return &temp[0];
  }
  double __getitem__(int i)
  {
    return (*self)(i);
  }
  void __setitem__(int i, double a)
  {
    (*self)(i) = a;
  }
}

%include "Function.h"
%include "DiscreteFunction.h"
%include "DiscreteFunctionSet.h"
%include "RampFunction.h"
%include "PolynomialFunction.h"
%include "SinusFunction.h"
%include "ColorMap.h"
