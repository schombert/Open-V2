#pragma once
#include "streams.h"
#include <type_traits>
#include <tuple>

template<typename A, typename B, typename NEXT>
struct _has_yield : public std::false_type { };
template<typename A, typename NEXT>
struct _has_yield<A, decltype(void(std::declval<A>().yield(std::declval<NEXT&>()))), NEXT> : public std::true_type { };
template<typename A, typename NEXT>
constexpr bool has_yield = _has_yield<A, void, NEXT>::value;

template<typename A, typename B>
struct _has_reset : public std::false_type { };
template<typename A>
struct _has_reset<A, decltype(void(std::declval<A>().reset()))> : public std::true_type { };
template<typename A>
constexpr bool has_reset = _has_reset<A, void>::value;

template<typename A, typename B, typename VALUE, typename NEXT>
struct _has_next_operator : public std::false_type {
};
template<typename A, typename VALUE, typename NEXT>
struct _has_next_operator<A, decltype(void(std::declval<A>()(std::declval<VALUE>(), std::declval<NEXT&>()))), VALUE, NEXT> : public std::true_type {
};
template<typename A, typename VALUE, typename NEXT>
constexpr bool has_next_operator = _has_next_operator<A, void, VALUE, NEXT>::value;


template<typename A, typename B>
struct _has_yield_v : public std::false_type { };
template<typename A>
struct _has_yield_v<A, decltype(void(std::declval<A>().yield()))> : public std::true_type { };
template<typename A>
constexpr bool has_yield_v = _has_yield_v<A, void>::value;

template<typename T, typename NEXT>
struct ptr_range_source {
private:
	T* start;
	T* end;
	NEXT next;
public:
	ptr_range_source(T* s, T* e, const NEXT& n) : start(s), end(e), next(n) {}
	auto yield() {
		for (; start != end; ++start) {
			next(start);
		}
		if constexpr(has_yield_v<NEXT>)
			return next.yield();
	}
	void reset() { next.reset();  };
};

template<typename WRAPPED, typename NEXT>
struct intermediate_stream {
private:
	WRAPPED wrapped;
	NEXT next;
public:
	intermediate_stream(const WRAPPED& w, const NEXT& n) : wrapped(w), next(n) {}
	template<typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
	void operator()(T i) {
		if constexpr(has_next_operator<WRAPPED, T, NEXT>)
			wrapped(i, next);
		else if constexpr(std::is_reference_v<decltype(wrapped(i))> || std::is_scalar_v<decltype(wrapped(i))>)
			next(wrapped(i));
		else {
			auto r = wrapped(i);
			next(r);
		}
	}
	template<typename T, typename = std::enable_if_t<!std::is_scalar_v<T>>>
	void operator()(T& i) {
		if constexpr(has_next_operator<WRAPPED, T, NEXT>)
			wrapped(i, next);
		else if constexpr(std::is_reference_v<decltype(wrapped(i))> || std::is_scalar_v<decltype(wrapped(i))>)
			next(wrapped(i));
		else {
			auto r = wrapped(i);
			next(r);
		}
	}

	auto yield() {
		if constexpr(has_yield<WRAPPED, NEXT>)
			wrapped.yield(next);
		if constexpr(has_yield_v<NEXT>)
			return next.yield();
	}
	void reset() {
		if constexpr(has_reset<WRAPPED>)
			wrapped.reset();
		next.reset();
	}
};

template<typename WRAPPED>
struct terminal_stream {
private:
	WRAPPED wrapped;
public:
	terminal_stream(const WRAPPED& w) : wrapped(w) {}
	template<typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
	void operator()(T i) {
		wrapped(i);
	}
	template<typename T, typename = std::enable_if_t<!std::is_scalar_v<T>>>
	void operator()(T& i) {
		wrapped(i);
	}

	auto yield() {
		if constexpr(has_yield_v<WRAPPED>)
			return wrapped.yield();
	}
	void reset() {
		if constexpr(has_reset<WRAPPED>)
			wrapped.reset();
	}
};

template<typename WRAPPED>
struct stream_filter_t {
private:
	WRAPPED wrapped;
public:
	stream_filter_t(const WRAPPED& w) : wrapped(w) {}
	template<typename T, typename NEXT, typename = std::enable_if_t<std::is_scalar_v<T>>>
	void operator()(T i, NEXT& n) {
		if (wrapped(i))
			n(i);
	}
	template<typename T, typename NEXT, typename = std::enable_if_t<!std::is_scalar_v<T>>>
	void operator()(T& i, NEXT& n) {
		if (wrapped(i))
			n(i);
	}
};

template<typename WRAPPED>
auto stream_filter(const WRAPPED& w) -> stream_filter_t<WRAPPED> {
	return stream_filter_t<WRAPPED>(w);
}

template<typename T, typename NEXT>
auto _make_stream(const std::pair<T*, T*>& p, const NEXT& n) -> ptr_range_source<T, NEXT> {
	return ptr_range_source<T, NEXT>(p.first, p.second, n);
}

template<typename T, size_t N, typename NEXT>
auto _make_stream(T(&a)[N], const NEXT& x) -> ptr_range_source<T, NEXT> {
	return ptr_range_source<T, NEXT>(a, a + N, x);
}

template<size_t N, typename NEXT>
auto _make_stream(char(&a)[N], const NEXT& x) -> ptr_range_source<char, NEXT> {
	return ptr_range_source<char, NEXT>(a, a + N - 1, x);
}

template<size_t N, typename NEXT>
auto _make_stream(const char(&a)[N], const NEXT& x) -> ptr_range_source<const char, NEXT> {
	return ptr_range_source<const char, NEXT>(a, a + N - 1, x);
}

template<typename F, typename NEXT>
auto _make_stream(const F& f, const NEXT& x) -> intermediate_stream<F, NEXT> {
	return intermediate_stream<F, NEXT>(f, x);
}

template<typename ... REST>
auto make_curried_stream(const REST& ... rest) {
	return [=](const auto& ... tail) {
		return make_stream(rest ..., tail...);
	};
}

template<typename T>
auto make_stream(const T& in) -> terminal_stream<T> {
	return terminal_stream<T>(in);
}

template<typename T, typename ... REST>
auto make_stream(const T& in, const REST& ... rest) {
	return _make_stream(in, make_stream(rest...));
}