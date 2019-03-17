#include "DSAlgorithm.h"
#include "../Algorithm.h"
#include "../Formulas.h"
#include "Formulas.h"
#include "Integrals.h"
#include "../RightSweep.h"
#include "../../utils/Utils.h"
#include "../Exceptions.h"
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
}


void algorithm::ds::fillMatrices(std::vector<std::vector<double>> &radiusMatrix, std::vector<double> &radiusFreeMembers,
									 std::vector<std::vector<double>> &heightMatrix, std::vector<double> &heightFreeMembers,
									 Variables *variables)
{
	double I0 = calcIntegral0(variables);
	variables->L = calcL(I0, variables->U);
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

	calcRightSweep(radiusMatrix, radiusFreeMembers, variables->r);
	calcRightSweep(heightMatrix, heightFreeMembers, variables->z);
}


void algorithm::ds::calcIteration(Variables *variables)
{
	calcVariables(variables);
}


void algorithm::ds::runIterationProcess(Variables &variables, long long &iterationsCounter) noexcept(false)
{
	long long startIterationsCounter = iterationsCounter;
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
			variables.r[i] = (1 - variables.TAU) * prevR[i] + variables.TAU * variables.r[i];
			variables.z[i] = (1 - variables.TAU) * prevZ[i] + variables.TAU * variables.z[i];
		}

		iterationsCounter++;

		if (iterationsCounter - startIterationsCounter > MAX_ITERATIONS_NUMBER)
		{
			throw IterationsLimitException();
		}
	} while (residual > ACCURACY);

	if (!utils::isValid(variables.r) || !utils::isValid(variables.z))
	{
		throw InvalidResultException();
	}
}
