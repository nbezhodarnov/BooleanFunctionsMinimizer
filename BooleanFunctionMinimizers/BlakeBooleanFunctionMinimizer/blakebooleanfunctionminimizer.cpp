#include <stdexcept>

#include "blakebooleanfunctionminimizer.h"

BooleanFunction BlakeBooleanFunctionMinimizer::Minimize(const BooleanFunction &function) const
{
    std::vector < IntervalWithStatus > intervals;
    for (const Interval &interval : function.GetIntervals()) {
        intervals.push_back({interval, false});
    }

    intervals = ApplyAllAbsorbs(intervals);

    for (uint64_t i = 1; i < intervals.size(); i++) {
        if (intervals[i].is_absorbed) {
            continue;
        }
        Interval conjuction = intervals[i].interval;
        for (uint64_t j = 0; j < i; j++) {
            if (intervals[j].is_absorbed) {
                continue;
            }
            Interval other_conjuction = intervals[j].interval;
            Interval generalized_sum = TryToCalculateGeneralizedSum(conjuction, other_conjuction);
            if (generalized_sum == Interval() || CouldBeAbsorbed(generalized_sum, intervals)) {
                continue;
            }

            bool intersects_negative_intervals = false;
            for (const Interval &negative_intervals : function.GetNegativeIntervals()) {
                if (negative_intervals.IntersectionExists(generalized_sum)) {
                    intersects_negative_intervals = true;
                    break;
                }
            }
            if (intersects_negative_intervals) {
                continue;
            }

            intervals.push_back({generalized_sum, false});
            intervals = ApplyAllAbsorbsFromInterval(generalized_sum, intervals);
            if (intervals[i].is_absorbed) {
                break;
            }
        }
    }

    std::vector < std::string > variables = function.GetVariables();
    return GenerateBooleanFunction(variables, intervals, function.GetName());
}

std::vector < IntervalWithStatus > BlakeBooleanFunctionMinimizer::ApplyAllAbsorbs(const std::vector < IntervalWithStatus > &unabsorbed_intervals) const
{
    std::vector < IntervalWithStatus > intervals = unabsorbed_intervals;
    for (auto conjuction = intervals.begin(); conjuction != intervals.end(); conjuction++) {
        if (conjuction->is_absorbed) {
            continue;
        }
        for (auto other_conjuction = conjuction + 1; other_conjuction != intervals.end(); other_conjuction++) {
            if (other_conjuction->is_absorbed) {
                continue;
            }
            if (other_conjuction->interval.Absorbs(conjuction->interval)) {
                conjuction->is_absorbed = true;
                break;
            }
        }
    }
    return intervals;
}

std::vector < IntervalWithStatus > BlakeBooleanFunctionMinimizer::ApplyAllAbsorbsFromInterval(const Interval &some_interval, const std::vector < IntervalWithStatus > &unabsorbed_intervals) const
{
    std::vector < IntervalWithStatus > intervals = unabsorbed_intervals;
    for (auto conjuction = intervals.begin(); conjuction != intervals.end(); conjuction++) {
        if (conjuction->is_absorbed) {
            continue;
        }
        if (some_interval == conjuction->interval) {
            continue;
        }
        if (some_interval.Absorbs(conjuction->interval)) {
            conjuction->is_absorbed = true;
            continue;
        }
    }
    return intervals;
}

bool BlakeBooleanFunctionMinimizer::CouldBeAbsorbed(const Interval &some_interval, const std::vector < IntervalWithStatus > &intervals_with_statuses) const
{
    for (const IntervalWithStatus &interval_with_status : intervals_with_statuses) {
        if (interval_with_status.is_absorbed) {
            continue;
        }
        if (interval_with_status.interval.Absorbs(some_interval)) {
            return true;
        }
    }
    return false;
}

Interval BlakeBooleanFunctionMinimizer::TryToCalculateGeneralizedSum(const Interval &first, const Interval &second) const
{
    Interval result;
    try {
        result = Interval::CalculateGeneralizedSum(first, second);
    } catch (std::runtime_error const &exception) {}
    return result;
}

BooleanFunction BlakeBooleanFunctionMinimizer::GenerateBooleanFunction(const std::vector < std::string > &variables, const std::vector < IntervalWithStatus > &intervals_with_statuses, const std::string &name) const
{
    BooleanFunction result(variables);
    for (const IntervalWithStatus &interval_with_status : intervals_with_statuses) {
        if (interval_with_status.is_absorbed) {
            continue;
        }
        result.AddInterval(interval_with_status.interval);
    }
    result.SetName(name);
    return result;
}
