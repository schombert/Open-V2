#include "text_classifiers.h"
#include <cctype>
#include <algorithm>
#include <numeric>
#include "common\\common.h"

bool get_nth_bit(uint32_t nth, const char* start, const char* end) {
	const uint32_t byte = nth / 5;
	const uint32_t bit_in_byte = nth % 5;
	const unsigned char mask = bit_in_byte == 0 ? 0x01 : (bit_in_byte == 1 ? 0x02 : (bit_in_byte == 2 ? 0x04 : (bit_in_byte == 3 ? 0x08 : 0x10)));

	return ((end - start) <= byte) ? false : (((ascii_to_lower(start[byte]) - '_') & mask) != 0);
}

auto get_nth_bit_function(uint32_t nth) {
	const uint32_t bit_in_byte = nth % 5;
	return [byte = uint32_t(nth / 5), mask = bit_in_byte == 0 ? 0x01 : (bit_in_byte == 1 ? 0x02 : (bit_in_byte == 2 ? 0x04 : (bit_in_byte == 3 ? 0x08 : 0x10)))](const char* start, const char* end) {
		return ((end - start) <= byte) ? false : (((ascii_to_lower(start[byte]) - '_') & mask) != 0);
	};
}

uint32_t count_by_bit_classifier(const decltype(get_nth_bit_function(0)) &fa,
							   const std::vector<text_identifier>& options) {
	uint32_t total = 0;
	for (const auto& ident : options) {
		if (fa(ident.text_start, ident.text_end))
			++total;
	}
	return std::max(total, (uint32_t)options.size() - total);
}

uint32_t classifier_set_to_slot(const std::vector<std::function<bool(const char* start, const char* end)>> &vec, const char* ts, const char* te) {
	uint32_t slot = 0;
	uint32_t count = 0;
	for (const auto& fn : vec) {
		slot |= (fn(ts, te) ? 1 : 0) << count++;
	}
	return slot;

}

uint32_t classifier_measure(uint32_t target_size, const std::vector<std::function<bool(const char* start, const char* end)>> &vec, const std::vector<text_identifier>& options) {
	unsigned char* tarray = (unsigned char*)_alloca(target_size);
	memset(tarray, 0, target_size);

	for (const auto& ident : options) {
		uint32_t slot = classifier_set_to_slot(vec, ident.text_start, ident.text_end) ;
		++tarray[slot];
	}
	return std::accumulate(tarray, tarray + target_size, 0ui32, [](uint32_t a, uint32_t b) {
		return a + (b > 0 ? b - 1 : 0);
	});
}

bool fixed_str_equality_ci(const char* as, const char* ae, const char* bs, const char* be) {
	if ((ae - as) != (be - bs))
		return false;
	while (as != ae) {
		if (ascii_to_lower(*(as++)) != *(bs++))
			return false;
	}
	return true;
}

