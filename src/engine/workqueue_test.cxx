#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>

#include "workqueue.hxx"

using namespace std;


TEST_CASE("creates a worker and give it a task") {
    bool calledTask = false;

    auto task {[&calledTask]() {
        calledTask = true;
    }};

    WorkQueue workQueue {1};

    workQueue.push(task);
    
    workQueue.wait();

    REQUIRE(calledTask == true);
}

TEST_CASE("creates a two workers and give them one task") {
    int coutCalledTask = 0;

    auto task {[&coutCalledTask]() {
        coutCalledTask++;
    }};

    WorkQueue workQueue {2};

    workQueue.push(task);
    
    workQueue.wait();

    REQUIRE(coutCalledTask == 1);
}

TEST_CASE("creates a one worker and give it two tasks") {
    bool calledTaskOne = false;
    bool calledTaskTwo = false;

    auto taskOne {[&calledTaskOne]() {
        calledTaskOne = true;
    }};

    auto taskTwo {[&calledTaskTwo]() {
        calledTaskTwo = true;
    }};


    WorkQueue workQueue {1};

    workQueue.push(taskOne);
    workQueue.push(taskTwo);

    
    workQueue.wait();

    REQUIRE(calledTaskOne == true);
    REQUIRE(calledTaskTwo == true);
}


TEST_CASE("creates a two worker and give them two tasks") {
    bool calledTaskOne = false;
    bool calledTaskTwo = false;

    auto taskOne {[&calledTaskOne]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        calledTaskOne = true;
    }};

    auto taskTwo {[&calledTaskTwo]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        calledTaskTwo = true;
    }};


    WorkQueue workQueue {2};

    workQueue.push(taskOne);
    workQueue.push(taskTwo);

    
    workQueue.wait();

    REQUIRE(calledTaskOne == true);
    REQUIRE(calledTaskTwo == true);
}
