#ifndef LOGIC_H
#define LOGIC_H

#include <array>
#include <vector>
#include <utility>
#include <algorithm>

#include <QDir>
#include <QDirIterator>
#include <QDate>

template<typename T>
using data_t = std::pair<T, T>;

template<typename T>
T interpolate(const std::vector<data_t<T> > &dataSet, T atX) {
    T result = 0;

    for (unsigned i = 0; i < dataSet.size(); i++) {
        T term = dataSet[i].second;
        for (unsigned j = 0; j < dataSet.size(); j++) {
            if (j != i)
                term = term * (atX - dataSet[j].first) / (T) (dataSet[i].first - dataSet[j].first);
        }

        result += term;
    }

    return result;
}

template <typename T>
void pairsort(std::vector<QDate> &a, std::vector<data_t<T>> &b)
{
    assert(a.size() == b.size());
    std::vector<std::pair<QDate, data_t<T> > > pairs(a.size());

    for (size_t i = 0; i < a.size(); i++)
    {
        pairs[i].first = a[i];
        pairs[i].second = b[i];
    }

    std::sort(pairs.begin(), pairs.end());

    // Modifying original arrays
    for (size_t i = 0; i < a.size(); i++)
    {
        a[i] = pairs[i].first;
        b[i] = pairs[i].second;
    }
}


#endif // LOGIC_H
