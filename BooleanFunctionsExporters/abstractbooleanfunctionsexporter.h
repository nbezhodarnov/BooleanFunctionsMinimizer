#ifndef ABSTRACTBOOLEANFUNCTIONSEXPORTER_H
#define ABSTRACTBOOLEANFUNCTIONSEXPORTER_H

#include <string>
#include <vector>

#include "BooleanFunction/booleanfunction.h"

#define IMPORTER_ERROR_VALUE std::vector < BooleanFunction > {}

class AbstractBooleanFunctionsExporter
{
public:
    AbstractBooleanFunctionsExporter() = default;
    virtual void ExportBooleanFunctions(const std::string &file_name, const std::vector < BooleanFunction > &functions) const = 0;
    virtual ~AbstractBooleanFunctionsExporter() = default;
};

#endif // ABSTRACTBOOLEANFUNCTIONSEXPORTER_H
