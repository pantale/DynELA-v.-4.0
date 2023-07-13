set PATH=%PATH%;D:\Luciano\Numerico\Libs\swigwin-4.1.1;D:\Luciano\Numerico\Libs\zlib-1.2.13


CHANGE NAME TO lapacke_mangling.in to .h

INCLUDE_DIRECTORIES("${LAPACK_SRC_DIR}/LAPACKE/include")
INCLUDE_DIRECTORIES("${BLAS_SRC_DIR}/include")

RENAME both LPACKE AND BLAS mangling .pc to in

There is also the following error:

remember to generate makefiles of lapack with -DLAPACKE=ON -BUILD_SHARED_LIBS=ON -DCBLAS=ON
undefined reference to `cblas_dgemm'


--------------------------------------------------------
Error `_gfortran_concat_string'

https://github.com/Reference-LAPACK/lapack/issues/305

Verified this issue with MinGW 8.1.0 and LAPACK 3.9.0. Found a solution here.

After running CMake, open the SRC\CMakeFiles\lapack.dir\link.txt in the build directory

Add the following sentence as an option of x86_64-w64-mingw32-gfortran.exe:
-Wl,--allow-multiple-definition

Just paste it at the end, was a long line in the third line of the link.txt file.




cmake ..\DynELA-v.-4.0\Sources -G "MinGW Makefiles" -DLAPACK_SRC_DIR=D:\Luciano\Numerico\Libs\lapack-3.11.0 -DLAPACK_BIN_DIR=D:\Luciano\Numerico\Libs\lapack-3.11.0_bin\lib -DSWIG_DIR=D:\Luciano\Numerico\Libs\swigwin-4.1.1

if zlib is not found:

cmake ..\DynELA-v.-4.0\Sources -G "MinGW Makefiles" -DLAPACK_SRC_DIR=D:\Luciano\Numerico\Libs\lapack-3.11.0 -DLAPACK_BIN_DIR=D:\Luciano\Numerico\Libs\lapack-3.11.0_bin\lib -DSWIG_DIR=D:\Luciano\Numerico\Libs\swigwin-4.1.1 -DZLIB_LIBRARY_RELEASE=D:\Luciano\Numerico\Libs\zlib-1.2.13_bin


set PATH=%PATH%;D:\Luciano\Numerico\Libs\swigwin-4.1.1;D:\Luciano\Numerico\Libs\zlib-1.2.13;D:\Luciano\Numerico\Libs\zlib-1.2.13_bin

Could be an error in with ZLIB: No rule to make target 'XXX/zlib-1.2.13_binset', needed by 'lib/_dnlPython.pyd'.  Stop.


