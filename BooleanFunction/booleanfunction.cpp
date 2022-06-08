#include "booleanfunction.h"

#include <algorithm>
#include <iostream>

Interval::Interval(const std::vector < IntervalUnit > &new_units) : units(new_units)
{

}

bool IntervalUnit::operator==(const IntervalUnit &other) const
{
    return variable_name == other.variable_name && value == other.value;
}

void Interval::AppendUnit(const IntervalUnit &new_unit)
{
    for (unsigned int i = 0; i < units.size(); i++)
    {
        if (new_unit.variable_name > units[i].variable_name)
        {
            continue;
        }
        if (new_unit.variable_name == units[i].variable_name)
        {
            units[i].value = new_unit.value;
            return;
        }
        units.insert(units.begin() + i, new_unit);
        return;
    }
    units.push_back(new_unit);
}

void Interval::RemoveUnitAt(const unsigned int &index)
{
    if (index >= units.size())
    {
        throw std::runtime_error("Interval: Index is out of array!");
    }
    units.erase(units.begin() + index);
}

void Interval::RemoveUnitByVariableName(const std::string &variable_name)
{
    units.erase(std::find_if(
                          units.begin(),
                          units.end(),
                          [&](const IntervalUnit &unit)
                          {return unit.variable_name == variable_name;}
    ));
}

void Interval::EditUnitByVariableName(const std::string &variable_name, const BooleanValue &new_value)
{
    auto unit = std::find_if(
                    units.begin(),
                    units.end(),
                    [&](const IntervalUnit &unit)
                    {return unit.variable_name == variable_name;}
    );
    if (unit != units.end()) {
        unit->value = new_value;
    } else {
        throw std::runtime_error("Interval: Unit with name \"" + variable_name + "\" does not exist!");
    }
}

std::vector<IntervalUnit> Interval::GetUnits() const
{
    return units;
}

IntervalUnit Interval::GetUnitByVariableName(const std::string &variable) const
{
    IntervalUnit result;
    for (const IntervalUnit &unit : units)
    {
        if (unit.variable_name == variable)
        {
            result = unit;
            break;
        }
    }
    return result;
}

bool Interval::IsAbsorbed(const Interval &other) const
{
    if (*this == other) {
        return true;
    } else if (units.size() != other.units.size()) {
        return false;
    }

    for (uint64_t i = 0; i < units.size(); i++) {
        if (units[i].variable_name != other.units[i].variable_name) {
            return false;
        }
        if (units[i].value != other.units[i].value && other.units[i].value != AnyValue) {
            return false;
        }
    }

    return true;
}

bool Interval::Absorbs(const Interval &other) const
{
    return other.IsAbsorbed(*this);
}

bool Interval::IntersectionExists(const Interval &other) const
{
    if (*this == other) {
        return true;
    } else if (units.size() != other.units.size()) {
        return false;
    }

    for (uint64_t i = 0; i < units.size(); i++) {
        if (units[i].variable_name != other.units[i].variable_name) {
            return false;
        }
        bool different_values = units[i].value != other.units[i].value;
        bool contains_any_value = units[i].value == AnyValue || other.units[i].value == AnyValue;
        if (different_values || !contains_any_value) {
            return false;
        }
    }

    return true;
}

Interval Interval::CalculateSum(const Interval &first, const Interval &second)
{
    if (first == second) {
        return first;
    } else if (first.units.size() != second.units.size()) {
        throw std::runtime_error("Interval: Impossible to sum intervals with different dimentions!");
    }

    int64_t different_values_count = 0;
    Interval sum = {};
    for (uint64_t i = 0; i < first.units.size(); i++) {
        if (first.units[i].variable_name != second.units[i].variable_name) {
            throw std::runtime_error("Interval: Impossible to sum intervals with different variables!");
        }
        bool different_values = first.units[i].value != second.units[i].value;
        bool contains_any_value = first.units[i].value == AnyValue || second.units[i].value == AnyValue;

        if (different_values && contains_any_value) {
            throw std::runtime_error("Interval: Impossible to sum intervals with different values that contains \"AnyValue\"!");
        }

        IntervalUnit new_unit = first.units[i];
        if (different_values) {
            new_unit.value = AnyValue;
            different_values_count++;
            if (different_values_count >= 2) {
                throw std::runtime_error("Interval: Impossible to sum intervals with amount of different values!");
            }
        }
        sum.AppendUnit(new_unit);
    }

    return sum;
}

