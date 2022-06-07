#ifndef PLAFILEBOOLEANFUNCTIONSIMPORTER_H
#define PLAFILEBOOLEANFUNCTIONSIMPORTER_H

#include "BooleanFunctionsImporters/abstractbooleanfunctionsimporter.h"

class PlaFileBooleanFunctionsImporter : public AbstractBooleanFunctionsImporter
{
public:
    PlaFileBooleanFunctionsImporter() = default;
    std::vector < BooleanFunction > ImportBooleanFunctions(const std::string &file_name) const override;

private:
    std::vector < std::string > SplitStringByDelemeter(const std::string &string, const std::string &delimiter) const;
    BooleanFunction GenerateFunctionWithoutContradictions(const std::vector < std::string > &variables, const std::vector < Interval > &intervals, const std::vector < bool > &values) const;
};

#endif // PLAFILEBOOLEANFUNCTIONSIMPORTER_H