movable_function<unsigned char, const char*, const char*> make_bit_function_classifier_function(const std::vector<text_identifier>& options) {
	uint32_t longest_option = 0;
	for (const auto& o : options) {
		longest_option = std::max(longest_option, (uint32_t)(o.text_end - o.text_start));
	}
	std::vector<std::pair<std::function<bool(const char* start, const char* end)>, uint32_t>> vec;
	for (uint32_t i = 0; i < longest_option * 5; ++i) {
		const auto c = get_nth_bit_function(i);
		vec.emplace_back(c, count_by_bit_classifier(c, options));
	}
	std::sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.second < b.second; });

	std::vector<std::function<bool(const char* start, const char* end)>> ok_vec;
	uint32_t last_measure = std::numeric_limits<uint32_t>::max();
	for (const auto& v : vec) {
		ok_vec.push_back(v.first);
		if ((1ui64 << ok_vec.size()) >= options.size()) {
			const uint32_t measure = classifier_measure(1 << ok_vec.size(), ok_vec, options);
			if (measure >= last_measure) {
				ok_vec.pop_back();
			} else if (measure == 0) {
				if ((1ui64 << (ok_vec.size() - 1)) >= options.size()) {
					bool removed = false;
					do {
						removed = false;
						for (uint32_t u = 0; u < ok_vec.size(); ++u) {
							std::vector<std::function<bool(const char* start, const char* end)>> subvector;
							for (uint32_t v = 0; v < ok_vec.size(); ++v) {
								if (v != u)
									subvector.push_back(ok_vec[v]);
							}
							if (classifier_measure(1 << subvector.size(), subvector, options) == 0) {
								ok_vec = std::move(subvector);
								removed = true;
								break;
							}
						}
					} while (removed && (1ui64 << (ok_vec.size() - 1)) >= options.size());
				}

				std::unique_ptr<unsigned char[]> r_array = std::make_unique<unsigned char[]>(1ui64 << ok_vec.size());
				memset(r_array.get(), 0, 1ui64 << ok_vec.size());

				for (uint32_t k = 0; k < options.size(); ++k) {
					r_array[classifier_set_to_slot(ok_vec, options[k].text_start, options[k].text_end)] = (unsigned char)(k + 1);
				}
				return [result_map = std::move(r_array), options, ok_vec](const char* ts, const char* te) {
					if (unsigned char mapped = result_map[classifier_set_to_slot(ok_vec, ts, te)]; mapped != 0) {
						if (fixed_str_equality_ci(ts, te, options[mapped - 1].text_start, options[mapped - 1].text_end)) {
							return options[mapped - 1].identifier;
						}
					}
					return (unsigned char)0;
				};
			} else {
				last_measure = measure;
			}
		}
	}
	return [options](const char* ts, const char* te) {return (unsigned char)(0); };
}

int fixed_str_compare_ci(const char* as, const char* ae, const char* bs, const char* be) {
	int size_ordering = (int)(ae - as) - (int)(be - bs);
	if (size_ordering != 0)
		return size_ordering;
	while (as != ae) {
		const auto difference = ascii_to_lower(*as) - *bs;
		if (difference != 0)
			return difference;
		++as; ++bs;
	}
	return 0;
}

std::function<unsigned char(const char*, const char*)> make_linear_scan_classifier_function(const std::vector<text_identifier>& options) {
	return [options](const char* ts, const char* te) {
		for (const auto& o : options) {
			if (fixed_str_equality_ci(ts, te, o.text_start, o.text_end))
				return o.identifier;
		}
		return (unsigned char)0;
	};
}


std::function<unsigned char(const char*, const char*)> make_binary_search_classifier_function(const std::vector<text_identifier>& options) {
	return [c = binary_search_classifier(options)](const char* ts, const char* te) {
		return c.classify(ts, te);
	};
}

binary_search_classifier::binary_search_classifier(std::vector<text_identifier>&& o) : options(std::move(o)) {
	std::sort(options.begin(), options.end(),
			  [](const text_identifier& a, const text_identifier& b) {
		return fixed_str_compare_ci(a.text_start, a.text_end, b.text_start, b.text_end) < 0;
	});
}

binary_search_classifier::binary_search_classifier(const std::vector<text_identifier>& o) : options(o) {
	std::sort(options.begin(), options.end(),
			  [](const text_identifier& a, const text_identifier& b) {
		return fixed_str_compare_ci(a.text_start, a.text_end, b.text_start, b.text_end) < 0;
	});
}

void binary_search_classifier::add_option(text_identifier opt) {
	options.push_back(opt);
	std::sort(options.begin(), options.end(),
			  [](const text_identifier& a, const text_identifier& b) {
		return fixed_str_compare_ci(a.text_start, a.text_end, b.text_start, b.text_end) < 0;
	});
}

unsigned char binary_search_classifier::classify(const char * ts, const char * te) const {
	uint32_t lower = 0;
	uint32_t upper = (uint32_t)options.size();

	while (lower != upper) {
		uint32_t partition = lower + (upper - lower) / 2;
		const auto& p = options[partition];
		const auto comparison = fixed_str_compare_ci(ts, te, p.text_start, p.text_end);
		if (comparison < 0) {
			upper = partition;
		} else if (comparison > 0) {
			lower = partition + 1;
		} else {
			return p.identifier;
		}
	}
	return (unsigned char)0;
}
