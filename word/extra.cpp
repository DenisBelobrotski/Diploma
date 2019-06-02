// DifferenceMethod.h

namespace algorithm
{
    class DifferenceMethod
    {
    protected:
        Variables variables;
        long long iterationsCounter;
        long long experimentsCounter;
        std::function<void(long long, long long)>* iterationFinishedCallback;
        InitialParameters* initialParameters;
    private:
        std::vector<IterationInfo>* iterationsInfo;
        std::vector<algorithm::TargetParameter>* targetParameters;
        bool isLastExperiment;
        bool isNeedResetTau;
        std::string currentParameterName;
    public:
        DifferenceMethod(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);
        virtual ~DifferenceMethod();
        void calcResult();
        void setIsNeedResetTau(bool isNeedResetTau);
        bool getIsNeedResetTau();
        void setIterationFinishedCallback(std::function<void(long long, long long)>* iterationFinishedCallback);
        std::function<void(long long, long long)>* getIterationFinishedCallback();
        void setTargetParameters(std::vector<TargetParameter>* targetParameters);
        std::vector<TargetParameter>* getTargetParameters();
    protected:
        DifferenceMethod();
        virtual void runIterationProcess() = 0;
    private:
        void fillVariables();
        void resetFields();
        /**
         * @param drawRate:
         * -2 doesn't push any experiments
         * -1 pushes only last experiment
         */
        void changeParameter(
                double& parameter, double target, double step, long long drawRate, const std::string& parameterName)
        noexcept(false);
        void increaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);
        void decreaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);
        void runExperiment(long long drawRate) noexcept(false);
        void pushExperimentResults();
        double* getVariableParameterPointerByName(const std::string& parameterName);
    };
}


// DifferenceMethod.cpp

