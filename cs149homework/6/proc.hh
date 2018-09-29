#ifndef PROC_HH
#define PROC_HH

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <algorithm>
#include <utility>


struct proc {
    pid_t id;
    unsigned child_num;
    unsigned message_count;
    unsigned sequence_count;
};

#endif //PROC_HH
