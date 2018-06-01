#include "pch.h"
#include "simple_fs\\simple_fs.h"
#include "fake_fs\\fake_fs.h"
#include <algorithm>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

#define TEST_METHOD(x, y) TEST(x, y)

class test_file_structure {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation VS2007Projects = directory_representation(u"VS2007Projects", f_root);
	directory_representation open_v2_test_data = directory_representation(u"open_v2_test_data", VS2007Projects);
	directory_representation impossible_dir = directory_representation(u"impossible", VS2007Projects);
	directory_representation Debug = directory_representation(u"Debug", open_v2_test_data);
	directory_representation Release= directory_representation(u"Release", open_v2_test_data);
	directory_representation mod_dir = directory_representation(u"mod", open_v2_test_data);
	directory_representation sub_dir = directory_representation(u"sub", open_v2_test_data);
	directory_representation mod_sub_dir = directory_representation(u"sub", mod_dir);
	directory_representation directories_dir = directory_representation(u"directories", open_v2_test_data);
	directory_representation new_root = directory_representation(u"new_root", directories_dir);
	directory_representation sub_dir_a = directory_representation(u"sub_dir_a", directories_dir);
	directory_representation alt_sub_dir_a = directory_representation(u"sub_dir_a", new_root);
	directory_representation sub_dir_b = directory_representation(u"sub_dir_b", directories_dir);

	file_representation alt_a_file_b = file_representation(u"a_file_b.txt", alt_sub_dir_a, "content content content content");
	file_representation a_file_a = file_representation(u"a_file_a.txt", sub_dir_a, "");
	file_representation a_file_b = file_representation(u"a_file_b.txt", sub_dir_a, "");

	file_representation b_file_a = file_representation(u"b_file_a.txt", sub_dir_b, "");
	file_representation b_file_b = file_representation(u"b_file_b.txt", sub_dir_b, "");
	file_representation ignore = file_representation(u"ignore.bat", sub_dir_b, "");

	test_file_structure() {
		set_default_root(f_root);
	}
};

TEST_METHOD(fake_fs_tests, set_root) {
	test_file_structure real_fs;

	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));

	auto path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data", (const wchar_t*)(path_list.front().c_str()));

	
	f.set_root(RANGE("F:\\VS2007Projects\\impossible"));

    path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\impossible", (const wchar_t*)(path_list.front().c_str()));
}

TEST_METHOD(fake_fs_tests, adding_root) {
	test_file_structure real_fs;

	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));
	f.add_root_relative(RANGE("mod"));

	const auto path_list = f.get_root().list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data", (const wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\mod", (const wchar_t*)(path_list[1].c_str()));

	const auto single_string = f.get_root().to_string();
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data;F:\\VS2007Projects\\open_v2_test_data\\mod", (const wchar_t*)(single_string.c_str()));
}

TEST_METHOD(fake_fs_tests, directory_descent) {
	test_file_structure real_fs;

	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data"));
	f.add_root_relative(RANGE("mod"));
	const auto r = f.get_root();

	const auto sub_dir = r.get_directory(RANGE("sub"));

	const auto path_list = sub_dir.list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\sub", (const wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (const wchar_t*)(path_list[1].c_str()));

	const auto single_string = sub_dir.to_string();
	EXPECT_STREQ(L"F:\\VS2007Projects\\open_v2_test_data\\sub;F:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (const wchar_t*)(single_string.c_str()));
}

TEST_METHOD(fake_fs_tests, list_dir) {
	test_file_structure real_fs;

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

TEST_METHOD(fake_fs_tests, list_non_replaced_dir_files) {
	test_file_structure real_fs;

	file_system fs;

	fs.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	fs.add_root_relative(RANGE("new_root"));

	const auto dirb = fs.get_root().get_directory(u"sub_dir_b");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_a.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_b.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_b") != 0; }) == dirb_contents.end());
}

TEST_METHOD(fake_fs_tests, list_replaced_dir_files) {
	test_file_structure real_fs;

	file_system fs;

	fs.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	fs.add_root_relative(RANGE("new_root"));

	const auto dirb = fs.get_root().get_directory(u"sub_dir_a");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_a.txt") == 0 && f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_a") == 0;
	}) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_b.txt") == 0 && f.file_path().compare(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\new_root\\sub_dir_a") == 0;
	}) != dirb_contents.end());
}

TEST_METHOD(fake_fs_tests, open_existing_and_non_existing) {
	test_file_structure real_fs;

	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"b_file_a.txt");
	EXPECT_TRUE(afile.has_value());
	EXPECT_FALSE(bfile.has_value());
}

TEST_METHOD(fake_fs_tests, open_correct_version) {
	test_file_structure real_fs;

	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"a_file_b.txt");
	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);
}

TEST_METHOD(fake_fs_tests, open_correct_nested_file_version) {
	test_file_structure real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto afile = f.get_root().open_file(u"sub_dir_a\\a_file_a.txt");
	const auto bfile = f.get_root().open_file(u"sub_dir_a\\a_file_b.txt");
	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);
}

TEST_METHOD(fake_fs_tests, peek_file) {
	test_file_structure real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories"));
	f.add_root_relative(RANGE("new_root"));

	const auto ua = f.get_root().peek_file(u"sub_dir_a\\a_file_a.txt");
	const auto ub = f.get_root().peek_file(u"sub_dir_a\\a_file_b.txt");

	EXPECT_TRUE(bool(ua));
	EXPECT_TRUE(bool(ub));

	const auto afile = ua->open_file();
	const auto bfile = ub->open_file();

	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);

	EXPECT_FALSE(bool(f.get_root().peek_file(u"bad_file_name.txt")));
}

TEST_METHOD(fake_fs_tests, peek_file_b) {
	test_file_structure real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_a"));

	const auto ua = f.get_root().peek_file(u"a_file_a.txt");
	const auto ub = f.get_root().peek_file(u"a_file_b.txt");

	EXPECT_TRUE(bool(ua));
	EXPECT_TRUE(bool(ub));

	const auto afile = ua->open_file();
	const auto bfile = ub->open_file();

	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() == 0);
}
