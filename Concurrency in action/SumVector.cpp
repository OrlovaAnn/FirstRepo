#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>

template<typename Iter,
	typename ValueType = typename std::iterator_traits<Iter>::value_type>
	void sumVector(Iter i_begin, Iter i_end, ValueType& o_result)
{
		o_result = ValueType(); // default value
		for (Iter it = i_begin; it != i_end; ++it)
		{
			o_result += *it;
		}
}

template<typename Iter,
	typename ValueType = typename std::iterator_traits<Iter>::value_type>
	ValueType parrallel_sum(Iter i_begin, Iter i_end)
{
	unsigned long const length = std::distance(i_begin, i_end);
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;
	std::vector<ValueType> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);

	Iter block_start = i_begin;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iter block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(sumVector<Iter>, block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}
	sumVector<Iter>(block_start, i_end, results[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));
	ValueType res = ValueType();
	return std::accumulate(results.begin(), results.end(), res);

}

int main()
{

	std::vector<float> vect{1, 2, 3, 4, 5, 6, 7, 8, 9, 10.5, 11, 12, 13, 14, 15, 16};
	float res = parrallel_sum(vect.begin(), vect.end());
	std::cout << res << std::endl;
	system("pause");
	return 0;
}