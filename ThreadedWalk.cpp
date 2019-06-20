#include <iostream>
#include <fstream>
#include <future>
#include <chrono>
#include <vector>
#include <random>

static const int UP = 1;
static const int DOWN = 2;
static const int START = 100;
static const int M = 75000000;
static const int THREADS = 50;

std::vector<int> partial_histogram(int walks, int threads, int start, int up, int down);

int main(int argc, char** argv) {

	auto start = std::chrono::system_clock::now();

	/* Spawn threads. */
	std::future<std::vector<int>> threads[THREADS];
	for (int i=0; i<THREADS; i++) {
		threads[i] = std::async(std::launch::async, partial_histogram, M, THREADS, START, UP, DOWN);
	}

	/* Retrieve the partial histograms vectors. */
	std::vector<int> data[THREADS];
	for (int i=0; i<THREADS; i++) {
		data[i] = threads[i].get();
	}

	/* Get the maximum time units taken. */
	unsigned int max_time = 0;
	for (int i=0; i<THREADS; i++) {
		if (data[i].size() > max_time) max_time = data[i].size();
	}

	/* Construct the histogram. */
	int histogram[max_time];
	for (int i=0; i<max_time; i++) histogram[i] = 0;
	for (int i=0; i<THREADS; i++) {
		for (int j=0; j<data[i].size(); j++) {
			histogram[j] += data[i][j];
		}
	}

	/* Print the output. */
	std::ofstream output;
	output.open("histogram.txt");
	for (int i=0; i<max_time; i++) {
		output << i+1 << " " << (double)histogram[i]/M << std::endl;
		//output << (double)histogram[i]/M << std::endl;
	}
	output.close();

	/* https://en.cppreference.com/w/cpp/chrono
	 * Print the time elapsed. */
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s\n";

	return 0;

}

std::vector<int> partial_histogram(int walks, int threads, int start, int up, int down) {

	std::mt19937 gen = std::mt19937(std::random_device()());
	std::uniform_int_distribution<int> dist(0,1);

	std::vector<int> v;

	int x;
	unsigned int time;

	for (int m = 0; m < walks/threads; m++) {

		/* Get value for time. */
		time = 0;
		for (x = start; x > 0; time++) {
			if (dist(gen))
				x += up;
			else
				x -= down;
		}

		/* Make space if necessary in v. */
		for (int t = v.size(); t < time+1; t++) v.push_back(0);

		/* Increment. */
		++v.at(time);

	}

	return v;

}
