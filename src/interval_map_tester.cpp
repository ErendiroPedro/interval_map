#include "interval_map_tester.h"
#include <cassert>
#include <iostream>
#include <set>

std::random_device IntervalMapTester::rd;
std::mt19937 IntervalMapTester::gen(IntervalMapTester::rd());

void IntervalMapTester::run_all_tests() {
    std::vector<std::pair<std::string, bool>> results = {
        {"Basic Functionality", test_basic_functionality()},
        {"Value Queries", test_value_queries()},
        {"Default Value Behavior", test_default_value_behavior()},
        {"Empty Intervals", test_empty_intervals()},
        {"Invalid Intervals", test_invalid_intervals()},
        {"Single Point Intervals", test_single_point_intervals()},
        {"Overlapping Intervals", test_overlapping_intervals()},
        {"Adjacent Intervals", test_adjacent_intervals()},
        {"Boundary Conditions", test_boundary_conditions()},
        {"Random Intervals", test_random_intervals()},
        {"Large Operations", test_large_operations()},
        {"Memory Efficiency", test_memory_efficiency()}
    };

    bool all_passed = true;
    for (const auto& [test_name, result] : results) {
        print_test_result(test_name, result);
        if (!result) all_passed = false;
    }

    std::cout << "\nFinal Result: " << (all_passed ? "All tests passed!" : "Some tests failed.") << std::endl;
}

