#ifndef UTIL_HH
#define UTIL_HH

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <errno.h>
#include <algorithm>
#include <utility>
#include <vector>
#include <iomanip>

using namespace std;
using namespace std::chrono;

namespace util {
    time_point<steady_clock> start;

    bool failed_fork(const vector<proc>& v) {
        return find_if(begin(v), end(v), 
                [&](proc p) { return p.id < 0; }) != end(v);
    }

    string format(long long count) {
        double result = (double) count / 1000000000;
        stringstream s;
        s << "0:" << setprecision(9) << std::fixed << result;
        return s.str();
    }

    string getTimestamp() {
        auto now = steady_clock::now();
        long long timeStamp = duration_cast<nanoseconds>(now - start).count();
        return format(timeStamp);
    }

    int getTimeInSeconds() {
        auto now = steady_clock::now();
        int time = duration_cast<seconds>(now - start).count();
        return time;
    }

    void setStart(time_point<steady_clock> input) {
        start = input;
    }
}

#endif //UTIL_HH
