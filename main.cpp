#include <map>
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <sstream>

template<typename K, typename V>
class interval_map {
private:
    V m_valBegin;
    std::map<K,V> m_map;

    // Helper function to check if the interval is valid
    bool is_valid_interval(K const& keyBegin, K const& keyEnd) const {
        return keyBegin < keyEnd;
    }

    // Helper function to make the map canonical
    void canonicalize() {
        if (m_map.empty()) return;
        
        auto it = m_map.begin();
        V prevVal = it->second;
        ++it;
        
        while (it != m_map.end()) {
            if (it->second == prevVal) {
                it = m_map.erase(it);
            } else {
                prevVal = it->second;
                ++it;
            }
        }
    }

public:
    // Constructor
    interval_map(V const& val) : m_valBegin(val) {}

    // Assignment operator deleted to prevent accidental copying
    interval_map& operator=(interval_map const&) = delete;
    
    // Get the internal map (for testing purposes only)
    const std::map<K,V>& get_map() const { return m_map; }
    
    // Get the beginning value
    V const& get_begin_value() const { return m_valBegin; }

    void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        // Handle empty interval
        if (!is_valid_interval(keyBegin, keyEnd)) return;

        // Find the value that should be restored after keyEnd
        auto afterEnd = m_map.upper_bound(keyEnd);
        V valueAfter;
        if (afterEnd == m_map.begin()) {
            valueAfter = m_valBegin;
        } else {
            auto prev = afterEnd;
            --prev;
            valueAfter = prev->second;
        }

        // Insert new boundaries
        m_map[keyBegin] = val;
        m_map[keyEnd] = valueAfter;

        // Remove all points in between
        auto it = m_map.upper_bound(keyBegin);
        while (it != m_map.end() && it->first < keyEnd) {
            it = m_map.erase(it);
        }

        // Make map canonical
        canonicalize();
    }

    V const& operator[](K const& key) const {
        auto it = m_map.upper_bound(key);
        return (it == m_map.begin()) ? m_valBegin : (--it)->second;
    }

    // Clear the map
    void clear() {
        m_map.clear();
    }
};

// Test utilities
class IntervalMapTester {
private:
    static std::random_device rd;
    static std::mt19937 gen;

    template<typename K>
    static K random_key(K min, K max) {
        std::uniform_int_distribution<K> dis(min, max);
        return dis(gen);
    }

    // Helper function to verify interval consistency
    template<typename K, typename V>
    static void verify_interval(interval_map<K, V>& imap, K start, K end, V val) {
        // Check that all points in the interval have the expected value
        for (K i = start; i < end; ++i) {
            assert(imap[i] == val);
        }
        
        // Check that the point just before the interval (if applicable) is different
        if (start > 0) {
            K before = start - 1;
            // The value before might be different or might be the same
            // We can't make any strong assertions about it
        }
        
        // We can't make strong assertions about the end point
        // as it depends on what intervals were assigned before
    }

public:
    // Run comprehensive tests
    static void run_all_tests() {
        test_basic_functionality();
        test_empty_intervals();
        test_overlapping_intervals();
        test_random_intervals();
        test_boundary_conditions();
        std::cout << "All tests passed!\n";
    }

    static void test_basic_functionality() {
        interval_map<int, char> imap('A');
        
        // Test single interval
        imap.assign(1, 5, 'B');
        assert(imap[0] == 'A');
        assert(imap[1] == 'B');
        assert(imap[4] == 'B');
        assert(imap[5] == 'A');
        
        imap.clear();
    }

    static void test_empty_intervals() {
        interval_map<int, char> imap('A');
        
        // Test invalid intervals
        imap.assign(5, 5, 'B');  // Empty interval
        imap.assign(6, 3, 'B');  // Invalid interval
        assert(imap[4] == 'A');
        assert(imap[5] == 'A');
        assert(imap[6] == 'A');
        
        imap.clear();
    }

    static void test_overlapping_intervals() {
        interval_map<int, char> imap('A');
        
        imap.assign(1, 5, 'B');
        imap.assign(3, 7, 'C');
        assert(imap[2] == 'B');
        assert(imap[4] == 'C');
        assert(imap[6] == 'C');
        assert(imap[7] == 'A');
        
        imap.clear();
    }

    static void test_boundary_conditions() {
        interval_map<int, char> imap('A');
        
        // Test overlapping intervals at boundaries
        imap.assign(1, 5, 'B');
        imap.assign(5, 10, 'C');
        assert(imap[4] == 'B');
        assert(imap[5] == 'C');
        
        // Test adjacent intervals with same value
        imap.assign(10, 15, 'C');
        auto map_size_before = imap.get_map().size();
        imap.assign(15, 20, 'C');
        assert(imap.get_map().size() <= map_size_before); // Should be canonicalized
        
        imap.clear();
    }

    static void test_random_intervals() {
        interval_map<int, char> imap('A');
        const int NUM_TESTS = 100;
        
        for (int i = 0; i < NUM_TESTS; ++i) {
            int start = random_key<int>(0, 50);
            int end = random_key<int>(start + 1, 100);
            char val = static_cast<char>('B' + (i % 20));  // Cycle through characters
            
            imap.assign(start, end, val);
            verify_interval(imap, start, end, val);
        }
        
        imap.clear();
    }
};

std::random_device IntervalMapTester::rd;
std::mt19937 IntervalMapTester::gen(IntervalMapTester::rd());

int main() {
    IntervalMapTester::run_all_tests();
    return 0;
}