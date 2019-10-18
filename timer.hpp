#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <utility>
#include <cstdint>

namespace timer
{

template <typename Duration, typename Callable, typename ... Args>
Duration duration(Callable&& f, Args&& ... args)
{
	auto begin = std::chrono::steady_clock::now();
	f(std::forward<Args>(args) ...);
	return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - begin);
}

template <typename Callable, typename ... Args>
uint64_t seconds(Callable&& f, Args&& ... args)
{
	typedef std::chrono::seconds Period;
	return duration<Period>(std::forward<Callable>(f), std::forward<Args>(args) ...).count();
}

template <typename Callable, typename ... Args>
uint64_t microseconds(Callable&& f, Args&& ... args)
{
	typedef std::chrono::microseconds Period;
	return duration<Period>(std::forward<Callable>(f), std::forward<Args>(args) ...).count();
}

template <typename Callable, typename ... Args>
uint64_t milliseconds(Callable&& f, Args&& ... args)
{
	typedef std::chrono::milliseconds Period;
	return duration<Period>(std::forward<Callable>(f), std::forward<Args>(args) ...).count();
}

template <typename Callable, typename ... Args>
uint64_t nanoseconds(Callable&& f, Args&& ... args)
{
	typedef std::chrono::nanoseconds Period;
	return duration<Period>(std::forward<Callable>(f), std::forward<Args>(args) ...).count();
}

} // namespace timer

#endif
