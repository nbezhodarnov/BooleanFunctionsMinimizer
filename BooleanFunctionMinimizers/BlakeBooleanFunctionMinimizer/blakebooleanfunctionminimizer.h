#ifndef BLAKEBOOLEANFUNCTIONMINIMIZER_H
#define BLAKEBOOLEANFUNCTIONMINIMIZER_H

#include <vector>

#include "BooleanFunctionMinimizers/abstractbooleanfunctionminimizer.h"

struct IntervalWithStatus {
    Interval interval;
    bool is_absorbed;
};

class BlakeBooleanFunctionMinimizer : public AbstractBooleanFunctionMinimizer
{
public:
    BlakeBooleanFunctionMinimizer() = default;
    BooleanFunction Minimize(const BooleanFunction &function) const override;

private:
    std::vector < IntervalWithStatus > ApplyAllAbsorbs(const std::vector < IntervalWithStatus > &unabsorbed_intervals) const;
    std::vector < IntervalWithStatus > ApplyAllAbsorbsFromInterval(const Interval &some_interval, const std::vector < IntervalWithStatus > &unabsorbed_intervals) const;
    bool CouldBeAbsorbed(const Interval &some_interval, const std::vector < IntervalWithStatus > &intervals_with_statuses) const;
    Interval TryToCalculateGeneralizedSum(const Interval &first, const Interval &second) const;
    BooleanFunction GenerateBooleanFunction(const std::vector < std::string > &variables, const std::vector < IntervalWithStatus > &intervals, const std::string &name) const;
};

#endif // BLAKEBOOLEANFUNCTIONMINIMIZER_H
