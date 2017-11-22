#include "pch.h"
#include "simple_fs\\simple_fs.h"
#include <algorithm>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

#define TEST_METHOD(x, y) TEST(x, y)


TEST_METHOD(default_root, simple_fs_tests) {
	file_system f;
	const auto path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
#ifdef _DEBUG
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\Debug", (wchar_t*)(path_list.front().c_str()));
#else
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\Release", (wchar_t*)(path_list.front().c_str()));
#endif
}


TEST_METHOD(set_root, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));

	auto path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data", (wchar_t*)(path_list.front().c_str()));

	f.set_root(RANGE("F:\\VS2007Projects\\impossible"));

	path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\impossible", (wchar_t*)(path_list.front().c_str()));
}

TEST_METHOD(adding_root, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));
	f.add_root_relative(RANGE("mod"));

	const auto path_list = f.get_root().list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data", (wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\mod", (wchar_t*)(path_list[1].c_str()));

	const auto single_string = f.get_root().to_string();
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data;F:\\VS2007Projects\\open_v2_test_data\\mod", (wchar_t*)(single_string.c_str()));
}

TEST_METHOD(directory_descent, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));
	f.add_root_relative(RANGE("mod"));
	const auto r = f.get_root();

	const auto sub_dir = r.get_directory(RANGE("sub"));

	const auto path_list = sub_dir.list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\sub", (wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (wchar_t*)(path_list[1].c_str()));

	const auto single_string = sub_dir.to_string();
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\sub;F:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (wchar_t*)(single_string.c_str()));
}

TEST_METHOD(list_dir, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto sub_dir_list = f.get_root().list_directories();
	EXPECT_EQ(3ui64, sub_dir_list.size());
	const auto r1 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\new_root") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r1);
	const auto r2 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\sub_dir_a") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r2);
	const auto r3 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\sub_dir_b") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r3);
}

TEST_METHOD(list_non_replaced_dir_files, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_b");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_a.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_b.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_b") != 0; }) == dirb_contents.end());
}

TEST_METHOD(list_replaced_dir_files, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_a.txt") == 0 && f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_a") == 0;
	}) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_b.txt") == 0 && f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\new_root\\sub_dir_a") == 0;
	}) != dirb_contents.end());
}

TEST_METHOD(open_existing_and_non_existing, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"b_file_a.txt");
	EXPECT_TRUE(afile.has_value());
	EXPECT_FALSE(bfile.has_value());
}

TEST_METHOD(open_correct_version, simple_fs_tests) {
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"a_file_b.txt");
	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);
}