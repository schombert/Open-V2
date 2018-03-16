#include "technologies\\technologies.h"
#include "gtest/gtest.h"

const char single_tech[] = 
"tech_name = {\r\n"
" area = army_doctrine\r\n"
"other_date = {}\r\n"
"}";

auto fake_text_handle_lookup();

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}

using namespace technologies;

TEST(technoglogies_tests, pre_parse_single_tech) {
	std::vector<token_group> results;

	tech_category_tag fake_cat(1);
	technologies_manager manager;

	parse_pdx_file(results, single_tech, single_tech + sizeof(single_tech) - 1);
	parse_single_tech_file(fake_cat, fake_text_handle_lookup(), manager, results);

	ASSERT_EQ(1ui64, manager.technologies_container.size());
	ASSERT_EQ(tech_tag(0), manager.technologies_container[tech_tag(0)].id);
	ASSERT_EQ(text_data::text_tag(0), manager.technologies_container[tech_tag(0)].name);

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	ASSERT_NE(manager.named_technology_index.end(), find_result);
	ASSERT_EQ(tech_tag(0), find_result->second);
}
