#include "pch.h"
#include "simple_fs\\simple_fs.h"
#include <algorithm>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

#define TEST_METHOD(x, y) TEST(x, y)


TEST_METHOD(simple_fs_tests, default_root) {
	file_system f;
	const auto path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
#ifdef _DEBUG
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\Debug", (const wchar_t*)(path_list.front().c_str()));
#else
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\Release", (const wchar_t*)(path_list.front().c_str()));
#endif
}


TEST_METHOD(simple_fs_tests, set_root) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data");

	auto path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data", (const wchar_t*)(path_list.front().c_str()));

	f.set_root(u"D:\\VS2007Projects\\impossible");

	path_list = f.get_root().list_paths();
	EXPECT_EQ(1ui64, path_list.size());
	EXPECT_STREQ(L"D:\\VS2007Projects\\impossible", (const wchar_t*)(path_list.front().c_str()));
}

TEST_METHOD(simple_fs_tests, adding_root) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data");
	f.add_root_relative(u"mod");

	const auto path_list = f.get_root().list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data", (const wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\mod", (const wchar_t*)(path_list[1].c_str()));

	const auto single_string = f.get_root().to_string();
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data;D:\\VS2007Projects\\open_v2_test_data\\mod", (const wchar_t*)(single_string.c_str()));
}

TEST_METHOD(simple_fs_tests, directory_descent) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data");
	f.add_root_relative(u"mod");
	const auto r = f.get_root();

	const auto sub_dir = r.get_directory(u"sub");

	const auto path_list = sub_dir.list_paths();
	EXPECT_EQ(2ui64, path_list.size());
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\sub", (const wchar_t*)(path_list[0].c_str()));
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (const wchar_t*)(path_list[1].c_str()));

	const auto single_string = sub_dir.to_string();
	EXPECT_STREQ(L"D:\\VS2007Projects\\open_v2_test_data\\sub;D:\\VS2007Projects\\open_v2_test_data\\mod\\sub", (const wchar_t*)(single_string.c_str()));
}

TEST_METHOD(simple_fs_tests, list_dir) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto sub_dir_list = f.get_root().list_directories();
	EXPECT_EQ(3ui64, sub_dir_list.size());
	const auto r1 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\new_root") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r1);
	const auto r2 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\sub_dir_a") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r2);
	const auto r3 = std::find_if(sub_dir_list.begin(), sub_dir_list.end(), [](const auto& d) { return d.name().compare(u"\\sub_dir_b") == 0; }) != sub_dir_list.end();
	EXPECT_TRUE(r3);
}

TEST_METHOD(simple_fs_tests, list_non_replaced_dir_files) {
	file_system fs;

	fs.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	fs.add_root_relative(u"new_root");

	const auto dirb = fs.get_root().get_directory(u"sub_dir_b");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_a.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_name().compare(u"b_file_b.txt") == 0; }) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) { return f.file_path().compare(u"D:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_b") != 0; }) == dirb_contents.end());
}

TEST_METHOD(simple_fs_tests, list_replaced_dir_files) {
	file_system fs;

	fs.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	fs.add_root_relative(u"new_root");

	const auto dirb = fs.get_root().get_directory(u"sub_dir_a");
	const auto dirb_contents = dirb.list_files(u".txt");
	EXPECT_EQ(2ui64, dirb_contents.size());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_a.txt") == 0 && f.file_path().compare(u"D:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_a") == 0;
	}) != dirb_contents.end());
	EXPECT_TRUE(std::find_if(dirb_contents.begin(), dirb_contents.end(), [](const auto &f) {
		return f.file_name().compare(u"a_file_b.txt") == 0 && f.file_path().compare(u"D:\\VS2007Projects\\open_v2_test_data\\directories\\new_root\\sub_dir_a") == 0;
	}) != dirb_contents.end());
}

TEST_METHOD(simple_fs_tests, open_existing_and_non_existing) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"b_file_a.txt");
	EXPECT_TRUE(afile.has_value());
	EXPECT_FALSE(bfile.has_value());
}

TEST_METHOD(simple_fs_tests, open_correct_version) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto dirb = f.get_root().get_directory(u"sub_dir_a");
	const auto afile = dirb.open_file(u"a_file_a.txt");
	const auto bfile = dirb.open_file(u"a_file_b.txt");
	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);
}

TEST_METHOD(simple_fs_tests, open_correct_nested_file_version) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto afile = f.get_root().open_file(u"sub_dir_a\\a_file_a.txt");
	const auto bfile = f.get_root().open_file(u"sub_dir_a\\a_file_b.txt");
	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);
}

TEST_METHOD(simple_fs_tests, peek_file) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto ua = f.get_root().peek_file(u"sub_dir_a\\a_file_a.txt");
	const auto ub = f.get_root().peek_file(u"sub_dir_a\\a_file_b.txt");

	const auto afile = ua->open_file();
	const auto bfile = ub->open_file();

	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() > 0);

	EXPECT_FALSE(bool(f.get_root().peek_file(u"bad_file_name.txt")));
}

TEST_METHOD(simple_fs_tests, peek_file_b) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories\\sub_dir_a");

	const auto ua = f.get_root().peek_file(u"a_file_a.txt");
	const auto ub = f.get_root().peek_file(u"a_file_b.txt");

	EXPECT_TRUE(bool(ua));
	EXPECT_TRUE(bool(ub));

	const auto afile = ua->open_file();
	const auto bfile = ub->open_file();

	EXPECT_TRUE(afile->size() == 0);
	EXPECT_TRUE(bfile->size() == 0);
}

TEST(simple_fs_tests, fs_serialize) {
	file_system f;

	f.set_root(u"D:\\VS2007Projects\\open_v2_test_data\\directories");
	f.add_root_relative(u"new_root");

	const auto fslist = f.get_root().list_paths();
	EXPECT_EQ(2ui64, fslist.size());
	EXPECT_EQ(std::u16string(u"D:\\VS2007Projects\\open_v2_test_data\\directories"), fslist[0]);
	EXPECT_EQ(std::u16string(u"D:\\VS2007Projects\\open_v2_test_data\\directories\\new_root"), fslist[1]);

	const auto ssize = serialization::serialize_size(f);
	std::vector<std::byte> d;
	d.resize(ssize);

	std::byte* iptr = d.data();
	serialization::serialize(iptr, f);

	file_system fsb;

	fsb.set_root(u"F:\\nloc");

	std::byte const* optr = d.data();
	serialization::deserialize(optr, fsb);

	const auto fsblist = fsb.get_root().list_paths();
	EXPECT_EQ(2ui64, fsblist.size());
	EXPECT_EQ(std::u16string(u"F:\\nloc"), fsblist[0]);
	EXPECT_EQ(std::u16string(u"F:\\nloc\\new_root"), fsblist[1]);
}