algorithm::DifferenceMethod::DifferenceMethod() = default;
algorithm::DifferenceMethod::DifferenceMethod(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
{
    if (iterationsInfo == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: iterationsInfo is null");
    }
    if (initialParameters == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: initialParameters is null");
    }
    this->iterationsInfo = iterationsInfo;
    this->initialParameters = initialParameters;
    variables.s = std::vector<double>(initialParameters->N + 1);
    variables.r = std::vector<double>(initialParameters->N + 1);
    variables.z = std::vector<double>(initialParameters->N + 1);
    variables.beta = std::vector<double>(initialParameters->N + 1);
    isNeedResetTau = false;
}
algorithm::DifferenceMethod::~DifferenceMethod() = default;
void algorithm::DifferenceMethod::fillVariables()
{
    for (auto i = 0; i < initialParameters->N + 1; i++)
    {
        variables.s[i] = LOWER_BOUND + i * initialParameters->STEP;

        variables.r[i] = 1 + variables.s[i];

        variables.z[initialParameters->N - i] = variables.s[i];

        variables.beta[i] = -M_PI_4;
    }
    variables.L = 0;
    variables.TAU = initialParameters->INITIAL_TAU;
    variables.U = initialParameters->INITIAL_U;
    variables.B0 = initialParameters->INITIAL_B0;
    variables.A1 = initialParameters->INITIAL_A1;
    variables.A2 = initialParameters->INITIAL_A2;
    variables.ALPHA = initialParameters->INITIAL_ALPHA;
}
void algorithm::DifferenceMethod::resetFields()
{
    iterationsCounter = 0;
    experimentsCounter = 0;
    isLastExperiment = false;
    fillVariables();
}
void algorithm::DifferenceMethod::changeParameter(
        double& parameter, double target, double step, long long drawRate,
        const std::string& parameterName) noexcept(false)
{
    if (isNeedResetTau)
    {
        variables.TAU = initialParameters->INITIAL_TAU;
    }
    bool isCalculated = false;
    const Variables startVariables = variables;
    if (drawRate != -2)
    {
        std::cout << "Start changing parameter " << parameterName << std::endl;
        std::cout << "Current value: " << parameter << ", target value: " << target << std::endl;
        std::cout << "Step: " << step << std::endl;
        std::cout << "////////////" << std::endl;
    }
    while (variables.TAU >= initialParameters->MIN_RELAXATION_PARAMETER && !isCalculated)
    {
        try
        {
            isLastExperiment = false;
            if (target > parameter)
            {
                increaseParameter(parameter, target, step, drawRate);
            }
            else if (target < parameter)
            {
                decreaseParameter(parameter, target, step, drawRate);
            }
            isCalculated = true;
        }
        catch (InvalidResultException& e)
        {
            std::cerr << "Invalid result:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= initialParameters->RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }
        catch (IterationsLimitException& e)
        {
            std::cerr << "Too many iterations:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= initialParameters->RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }
        if (!isCalculated)
        {
            double newTau = variables.TAU;
            variables = startVariables;
            variables.TAU = newTau;
        }
    }
    if (!isCalculated)
    {
        throw ParameterNotReachTargetValue(parameterName, target);
    }
    if (drawRate != -2)
    {
        std::cout << "Finish changing parameter " << parameterName << " to target value " << target << std::endl;
        std::cout << "////////////" << std::endl << std::endl;
    }
}
void algorithm::DifferenceMethod::increaseParameter(double& parameter, double target, double step, long long drawRate)
{
    while (parameter <= target)
    {
        if (parameter + step >= target)
        {
            isLastExperiment = true;
        }
        runExperiment(drawRate);
        if (isLastExperiment)
        {
            break;
        }
        parameter += step;
    }
}

void algorithm::DifferenceMethod::decreaseParameter(double& parameter, double target, double step, long long drawRate)
{
    while (parameter >= target)
    {
        if (parameter - step <= target)
        {
            isLastExperiment = true;
        }
        runExperiment(drawRate);
        if (isLastExperiment)
        {
            break;
        }
        parameter -= step;
    }
}


void algorithm::DifferenceMethod::runExperiment(long long drawRate)
{
    runIterationProcess();
    if (
            (drawRate > 0 && experimentsCounter % drawRate == 0)
            || (isLastExperiment && drawRate != -2)
            )
    {
        pushExperimentResults();
    }
    experimentsCounter++;
}


void algorithm::DifferenceMethod::pushExperimentResults()
{
    IterationInfo currentIterationInfo{};
    Variables resultVariables = variables;
    convertLengthToRadiusDimensionedVariables(resultVariables);
    currentIterationInfo.index = experimentsCounter;
    currentIterationInfo.variables = resultVariables;
    currentIterationInfo.mainValueName = currentParameterName;
    iterationsInfo->push_back(currentIterationInfo);
}


double* algorithm::DifferenceMethod::getVariableParameterPointerByName(const std::string& parameterName)
{
    double* resultPtr = nullptr;
    if (parameterName == "U")
    {
        resultPtr = &(variables.U);
    }
    if (parameterName == "B0")
    {
        resultPtr = &(variables.B0);
    }
    if (parameterName == "A1")
    {
        resultPtr = &(variables.A1);
    }
    if (parameterName == "A2")
    {
        resultPtr = &(variables.A2);
    }
    if (parameterName == "ALPHA")
    {
        resultPtr = &(variables.ALPHA);
    }
    return resultPtr;
}
void algorithm::DifferenceMethod::calcResult()
{
    resetFields();
    iterationsCounter = 0;
    experimentsCounter = 0;
    try
    {
        if (targetParameters != nullptr)
        {
            for (auto& targetParameter : *targetParameters)
            {
                double* variableParameterPtr = getVariableParameterPointerByName(targetParameter.parameterName);
                currentParameterName = targetParameter.parameterName;
                if (variableParameterPtr != nullptr)
                {
                    changeParameter(
                            *variableParameterPtr, targetParameter.targetValue, targetParameter.step,
                            targetParameter.drawRate, targetParameter.parameterName);
                }
            }
        }
    }
    catch (ParameterNotReachTargetValue& e)
    {
        std::cerr << "Parameter " << e.getParameterName() << " didn't reach target value "
                  << e.getTargetValue() << std::endl;
    }

    std::cout << "iterations count: " << iterationsCounter << std::endl;
    std::cout << "experiments count: " << experimentsCounter << std::endl;
    std::cout << "************************" << std::endl;
}
void algorithm::DifferenceMethod::setIsNeedResetTau(bool isNeedResetTau)
{
    this->isNeedResetTau = isNeedResetTau;
}
bool algorithm::DifferenceMethod::getIsNeedResetTau()
{
    return isNeedResetTau;
}
void algorithm::DifferenceMethod::setIterationFinishedCallback(
        std::function<void(long long, long long)>* iterationFinishedCallback)
{
    this->iterationFinishedCallback = iterationFinishedCallback;
}
std::function<void(long long, long long)>* algorithm::DifferenceMethod::getIterationFinishedCallback()
{
    return iterationFinishedCallback;
}
void algorithm::DifferenceMethod::setTargetParameters(std::vector<TargetParameter>* targetParameters)
{
    this->targetParameters = targetParameters;
}
std::vector<algorithm::TargetParameter>* algorithm::DifferenceMethod::getTargetParameters()
{
    return targetParameters;
}


// ExplicitDifferenceMethod.h

#define SIMPLE_RELAXATION_FORMULA 1
namespace algorithm
{
    class ExplicitDifferenceMethod : public DifferenceMethod
    {
    public:
        ExplicitDifferenceMethod(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);
        ~ExplicitDifferenceMethod() override;
        void runIterationProcess() noexcept(false) override;
    protected:
        ExplicitDifferenceMethod();
        virtual double calcLowerPhi(double r, double L, double A2);
        virtual double calcLowerGamma(
                double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA);
        virtual double calcUpperPhi(
                double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U,
                double B0, double A1, double A2);
        virtual double calcIntegral0();
        virtual double calcIntegral1();
        virtual double calcIntegral2();
    private:
        void calcBeta();
        void calcRadius();
        void calcHeight();
        void calcIteration();
    };
}


// ExplicitDifferenceMethod.cpp

algorithm::ExplicitDifferenceMethod::ExplicitDifferenceMethod() : DifferenceMethod()
{}
algorithm::ExplicitDifferenceMethod::ExplicitDifferenceMethod(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        DifferenceMethod(iterationsInfo, initialParameters)
{}
algorithm::ExplicitDifferenceMethod::~ExplicitDifferenceMethod() = default;
void algorithm::ExplicitDifferenceMethod::calcBeta()
{
    double r0 = variables.r[0];
    double r1 = variables.r[initialParameters->N];

    double I0 = calcIntegral0();
    double L = variables.L = calcL(I0, variables.U);
    double I1 = calcIntegral1();
    double I2 = calcIntegral2();

    double lowerGamma = calcLowerGamma(r0, r1, I1, I2, L,
                                       variables.U, variables.B0,
                                       variables.A1, variables.ALPHA);
#if SIMPLE_RELAXATION_FORMULA
    std::vector<double> prevBeta = variables.beta;
#endif
    for (auto i = initialParameters->N - 1; i > 0; i--)
    {
        double tmpZ = (variables.z[i] + variables.z[i + 1]) / 2;
        double tmpR = (variables.r[i] + variables.r[i + 1]) / 2;
        double tmpBeta = (variables.beta[i] + variables.beta[i + 1]) / 2;
        double upperPhi = calcUpperPhi(lowerGamma, I1, I2, tmpBeta, tmpR, tmpZ, L,
                                       variables.U, variables.B0, variables.A1, variables.A2);
#if SIMPLE_RELAXATION_FORMULA
        variables.beta[i] = variables.beta[i + 1] - initialParameters->STEP * upperPhi;
#else
        variables.beta[i] = variables.beta[i + 1] - STEP * upperPhi +
            (1 - variables.TAU) * (variables.beta[i] - variables.beta[i + 1] + STEP * upperPhi);
#endif
    }
#if SIMPLE_RELAXATION_FORMULA
    for (auto i = 0; i < initialParameters->N + 1; i++)
    {
        variables.beta[i] = (1 - variables.TAU) * prevBeta[i] + variables.TAU * variables.beta[i];
    }
#endif
}
void algorithm::ExplicitDifferenceMethod::calcRadius()
{
    variables.r[0] = 1 / variables.L;

    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        variables.r[i] = variables.r[i - 1] + initialParameters->STEP * cos(tmpBeta);
    }
}
void algorithm::ExplicitDifferenceMethod::calcHeight()
{
    variables.z[initialParameters->N] = 0;
    for (auto i = initialParameters->N - 1; i >= 0; i--)
    {
        double tmpBeta = (variables.beta[i] + variables.beta[i + 1]) / 2;
        variables.z[i] = variables.z[i + 1] - initialParameters->STEP * sin(tmpBeta);
    }
}
void algorithm::ExplicitDifferenceMethod::calcIteration()
{
    calcBeta();
    calcRadius();
    calcHeight();
}
void algorithm::ExplicitDifferenceMethod::runIterationProcess() noexcept(false)
{
    long long startIterationsCounter = iterationsCounter;
    double residual;
    std::vector<double> prevR;
    std::vector<double> prevZ;
    do
    {
        prevR = variables.r;
        prevZ = variables.z;
        calcIteration();
        residual = std::max(calcResidual(prevR, variables.r),
                            calcResidual(prevZ, variables.z));
        iterationsCounter++;
        if (iterationsCounter - startIterationsCounter > initialParameters->MAX_ITERATIONS_NUMBER)
        {
            throw IterationsLimitException();
        }
        else
        {
            if (iterationFinishedCallback != nullptr)
            {
                (*iterationFinishedCallback)(
                        iterationsCounter - startIterationsCounter, initialParameters->MAX_ITERATIONS_NUMBER);
            }
        }
    } while (residual > initialParameters->ACCURACY);
    if (!isValid(variables.r) || !isValid(variables.z))
    {
        throw InvalidResultException();
    }
}
double algorithm::ExplicitDifferenceMethod::calcLowerPhi(double r, double L, double A2)
{
    if (A2 == 0.0)
    {
        return 1;
    }
    double tmp = r * L / A2;
    double invertTmp = 1 / tmp;
    double sinH = sinh(invertTmp);
    double result = tmp * sinH;
    return result;
}
double algorithm::ExplicitDifferenceMethod::calcLowerGamma(
        double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA)
{
    return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) +
                                        (A1 * U * I2) / (I1 * L * L));
}
double algorithm::ExplicitDifferenceMethod::calcUpperPhi(
        double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U, double B0,
        double A1, double A2)
{
    return B0 * z * L * L - (A1 * U * calcLowerPhi(r, L, A2)) / (I1 * L * L) - sin(beta) / r + lowerGamma;
}
double algorithm::ExplicitDifferenceMethod::calcIntegral0()
{
    double result = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double tmpZ = (variables.z[i - 1] + variables.z[i]) / 2;
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double tmp = tmpZ * tmpR * cos(tmpBeta);
        result += tmp;
    }
    result *= 2 * M_PI * initialParameters->STEP;
    return result;
}
double algorithm::ExplicitDifferenceMethod::calcIntegral1()
{
    double result = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double tmpZ = (variables.z[i - 1] + variables.z[i]) / 2;
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double lowerPhi = calcLowerPhi(tmpR, variables.L, variables.A2);
        double tmp = lowerPhi * tmpZ * tmpR * cos(tmpBeta);
        result += tmp;
    }
    result *= 2 * M_PI * initialParameters->STEP;
    return result;
}
double algorithm::ExplicitDifferenceMethod::calcIntegral2()
{
    double result = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double tmpR = (variables.r[i - 1] + variables.r[i]) / 2;
        double tmpBeta = (variables.beta[i - 1] + variables.beta[i]) / 2;
        double lowerPhi = calcLowerPhi(tmpR, variables.L, variables.A2);
        double tmp = lowerPhi * tmpR * cos(tmpBeta);
        result += tmp;
    }
    result *= initialParameters->STEP;
    return result;
}


