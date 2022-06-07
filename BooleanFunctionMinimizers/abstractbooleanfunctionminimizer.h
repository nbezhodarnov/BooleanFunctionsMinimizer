#ifndef ABSTRACTBOOLEANFUNCTIONMINIMIZER_H
#define ABSTRACTBOOLEANFUNCTIONMINIMIZER_H

#include "BooleanFunction/booleanfunction.h"

class AbstractBooleanFunctionMinimizer
{
public:
    AbstractBooleanFunctionMinimizer() = default;
    virtual ~AbstractBooleanFunctionMinimizer() = default;
    virtual BooleanFunction Minimize(const BooleanFunction &function) const = 0;
};

#endif // ABSTRACTBOOLEANFUNCTIONMINIMIZER_H
