#ifndef INTERVAL_MAP_H
#define INTERVAL_MAP_H

#include <map>

template<typename K, typename V>
class interval_map {
private:
    V m_valBegin;
    std::map<K,V> m_map;

    bool is_valid_interval(K const& keyBegin, K const& keyEnd) const;
    void canonicalize();

public:
    interval_map(V const& val);
    interval_map& operator=(interval_map const&) = delete;
    
    const std::map<K,V>& get_map() const;
    V const& get_begin_value() const;

    void assign(K const& keyBegin, K const& keyEnd, V const& val);
    V const& operator[](K const& key) const;
    void clear();
};

#include "interval_map_impl.h"

#endif // INTERVAL_MAP_H
