// Diploma.cpp : Defines the entry point for the application.
//

#include "Diploma.h"
#include "plot/Plot.h"
#include "plot/Utils.h"

void fillVariables(Variables *variables);

double calcIntegral0(Variables *variables);

void printVector(std::ostream *os, std::vector<double> *vector);

int main()
{
	Variables variables;
	variables.s = std::vector<double>(N + 1);
	variables.r = std::vector<double>(N + 1);
	variables.z = std::vector<double>(N + 1);
	variables.beta = std::vector<double>(N + 1);

	fillVariables(&variables);

	printVector(&std::cout, &(variables.s));
	std::cout << "************************" << std::endl;
	printVector(&std::cout, &(variables.r));
	std::cout << "************************" << std::endl;
	printVector(&std::cout, &(variables.z));
	std::cout << "************************" << std::endl;
	printVector(&std::cout, &(variables.beta));
	std::cout << "************************" << std::endl;

	std::cout << calcIntegral0(&variables) << std::endl;

    std::vector<plot::Point> points0;
    plot::convertComponentsVectorsToPointsVector(&(variables.r), &(variables.z), &points0);

    plot::Graph graph0;
    graph0.title = "Graph 0";
    graph0.points = points0;

    std::vector<plot::Graph> graphs;
    graphs.push_back(graph0);

    plot::PlotConfig config0;
    config0.title = "Plot 0";
    config0.xAxisName = "Radius";
    config0.yAxisName = "Height";
    config0.windowWidth = 800;
    config0.windowHeight = 800;

    plot::Plot plot0(&config0, &graphs);

    plot0.makeGraphs();

#ifdef _MSC_VER
    system("pause");
#else
    std::cout << "Press enter to exit...";
    std::cin.clear();
    std::cin.get();
#endif

	return 0;
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


double calcIntegral0(Variables *variables)
{
	double result = 0;

	for (int i = 1; i < N + 1; i++)
	{
		double a = (variables->z[i - 1] + variables->z[i]) / 2;
		double b = (variables->r[i - 1] + variables->r[i]) / 2;
		double c = (variables->beta[i - 1] + variables->beta[i]) / 2;
		result += a * b * cos(c);
	}

	return 2 * M_PI * STEP * result;
}


void printVector(std::ostream *os, std::vector<double> *vector)
{
	for (const auto& currentComponent : *vector)
	{
		*os << currentComponent << std::endl;
	}
}
