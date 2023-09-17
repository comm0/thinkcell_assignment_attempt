#include <iostream>
#include <map>

template <typename K, typename V> class interval_map
{
  friend void printResult(const std::string &,
                 const std::map<int, char> &,
                 const interval_map<int, char> &);
  friend void IntervalMapTest();                                    // Do not modfy
  V m_valBegin;                                                     // Do not modfy
  std::map<K, V> m_map;                                             // Do not modfy
public:// Do not modfy
  // constructor associates whole range of K with val
  interval_map(V const &val) : m_valBegin(val) {}                   // Do not modfy

  // Assign value val to interval [keyBegin, keyEnd).
  // Overwrite previous values in this interval.
  // Conforming to the C++ Standard Library conventions, the interval
  // includes keyBegin, but excludes keyEnd.
  // If !( keyBegin < keyEnd ), this designates an empty interval,
  // and assign must do nothing.
  void assign(K const &keyBegin, K const &keyEnd, V const &val)     // Do not modfy
  {                                                                 // Do not modfy

    // Corner cases first
    if (!(keyBegin < keyEnd))
    {
      return;
    }

    if (m_map.empty() && val == m_valBegin)
    {
      return;
    }

    auto it_start = m_map.lower_bound(keyBegin);
    auto it_end = m_map.lower_bound(keyEnd);

    V before_keyBegin_val =
        (it_start != m_map.begin()) ? std::prev(it_start)->second : m_valBegin;
    V val_at_keyEnd = (it_end != m_map.end() && !(it_end->first < keyEnd) &&
                       !(keyEnd < it_end->first))
                          ? it_end->second
                          : (it_end != m_map.begin() ? std::prev(it_end)->second
                                                     : m_valBegin);

    m_map.erase(it_start, it_end);

    if (val != before_keyBegin_val)
    {
      m_map[keyBegin] = val; // [] operator usage unallowed?
    }

    if (val_at_keyEnd != val)
    {
      m_map[keyEnd] = val_at_keyEnd; // [] operator usage unallowed?
    }

    auto it = m_map.begin();
    while (it != m_map.end())
    {
      auto next_it = std::next(it);
      if (next_it != m_map.end() && it->second == next_it->second)
      {
        m_map.erase(next_it);
      }
      else
      {
        ++it;
      }
    }

  }                                                                 // Do not modfy

  // look-up of the value associated with key
  V const &operator[](K const &key) const                           // Do not modfy
  {                                                                 // Do not modfy
    auto it = m_map.upper_bound(key);                               // Do not modfy
    if (it == m_map.begin())                                        // Do not modfy
    {                                                               // Do not modfy
      return m_valBegin;                                            // Do not modfy
    }                                                               // Do not modfy
    else                                                            // Do not modfy
    {                                                               // Do not modfy
      return (--it)->second;                                        // Do not modfy
    }                                                               // Do not modfy
  }                                                                 // Do not modfy
};                                                                  // Do not modfy

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

template <typename K, typename V>
bool compareMaps(const std::map<K, V> &result, const std::map<K, V> &expected)
{
  return result == expected;
}

void printResult(const std::string &setName,
                 const std::map<int, char> &expected,
                 const interval_map<int, char> &m)
{
  std::cout << "--- " << setName << " ---\n";
  if (compareMaps(m.m_map, expected) && m.m_valBegin == 'A')
  {
    std::cout << "PASS\n";
  }
  else
  {
    std::cout << "FAIL! \nExpected:\n";
    for (const auto &pair : expected)
    {
      std::cout << pair.first << " -> " << pair.second << "\n";
    }
    std::cout << "Got:\n";
    for (const auto &pair : m.m_map)
    {
      std::cout << pair.first << " -> " << pair.second << "\n";
    }
    std::cout << "-----------------------\n";
  }
}

