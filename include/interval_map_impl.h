#ifndef INTERVAL_MAP_IMPL_H
#define INTERVAL_MAP_IMPL_H

#include "interval_map.h"
#include <cassert>
#include <algorithm>

template<typename K, typename V>
interval_map<K, V>::interval_map(V const& val) : m_valBegin(val) {}

template<typename K, typename V>
const std::map<K, V>& interval_map<K, V>::get_map() const {
    return m_map;
}

template<typename K, typename V>
V const& interval_map<K, V>::get_begin_value() const {
    return m_valBegin;
}

template<typename K, typename V>
bool interval_map<K, V>::is_valid_interval(K const& keyBegin, K const& keyEnd) const {
    return keyBegin < keyEnd;
}

template<typename K, typename V>
void interval_map<K, V>::assign(K const& keyBegin, K const& keyEnd, V const& val) {
    if (!is_valid_interval(keyBegin, keyEnd)) return;

    auto afterEnd = m_map.upper_bound(keyEnd);
    V valueAfter = (afterEnd == m_map.begin()) ? m_valBegin : (--afterEnd)->second;

    m_map[keyBegin] = val;
    m_map[keyEnd] = valueAfter;

    auto it = m_map.upper_bound(keyBegin);
    while (it != m_map.end() && it->first < keyEnd) {
        it = m_map.erase(it);
    }

    canonicalize();
}

template<typename K, typename V>
V const& interval_map<K, V>::operator[](K const& key) const {
    auto it = m_map.upper_bound(key);
    return (it == m_map.begin()) ? m_valBegin : (--it)->second;
}

template<typename K, typename V>
void interval_map<K, V>::clear() {
    m_map.clear();
}

template<typename K, typename V>
void interval_map<K, V>::canonicalize() {
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

#endif // INTERVAL_MAP_IMPL_H
