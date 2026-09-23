#include <bits/stdc++.h>
using namespace std;


int tob = 0; int match = 0; int paper = 0;

mutex mtx;

condition_variable consume;
condition_variable produce;

// conditions : agent does not know abt smoker threads !!

// agent waits after producing the ingredients !
// no condition that agent should waits for smoker to consume

// no need for produce condition_variable, just that we need to keep the count

void agent() {

    while (1) {
        {
            unique_lock<mutex> lock(mtx);


            int r1 = -1; int r2 = -1;

            while (r1 == r2) {
                r1 = rand() % 3;
                r2 = rand() % 3;
            }

            if (r1 == 0) {
                tob ++;
            } else if (r1 == 1) {
                match ++;
            } else {
                paper ++;
            }

            if (r2 == 0) {
                tob ++;
            } else if (r2 == 1) {
                match ++;
            } else {
                paper ++;
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
            return (match > 0 && paper > 0);
        });

        match --;
        paper --;
    }

}

void s2() {

    {
        unique_lock<mutex> lock(mtx);

        consume.wait(lock, [] {   // wait till it gets match & paper
            return (tob > 0 && paper > 0);
        });

        tob --;
        paper --;
    }

}

void s3() {

    {
        unique_lock<mutex> lock(mtx);
        consume.wait(lock, [] {   // wait till it gets match & paper
            return (tob > 0 && match > 0);
        });

        tob --;
        match --;
    }

}

int main() {

}
