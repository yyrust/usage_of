#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

class Racer
{
public:
    size_t counter_;
    size_t duration_;
    pthread_mutex_t mutex_;

    Racer()
    : counter_(0)
    , duration_(1)
    , mutex_(PTHREAD_MUTEX_INITIALIZER)
    {
    }

    ~Racer()
    {
        pthread_mutex_destroy(&mutex_);
    }

    void inc()
    {
        pthread_mutex_lock(&mutex_);
        counter_++;
        pthread_mutex_unlock(&mutex_);
    }

    void setDuration(int seconds)
    {
        duration_ = seconds;
    }
};

void *thread_func(void *arg)
{
    Racer *racer = (Racer*)arg;
    struct timeval tv_start, tv_end;
    bool timed_out = false;
    const size_t kDurationUs = racer->duration_ * 1000000;
    gettimeofday(&tv_start, 0);
    do {
        const size_t kLoopCount = 10000;
        for (size_t i = 0; i < kLoopCount; i++) {
            racer->inc();
        }
        gettimeofday(&tv_end, 0);
        size_t diff_us = (tv_end.tv_sec * 1000000 + tv_end.tv_usec) -
                         (tv_start.tv_sec * 1000000 + tv_start.tv_usec);
        timed_out = kDurationUs <= diff_us;
    } while(!timed_out);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <duration-in-seconds>\n", argv[0]);
        return -1;
    }
    const size_t kNumThreads = 4;
    pthread_t threads[kNumThreads];
    Racer racer;
    racer.setDuration(atoi(argv[1]));
    for (size_t i = 0; i < kNumThreads; i++) {
        pthread_create(&threads[i], 0, thread_func, &racer);
    }
    for (size_t i = 0; i < kNumThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("result: %zu\n", racer.counter_);
    return 0;
}
