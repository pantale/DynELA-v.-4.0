BASE = /home/pantale/SynologyDrive/DynELA

INCS = -I${BASE}/Benchmarks/benchmarksLib/include/benchmark
INCS += -I${BASE}/Sources/dnlKernel
INCS += -I${BASE}/Sources/dnlMaths
INCS += -I${BASE}/Sources/dnlMaterials
INCS += -I${BASE}/Sources/dnlElements

LIBS = -L${BASE}/Benchmarks/benchmarksLib/build/src
LIBS += -L${BASE}/Build/lib

polarDecompose : polarDecompose.C
	g++ -std=c++17 polarDecompose.C -o polarDecompose ${INCS} ${LIBS} -ldnlElements -ldnlMaterials -ldnlMaths -ldnlKernel -lz -lbenchmark -lpthread

clean:
	rm polarDecompose