Interval Interval::CalculateGeneralizedSum(const Interval &first, const Interval &second)
{
    if (first == second) {
        return first;
    } else if (first.units.size() != second.units.size()) {
        throw std::runtime_error("Interval: Impossible to sum intervals with different dimentions!");
    }

    int64_t different_values_count = 0;
    Interval sum = {};
    for (uint64_t i = 0; i < first.units.size(); i++) {
        if (first.units[i].variable_name != second.units[i].variable_name) {
            throw std::runtime_error("Interval: Impossible to sum intervals with different variables!");
        }
        bool different_values = first.units[i].value != second.units[i].value;
        bool contains_any_value = first.units[i].value == AnyValue || second.units[i].value == AnyValue;

        IntervalUnit new_unit = first.units[i];
        if (different_values && contains_any_value) {
            new_unit.value = first.units[i].value != AnyValue ? first.units[i].value : second.units[i].value;
        } else if (different_values) {
            new_unit.value = AnyValue;
            different_values_count++;
            if (different_values_count >= 2) {
                throw std::runtime_error("Interval: Impossible to sum intervals with amount of different values!");
            }
        }
        sum.AppendUnit(new_unit);
    }

    return sum;
}

Interval Interval::CalculateIntersection(const Interval &first, const Interval &second)
{
    if (first == second) {
        return first;
    } else if (first.units.size() != second.units.size()) {
        throw std::runtime_error("Interval: Impossible to calculate intersection for intervals with different dimentions!");
    }

    Interval intersection = {};
    for (uint64_t i = 0; i < first.units.size(); i++) {
        if (first.units[i].variable_name != second.units[i].variable_name) {
            throw std::runtime_error("Interval: Impossible to calculate intersection for intervals with different variables!");
        }
        bool different_values = first.units[i].value != second.units[i].value;
        bool contains_any_value = first.units[i].value == AnyValue || second.units[i].value == AnyValue;

        IntervalUnit new_unit = first.units[i];
        if (different_values && contains_any_value) {
            new_unit.value = first.units[i].value != AnyValue ? first.units[i].value : second.units[i].value;
        } else if (different_values) {
            throw std::runtime_error("Interval: Impossible to calculate intersection for intervals with different constant values!");
        }
        intersection.AppendUnit(new_unit);
    }

    return intersection;
}

std::vector < Interval > Interval::CalculateSubstraction(const Interval &first, const Interval &second)
{
    if (first == second) {
        return {};
    } else if (first.units.size() != second.units.size()) {
        throw std::runtime_error("Interval: Impossible to calculate substraction for intervals with different dimentions!");
    }

    if (first.IsAbsorbed(second)) {
        return {};
    }

    std::vector < Interval > substraction = {};
    Interval substraction_interval = {};
    for (uint64_t i = 0; i < first.units.size(); i++) {
        substraction_interval = first;

        if (first.units[i].variable_name != second.units[i].variable_name) {
            throw std::runtime_error("Interval: Impossible to calculate substraction for intervals with different variables!");
        }

        bool different_values = first.units[i].value != second.units[i].value;
        bool contains_any_value = first.units[i].value == AnyValue || second.units[i].value == AnyValue;

        IntervalUnit new_unit = first.units[i];
        if (different_values && contains_any_value) {
            BooleanValue constant_value = first.units[i].value != AnyValue ? first.units[i].value : second.units[i].value;
            if (constant_value == False) {
                new_unit.value = True;
            } else if (constant_value == True) {
                new_unit.value = False;
            } else {
                throw std::runtime_error("Interval: Unknown error while calculating substraction!");
            }
            substraction_interval.EditUnitByVariableName(first.units[i].variable_name, new_unit.value);
            substraction.push_back(substraction_interval);
        } else if (different_values) {
            return {first};
        }
    }

    return substraction;
}

bool Interval::Empty() const
{
    return units.empty();
}

bool Interval::operator==(const Interval &other) const
{
    return units == other.units;
}

BooleanFunction::BooleanFunction(const std::vector < std::string > &new_variables, const std::vector < Interval > &new_intervals, const std::vector < Interval > &new_negative_intervals, const std::string &new_name)
{
    SetVariables(new_variables);
    for (const Interval &new_interval : new_intervals) {
        AddInterval(new_interval);
    }
    for (const Interval &new_interval : new_negative_intervals) {
        AddNegativeInterval(new_interval);
    }
    name = new_name;
}

void BooleanFunction::AddInterval(const Interval &new_interval)
{
    CheckAndAddInterval(new_interval, intervals, negative_intervals);
}

void BooleanFunction::AddNegativeInterval(const Interval &new_interval)
{
    CheckAndAddInterval(new_interval, negative_intervals, intervals);
}

void BooleanFunction::SetVariables(const std::vector<std::string> &new_variables)
{
    UpdateIntervalsVariables(new_variables, intervals);
    UpdateIntervalsVariables(new_variables, negative_intervals);
    variables = new_variables;
}

