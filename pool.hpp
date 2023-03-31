#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <future>
#include <condition_variable>

struct pool {
	int num_threads = 0;
	std::vector<std::thread> threads;
	std::mutex m;
	std::condition_variable cv;
	std::queue<std::packaged_task<void()>> tasks;
	std::atomic<int> remaining_tasks{0};

	pool();

	void start_pool();
	void join_threads();
	void run_tasks();

	//Visual studio has a broken complier so this code doesnt actually work but it should
	//There is a thread spanning 5 years that ends with the devs saying it will never be fixed
	//on the main version of the compiler and a branch with the fix has no known release date
	/*template <typename F, typename... Args>
	std::future<typename std::result_of<F(Args...)>::type> add_job(F&& f, Args&&... args) {
		std::packaged_task<typename std::result_of<F(Args...)>::type()> task(std::bind(f, args...));
		auto result = task.get_future();
		tasks.emplace(std::move(task));
		remaining_tasks++;
		return result;
	}*/

	template<typename F, typename... Args>
	auto enqueue_task(F&& f, Args&&... args) {
		using return_type = typename std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
		auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<return_type> res = task->get_future();
		tasks.emplace([task]() { (*task)(); });
		remaining_tasks++;
		return res;
	}
};