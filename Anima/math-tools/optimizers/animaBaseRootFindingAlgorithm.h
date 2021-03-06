#pragma once
#include "AnimaOptimizersExport.h"
#include <itkSingleValuedCostFunction.h>

#include <limits>
#include <cmath>

namespace anima
{

class RootToleranceBoostBridge
{
public:
    RootToleranceBoostBridge()
    {
        m_RootRelativeTolerance = std::sqrt(std::numeric_limits<double>::epsilon());
    }
    
    void SetRootRelativeTolerance(const double &tol) {m_RootRelativeTolerance = tol;}
    bool operator()(const double &a, const double &b);
    
private:
    double m_RootRelativeTolerance;
};

class RootFindingFunctionBoostBridge
{
public:
    using BaseCostFunctionType = itk::SingleValuedCostFunction;
    using BaseCostFunctionPointer = BaseCostFunctionType::Pointer;
    using ParametersType = BaseCostFunctionType::ParametersType;

    double operator()(const double &x);

    void SetRootFindingFunction(BaseCostFunctionType *f) {m_RootFindingFunction = f;}

    RootFindingFunctionBoostBridge()
    {
        m_ParametersVector.set_size(1);
    }

private:
    BaseCostFunctionPointer m_RootFindingFunction;
    ParametersType m_ParametersVector;
};

class ANIMAOPTIMIZERS_EXPORT BaseRootFindingAlgorithm
{
public:
    using BaseCostFunctionType = itk::SingleValuedCostFunction;
    using BaseCostFunctionPointer = BaseCostFunctionType::Pointer;
    using ParametersType = BaseCostFunctionType::ParametersType;

    void SetRootRelativeTolerance(const double &val) {m_RootRelativeTolerance = val;}
    void SetCostFunctionTolerance(const double &val) {m_CostFunctionTolerance = val;}
    void SetMaximumNumberOfIterations(const unsigned int &val) {m_MaximumNumberOfIterations = val;}
    void SetRootFindingFunction(BaseCostFunctionType *f) {m_RootFindingFunction = f;}
    void SetLowerBound(const double &val) {m_LowerBound = val;}
    void SetUpperBound(const double &val) {m_UpperBound = val;}
    void SetFunctionValueAtInitialLowerBound(const double &val);
    void SetFunctionValueAtInitialUpperBound(const double &val);
    
    bool GetProvidedFunctionValueAtInitialLowerBound() {return m_ProvidedFunctionValueAtInitialLowerBound;}
    bool GetProvidedFunctionValueAtInitialUpperBound() {return m_ProvidedFunctionValueAtInitialUpperBound;}
    double GetFunctionValueAtInitialLowerBound() {return m_FunctionValueAtInitialLowerBound;}
    double GetFunctionValueAtInitialUpperBound() {return m_FunctionValueAtInitialUpperBound;}
    double GetLowerBound() {return m_LowerBound;}
    double GetUpperBound() {return m_UpperBound;}
    unsigned int GetMaximumNumberOfIterations() {return m_MaximumNumberOfIterations;}
    double GetRootRelativeTolerance() {return m_RootRelativeTolerance;}
    double GetCostFunctionTolerance() {return m_CostFunctionTolerance;}
    BaseCostFunctionType *GetRootFindingFunction() {return m_RootFindingFunction;}

    virtual double Optimize() = 0;

    BaseRootFindingAlgorithm()
    {
        m_RootRelativeTolerance = std::sqrt(std::numeric_limits<double>::epsilon());
        m_CostFunctionTolerance = std::sqrt(std::numeric_limits<double>::epsilon());
        m_MaximumNumberOfIterations = 50;
        m_ProvidedFunctionValueAtInitialLowerBound = false;
        m_ProvidedFunctionValueAtInitialUpperBound = false;
        m_FunctionValueAtInitialLowerBound = 0.0;
        m_FunctionValueAtInitialUpperBound = 0.0;
    }

    virtual ~BaseRootFindingAlgorithm() {}

private:
    //! Root relative tolerance: if upper bound and lower bound are too close to each other -> consider solution is found
    double m_RootRelativeTolerance;
    //! ZCost function tolerance: if cost function value at root is close enough to zero -> consider solution is found
    double m_CostFunctionTolerance;
    unsigned int m_MaximumNumberOfIterations;
    double m_LowerBound;
    double m_UpperBound;
    bool m_ProvidedFunctionValueAtInitialLowerBound;
    bool m_ProvidedFunctionValueAtInitialUpperBound;
    double m_FunctionValueAtInitialLowerBound;
    double m_FunctionValueAtInitialUpperBound;
    BaseCostFunctionPointer m_RootFindingFunction;
};

} // end of namespace anima
