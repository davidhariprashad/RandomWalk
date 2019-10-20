#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <utility>
#include <cstdint>

template <typename Callable, typename ... Args>
std::chrono::nanoseconds elapsed_duration(Callable&& f, Args&& ... args)
{
	const auto begin = std::chrono::steady_clock::now();
	f(std::forward<Args>(args) ...);
	return (std::chrono::steady_clock::now() - begin);
}

template <typename Callable, typename ... Args>
int64_t elapsed_seconds(Callable&& f, Args&& ... args)
{
	return std::chrono::duration_cast<std::chrono::seconds>(elapsed_duration(
		std::forward<Callable>(f), std::forward<Args>(args) ...)).count();
}

template <typename Callable, typename ... Args>
int64_t elapsed_microseconds(Callable&& f, Args&& ... args)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_duration(
		std::forward<Callable>(f), std::forward<Args>(args) ...)).count();
}

template <typename Callable, typename ... Args>
int64_t elapsed_milliseconds(Callable&& f, Args&& ... args)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_duration(
		std::forward<Callable>(f), std::forward<Args>(args) ...)).count();
}

template <typename Callable, typename ... Args>
int64_t elapsed_nanoseconds(Callable&& f, Args&& ... args)
{
	return (elapsed_duration(std::forward<Callable>(f), std::forward<Args>(args) ...)).count();
}

#endif