// Test function (not exhaustive)
void IntervalMapTest()
{
  interval_map<int, char> m('A');

  // Set 1
  m.assign(1, 3, 'B');
  m.assign(3, 8, 'B');
  printResult("Set 1", {{1, 'B'}, {8, 'A'}}, m);

  // Set 2
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 4, 'B');
  m.assign(2, 5, 'C');
  printResult("Set 2", {{1, 'B'}, {2, 'C'}, {5, 'A'}}, m);

  // Set 3
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 5, 'B');
  m.assign(4, 7, 'C');
  printResult("Set 3", {{1, 'B'}, {4, 'C'}, {7, 'A'}}, m);

  // Set 4
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 6, 'B');
  m.assign(3, 8, 'C');
  m.assign(4, 5, 'D');
  printResult("Set 4", {{1, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'C'}, {8, 'A'}}, m);

  // Set 5
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 8, 'B');
  m.assign(3, 6, 'C');
  printResult("Set 5", {{1, 'B'}, {3, 'C'}, {6, 'B'}, {8, 'A'}}, m);

  // Set 6
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 3, 'B');
  m.assign(0, 1, 'C');
  printResult("Set 6", {{0, 'C'}, {1, 'B'}, {3, 'A'}}, m);

  // Set 7
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 3, 'B');
  m.assign(3, 4, 'C');
  printResult("Set 7", {{1, 'B'}, {3, 'C'}, {4, 'A'}}, m);

  // Set 8
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 3, 'B');
  m.assign(2, 4, 'B');
  printResult("Set 8", {{1, 'B'}, {4, 'A'}}, m);

  // Set 9
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 3, 'A');
  printResult("Set 9", {}, m);

  // Set 10
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 3, 'B');
  m.assign(1, 3, 'B');
  printResult("Set 10", {{1, 'B'}, {3, 'A'}}, m);

  // Set 11
  m = interval_map<int, char>('A'); // Reset
  m.assign(2, 2, 'B');
  printResult("Set 11", {}, m);

  // Set 12
  m = interval_map<int, char>('A'); // Reset
  m.assign(4, 3, 'B');
  printResult("Set 12", {}, m);

  // // Set 13
  m = interval_map<int, char>('A'); // Reset
  m.assign(1, 10, 'B');
  m.assign(3, 7, 'C');
  m.assign(6, 8, 'D');
  m.assign(0, 11, 'A');
  m.assign(5, 6, 'E');
  printResult("Set 13", {{5, 'E'}, {6, 'A'}}, m);

  // Set 14: Assignments with overlapping borders
  m = interval_map<int, char>('A');
  m.assign(1, 5, 'B');
  m.assign(5, 10, 'C');
  printResult("Set 14", {{1, 'B'}, {5, 'C'}, {10, 'A'}}, m);

  // Set 15: Multiple small assignments within a larger assignment
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(2, 3, 'C');
  m.assign(4, 5, 'D');
  m.assign(6, 7, 'E');
  printResult("Set 15",
              {{1, 'B'},
               {2, 'C'},
               {3, 'B'},
               {4, 'D'},
               {5, 'B'},
               {6, 'E'},
               {7, 'B'},
               {10, 'A'}},
              m);

  // Set 16: Multiple assignments that reverse the effect of the previous
  // assignments
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(2, 8, 'A');
  printResult("Set 16", {{1, 'B'}, {2, 'A'}, {8, 'B'}, {10, 'A'}}, m);

  // Set 17: Multiple overlapping assignments with the same value
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(2, 8, 'B');
  m.assign(3, 7, 'B');
  printResult("Set 17", {{1, 'B'}, {10, 'A'}}, m);

  // Set 18: Nested assignments
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(2, 9, 'C');
  m.assign(3, 8, 'D');
  m.assign(4, 7, 'E');
  printResult("Set 18",
              {{1, 'B'},
               {2, 'C'},
               {3, 'D'},
               {4, 'E'},
               {7, 'D'},
               {8, 'C'},
               {9, 'B'},
               {10, 'A'}},
              m);

  // Set 19: Assignments with empty intervals
  m = interval_map<int, char>('A');
  m.assign(1, 5, 'B');
  m.assign(3, 3, 'C');
  m.assign(5, 2, 'D');
  printResult("Set 19", {{1, 'B'}, {5, 'A'}}, m);

  // Disabled. Todo: Can K varied type limits be defined?
  // Set 20: Assignments that cover the entire range
  // m = interval_map<int, char>('A');
  // m.assign(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(),
  // 'B'); printResult("Set 20", {{std::numeric_limits<int>::min(), 'B'}}, m);

  // Set 21: Overwriting with original value
  m = interval_map<int, char>('A');
  m.assign(1, 5, 'B');
  m.assign(3, 4, 'B');
  printResult("Set 21", {{1, 'B'}, {5, 'A'}}, m);

  // Set 22: Assigning a single element in between
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(5, 6, 'C');
  printResult("Set 22", {{1, 'B'}, {5, 'C'}, {6, 'B'}, {10, 'A'}}, m);

  // Set 23: Removing a single element from between
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(5, 6, 'A');
  printResult("Set 23", {{1, 'B'}, {5, 'A'}, {6, 'B'}, {10, 'A'}}, m);

  // Set 24:
  m = interval_map<int, char>('A');
  m.assign(1, 10, 'B');
  m.assign(5, 6, 'C');
  m.assign(std::numeric_limits<int>::min(), 1, 'B');
  printResult(
      "Set 24",
      {{std::numeric_limits<int>::min(), 'B'}, {5, 'C'}, {6, 'B'}, {10, 'A'}},
      m);
}

int main()
{
  IntervalMapTest();
  return 0;
}