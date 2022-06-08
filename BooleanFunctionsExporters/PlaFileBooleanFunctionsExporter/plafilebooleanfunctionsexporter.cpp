#include "plafilebooleanfunctionsexporter.h"

#include <iostream>
#include <fstream>

void PlaFileBooleanFunctionsExporter::ExportBooleanFunctions(const std::string &file_name, const std::vector < BooleanFunction > &functions) const
{
    if (functions.empty()) {
        std::cout << "PlaFileBooleanFunctionsExporter: No need to export!\n";
        return;
    }

    if (functions.size() > 1) {
        throw std::runtime_error("PlaFileBinaryFunctionsImporter: Does not support exporting few functions!");
    }

    std::ofstream file(file_name);
    if (!file.is_open())
    {
        throw std::runtime_error("PlaFileBinaryFunctionsImporter: Error while accessing the file!");
    }

    std::vector < std::string > variables = functions[0].GetVariables();
    uint64_t input_variables_count = variables.size();
    uint64_t output_variables_count = functions.size();
    std::vector < Interval > positive_intervals = functions[0].GetIntervals();
    std::vector < Interval > negative_intervals = functions[0].GetNegativeIntervals();
    uint64_t lines_count = positive_intervals.size() + negative_intervals.size();

    file << ".i " << std::to_string(input_variables_count) << "\n";
    file << ".o " << std::to_string(output_variables_count) << "\n";

    file << ".ilb";
    for (const std::string &variable : variables) {
        file << " " << variable;
    }
    file << "\n";

    file << ".ob";
    for (uint64_t i = 0; i < output_variables_count; i++) {
        std::string function_name = functions[i].GetName();
        if (function_name.empty()) {
            function_name = "o_" + std::to_string(i) + "_";
        }
        file << " " << function_name;
    }
    file << "\n";

    file << ".p " << std::to_string(lines_count) << "\n";

    for (const Interval &interval : negative_intervals) {
        for (const IntervalUnit &interval_unit : interval.GetUnits()) {
            char print_char = '-';
            if (interval_unit.value == False) {
                print_char = '0';
            } else if (interval_unit.value == True) {
                print_char = '1';
            }
            file << print_char;
        }

        file << " 0\n";
    }

    for (const Interval &interval : positive_intervals) {
        for (const IntervalUnit &interval_unit : interval.GetUnits()) {
            char print_char = '-';
            if (interval_unit.value == False) {
                print_char = '0';
            } else if (interval_unit.value == True) {
                print_char = '1';
            }
            file << print_char;
        }

        file << " 1\n";
    }
    file << ".e\n";
    file.close();
}
