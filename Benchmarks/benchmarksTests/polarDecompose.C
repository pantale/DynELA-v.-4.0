#include <benchmark.h>
#include <Tensor2.h>
#include <SymTensor2.h>

void polarLapack(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLapack(U, R);
}
BENCHMARK(polarLapack);

void polar(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polar(U, R);
}
BENCHMARK(polar);

void polarQL(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarQL(U, R);
}
BENCHMARK(polarQL);

void polarJacobi(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarJacobi(U, R);
}
BENCHMARK(polarJacobi);

void polarCuppen(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarCuppen(U, R);
}
BENCHMARK(polarCuppen);

void polarLapackLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLapackLnU(U, R);
}
BENCHMARK(polarLapackLnU);

void polarLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLnU(U, R);
}
BENCHMARK(polarLnU);

void polarQLLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarQLLnU(U, R);
}
BENCHMARK(polarQLLnU);

void polarJacobiLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarJacobiLnU(U, R);
}
BENCHMARK(polarJacobiLnU);

void polarCuppenLnU(benchmark::State &state)
{
	Tensor2 T(1, 5, 3, 7, 4, 5, 6, 8, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarCuppenLnU(U, R);
}
BENCHMARK(polarCuppenLnU);

void symmetricPolarDecomposeLapack(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLapack(U, R);
}
BENCHMARK(symmetricPolarDecomposeLapack);

void symmetricPolarDecompose(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polar(U, R);
}
BENCHMARK(symmetricPolarDecompose);

void symmetricPolarDecomposeQL(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarQL(U, R);
}
BENCHMARK(symmetricPolarDecomposeQL);

void symmetricPolarDecomposeJacobi(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarJacobi(U, R);
}
BENCHMARK(symmetricPolarDecomposeJacobi);

void symmetricPolarDecomposeCuppen(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarCuppen(U, R);
}
BENCHMARK(symmetricPolarDecomposeCuppen);

void symmetricPolarDecomposeLapackLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLapackLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeLapackLnU);

void symmetricPolarDecomposeLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeLnU);

void symmetricPolarDecomposeQLLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarQLLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeQLLnU);

void symmetricPolarDecomposeJacobiLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarJacobiLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeJacobiLnU);

void symmetricPolarDecomposeCuppenLnU(benchmark::State &state)
{
	SymTensor2 T(1, 6, 4.5, 4, 6.5, -1);
	Tensor2 R;
	SymTensor2 U;
	for (auto _ : state)
		T.polarCuppenLnU(U, R);
}
BENCHMARK(symmetricPolarDecomposeCuppenLnU);

BENCHMARK_MAIN();
