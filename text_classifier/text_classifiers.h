#pragma once
#include <vector>
#include <functional>
#include "simple_mpl\simple_mpl.hpp"
#include <memory>

template<typename map_type, typename value_type>
auto find_value_or(const char* const as, const char* const ae, value_type def) {
	return map_functions<map_type>::scan_ci(as, ae, def);
};
template<typename map_type, typename value_type>
auto bt_find_value_or(const char* const as, const char* const ae, value_type def) {
	return map_functions<map_type>::bt_scan_ci(as, ae, def);
};

struct text_identifier {
	const char* text_start;
	const char* text_end;
	unsigned char identifier;
};

template<typename return_type, typename ... args>
struct base_callable {
	virtual return_type operator()(args ... a) = 0;
	virtual ~base_callable() {}
};

template<typename holds, typename return_type, typename ... args>
struct movable_function_actual : public holds, base_callable<return_type, args ...> {
	movable_function_actual(holds&& o) : holds(std::move(o)) {
	}
	virtual return_type operator()(args ... a) override {
		return holds::operator()(std::forward<args>(a) ...);
	}
	const movable_function_actual& operator=(movable_function_actual&& other) {
		holds::operator=(std::move(other));
		return *this;
	}
};

template<typename return_type, typename ... args>
struct movable_function {
private:
	std::unique_ptr<base_callable<return_type, args...>> actual;
public:
	movable_function(movable_function&& other) : actual(std::move(other.actual)) {}
	template<typename T>
	movable_function(T&& t) : actual(std::make_unique<movable_function_actual<T, return_type, args...>>(std::move(t))) {}
	return_type operator()(args ... a) const {
		return actual->operator()(std::forward<args>(a) ...);
	}
};

class binary_search_classifier {
private:
	std::vector<text_identifier> options;
public:
	binary_search_classifier(std::vector<text_identifier>&& o);
	binary_search_classifier(const std::vector<text_identifier>& o);
	binary_search_classifier() {}

	void add_option(text_identifier opt);
	unsigned char classify(const char* ts, const char* te) const;
};

movable_function<unsigned char, const char*, const char*> make_bit_function_classifier_function(const std::vector<text_identifier>& options);
std::function<unsigned char(const char*, const char*)> make_linear_scan_classifier_function(const std::vector<text_identifier>& options);
std::function<unsigned char(const char*, const char*)> make_binary_search_classifier_function(const std::vector<text_identifier>& options);

using classifier_function_type = std::function<unsigned char(const char*, const char*)>;
constexpr auto genrate_default_classifier = make_binary_search_classifier_function;
using classifier_generator_type = decltype(genrate_default_classifier);

