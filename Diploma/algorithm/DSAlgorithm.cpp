#include "DSAlgorithm.h"
#include "Algorithm.h"
#include "Formulas.h"
#include "Integrals.h"
#include "RightSweep.h"
#include "../utils/Utils.h"
#include <vector>
#include <algorithm>
#include <math.h>


namespace algorithm::ds
{
	void calcIteration(Variables *variables);

	void calcVariables(Variables *variables);

	void fillMatrices(std::vector<std::vector<double>> &radiusMatrix, std::vector<double> &radiusFreeMembers,
					  std::vector<std::vector<double>> &heightMatrix, std::vector<double> &heightFreeMembers,
					  Variables *variables);

	double calcUpperPhi(int i, Variables *variables, double lowerGamma, double I1);

	double calcLowerPhi(int i, Variables *variables);

	double calcLowerGamma(Variables *variables, double I1, double I2);

	double calcIntegral0(Variables *variables);

	double calcIntegral1(Variables *variables);

	double calcIntegral2(Variables *variables);
}


double algorithm::ds::calcIntegral0(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double height = (variables->z[i] + variables->z[i - 1]) / 2;
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
								variables->r[i - 1] * variables->r[i - 1];

		sum += height * radiusQuadDiff;
	}

	return M_PI * sum;
}

double algorithm::ds::calcIntegral1(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double height = (variables->z[i] + variables->z[i - 1]) / 2;
		double radius = (variables->r[i] + variables->r[i - 1]) / 2;
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
								variables->r[i - 1] * variables->r[i - 1];		
		
		//TODO: call calcLowerPhi
		double tmp = variables->L * radius / variables->A2;
		double phi = tmp * sinh(1 / tmp);

		sum += height * radiusQuadDiff * phi;
	}

	return M_PI * sum;
}

double algorithm::ds::calcIntegral2(Variables *variables)
{
	double sum = 0;

	for (auto i = 1; i < N + 1; i++)
	{
		double radius = (variables->r[i] + variables->r[i - 1]) / 2;
		double radiusQuadDiff = variables->r[i] * variables->r[i] -
			variables->r[i - 1] * variables->r[i - 1];
		double tmp = variables->L * radius / variables->A2;

		//TODO: call calcLowerPhi
		double phi = tmp * sinh(1 / tmp);
		sum += radiusQuadDiff * phi;
	}

	return sum / 2;
}


double algorithm::ds::calcLowerGamma(Variables *variables, double I1, double I2)
{
	double sum = variables->r[0] * cos(INITIAL_ALPHA) - variables->r[N] * sin(INITIAL_ALPHA) -
		variables->B0 * variables->U / (2 * M_PI * variables->L) +
		variables->A1 * variables->U * I2 / (I1 * variables->L * variables->L);

	return 2 * sum / (variables->r[N] * variables->r[N] -
		variables->r[0] * variables->r[0]);
}


double algorithm::ds::calcLowerPhi(int i, Variables *variables)
{
	if (i == 0)
	{
		return sinh(variables->A2) / variables->A2;
	}

	double tmp = variables->L * variables->r[i] / variables->A2;
	return tmp * sinh(1 / tmp);
}


double algorithm::ds::calcUpperPhi(int i, Variables *variables, double lowerGamma, double I1)
{
	double sum = -variables->A1 * variables->U * calcLowerPhi(i, variables) / (I1 * variables->L * variables->L) + lowerGamma;

	if (i == 0)
	{
		return variables->B0 * variables->L * variables->L * variables->z[i] + sum +
			variables->L * cos(INITIAL_ALPHA);
	}

	if (i == N)
	{
		return sum + sin(INITIAL_ALPHA) / variables->r[i];
	}

	return variables->B0 * variables->L * variables->L * variables->z[i] + sum -
		((variables->z[i + 1] - variables->z[i - 1]) / (2 * STEP * variables->r[i]));
}


