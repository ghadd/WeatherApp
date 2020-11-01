#ifndef LOGIC_H
#define LOGIC_H

#include <array>
#include <vector>
#include <utility>

#include <QDir>
#include <QDirIterator>
#include <QDate>

template <typename T>
using data_t = std::pair<T, T>;

template <typename T>
T interpolate(const std::vector<data_t<T> > &dataSet, T atX)
{
    T result = 0;

    for (unsigned i = 0; i < dataSet.size(); i++)
    {
        T term = dataSet[i].second;
        for (unsigned j = 0; j < dataSet.size(); j++)
        {
            if (j!=i)
                term = term * (atX - dataSet[j].first) / (T)(dataSet[i].first - dataSet[j].first);
        }

        result += term;
    }

    return result;
}

#endif // LOGIC_H
