// Diploma.cpp : Defines the entry point for the application.
//

#include "Diploma.h"
#include "plot/Plot.h"
#include "plot/Utils.h"

void printVector(std::ostream *os, std::vector<double> *vector);

void fillVariables(Variables *variables);

double calcIntegral0(Variables *variables);

plot::Plot* configMagneticFluidPlot(std::vector<Variables> *iterationVariables, std::vector<int> *iterationNumbers);

plot::Plot* configAnglesPlot(std::vector<Variables> *iterationVariables, std::vector<int> *iterationNumbers);

void calcResult(std::vector<Variables> &iterationVariables, std::vector<int> &iterationNumbers);

int main()
{
	std::vector<Variables> iterationVariables;

	std::vector<int> iterationNumbers;

	calcResult(iterationVariables, iterationNumbers);

	plot::Plot *magneticFluidPlot = configMagneticFluidPlot(&iterationVariables, &iterationNumbers);
	magneticFluidPlot->makeGraphs();

#ifdef _MSC_VER
    system("pause");
#else
    std::cout << "Press enter to exit...";
    std::cin.clear();
    std::cin.get();
#endif

	delete magneticFluidPlot;

	return 0;
}


void printVector(std::ostream *os, std::vector<double> *vector)
{
	for (const auto& currentComponent : *vector)
	{
		*os << currentComponent << std::endl;
	}
}


void fillVariables(Variables *variables)
{
	for (int i = 0; i < N + 1; i++)
	{
		variables->s[i] = LOWER_BOUND + i * STEP;

		variables->r[i] = variables->s[i];

		variables->z[N - i] = variables->s[i];

		variables->beta[i] = -M_PI_4;
	}
}


plot::Plot* configMagneticFluidPlot(std::vector<Variables> *iterationVariables, std::vector<int> *iterationNumbers)
{
	auto graphs = new std::vector<plot::Graph>();

	for (int i = 0; i < iterationNumbers->size(); i++)
	{
		int currentIteration = (*iterationNumbers)[i];

		std::vector<plot::Point> points;
		plot::convertComponentsVectorsToPointsVector(&((*iterationVariables)[currentIteration].r),
			&((*iterationVariables)[currentIteration].z),
			&points);

		std::stringstream titleStream;
		titleStream << "Iteration " << currentIteration;

		plot::Graph graph;
		graph.title = titleStream.str();
		graph.points = points;

		graphs->push_back(graph);
	}


	auto config = new plot::PlotConfig();
	config->title = "Magnetic fluid";
	config->xAxisName = "Radius";
	config->yAxisName = "Height";
	config->windowWidth = 800;
	config->windowHeight = 800;

	auto plot = new plot::Plot(config, graphs);

	return plot;
}


plot::Plot* configAnglesPlot(std::vector<Variables> *iterationVariables, std::vector<int> *iterationNumbers)
{
	return nullptr;
}


double calcIntegral0(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		result += tmpZ * tmpR * cos(tmpBeta);
	}

	return 2 * M_PI * STEP * result;
}


int sgn(double value)
{
	if (value < 0)
	{
		return -1;
	}
	else if (value > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


double calcL(double I0)
{
	return sgn(I0) * pow(U / fabs(I0), 1.0 / 3);
}


double calcLowerPhi(double r, double L)
{
	if (A2 == 0.0)
	{
		return 1;
	}
	double tmp = r * L / A2;

	double result = tmp * sinh(1 / tmp);

	return result;
}


double calcIntegral1(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpZ = (variables->z[i - 1] + variables->z[i]) / 2;
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L);
		result += lowerPhi * tmpZ * tmpR * cos(tmpBeta);
	}

	return 2 * M_PI * STEP * result;
}


double calcIntegral2(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpR = (variables->r[i - 1] + variables->r[i]) / 2;
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		double lowerPhi = calcLowerPhi(tmpR, variables->L);
		result += lowerPhi * tmpR * cos(tmpBeta);
	}

	return STEP * result;
}


