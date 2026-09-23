#include <bits/stdc++.h>
using namespace std;


int tob = 0; int match = 0; int paper = 0;

mutex mtx;

condition_variable consume;
condition_variable produce;

// conditions : agent does not know abt smoker threads !!


void agent() {

    while (1) {
        {
            unique_lock<mutex> lock(mtx);

            // checks the condition first & decides whether to go to sleep or not !!
            produce.wait(lock, [] {   // wait for tob, match, paper to consume !!
                return !(tob || match || paper);
            });

            int r1 = -1; int r2 = -1;

            while (r1 == r2) {
                r1 = rand() % 3;
                r2 = rand() % 3;
            }

            if (r1 == 0) {
                tob = 1;
            } else if (r1 == 1) {
                match = 1;
            } else {
                paper = 1;
            }

            if (r2 == 0) {
                tob = 1;
            } else if (r2 == 1) {
                match = 1;
            } else {
                paper = 1;
            }
        }

        // releases lock
        consume.notify_all();  // signals all smokers

    }
}

void s1() {
    // need match, paper

    {
        unique_lock<mutex> lock(mtx);
        consume.wait(lock, [] {   // wait till it gets match & paper
            return (match && paper);
        });

        match = 0;
        paper = 0;
    }

    // release lock & wake agent
    produce.notify_one();  // agent
}

void s2() {

    {
        unique_lock<mutex> lock(mtx);

        consume.wait(lock, [] {   // wait till it gets match & paper
            return (tob && paper);
        });

        tob = 0;
        paper = 0;
    }

    // consume, wake agent thread
    produce.notify_one();  // agent
}

void s3() {

    {
        unique_lock<mutex> lock(mtx);
        consume.wait(lock, [] {   // wait till it gets match & paper
            return (tob && match);
        });

        tob = 0;
        match = 0;
    }

    //
    produce.notify_one();  // notifies agent
}

int main() {
    
}
