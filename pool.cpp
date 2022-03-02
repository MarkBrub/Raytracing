#include "pool.hpp"

pool::pool() {
	num_threads = std::thread::hardware_concurrency() / 2;
}

void pool::start_pool() {
	for(int x = 0; x < num_threads; x++) {
		threads.emplace_back(std::thread(&pool::run_tasks, this));
	}
}

void pool::join_threads() {
	for(auto& thread : threads) {
		thread.join();
	}
}

void pool::run_tasks() {
	while(remaining_tasks > 0) {
		std::packaged_task<void(void)> f;
		{
			std::unique_lock<std::mutex> l(m);
			//Exit thread when there are no tasks left to run
			if(tasks.empty()) return;

			f = std::move(tasks.front());
			tasks.pop();
		}

		//Abort if the function is not valid
		if(!f.valid()) return;

		//Run the task
		f();

		remaining_tasks--;
	}
}