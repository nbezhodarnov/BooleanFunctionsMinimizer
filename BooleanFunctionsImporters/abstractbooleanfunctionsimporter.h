#ifndef ABSTRACTBOOLEANFUNCTIONSIMPORTER_H
#define ABSTRACTBOOLEANFUNCTIONSIMPORTER_H

#include <string>
#include <vector>

#include "BooleanFunction/booleanfunction.h"

#define IMPORTER_ERROR_VALUE std::vector < BooleanFunction > {}

class AbstractBooleanFunctionsImporter
{
public:
    AbstractBooleanFunctionsImporter() = default;
    virtual std::vector < BooleanFunction > ImportBooleanFunctions(const std::string &file_name) const = 0;
    virtual ~AbstractBooleanFunctionsImporter() = default;
};

#endif // ABSTRACTBOOLEANFUNCTIONSIMPORTER_H
