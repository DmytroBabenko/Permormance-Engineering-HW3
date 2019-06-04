//
// Created by Dmytro Babenko on 2019-01-30.
//


#include "Worker.h"


Worker::Worker()
: mEnabled(true)
, mThread(&Worker::threadExec, this)
, mIsRunning(false)
{
}

Worker::~Worker()
{
    mEnabled = false;
    mCv.notify_one();
    mThread.join();
}


bool Worker::isFree() const
{
    std::unique_lock<std::mutex> locker(mMutex);
    return  mTasks.empty() && !mIsRunning;
}

size_t Worker::getNumberOfTasks() const
{
    std::unique_lock<std::mutex> locker(mMutex);
    return mTasks.size();
}


void Worker::threadExec()
{
    while (mEnabled)
    {
        std::unique_lock<std::mutex> locker(mMutex);
        mCv.wait(locker, [&]()
        {
            return !mTasks.empty() || !mEnabled;
        });

        while (!mTasks.empty())
        {
            auto task = mTasks.front();
            mTasks.pop();

            locker.unlock();
            mIsRunning = true;
            task();
            mIsRunning = false;
            locker.lock();
        }
    }
}

void Worker::addTaskToQueue(std::function<void()> task)
{
    std::unique_lock<std::mutex> locker(mMutex);
    mTasks.push(task);
    mCv.notify_one();
}


