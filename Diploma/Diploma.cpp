// Diploma.cpp : Defines the entry point for the application.
//

#include "Diploma.h"
#include "Plot.h"

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

//	**** working dir ****

//    char workingDir[FILENAME_MAX];
//    size_t size = sizeof(workingDir);
//    if (!GetCurrentDir(workingDir, size))
//    {
//        printf("Could not find working directory\n");
//        return errno;
//    }
//    workingDir[size - 1] = '\0';
//    printf("The current working directory is %s\n", workingDir);

//    **** file output ****

//    const char *outputFilePath = "result.dat";
//
//	FILE* resultOutput;
//
//#ifdef __clang__
//	resultOutput = fopen(outputFilePath, "w");
//#else
//	fopen_s(&resultOutput, "result.dat", "w");
//#endif
//
//	if (resultOutput == nullptr)
//	{
//		printf("Could not open output file\n");
//		return 0;
//	}

//	for(int i = 0; i < N + 1; i++)
//    {
//        fprintf(resultOutput, "%e %e\n", variables.r[i], variables.z[i]);
//    }
//
//    fclose(resultOutput);

//	**** pipe output ****

    FILE* plotPipe;

#ifdef WIN32
    plotPipe = _popen(GNUPLOT, "w");
#else
    plotPipe = popen(GNUPLOT, "w");
#endif

    if (plotPipe == nullptr)
    {
        printf("Could not open gnuplot pipe\n");
        return -1;
    }

#ifdef WIN32
    fprintf(plotPipe, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
#else
    fprintf(plotPipe, "set term qt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
#endif
    fprintf(plotPipe, "set title \"Magnetic fluid model\"\n");
    fprintf(plotPipe, "set xlabel \"Radius\"\n");
    fprintf(plotPipe, "set ylabel \"Height, m\"\n");
//#ifdef WIN32
//    fprintf(plotPipe, "plot '%s\\%s' using 1:2 with line notitle\n", workingDir, outputFilePath);
//#else
//    fprintf(plotPipe, "plot '%s/%s' using 1:2 with line notitle\n", workingDir, outputFilePath);
//#endif

    fprintf(plotPipe, "plot '-' title 'A2 = 0' with lines \n");
    for(int i = 0; i < N + 1; i++)
    {
        fprintf(plotPipe, "%lf %lf\n", variables.r[i], variables.z[i]);
    }

    fprintf(plotPipe, "e\n");

    fflush(plotPipe);

//    std::cin.clear();
//    std::cin.get();

#ifdef WIN32
    _pclose(plotPipe);
#else
    pclose(plotPipe);
#endif

#ifdef _MSC_VER
	system("pause");
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