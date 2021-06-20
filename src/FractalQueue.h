#ifndef FRACTALQUEUE_H_
#define FRACTALQUEUE_H_

#include <vector>
#include <thread>
#include <functional>
#include <queue>

class FractalQueue
{
    typedef std::function<void(void)> fp_t;

public:
    FractalQueue(std::string name, int numThreads = 1);
    ~FractalQueue();

    // dispatch and copy
    void dispatch(const fp_t &op);
    // dispatch and move
    void dispatch(fp_t &&op);

    // Deleted operations
    FractalQueue(const FractalQueue &rhs) = delete;
    FractalQueue &operator=(const FractalQueue &rhs) = delete;
    FractalQueue(FractalQueue &&rhs) = delete;
    FractalQueue &operator=(FractalQueue &&rhs) = delete;

private:
    std::string _name;
    std::mutex _lock;
    std::vector<std::thread> _threads;
    std::queue<fp_t> _q;
    std::condition_variable _cv;
    bool _quit = false;

    void dispatchThreadHandler(void);
};

#endif // FRACTALQUEUE_H_
