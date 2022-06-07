#ifndef PLAFILEBOOLEANFUNCTIONSEXPORTER_H
#define PLAFILEBOOLEANFUNCTIONSEXPORTER_H

#include "BooleanFunctionsExporters/abstractbooleanfunctionsexporter.h"

class PlaFileBooleanFunctionsExporter : public AbstractBooleanFunctionsExporter
{
public:
    PlaFileBooleanFunctionsExporter() = default;
    void ExportBooleanFunctions(const std::string &file_name, const std::vector < BooleanFunction > &functions) const override;
};

#endif // PLAFILEBOOLEANFUNCTIONSEXPORTER_H
