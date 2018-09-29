/*
   SJSU, CS 149, Mak
Assignment:6
Team: forkbomb
Members: Joshua Filstrup, 
Patrick Roteman, 
Tim Stullich

Compiling instructions:

on GCC:
$ g++ -std=c++0x *.cc
$ ./a.out

on Clang:
$ clang++ -std=c++11 -stdlib=libc++ *.cc
$ ./a.out

 */


#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <errno.h>
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
#include <string.h>
#include <fcntl.h>

#include "proc.hh"
#include "util.hh"

using namespace std;
using namespace std::chrono;

const int PIPE_READ = 0;
const int PIPE_WRITE = 1;
const int NUM_CHILDREN = 5;

vector<int*> pipes ( NUM_CHILDREN, new int[2] );
vector<char*> buffers (NUM_CHILDREN, new char[200]);
vector<proc> procs;
vector<char*> fifos;

void normalChildProc(int procNum) {
    int messageNum = 1;
    srand(time(NULL) - procNum);
    while(util::getTimeInSeconds() < 30) {
        int mypipe = open(fifos[procNum], O_WRONLY);
        sprintf(buffers[procNum], "%s: Child %i Message %i",
            util::getTimestamp().c_str(), procNum+1, messageNum);
        write(mypipe, buffers[procNum], strlen(buffers[procNum])+1);
        close(mypipe);
        sleep(rand() % 3 + 1);
        messageNum++;
    }
}

void lastChildProc() {
    int procNum = 4;
    while(util::getTimeInSeconds() < 30) {
        int mypipe = open(fifos[procNum], O_WRONLY);
        string input("Something hi");
        getline(cin, input);
        sprintf(buffers[procNum], "%s %s: Child 5 Message %s",
            util::getTimestamp().c_str(), util::getTimestamp().c_str(), input.c_str());
        write(mypipe, buffers[procNum], strlen(buffers[procNum])+1);
        close(mypipe);
        cout << buffers[procNum] << endl;
    }
}

void unlink() {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        unlink(fifos[i]);
    }
    exit(0);
}

int main(int argc, char** argv) {
    util::setStart(steady_clock::now());
    fifos.push_back("/tmp/myfifo0");
    fifos.push_back("/tmp/myfifo1");
    fifos.push_back("/tmp/myfifo2");
    fifos.push_back("/tmp/myfifo3");
    fifos.push_back("/tmp/myfifo4");
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (mkfifo(fifos[i], 0666) == -1) {
            //cout << errno << endl;
        }
    }
    //unlink();


    //Fork five child procs
    for(int i = 0; i < NUM_CHILDREN; i++) {
        pid_t id = fork();
        if (id > 0) {
            //Parent stores proc
            proc theChild = {id, i, 0, 0};
            procs.push_back(theChild);
        } else {
            //Call whatever the child is going to do
            if (i < NUM_CHILDREN - 1) {
                normalChildProc(i);
            } else {
                lastChildProc();
            }
            //Exit so that children dont do anything in the parent.
            exit(0);
        }
    }
	
    //Everything after this is parent code

	if(util::failed_fork(procs)) {
		cerr << "Forking failed, exiting" << endl;
		exit(1);
    }
    int pipeList[NUM_CHILDREN];
    int bufferSize = 200;
    char* readbuff = new char[bufferSize];
    fd_set inputs[5];
    for(int i = 0; i < NUM_CHILDREN; i++) {
        pipeList[i] = open(fifos[i], O_RDONLY);
        FD_ZERO(&inputs[i]);
        FD_SET(pipeList[i], &inputs[i]);
    }
    struct timespec timeout;
    timeout.tv_nsec = 500; // a very short time
    timeout.tv_sec = 0;
    while (util::getTimeInSeconds() < 30) {
        for(int i = 0; i < NUM_CHILDREN; i++) {
            int selectRet = pselect(pipeList[i]+1, &inputs[i], NULL, NULL, &timeout, NULL);
            if(selectRet == -1) {
                cout << "pselect failed" << endl;
            } else if (selectRet != 0) {
                int numBytes = read(pipeList[i], readbuff, bufferSize);
                if (numBytes != 0) {
                    if(i == 4) cout << "last proc read" << endl;
                    cout << util::getTimestamp() << " " << readbuff << endl;
                    memset(readbuff, 0, bufferSize);
                }
            } else {
                //No input
            }
        }
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        unlink(fifos[i]);
    }

}
