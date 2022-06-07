#include <iostream>

#include "BooleanFunction/booleanfunction.h"
#include "BooleanFunctionMinimizers/BlakeBooleanFunctionMinimizer/blakebooleanfunctionminimizer.h"
#include "BooleanFunctionsExporters/PlaFileBooleanFunctionsExporter/plafilebooleanfunctionsexporter.h"
#include "BooleanFunctionsImporters/PlaFileBooleanFunctionsImporter/plafilebooleanfunctionsimporter.h"

int main()
{
    try {
        AbstractBooleanFunctionsImporter *importer = new PlaFileBooleanFunctionsImporter();
        std::string input_file_name = "first_test.pla";
        std::vector < BooleanFunction > functions = importer->ImportBooleanFunctions(input_file_name);
        AbstractBooleanFunctionMinimizer *minimizer = new BlakeBooleanFunctionMinimizer();
        BooleanFunction minimized_function = minimizer->Minimize(functions[0]);
        AbstractBooleanFunctionsExporter *exporter = new PlaFileBooleanFunctionsExporter();
        std::string output_file_name = "first_test_minimized.pla";
        exporter->ExportBooleanFunctions(output_file_name, {minimized_function});
    } catch (std::runtime_error const &exception) {
        std::cerr << exception.what() << "\n";
    }
    return 0;
}