// Basic Functionality Tests
bool IntervalMapTester::test_basic_functionality() {
    try {
        interval_map<int, char> imap('A');
        imap.assign(1, 5, 'B');
        assert(imap[0] == 'A');
        assert(imap[1] == 'B');
        assert(imap[4] == 'B');
        assert(imap[5] == 'A');
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_value_queries() {
    try {
        interval_map<int, char> imap('X');
        imap.assign(-10, 0, 'Y');
        imap.assign(0, 10, 'Z');
        
        // Test negative range
        assert(imap[-15] == 'X');
        assert(imap[-10] == 'Y');
        assert(imap[-5] == 'Y');
        
        // Test zero boundary
        assert(imap[-1] == 'Y');
        assert(imap[0] == 'Z');
        assert(imap[1] == 'Z');
        
        // Test positive range
        assert(imap[9] == 'Z');
        assert(imap[10] == 'X');
        assert(imap[15] == 'X');
        
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_default_value_behavior() {
    try {
        interval_map<int, char> imap('D');
        
        // Test initial state
        assert(imap[-100] == 'D');
        assert(imap[0] == 'D');
        assert(imap[100] == 'D');
        
        // Test after assignments
        imap.assign(0, 10, 'E');
        assert(imap[-1] == 'D');
        assert(imap[10] == 'D');
        
        // Test after clearing interval
        imap.assign(5, 7, 'D');  // Should merge with default value
        assert(imap[6] == 'D');
        
        return true;
    } catch (...) {
        return false;
    }
}

// Edge Cases Tests
bool IntervalMapTester::test_empty_intervals() {
    try {
        interval_map<int, char> imap('A');
        imap.assign(5, 5, 'B');  // Empty interval
        assert(imap[4] == 'A');
        assert(imap[5] == 'A');
        assert(imap[6] == 'A');
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_invalid_intervals() {
    try {
        interval_map<int, char> imap('A');
        
        // Test intervals where end < begin
        imap.assign(10, 5, 'B');
        assert(imap[7] == 'A');
        
        // Test negative intervals
        imap.assign(-5, -10, 'C');
        assert(imap[-7] == 'A');
        
        // Test extreme values
        imap.assign(std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 'D');
        assert(imap[0] == 'A');
        
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_single_point_intervals() {
    try {
        interval_map<int, char> imap('A');
        
        // Test single point interval
        imap.assign(5, 6, 'B');
        assert(imap[4] == 'A');
        assert(imap[5] == 'B');
        assert(imap[6] == 'A');
        
        // Test adjacent single point intervals
        imap.assign(6, 7, 'C');
        assert(imap[5] == 'B');
        assert(imap[6] == 'C');
        assert(imap[7] == 'A');
        
        return true;
    } catch (...) {
        return false;
    }
}

// Complex Scenarios Tests
bool IntervalMapTester::test_adjacent_intervals() {
    try {
        interval_map<int, char> imap('A');
        
        // Test adjacent intervals with different values
        imap.assign(0, 5, 'B');
        imap.assign(5, 10, 'C');
        assert(imap[4] == 'B');
        assert(imap[5] == 'C');
        
        // Test adjacent intervals with same value
        imap.assign(10, 15, 'C');
        assert(imap[9] == 'C');
        assert(imap[10] == 'C');
        assert(imap[14] == 'C');
        assert(imap[15] == 'A');
        
        return true;
    } catch (...) {
        return false;
    }
}

// Stress Testing
bool IntervalMapTester::test_large_operations() {
    try {
        interval_map<int, char> imap('A');
        const int NUM_OPERATIONS = 10000;
        
        // Perform many operations
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            int start = random_key(-1000, 1000);
            int end = start + random_key(1, 100);
            char val = static_cast<char>('B' + (i % 20));
            imap.assign(start, end, val);
        }
        
        // Verify some random points
        for (int i = 0; i < 100; ++i) {
            int key = random_key(-1000, 1000);
            char val = imap[key];
            assert(val >= 'A' && val <= 'Z');
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_memory_efficiency() {
    try {
        interval_map<int, char> imap('A');
        const int NUM_OPERATIONS = 1000;
        
        // Record initial memory usage
        size_t initial_size = get_memory_usage(imap);
        
        // Perform operations that should merge
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            imap.assign(i, i + 2, 'B');
            imap.assign(i + 1, i + 3, 'B');  // Should merge with previous
        }
        
        // Record final memory usage
        size_t final_size = get_memory_usage(imap);
        
        // Verify that memory usage is reasonable
        // We expect much fewer nodes than operations due to merging
        assert(final_size < NUM_OPERATIONS / 2);
        
        return true;
    } catch (...) {
        return false;
    }
}

// Complex Scenarios Tests
bool IntervalMapTester::test_overlapping_intervals() {
    try {
        interval_map<int, char> imap('A');
        
        // Test completely overlapping intervals
        imap.assign(0, 10, 'B');
        imap.assign(2, 8, 'C');
        assert(imap[1] == 'B');
        assert(imap[2] == 'C');
        assert(imap[7] == 'C');
        assert(imap[8] == 'B');
        assert(imap[9] == 'B');
        
        // Test partially overlapping intervals
        imap.assign(5, 15, 'D');
        assert(imap[4] == 'C');
        assert(imap[5] == 'D');
        assert(imap[14] == 'D');
        assert(imap[15] == 'A');
        
        // Test multiple overlapping intervals
        imap.assign(3, 12, 'E');
        assert(imap[2] == 'C');
        assert(imap[3] == 'E');
        assert(imap[11] == 'E');
        assert(imap[12] == 'D');
        
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_boundary_conditions() {
    try {
        interval_map<int, char> imap('A');
        
        // Test boundaries near integer limits
        imap.assign(std::numeric_limits<int>::min(), 0, 'B');
        imap.assign(0, std::numeric_limits<int>::max(), 'C');
        
        assert(imap[std::numeric_limits<int>::min()] == 'B');
        assert(imap[-1] == 'B');
        assert(imap[0] == 'C');
        assert(imap[std::numeric_limits<int>::max() - 1] == 'C');
        
        // Test updating boundary points
        imap.assign(-1, 1, 'D');
        assert(imap[-2] == 'B');
        assert(imap[-1] == 'D');
        assert(imap[0] == 'D');
        assert(imap[1] == 'C');
        
        // Test zero-width intervals at boundaries
        imap.assign(std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), 'E');
        imap.assign(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), 'E');
        
        return true;
    } catch (...) {
        return false;
    }
}

bool IntervalMapTester::test_random_intervals() {
    try {
        interval_map<int, char> imap('A');
        const int NUM_TESTS = 100;
        
        // Generate random intervals and verify they don't violate invariants
        for (int i = 0; i < NUM_TESTS; ++i) {
            int start = random_key(-1000, 1000);
            int end = start + random_key(0, 100);  // Ensure end >= start
            char val = static_cast<char>('B' + (i % 20));
            
            if (start < end) {
                // Store the value before assignment at the end point
                char prev_end_val = imap[end];
                
                // Make the assignment
                imap.assign(start, end, val);
                
                // Verify the interval
                assert(imap[start] == val);  // Start point should have new value
                
                // Check a point just before end (if possible)
                if (start < end - 1) {
                    assert(imap[end - 1] == val);
                }
                
                // Check points within interval
                for (int j = start; j < end; ++j) {
                    assert(imap[j] == val);
                }
                
                // The end point should either:
                // 1. Have the previous value at that point (if different from val)
                // 2. Have the value 'val' if it was canonicalized due to matching next interval
                char end_val = imap[end];
                assert(end_val == prev_end_val || end_val == val);
            }
        }
        
        // Verify internal consistency
        auto& map = imap.get_map();
        if (!map.empty()) {
            auto it = map.begin();
            char prev_val = it->second;
            ++it;
            
            // Verify canonicalization - no adjacent intervals with same value
            while (it != map.end()) {
                assert(it->second != prev_val);
                prev_val = it->second;
                ++it;
            }
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

// Helper Methods
void IntervalMapTester::print_test_result(const std::string& test_name, bool result) {
    std::cout << test_name << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

int IntervalMapTester::random_key(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void IntervalMapTester::verify_interval(const interval_map<int, char>& imap, int start, int end, char val) {
    for (int i = start; i < end; ++i) {
        assert(imap[i] == val);
    }
}

size_t IntervalMapTester::get_memory_usage(const interval_map<int, char>& imap) {
    // Returns the number of nodes in the internal map
    return imap.get_map().size();
}