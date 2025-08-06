#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>

class AsyncRunner
{
public:

	using Job = std::fucntion<void()>;
	const int8_t Size = 16;

	AysncRunner()
	{
        jobsQueue.reserve(Size);

        jobsThread = std::make_shared<std::thread>([this] () 
        {
            std::vector<Job> workQueue;
            workQueue.reserve(Size);

            while(true)
            {
                workQueue.clear();
                {
                    std::unique_lock<std::mutex> lock(jobsLock);
                    jobsCond.wait(lock, [this] {return !jobsQueue.empty();});
                    workQueue.swap(jobsQueue);
                }
                for(auto const &work: workQueue )
                {
                    if(!work)  // on null job type
                        return;
                    try
                    {
                        work();
                    }
                    catch(std::runtime_error const &e)
                    {
                    }
                }
            }
        }
	}
    ˜AsyncRunner()
    {
        postJob(Job()); //empty job for exit;
        jobsThread->join();
    }

    template<typename JobType>
    void postJob( JobType &&job)
    {
        {
            std::lock_guard<std::mutex> guard(jobsLock);
            jobsQueue.emplace_back(std::forward<JobType>(job);
        }
        jobsCond.notify_one();
    }

private:
	std::unique_ptr<std::thread> jobsThread;
	std::vector<Job>             jobsQueue;
	std::mutex	                 jobsLock;
    std::contidition_variable    jobsCond;	
};
