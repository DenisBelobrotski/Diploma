#include "RightSweep.h"
#include <vector>
#include <cmath>
#include <iostream>
#include "../utils/Utils.h"


namespace algorithm
{
	bool isCorrectMatrix(std::vector<std::vector<double>> &matrix, size_t dimension);

	void calcRightSweepAlpha(std::vector<std::vector<double>> &matrix, size_t dimension, std::vector<double> &alpha);

	void calcRightSweepBeta(std::vector<std::vector<double>> &matrix, std::vector<double> &freeMembers,
		std::vector<double> &alpha, std::vector<double> &beta);

	void reversal(std::vector<double> &alpha, std::vector<double> &beta, std::vector<double> &result);
}


void algorithm::calcRightSweep(std::vector<std::vector<double>> &matrix, 
							   std::vector<double> &freeMembers, 
							   std::vector<double> &result)
{
	const auto dimension = freeMembers.size();
	if (!isCorrectMatrix(matrix, dimension))
	{
		std::cerr << "Incorrect matrix (right sweep)" << std::endl;
	}
	std::vector<double> alpha(dimension - 1);
	std::vector<double> beta(dimension);
	calcRightSweepAlpha(matrix, dimension, alpha);
	calcRightSweepBeta(matrix, freeMembers, alpha, beta);
	reversal(alpha, beta, result);
}

bool algorithm::isCorrectMatrix(std::vector<std::vector<double>> &matrix, const size_t dimension)
{
	if (fabs(matrix[0][0]) < fabs(matrix[0][1]))
	{
		return false;
	}

	if (fabs(matrix[dimension - 1][dimension - 1]) < fabs(matrix[dimension - 1][dimension - 2]))
	{
		return false;
	}

	for (auto i = 1; i < dimension - 1; i++)
	{
		if (fabs(matrix[i][i]) < (fabs(matrix[i][i - 1]) + fabs(matrix[i][i + 1])))
		{
			return false;
		}
	}

	return true;
}

void algorithm::calcRightSweepAlpha(std::vector<std::vector<double>> &matrix, const size_t dimension, std::vector<double> &alpha)
{
	alpha[0] = -matrix[0][1] / matrix[0][0];
	for (auto i = 1; i < dimension - 1; i++) 
	{
		alpha[i] = -matrix[i][i + 1] / (matrix[i][i] - (-matrix[i][i - 1]) * alpha[i - 1]);
	}
}

void algorithm::calcRightSweepBeta(std::vector<std::vector<double>> &matrix, std::vector<double> &freeMembers,
			  std::vector<double> &alpha, std::vector<double> &beta)
{
	const auto dimension = freeMembers.size();
	beta[0] = freeMembers[0] / matrix[0][0];
	for (auto i = 1; i < dimension; i++) 
	{
		beta[i] = freeMembers[i] + (-matrix[i][i - 1]) * beta[i - 1];
		beta[i] /= (matrix[i][i] - (-matrix[i][i - 1]) * alpha[i - 1]);
	}
}


void algorithm::reversal(std::vector<double> &alpha, std::vector<double> &beta, std::vector<double> &result)
{
	const auto dimension = beta.size();
	result[dimension - 1] = beta[dimension - 1];
	for (long long i = dimension - 2; i >= 0; i--) 
	{
		result[i] = alpha[i] * result[i + 1] + beta[i];
	}
}