void algorithm::ds::fillMatrices(std::vector<std::vector<double>> &radiusMatrix, std::vector<double> &radiusFreeMembers,
									 std::vector<std::vector<double>> &heightMatrix, std::vector<double> &heightFreeMembers,
									 Variables *variables)
{
	double r0 = variables->r[0];
	double r1 = variables->r[N];

	double I0 = calcIntegral0(variables);
	double L = variables->L = calcL(I0, variables->U);
	double I1 = calcIntegral1(variables);
	double I2 = calcIntegral2(variables);

	double lowerGamma = calcLowerGamma(variables, I1, I2);

	double upperPhi0 = calcUpperPhi(0, variables, lowerGamma, I1);
	double upperPhiN = calcUpperPhi(N, variables, lowerGamma, I1);

	heightMatrix[0][0] = 1;
	heightMatrix[0][1] = -1;
	heightFreeMembers[0] = -0.5 * STEP * STEP * upperPhi0 * sin(INITIAL_ALPHA) + STEP * cos(INITIAL_ALPHA);

	radiusMatrix[0][0] = 1;
	radiusMatrix[0][1] = 0;
	radiusFreeMembers[0] = 1 / variables->L;

	for (int i = 1; i < N; i++)
	{
		double upperFi = calcUpperPhi(i, variables, lowerGamma, I1);

		heightMatrix[i][i - 1] = 1;
		heightMatrix[i][i] = -2;
		heightMatrix[i][i + 1] = 1;
		heightFreeMembers[i] = 0.5 * STEP * (variables->r[i + 1] - variables->r[i - 1]) * upperFi;

		radiusMatrix[i][i - 1] = 1;
		radiusMatrix[i][i] = -2;
		radiusMatrix[i][i + 1] = 1;
		radiusFreeMembers[i] = -0.5 * STEP * (variables->z[i + 1] - variables->z[i - 1]) * upperFi;
	}

	heightMatrix[N][N - 1] = 0;
	heightMatrix[N][N] = 1;
	heightFreeMembers[N] = 0;

	radiusMatrix[N][N - 1] = -1;
	radiusMatrix[N][N] = 1;
	radiusFreeMembers[N] = -0.5 * STEP * STEP * upperPhiN * sin(INITIAL_ALPHA) + STEP * cos(INITIAL_ALPHA);
}


void algorithm::ds::calcVariables(Variables *variables)
{
	std::vector<double> radiusFreeMembers(N + 1);
	std::vector<double> heightFreeMembers(N + 1);
	std::vector<std::vector<double>> radiusMatrix(N + 1);
	std::vector<std::vector<double>> heightMatrix(N + 1);
	for (auto i = 0; i < N + 1; i++)
	{
		radiusMatrix[i].resize(N + 1);
		heightMatrix[i].resize(N + 1);
	}

	fillMatrices(radiusMatrix, radiusFreeMembers, heightMatrix, heightFreeMembers, variables);

	// std::vector<double> prevR = variables->r;
	// std::vector<double> prevZ = variables->z;

	calcRightSweep(radiusMatrix, radiusFreeMembers, variables->r);
	calcRightSweep(heightMatrix, heightFreeMembers, variables->z);
}


void algorithm::ds::calcIteration(Variables *variables)
{
	calcVariables(variables);
}


void algorithm::ds::runIterationProcess(Variables &variables, long long &iterationsCounter)
{
	double residual;

	std::vector<double> prevR;
	std::vector<double> prevZ;

	do
	{
		prevR = variables.r;
		prevZ = variables.z;

		calcIteration(&variables);

		residual = std::max(utils::calcResidual(prevR, variables.r),
							utils::calcResidual(prevZ, variables.z));

		for (auto i = 0; i < N + 1; i++)
		{
			variables.r[i] = (1 - TAU) * prevR[i] + TAU * variables.r[i];
			variables.z[i] = (1 - TAU) * prevZ[i] + TAU * variables.z[i];
		}

		iterationsCounter++;
	} while (residual > ACCURACY);
}
