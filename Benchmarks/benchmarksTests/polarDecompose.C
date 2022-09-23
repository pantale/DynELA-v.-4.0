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

void symmetricPolarDecomposeLapack(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLapack(U, R);
}
BENCHMARK(symmetricPolarDecomposeLapack);

void symmetricPolarDecompose(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecompose(U, R);
}
BENCHMARK(symmetricPolarDecompose);

void symmetricPolarDecomposeQL(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeQL(U, R);
}
BENCHMARK(symmetricPolarDecomposeQL);

void symmetricPolarDecomposeJacobi(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeJacobi(U, R);
}
BENCHMARK(symmetricPolarDecomposeJacobi);

void symmetricPolarDecomposeCuppen(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeCuppen(U, R);
}
BENCHMARK(symmetricPolarDecomposeCuppen);

void symmetricPolarDecomposeLapackLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLapackLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeLapackLnU);

void symmetricPolarDecomposeLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeLnU);

void symmetricPolarDecomposeQLLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeQLLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeQLLnU);

void symmetricPolarDecomposeJacobiLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeJacobiLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeJacobiLnU);

void symmetricPolarDecomposeCuppenLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarDecomposeCuppenLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeCuppenLnU);

BENCHMARK_MAIN();