double calcLowerGamma(double r0, double r1, double I1, double I2, double L)
{
	return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) + (A1 * U * I2) / (I1 * L * L));
}


double calcUpperPhi(double lowerGamma, double I1, double I2, double beta, double r, double z, double L)
{
	return B0 * z * L * L - (A1 * U * calcLowerPhi(r, L)) / (I1 * L * L) - sin(beta) / r + lowerGamma;
}


void calcBeta(Variables *variables, Variables *prevVariables)
{
	double r0 = variables->r[0];
	double r1 = variables->r[N];

	double I0 = calcIntegral0(variables);
	double L = variables->L = calcL(I0);
	double I1 = calcIntegral1(variables);
	double I2 = calcIntegral2(variables);

	double lowerGamma = calcLowerGamma(r0, r1, I1, I2, L);

	for (int i = N - 1; i > 0; i--)
	{
		double tmpZ = (variables->z[i] + variables->z[i + 1]) / 2;
		double tmpR = (variables->r[i] + variables->r[i + 1]) / 2;
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;

		double upperPhi = calcUpperPhi(lowerGamma, I1, I2, tmpBeta, tmpR, tmpZ, L);

		variables->beta[i] = variables->beta[i + 1] - STEP * upperPhi + 
							(1 - TAU) * (variables->beta[i] - variables->beta[i + 1] + STEP * upperPhi);
	}
}


void calcRadius(Variables *variables)
{
	variables->r[0] = 1 / variables->L;

	for (int i = 1; i < N + 1; i++)
	{
		double tmpBeta = (variables->beta[i - 1] + variables->beta[i]) / 2;
		variables->r[i] = variables->r[i - 1] + STEP * cos(tmpBeta);
	}
}


void calcHeight(Variables *variables)
{
	variables->z[N] = 0;

	for (int i = N - 1; i >= 0; i--)
	{
		double tmpBeta = (variables->beta[i] + variables->beta[i + 1]) / 2;
		variables->z[i] = variables->z[i + 1] - STEP * sin(tmpBeta);
	}
}


void calcIteration(Variables *variables, Variables *prevVariables)
{
	calcBeta(variables, prevVariables);
	calcRadius(variables);
	calcHeight(variables);
}


void calcResult(std::vector<Variables> &iterationVariables, std::vector<int> &iterationNumbers)
{
	Variables variables0;
	variables0.s = std::vector<double>(N + 1);
	variables0.r = std::vector<double>(N + 1);
	variables0.z = std::vector<double>(N + 1);
	variables0.beta = std::vector<double>(N + 1);

	fillVariables(&variables0);

	for (int i = 0; i < 1; i++)
	{
		calcIteration(&variables0, nullptr);
	}

	// printVector(&std::cout, &(variables0.s));
	// std::cout << "************************" << std::endl;
	// printVector(&std::cout, &(variables0.r));
	// std::cout << "************************" << std::endl;
	// printVector(&std::cout, &(variables0.z));
	// std::cout << "************************" << std::endl;
	// printVector(&std::cout, &(variables0.beta));
	// std::cout << "************************" << std::endl;

	iterationVariables.push_back(variables0);

	iterationNumbers.push_back(0);

	// for (int i = 1; i < 2; i++)
	// {
	// 	Variables variables;
	// 	variables.s = std::vector<double>(N + 1);
	// 	variables.r = std::vector<double>(N + 1);
	// 	variables.z = std::vector<double>(N + 1);
	// 	variables.beta = std::vector<double>(N + 1);
	//
	// 	calcIteration(&variables, &(iterationVariables[i - 1]));
	//
	// 	iterationVariables.push_back(variables);
	//
	// 	printVector(&std::cout, &(variables.s));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.r));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.z));
	// 	std::cout << "************************" << std::endl;
	// 	printVector(&std::cout, &(variables.beta));
	// 	std::cout << "************************" << std::endl;
	// }
	//
	// iterationNumbers.push_back(1);
}