void BooleanFunction::SetName(const std::string &new_name)
{
    name = new_name;
}

bool BooleanFunction::Empty() const
{
    return intervals.empty();
}

BooleanValue BooleanFunction::GetValue() const
{
    if (variables.empty())
    {
        if (intervals.empty())
        {
            return False;
        }
        return True;
    }
    return AnyValue;
}

std::vector < std::string > BooleanFunction::GetVariables() const
{
    return variables;
}

std::string BooleanFunction::GetName() const
{
    return name;
}

std::vector < Interval > BooleanFunction::GetIntervals() const
{
    return intervals;
}

std::vector<Interval> BooleanFunction::GetNegativeIntervals() const
{
    return negative_intervals;
}

BooleanFunction BooleanFunction::FixVariable(const std::string &variable, const BooleanValue &value) const
{
    if (std::find(variables.begin(), variables.end(), variable) == variables.end())
    {
        throw std::runtime_error("BinaryFunction: This variable doesn't used in this function!");
    }
    if (value == AnyValue) {
        throw std::runtime_error("BinaryFunction: There is no need to fix the variable at value \"AnyValue\"!");
    }

    std::vector < std::string > variables_result = variables;
    variables_result.erase(std::find(variables_result.begin(), variables_result.end(), variable));

    std::vector < Interval > new_intervals = FixVariableInIntervals(variable, value, intervals);
    std::vector < Interval > new_negative_intervals = FixVariableInIntervals(variable, value, negative_intervals);
    return BooleanFunction(variables_result, new_intervals, new_negative_intervals);
}

bool BooleanFunction::operator==(const BooleanFunction &other) const
{
    return variables == other.variables && intervals == other.intervals;
}

void BooleanFunction::CheckAndAddInterval(const Interval &new_interval, std::vector<Interval> &similar_intervals, const std::vector<Interval> &opposite_intervals)
{
    for (IntervalUnit& unit : new_interval.GetUnits()) {
        if (std::find(
                    variables.begin(),
                    variables.end(),
                    unit.variable_name
                    ) == variables.end())
        {
            throw std::runtime_error("BinaryFunction: This variable doesn\'t used in this function!\n");
        }
    }
    Interval new_interval_copy = new_interval;
    for (const std::string &variable : variables)
    {
        bool variable_missing = true;
        for (IntervalUnit& unit : new_interval.GetUnits()) {
            if (unit.variable_name == variable)
            {
                variable_missing = false;
                break;
            }
        }
        if (variable_missing)
        {
            new_interval_copy.AppendUnit({variable, AnyValue});
        }
    }

    for (const Interval &opposite_interval : opposite_intervals) {
        if (opposite_interval.IntersectionExists(new_interval_copy)) {
            throw std::runtime_error("BinaryFunction: Can\'t add interval due to intersection of opposite interval!");
        }
    }

    similar_intervals.push_back(new_interval_copy);
}

void BooleanFunction::UpdateIntervalsVariables(const std::vector < std::string > &new_variables, std::vector < Interval > &some_intervals)
{
    std::vector < std::string > new_variables_copy = new_variables;
    std::sort(new_variables_copy.begin(), new_variables_copy.end());
    for (Interval &interval : some_intervals)
    {
        for (const std::string &variable : new_variables)
        {
            bool variable_missing = true;
            for (IntervalUnit &unit : interval.GetUnits())
            {
                if (unit.variable_name == variable)
                {
                    variable_missing = false;
                    break;
                }
            }
            if (variable_missing)
            {
                interval.AppendUnit(IntervalUnit({variable, AnyValue}));
            }
        }
        for (unsigned int i = 0; i < interval.GetUnits().size(); i++)
        {
            if (std::find(
                        new_variables.begin(),
                        new_variables.end(),
                        interval.GetUnits()[i].variable_name
                        ) == new_variables.end())
            {
                interval.RemoveUnitAt(i);
                i--;
            }
        }
    }
}

std::vector < Interval > BooleanFunction::FixVariableInIntervals(const std::string &variable, const BooleanValue &value, const std::vector < Interval > &some_intervals) const
{
    std::vector < Interval > result;
    for (const Interval &interval : some_intervals)
    {
        IntervalUnit variable_interval_unit;
        for (IntervalUnit& unit : interval.GetUnits())
        {
            if (unit.variable_name == variable)
            {
                variable_interval_unit = unit;
                break;
            }
        }
        if (variable_interval_unit.value == AnyValue || variable_interval_unit.value == value)
        {
            Interval new_interval = interval;
            new_interval.RemoveUnitByVariableName(variable_interval_unit.variable_name);
            result.push_back(new_interval);
        }
    }
    return result;
}
