#pragma once
#include "common.h"
#include "concurrency_tools\concurrency_tools.hpp"

namespace detail {
	template<typename T>
	struct addr_struct {
		static uint16_t value() {
			static int32_t v = 1;
			return uint64_t(&v);
		}
	};
}

struct index_tuple {
	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t c = 0;
};

template<>
struct std::hash<index_tuple> {
	typedef index_tuple argument_type;
	typedef size_t result_type;

	size_t operator()(index_tuple const& v) {
		return std::hash<uint64_t>()(v.a ^ v.b ^ v.c);
	}
};

class type_erased_vector_base {
public:
	virtual void* ptr() = 0;
	virtual ~type_erased_vector_base() {}
};

template<typename T>
using test_vector_type = tagged_vector<T, int32_t, padded_aligned_allocator_64<T>>;

template<typename T>
class type_erased_vector : public type_erased_vector_base {
public:
	test_vector_type<T> vec;

	type_erased_vector() {
		vec.resize(16);
	}
	virtual void* ptr() { return &vec; };
	virtual ~type_erased_vector() {}
};


class type_erased_storage_base {
public:
	virtual void* ptr() = 0;
	virtual ~type_erased_storage_base() {}
};

template<typename T>
class type_erased_storage : public type_erased_storage_base {
public:
	stable_variable_vector_storage_mk_2<T, 8, 2048, alignment_type::padded_cache_aligned> storage;

	virtual void* ptr() { return &storage; };
	virtual ~type_erased_storage() {}
};


template<typename T>
index_tuple get_index_tuple() {
	return index_tuple{ detail::addr_struct<T>::value(), 0, 0 };
}
template<typename T, typename U>
index_tuple get_index_tuple(U u) {
	return index_tuple{ detail::addr_struct<T>::value(), detail::addr_struct<U>::value(), to_index(u) };
}

template<typename imitation_type>
class test_ws {
public:
	mutable std::unordered_map<index_tuple, std::unique_ptr<type_erased_vector_base>> stored_values;
	mutable std::unordered_map<uint64_t, std::unique_ptr<type_erased_storage_base>> storage;
	mutable std::unordered_map<uint64_t, int32_t> tag_count;

	template<typename val_type>
	test_vector_type<val_type>& get_vector(index_tuple key) const {
		std::unordered_map<index_tuple, std::unique_ptr<type_erased_vector_base>>::iterator v_location = [_this = this]() {
			if(auto it = _this->stored_values.find(key); it != stored_values.end()) {
				return it;
			} else {
				return _this->stored_values.emplace(key, std::make_unique<type_erased_vector<val_type>>());
			}
		}();
		return *((test_vector_type<val_type>*)(v_location->second->ptr()));
	}

	template<typename val_type>
	stable_variable_vector_storage_mk_2<val_type, 8, 2048, alignment_type::padded_cache_aligned>& get_storage() const {
		auto v_location = [_this = this]() {
			if(auto it = _this->stored_values.find(detail::addr_struct<val_type>::value()); it != stored_values.end()) {
				return it;
			} else {
				return _this->stored_values.emplace(detail::addr_struct<val_type>::value(), std::make_unique<type_erased_storage<val_type>>());
			}
		}();
		return *((stable_variable_vector_storage_mk_2<val_type, 8, 2048, alignment_type::padded_cache_aligned>*)(v_location->second->ptr()));
	}

