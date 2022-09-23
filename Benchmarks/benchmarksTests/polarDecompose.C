#include <benchmark.h>
#include <Tensor2.h>
#include <SymTensor2.h>

void polarDecomposeLapack(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLapack(U, R);
}
BENCHMARK(polarDecomposeLapack);

void polarDecompose(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecompose(U, R);
}
BENCHMARK(polarDecompose);

void polarDecomposeQL(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeQL(U, R);
}
BENCHMARK(polarDecomposeQL);

void polarDecomposeJacobi(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeJacobi(U, R);
}
BENCHMARK(polarDecomposeJacobi);

void polarDecomposeCuppen(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeCuppen(U, R);
}
BENCHMARK(polarDecomposeCuppen);

void polarDecomposeLapackLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLapackLnU(U, R);
}
BENCHMARK(polarDecomposeLapackLnU);

void polarDecomposeLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLnU(U, R);
}
BENCHMARK(polarDecomposeLnU);

void polarDecomposeQLLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeQLLnU(U, R);
}
BENCHMARK(polarDecomposeQLLnU);

void polarDecomposeJacobiLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeJacobiLnU(U, R);
}
BENCHMARK(polarDecomposeJacobiLnU);

void polarDecomposeCuppenLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeCuppenLnU(U, R);
}
BENCHMARK(polarDecomposeCuppenLnU);

void symetricPolarDecomposeLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLnU(U, R);
}
BENCHMARK(symetricPolarDecomposeLnU);

void symetricPolarDecompose(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecompose(U, R);
}
BENCHMARK(symetricPolarDecompose);

BENCHMARK_MAIN();