// ExplicitMethodUniformConcentration.h

namespace algorithm
{
    class ExplicitMethodUniformConcentration : public ExplicitDifferenceMethod
    {
    public:
        ExplicitMethodUniformConcentration(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);
        ~ExplicitMethodUniformConcentration() override;
    protected:
        ExplicitMethodUniformConcentration();
        double calcLowerGamma(
                double r0, double r1, double I1, double I2, double L, double U, double B0, double A1,
                double ALPHA) override;
        double calcUpperPhi(
                double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U,
                double B0, double A1, double A2) override;
    };
}


// ExplicitMethodUniformConcentration.cpp

algorithm::ExplicitMethodUniformConcentration::ExplicitMethodUniformConcentration() : ExplicitDifferenceMethod()
{}
algorithm::ExplicitMethodUniformConcentration::ExplicitMethodUniformConcentration(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        ExplicitDifferenceMethod(iterationsInfo, initialParameters)
{}
algorithm::ExplicitMethodUniformConcentration::~ExplicitMethodUniformConcentration() = default;
double algorithm::ExplicitMethodUniformConcentration::calcLowerGamma(
        double r0, double r1, double I1, double I2, double L, double U, double B0, double A1, double ALPHA)
{
    return (2 / (r1 * r1 - r0 * r0)) * (r0 * cos(ALPHA) - r1 * sin(ALPHA) - (B0 * U) / (2 * M_PI * L) +
                                        A1 * I2);
}
double algorithm::ExplicitMethodUniformConcentration::calcUpperPhi(
        double lowerGamma, double I1, double I2, double beta, double r, double z, double L, double U, double B0,
        double A1, double A2)
{
    return B0 * z * L * L - A1 * calcLowerPhi(r, L, A2) - sin(beta) / r + lowerGamma;
}


// ImplicitDifferenceMethod.h

namespace algorithm
{
    class ImplicitDifferenceMethod : public DifferenceMethod
    {
    public:
        ImplicitDifferenceMethod(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);
        ~ImplicitDifferenceMethod() override;
        void runIterationProcess() noexcept(false) override;
    protected:
        ImplicitDifferenceMethod();
        virtual double calcUpperPhi(int i, double lowerGamma, double I1);
        virtual double calcLowerPhi(int i);
        virtual double calcLowerGamma(double I1, double I2);
        virtual double calcIntegral0();
        virtual double calcIntegral1();
        virtual double calcIntegral2();
    private:
        void calcIteration();
        void fillMatrices(
                std::vector<std::vector<double>>& radiusMatrix, std::vector<double>& radiusFreeMembers,
                std::vector<std::vector<double>>& heightMatrix, std::vector<double>& heightFreeMembers);
    };
}


// ImplicitDifferenceMethod.cpp

algorithm::ImplicitDifferenceMethod::ImplicitDifferenceMethod() : DifferenceMethod()
{}
algorithm::ImplicitDifferenceMethod::ImplicitDifferenceMethod(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        DifferenceMethod(iterationsInfo, initialParameters)
{}
algorithm::ImplicitDifferenceMethod::~ImplicitDifferenceMethod() = default;
void algorithm::ImplicitDifferenceMethod::fillMatrices(
        std::vector<std::vector<double>>& radiusMatrix, std::vector<double>& radiusFreeMembers,
        std::vector<std::vector<double>>& heightMatrix, std::vector<double>& heightFreeMembers)
{
    double I0 = calcIntegral0();
    variables.L = calcL(I0, variables.U);
    double I1 = calcIntegral1();
    double I2 = calcIntegral2();
    double lowerGamma = calcLowerGamma(I1, I2);
    double upperPhi0 = calcUpperPhi(0, lowerGamma, I1);
    double upperPhiN = calcUpperPhi((int) (initialParameters->N), lowerGamma, I1);
    heightMatrix[0][0] = 1;
    heightMatrix[0][1] = -1;
    heightFreeMembers[0] = -0.5 * initialParameters->STEP * initialParameters->STEP * upperPhi0 *
                           sin(initialParameters->INITIAL_ALPHA) +
                           initialParameters->STEP * cos(initialParameters->INITIAL_ALPHA);

    radiusMatrix[0][0] = 1;
    radiusMatrix[0][1] = 0;
    radiusFreeMembers[0] = 1 / variables.L;
    for (auto i = 1; i < initialParameters->N; i++)
    {
        double upperFi = calcUpperPhi(i, lowerGamma, I1);
        heightMatrix[i][i - 1] = 1;
        heightMatrix[i][i] = -2;
        heightMatrix[i][i + 1] = 1;
        heightFreeMembers[i] = 0.5 * initialParameters->STEP * (variables.r[i + 1] - variables.r[i - 1]) * upperFi;
        radiusMatrix[i][i - 1] = 1;
        radiusMatrix[i][i] = -2;
        radiusMatrix[i][i + 1] = 1;
        radiusFreeMembers[i] = -0.5 * initialParameters->STEP * (variables.z[i + 1] - variables.z[i - 1]) * upperFi;
    }
    heightMatrix[initialParameters->N][initialParameters->N - 1] = 0;
    heightMatrix[initialParameters->N][initialParameters->N] = 1;
    heightFreeMembers[initialParameters->N] = 0;
    radiusMatrix[initialParameters->N][initialParameters->N - 1] = -1;
    radiusMatrix[initialParameters->N][initialParameters->N] = 1;
    radiusFreeMembers[initialParameters->N] = -0.5 * initialParameters->STEP * initialParameters->STEP * upperPhiN *
                                              sin(initialParameters->INITIAL_ALPHA) +
                                              initialParameters->STEP * cos(initialParameters->INITIAL_ALPHA);
}
void algorithm::ImplicitDifferenceMethod::calcIteration()
{
    std::vector<double> radiusFreeMembers(initialParameters->N + 1);
    std::vector<double> heightFreeMembers(initialParameters->N + 1);
    std::vector<std::vector<double>> radiusMatrix(initialParameters->N + 1);
    std::vector<std::vector<double>> heightMatrix(initialParameters->N + 1);
    for (auto i = 0; i < initialParameters->N + 1; i++)
    {
        radiusMatrix[i].resize(initialParameters->N + 1);
        heightMatrix[i].resize(initialParameters->N + 1);
    }
    fillMatrices(radiusMatrix, radiusFreeMembers, heightMatrix, heightFreeMembers);
    calcRightSweep(radiusMatrix, radiusFreeMembers, variables.r);
    calcRightSweep(heightMatrix, heightFreeMembers, variables.z);
}
void algorithm::ImplicitDifferenceMethod::runIterationProcess() noexcept(false)
{
    long long startIterationsCounter = iterationsCounter;
    double residual;
    std::vector<double> prevR;
    std::vector<double> prevZ;
    do
    {
        prevR = variables.r;
        prevZ = variables.z;
        calcIteration();
        residual = std::max(calcResidual(prevR, variables.r),
                            calcResidual(prevZ, variables.z));
        for (auto i = 0; i < initialParameters->N + 1; i++)
        {
            variables.r[i] = (1 - variables.TAU) * prevR[i] + variables.TAU * variables.r[i];
            variables.z[i] = (1 - variables.TAU) * prevZ[i] + variables.TAU * variables.z[i];
        }
        iterationsCounter++;
        if (iterationsCounter - startIterationsCounter > initialParameters->MAX_ITERATIONS_NUMBER)
        {
            throw IterationsLimitException();
        }
        else
        {
            if (iterationFinishedCallback != nullptr)
            {
                (*iterationFinishedCallback)(
                        iterationsCounter - startIterationsCounter, initialParameters->MAX_ITERATIONS_NUMBER);
            }
        }
    } while (residual > initialParameters->ACCURACY);
    if (!isValid(variables.r) || !isValid(variables.z))
    {
        throw InvalidResultException();
    }
}
double algorithm::ImplicitDifferenceMethod::calcLowerPhi(int i)
{
    if (i == 0)
    {
        return sinh(variables.A2) / variables.A2;
    }
    double tmp = variables.L * variables.r[i] / variables.A2;
    return tmp * sinh(1 / tmp);
}
double algorithm::ImplicitDifferenceMethod::calcLowerGamma(double I1, double I2)
{
    double sum = variables.r[0] * cos(initialParameters->INITIAL_ALPHA) -
                 variables.r[initialParameters->N] * sin(initialParameters->INITIAL_ALPHA) -
                 variables.B0 * variables.U / (2 * M_PI * variables.L) +
                 variables.A1 * variables.U * I2 / (I1 * variables.L * variables.L);
    return 2 * sum / (variables.r[initialParameters->N] * variables.r[initialParameters->N] -
                      variables.r[0] * variables.r[0]);
}
double algorithm::ImplicitDifferenceMethod::calcUpperPhi(int i, double lowerGamma, double I1)
{
    double sum = -variables.A1 * variables.U * calcLowerPhi(i) / (I1 * variables.L * variables.L) + lowerGamma;
    if (i == 0)
    {
        return variables.B0 * variables.L * variables.L * variables.z[i] + sum +
               variables.L * cos(initialParameters->INITIAL_ALPHA);
    }
    if (i == initialParameters->N)
    {
        return sum + sin(initialParameters->INITIAL_ALPHA) / variables.r[i];
    }
    return variables.B0 * variables.L * variables.L * variables.z[i] + sum -
           ((variables.z[i + 1] - variables.z[i - 1]) / (2 * initialParameters->STEP * variables.r[i]));
}
double algorithm::ImplicitDifferenceMethod::calcIntegral0()
{
    double sum = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double height = (variables.z[i] + variables.z[i - 1]) / 2;
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];

        sum += height * radiusQuadDiff;
    }
    return M_PI * sum;
}
double algorithm::ImplicitDifferenceMethod::calcIntegral1()
{
    double sum = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double height = (variables.z[i] + variables.z[i - 1]) / 2;
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];
        double lowerPhi = calcLowerPhi(i);
        sum += height * radiusQuadDiff * lowerPhi;
    }
    return M_PI * sum;
}
double algorithm::ImplicitDifferenceMethod::calcIntegral2()
{
    double sum = 0;
    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];
        double lowerPhi = calcLowerPhi(i);
        sum += radiusQuadDiff * lowerPhi;
    }
    return sum / 2;
}


