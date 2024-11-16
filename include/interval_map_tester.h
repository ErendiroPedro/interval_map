#ifndef INTERVAL_MAP_TESTER_H
#define INTERVAL_MAP_TESTER_H

#include "interval_map.h"
#include <random>
#include <string>
#include <vector>

class IntervalMapTester {
private:
    static std::random_device rd;
    static std::mt19937 gen;

    // Basic Functionality Tests
    static bool test_basic_functionality();
    static bool test_value_queries();
    static bool test_default_value_behavior();

    // Edge Cases Tests
    static bool test_empty_intervals();
    static bool test_invalid_intervals();
    static bool test_single_point_intervals();
    
    // Complex Scenarios Tests
    static bool test_overlapping_intervals();
    static bool test_adjacent_intervals();
    static bool test_boundary_conditions();
    
    // Stress Testing
    static bool test_random_intervals();
    static bool test_large_operations();
    static bool test_memory_efficiency();
    
    // Helper Methods
    static int random_key(int min, int max);
    static void verify_interval(const interval_map<int, char>& imap, int start, int end, char val);
    static size_t get_memory_usage(const interval_map<int, char>& imap);
    static void print_test_result(const std::string& test_name, bool result);

public:
    static void run_all_tests();
};

#endif // INTERVAL_MAP_TESTER_H