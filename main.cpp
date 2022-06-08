#include <iostream>

#include "BooleanFunction/booleanfunction.h"
#include "BooleanFunctionMinimizers/BlakeBooleanFunctionMinimizer/blakebooleanfunctionminimizer.h"
#include "BooleanFunctionsExporters/PlaFileBooleanFunctionsExporter/plafilebooleanfunctionsexporter.h"
#include "BooleanFunctionsImporters/PlaFileBooleanFunctionsImporter/plafilebooleanfunctionsimporter.h"

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "Usage: <input_file> <output_file> <function_index>\n";
        return 0;
    }
    try {
        AbstractBooleanFunctionsImporter *importer = new PlaFileBooleanFunctionsImporter();
        std::string input_file_name = argv[1];
        std::vector < BooleanFunction > functions = importer->ImportBooleanFunctions(input_file_name);
        AbstractBooleanFunctionMinimizer *minimizer = new BlakeBooleanFunctionMinimizer();
        uint64_t function_index = std::stoul(argv[3]);
        if (function_index >= functions.size()) {
            throw std::runtime_error("main: Index is greater or equal functions count!");
        }
        BooleanFunction minimized_function = minimizer->Minimize(functions[function_index]);
        AbstractBooleanFunctionsExporter *exporter = new PlaFileBooleanFunctionsExporter();
        std::string output_file_name = argv[2];
        exporter->ExportBooleanFunctions(output_file_name, {minimized_function});
    } catch (std::runtime_error const &exception) {
        std::cerr << exception.what() << "\n";
    }
    return 0;
}