// ImplicitMethodUniformConcentration.h

namespace algorithm
{
    class ImplicitMethodUniformConcentration : public ImplicitDifferenceMethod
    {
    public:
        ImplicitMethodUniformConcentration(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);
        ~ImplicitMethodUniformConcentration() override;
    protected:
        ImplicitMethodUniformConcentration();
        double calcLowerGamma(double I1, double I2) override;
        double calcUpperPhi(int i, double lowerGamma, double I1) override;
    };
}


// ImplicitMethodUniformConcentration.cpp

algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration() : ImplicitDifferenceMethod()
{}


algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        ImplicitDifferenceMethod(iterationsInfo, initialParameters)
{}
algorithm::ImplicitMethodUniformConcentration::~ImplicitMethodUniformConcentration() = default;
double algorithm::ImplicitMethodUniformConcentration::calcLowerGamma(double I1, double I2)
{
    double sum = variables.r[0] * cos(initialParameters->INITIAL_ALPHA) -
                 variables.r[initialParameters->N] * sin(initialParameters->INITIAL_ALPHA) -
                 variables.B0 * variables.U / (2 * M_PI * variables.L) + variables.A1 * I2;

    return 2 * sum / (variables.r[initialParameters->N] * variables.r[initialParameters->N] -
                      variables.r[0] * variables.r[0]);
}
double algorithm::ImplicitMethodUniformConcentration::calcUpperPhi(int i, double lowerGamma, double I1)
{
    double sum = -variables.A1 * calcLowerPhi(i) + lowerGamma;
    if (i == 0)
    {
        return variables.B0 * variables.L * variables.L * variables.z[i] + sum +
               variables.L * cos(initialParameters->INITIAL_ALPHA);
    }
    if (i == initialParameters->N)
    {
        return sum + sin(initialParameters->INITIAL_ALPHA) / variables.r[i];
    }
    return variables.B0 * variables.L * variables.L * variables.z[i] + sum -
           ((variables.z[i + 1] - variables.z[i - 1]) / (2 * initialParameters->STEP * variables.r[i]));
}

