#include "plafilebooleanfunctionsimporter.h"

#include <iostream>
#include <fstream>

std::vector < BooleanFunction > PlaFileBooleanFunctionsImporter::ImportBooleanFunctions(const std::string &file_name) const
{
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        throw std::runtime_error("PlaFileBinaryFunctionsImporter: Error while accessing the file!");
    }

    uint64_t input_variables_count = 0;
    uint64_t output_variables_count = 0;
    uint64_t lines_count = 0;
    uint64_t real_lines_count = 0;

    std::vector < std::string > variables;
    std::vector < std::string > functions_names;

    std::vector < Interval > intervals;
    std::vector < std::vector < bool > > values;

    while (!file.eof())
    {
        std::string file_data = "";
        std::getline(file, file_data);
        std::vector < std::string > splited_data = SplitStringByDelemeter(file_data, " ");

        if (splited_data[0] == ".i")
        {
            if (input_variables_count == 0)
            {
                input_variables_count = std::stoul(splited_data[1]);
                if (input_variables_count == 0)
                {
                    std::cerr << "PlaFileBinaryFunctionsImporter: No input variables!\n";
                }
                else
                {
                    for (unsigned int j = 0; j < input_variables_count; j++)
                    {
                        std::string variable_name = "x";
                        variable_name += std::to_string(j + 1);
                        variables.push_back(variable_name);
                    }
                }
            }
        }
        else if (splited_data[0] == ".ilb")
        {
            variables.clear();
            for (unsigned int j = 0; j < input_variables_count; j++)
            {
                std::string variable_name = splited_data[1 + j];
                variables.push_back(variable_name);
            }
        }
        else if (splited_data[0] == ".o")
        {
            if (output_variables_count == 0)
            {
                output_variables_count = std::stoul(splited_data[1]);
                if (output_variables_count == 0)
                {
                    std::cerr << "PlaFileBinaryFunctionsImporter: No output variables!\n";
                }
                else
                {
                    values.resize(output_variables_count);
                    for (unsigned int j = 0; j < output_variables_count; j++)
                    {
                        std::string function_name = "o_";
                        function_name += std::to_string(j);
                        functions_names.push_back(function_name);
                    }
                }
            }
        }
        else if (splited_data[0] == ".ob")
        {
            functions_names.clear();
            for (unsigned int j = 0; j < output_variables_count; j++)
            {
                std::string function_name = splited_data[1 + j];
                functions_names.push_back(function_name);
            }
        }
        else if (splited_data[0] == ".p")
        {
            lines_count = std::stoul(splited_data[1]);
        }
        else if (splited_data[0] == ".e")
        {
            if (lines_count && real_lines_count != lines_count) {
                std::cerr << "PlaFileBinaryFunctionsImporter: Numbers of intervals differs!\n";
            }
            break;
        }
        else if (!file_data.empty())
        {
            real_lines_count++;
            if (splited_data[0].size() != input_variables_count)
            {
                std::cout << "PlaFileBinaryFunctionsImporter: Info about input variables and actual state are different!\n";
                variables.clear();
                input_variables_count = splited_data[0].size();
            }
            Interval interval;
            if (variables.empty())
            {
                for (unsigned int j = 0; j < splited_data[0].size(); j++)
                {
                    std::string variable_name = "x";
                    variable_name += std::to_string(j + 1);
                    variables.push_back(variable_name);
                }
            }
            for (unsigned int j = 0; j < splited_data[0].size(); j++)
            {
                BooleanValue value = AnyValue;
                if (splited_data[0][j] == '0') {
                    value = False;
                } else if (splited_data[0][j] == '1') {
                    value = True;
                }
                interval.AppendUnit({variables[j], value});
            }
            if (splited_data[1].back() == '\n')
            {
                splited_data[1].pop_back();
            }
            if (splited_data[1].size() != output_variables_count)
            {
                std::cout << "PlaFileBinaryFunctionsImporter: Info about output variables and actual state are different!\n";
                values.resize(splited_data[1].size());
                output_variables_count = splited_data[1].size();
            }

            intervals.push_back(interval);
            for (unsigned int j = 0; j < splited_data[1].size(); j++)
            {
                values[j].push_back(splited_data[1][j] != '0');
            }
        }
    }
    file.close();

    std::vector < BooleanFunction > functions;
    for (uint64_t i = 0; i < output_variables_count; i++) {
        functions.push_back(GenerateFunctionWithoutContradictions(variables, intervals, values[i], functions_names[i]));
    }
    return functions;
}

std::vector < std::string > PlaFileBooleanFunctionsImporter::SplitStringByDelemeter(const std::string &string, const std::string &delimiter) const
{
    std::string string_copy = string;

    std::vector < std::string > result = {};
    size_t position = 0;
    std::string token;
    while ((position = string_copy.find(delimiter)) != std::string::npos) {
        token = string_copy.substr(0, position);
        if (!token.empty()) {
            result.push_back(token);
        }
        string_copy.erase(0, position + delimiter.length());
    }
    if (!string_copy.empty()) {
        if (string_copy.back() == '\n') {
            string_copy.pop_back();
        }
        result.push_back(string_copy);
    }
    return result;
}

BooleanFunction PlaFileBooleanFunctionsImporter::GenerateFunctionWithoutContradictions(const std::vector < std::string > &variables, const std::vector < Interval > &intervals, const std::vector < bool > &values, const std::string &name) const
{
    std::vector < Interval > positive_intervals, negative_intervals;
    for (uint64_t i = 0; i < intervals.size(); i++) {
        bool interval_exists = false;
        auto positive_interval = positive_intervals.begin();
        for (; positive_interval != positive_intervals.end(); positive_interval++) {
            if (*positive_interval == intervals[i]) {
                interval_exists = true;
                break;
            }
        }
        if (interval_exists) {
            if (values[i] == false) {
                positive_interval = positive_intervals.erase(positive_interval);
            }
            continue;
        }

        interval_exists = false;
        auto negative_interval = negative_intervals.begin();
        for (; negative_interval != negative_intervals.end(); negative_interval++) {
            if (*negative_interval == intervals[i]) {
                interval_exists = true;
                break;
            }
        }
        if (interval_exists) {
            continue;
        }

        Interval interval = intervals[i];
        if (values[i] == false) {
            positive_interval = positive_intervals.begin();
            for (; positive_interval != positive_intervals.end(); positive_interval++) {
                if (positive_interval->IntersectionExists(interval)) {
                    std::vector < Interval > substraction = Interval::CalculateSubstraction(*positive_interval, interval);
                    positive_intervals.insert(positive_intervals.end(), substraction.begin(), substraction.end());
                    positive_interval = positive_intervals.erase(positive_interval);
                    positive_interval--;
                }
            }
            negative_intervals.push_back(interval);
        } else {
            std::vector < Interval > new_intervals = {interval};
            negative_interval = negative_intervals.begin();
            for (; negative_interval != negative_intervals.end(); negative_interval++) {
                for (auto new_interval = new_intervals.begin(); new_interval != new_intervals.end(); new_interval++) {
                    if (negative_interval->IntersectionExists(*new_interval)) {
                        std::vector < Interval > substraction = Interval::CalculateSubstraction(*new_interval, *negative_interval);
                        new_intervals.insert(new_intervals.end(), substraction.begin(), substraction.end());
                        new_interval = new_intervals.erase(new_interval);
                        new_interval--;
                    }
                }
            }
            if (interval == Interval()) {
                continue;
            }
            positive_intervals.push_back(interval);
        }
    }

    return BooleanFunction(variables, positive_intervals, negative_intervals, name);
}
