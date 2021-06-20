#include <queue>
#include <mutex>
#include <condition_variable>

#include "FractalQueue.h"

FractalQueue::FractalQueue(std::string name, int numThreads) : _name{std::move(name)}, _threads(numThreads)
{
    for (size_t i = 0; i < _threads.size(); i++)
    {
        _threads[i] = std::thread(&FractalQueue::dispatchThreadHandler, this);
    }
}

FractalQueue::~FractalQueue()
{
    // Signal to dispatch threads that it's time to wrap up
    std::unique_lock<std::mutex> lock(_lock);
    _quit = true;
    lock.unlock();
    _cv.notify_all();

    // Wait for threads to finish before we exit
    for (size_t i = 0; i < _threads.size(); i++)
    {
        if (_threads[i].joinable())
        {
            _threads[i].join();
        }
    }
}

void FractalQueue::dispatch(const fp_t &op)
{
    std::unique_lock<std::mutex> lock(_lock);
    _q.push(op);

    lock.unlock();
    _cv.notify_all();
}

void FractalQueue::dispatch(fp_t &&op)
{
    std::unique_lock<std::mutex> lock(_lock);
    _q.push(std::move(op));

    lock.unlock();
    _cv.notify_all();
}

void FractalQueue::dispatchThreadHandler(void)
{
    std::unique_lock<std::mutex> lock(_lock);

    do
    {
        //Wait until we have data or a quit signal
        _cv.wait(lock, [this]
                 { return (_q.size() || _quit); });

        //after wait, we own the lock
        if (!_quit && _q.size())
        {
            auto op = std::move(_q.front());
            _q.pop();

            //unlock now that we're done messing with the queue
            lock.unlock();

            op();

            lock.lock();
        }
    } while (!_quit);
}