	template<typename index, typename tag_type>
	auto get(tag_type t) const -> decltype(std::declval<imitation_type&>().get<index>(t)) { 
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t))>;

		static val_type bad_value = val_type();
		if(!is_valid_index(t))
			return bad_value;

		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index>());

		if(v.size() <= to_index(t))
			v.resize(to_index(t) + 1);

		return v[to_index(t)];
	}
	template<typename index, typename tag_type, typename value_type>
	auto set(tag_type t, value_type const& v) -> decltype(std::declval<imitation_type&>().set<index>(t, v)) {
		assert(is_valid_index(t));
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t))>;
		test_vector_type<val_type>& vec = get_vector<val_type>(get_index_tuple<index>());

		if(vec.size() <= to_index(t))
			vec.resize(to_index(t) + 1);

		vec[to_index(t)] = v;
	}
	template<typename index>
	auto get_row() const -> decltype(std::declval<imitation_type&>().get_row<index>()) {
		using row_type = decltype(std::declval<imitation_type&>().get_row<index>());
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t))>;

		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index>());

		return row_type(v.data(), int32_t(v.size()));
	}
	
	template<typename index>
	int32_t size() const {
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t))>;
		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index>());
		
		return int32_t(v.size());
	}

	template<typename index>
	void resize(int32_t sz) const {
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t))>;
		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index>());

		return v.resize(sz);
	}

	template<typename index, typename tag_type, typename inner_type>
	auto get(tag_type t, inner_type u) const -> decltype(std::declval<imitation_type&>().get<index>(t, u)) {
		assert(is_valid_index(t) && is_valid_index(u));
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t, u))>;
		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index, tag_type>(t));

		if(v.size() <= to_index(u))
			v.resize(to_index(u) + 1);

		return v[to_index(u)];
	}

	template<typename index, typename tag_type, typename inner_type, typename value_type>
	auto set(tag_type t, inner_type u, value_type const& v) -> decltype(std::declval<imitation_type&>().set<index>(t, u, v)) {
		assert(is_valid_index(t) && is_valid_index(u));
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t, u))>;
		test_vector_type<val_type>& vec = get_vector<val_type>(get_index_tuple<index, tag_type>(t));

		if(vec.size() <= to_index(u))
			vec.resize(to_index(u) + 1);

		vec[to_index(u)] = v;
	}
	template<typename index, typename tag_type>
	auto get_row(tag_type t) const -> decltype(std::declval<imitation_type&>().get_row<index>(t)) {
		assert(is_valid_index(t));
		using row_type = decltype(std::declval<imitation_type&>().get_row<index>(t));
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t, u))>;

		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index, tag_type>(t));

		return row_type(v.data(), int32_t(v.size()));
	}
	template<typename index, typename tag_type>
	int32_t size(tag_type t) const {
		assert(is_valid_index(t));
		using val_type = std::remove_reference_t<decltype(std::declval<imitation_type&>().get<index>(t, u))>;
		test_vector_type<val_type>& v = get_vector<val_type>(get_index_tuple<index, tag_type>(t));

		return int32_t(v.size());
	}

	template<typename tag_type>
	void set_max_tags(int32_t n) {
		tag_count[detail::addr_struct<tag_type>::value()] = n;
	}

	template<typename tag_type, typename fn>
	void for_each(fn const& f) {
		int32_t const cmax = tag_count[detail::addr_struct<tag_type>::value()];
		for(int32_t i = 0; i < cmax; ++i) {
			f(tag_type(typename tag_type::value_base_t(i)));
		}
	}
	template<typename tag_type, typename fn, typename pt = int32_t>
	void par_for_each(fn const& f, pt&& p = 0) {
		int32_t const cmax = tag_count[detail::addr_struct<tag_type>::value()];
		for(int32_t i = 0; i < cmax; ++i) {
			f(tag_type(typename tag_type::value_base_t(i)));
		}
	}
	template<typename tag_type, typename fn>
	void if_any(fn const& f) {
		int32_t const cmax = tag_count[detail::addr_struct<tag_type>::value()];
		for(int32_t i = 0; i < cmax; ++i) {
			if(f(tag_type(typename tag_type::value_base_t(i))))
				return true;
		}
		return false;
	}

	template<typename index_tag, typename value_type>
	void add_item(index_tag& i, value_type const& v) {
		::add_item(get_storage<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	void remove_item(index_tag& i, value_type const& v) {
		::remove_item(get_storage<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	void contains_item(index_tag i, value_type const& v) const {
		::contains_item(get_storage<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag>
	void resize(index_tag& i, uint32_t sz) {
		::resize(get_storage<individuator_of<index_tag>>(), i, sz);
	}
	template<typename index_tag>
	auto get_range(index_tag i) const -> decltype(::get_range(get_storage<individuator_of<index_tag>>(), i)) {
		return ::get_range(get_storage<individuator_of<index_tag>>(), i);
	}
	template<typename index_tag>
	auto get_size(index_tag i) const -> decltype(::get_size(get_storage<individuator_of<index_tag>>(), i)) {
		return ::get_size(get_storage<individuator_of<index_tag>>(), i);
	}
	template<typename index_tag, typename FN>
	void remove_item_if(index_tag& i, const FN& f) {
		::remove_item_if(get_storage<individuator_of<index_tag>>(), i, f);
	}
	template<typename index_tag>
	void clear(index_tag& i) {
		::clear(get_storage<individuator_of<index_tag>>(), i);
	}
};
