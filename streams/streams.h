#pragma once
#include <type_traits>

template<typename T, typename NEXT>
struct ptr_range_source;
template<typename WRAPPED, typename NEXT>
struct intermediate_stream;
template<typename WRAPPED>
struct terminal_stream;

template<typename WRAPPED>
struct stream_filter_t;
template<typename WRAPPED>
auto stream_filter(const WRAPPED& w)->stream_filter_t<WRAPPED>;

template<typename value>
struct type_erased_stream {
	virtual void operator()(const value& v) = 0;
	virtual void yield() {};
	virtual void reset() {};
	virtual ~type_erased_stream() {};
};

template<typename value, typename stream_type>
struct generic_stream_t : public type_erased_stream<value> {
	stream_type member_stream;
	generic_stream_t(const stream_type& i) : member_stream(i) {};
	template<typename ...T>
	generic_stream_t(std::true_type, T&& ... args) : member_stream(make_stream(std::forward<T>(args)...)) {}

	virtual void operator()(const value& v) override { member_stream(v); };
	virtual void yield() override { member_stream.yield(); };
	virtual void reset() override { member_stream.reset(); };
};

template<typename value, typename stream_type>
inline auto generic_stream(const stream_type& i) {
	return generic_stream_t<value, stream_type>(i);
}
template<typename value, typename ...T>
inline auto generic_stream_var(T&& ... args) {
	return generic_stream_t<value, decltype(make_stream(std::forward<T>(args)...))>(std::true_type(), std::forward<T>(args)...);
}