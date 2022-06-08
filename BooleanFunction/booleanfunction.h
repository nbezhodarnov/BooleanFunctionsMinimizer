#ifndef BOOLEANFUNCTION_H
#define BOOLEANFUNCTION_H

#include <string>
#include <vector>

enum BooleanValue : unsigned char
{
    False = '0',
    True = '1',
    AnyValue = '-'
};

struct IntervalUnit
{
    std::string variable_name;
    BooleanValue value;

    bool operator==(const IntervalUnit &other) const;
};

class Interval
{
public:
    Interval(const std::vector < IntervalUnit > &new_units = {});

    void AppendUnit(const IntervalUnit &new_unit);
    void RemoveUnitAt(const unsigned int &index);
    void RemoveUnitByVariableName(const std::string &variable_name);

    std::vector < IntervalUnit > GetUnits() const;
    IntervalUnit GetUnitByVariableName(const std::string &variable) const;

    bool IsAbsorbed(const Interval &other) const;
    bool Absorbs(const Interval &other) const;
    bool IntersectionExists(const Interval &other) const;

    static Interval CalculateSum(const Interval &first, const Interval &second);
    static Interval CalculateGeneralizedSum(const Interval &first, const Interval &second);
    static Interval CalculateIntersection(const Interval &first, const Interval &second);
    static Interval CalculateSubstraction(const Interval &first, const Interval &second);

    bool Empty() const;
    bool operator==(const Interval &other) const;

private:
    std::vector < IntervalUnit > units;
};


class BooleanFunction
{
public:
    BooleanFunction(const std::vector < std::string > &new_variables = {}, const std::vector < Interval > &new_intervals = {}, const std::vector < Interval > &new_negative_intervals = {}, const std::string &new_name = "");

    void AddInterval(const Interval &new_interval);
    void AddNegativeInterval(const Interval &new_interval);
    void SetVariables(const std::vector < std::string > &new_variables);
    void SetName(const std::string &new_name);

    bool Empty() const;

    BooleanValue GetValue() const;
    std::vector < std::string > GetVariables() const;
    std::string GetName() const;
    std::vector < Interval > GetIntervals() const;
    std::vector < Interval > GetNegativeIntervals() const;

    BooleanFunction FixVariable(const std::string &variable, const BooleanValue &value) const;

    bool operator==(const BooleanFunction &other) const;

private:
    std::vector < Interval > intervals;
    std::vector < Interval > negative_intervals;
    std::vector < std::string > variables;
    std::string name;

    void CheckAndAddInterval(const Interval &new_interval, std::vector < Interval > &similar_intervals, const std::vector < Interval > &opposite_intervals);
    void UpdateIntervalsVariables(const std::vector < std::string > &new_variables, std::vector < Interval > &some_intervals);
    std::vector < Interval > FixVariableInIntervals(const std::string &variable, const BooleanValue &value, const std::vector < Interval > &some_intervals) const;
};

#endif // BOOLEANFUNCTION_H
