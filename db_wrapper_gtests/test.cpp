#include "pch.h"
#include "db_wrapper\simple_db.h"

struct three_ints {
	int a;
	int b;
	int c;
};


DB_KEY(f_key);
DB_INTEGER(int_a);
DB_INTEGER(int_b);
DB_TABLE(first_table, db_f_key, db_int_a, db_int_b);

DB_HIDDEN_KEY(h_key);
DB_FLOAT(fval);
DB_TEXT(string_value);
DB_TEXT(string_value_b);
DB_OBJ(three_ints);
DB_VECTOR(vint, int);
DB_TABLE(second_table, db_h_key, db_fval, db_string_value, db_string_value_b, db_three_ints, db_vint);

using table_a = db_first_table;
using table_b = db_second_table;
using test_db = simple_db <table_a, table_b>;

TEST(create_string, db_wrapper_tests) {
	EXPECT_EQ(std::string("CREATE TABLE first_table (f_key INTEGER PRIMARY KEY, int_a INTEGER, int_b INTEGER)"), table_a::get_creation_string(table_a::table_name));
	EXPECT_EQ(std::string("CREATE TABLE second_table (h_key INTEGER PRIMARY KEY, fval REAL, string_value TEXT, string_value_b TEXT, three_ints BLOB, vint BLOB)"), table_b::get_creation_string(table_b::table_name));
}
TEST(insert_string, db_wrapper_tests) {
	EXPECT_EQ(std::string("INSERT INTO first_table (f_key, int_a, int_b) VALUES (?1, ?2, ?3)"), table_a::get_insert_string(table_a::table_name));
	EXPECT_EQ(std::string("INSERT INTO second_table (fval, string_value, string_value_b, three_ints, vint) VALUES (?1, ?2, ?3, ?4, ?5)"), table_b::get_insert_string(table_b::table_name));
}
TEST(select_string, db_wrapper_tests) {
	EXPECT_EQ(std::string("SELECT f_key, int_a, int_b FROM first_table"), table_a::get_select_string(table_a::table_name));
	EXPECT_EQ(std::string("SELECT fval, string_value, string_value_b, three_ints, vint FROM second_table"), table_b::get_select_string(table_b::table_name));
}
TEST(delete_string, db_wrapper_tests) {
	EXPECT_EQ(std::string("DELETE FROM first_table"), table_a::get_delete_string(table_a::table_name));
	EXPECT_EQ(std::string("DELETE FROM second_table"), table_b::get_delete_string(table_b::table_name));
}

TEST(open_close_retrieve_empty_table_a, db_wrapper_tests) {
	test_db db;
	EXPECT_TRUE(db.open_or_create("test.db"));
	db.begin_transaction();
	db.clear_all_tables();

	{
		auto inserter = db.insert_into<db_first_table>();
		inserter.set_column<db_f_key>(1);
		inserter.set_column<db_int_a>(10);
		inserter.set_column<db_int_b>(-50);
		inserter.execute();

		inserter.set_column<db_f_key>(2);
		inserter.set_column<db_int_a>(5);
		inserter.set_column<db_int_b>(-10);
		inserter.execute();
	}

	db.end_transaction();
	db.save_and_close("test.db");

	EXPECT_TRUE(db.open_or_create("test.db"));
	{
		auto selector = db.select_from<db_first_table>();

		int count_entries = 0;
		selector.forall([&count_entries](auto& s) {
			++count_entries;
			if (s.template get_column<db_f_key>() == 1) {
				EXPECT_EQ(10 , s.template get_column<db_int_a>());
				EXPECT_EQ(-50 , s.template get_column<db_int_b>());
			} else {
				EXPECT_EQ(5 , s.template get_column<db_int_a>());
				EXPECT_EQ(-10 , s.template get_column<db_int_b>());
			}
		});
		EXPECT_EQ(2, count_entries);
	}
	db.close();
}

TEST(open_close_retrieve_empty_table_b, db_wrapper_tests) {
	test_db db;
	std::vector<int> vi = { 0, 6, 18, -1, 9 };
	std::string strb("string b");

	EXPECT_TRUE(db.open_or_create("test_b.db"));
	db.begin_transaction();
	db.clear_all_tables();

	{
		auto inserter = db.insert_into<db_second_table>();
		inserter.set_column<db_fval>(0.75);
		inserter.set_column<db_string_value>("STRINGA");

		inserter.set_column<db_string_value_b>(strb);
		three_ints k{ 10,-5, 17275 };
		inserter.set_column<db_three_ints>(k);

		inserter.set_column<db_vint>(vi);
		inserter.execute();
	}

	db.end_transaction();
	db.save_and_close("test_b.db");

	EXPECT_TRUE(db.open_or_create("test_b.db"));
	{
		auto selector = db.select_from<db_second_table>();

		int count_entries = 0;
		selector.forall([&count_entries](auto& s) {
			++count_entries;
			EXPECT_EQ(0.75 , s.template get_column<db_fval>());
			EXPECT_EQ(0, std::string("STRINGA").compare(s.template get_column<db_string_value>()));
			EXPECT_EQ(0 , std::string("string b").compare(s.template get_column<db_string_value_b>()));
			three_ints o{ 0, 0, 0 };
			s.template get_column<db_three_ints>(o);
			EXPECT_EQ(10, o.a);
			EXPECT_EQ(-5, o.b);
			EXPECT_EQ(17275, o.c);
			std::vector<int> vo;
			s.template get_column<db_vint>(vo);
			EXPECT_EQ(5ui64, vo.size());
			EXPECT_EQ(18, vo[2]);
			EXPECT_EQ(9, vo[4]);
			EXPECT_EQ(0, vo[0]);
		});
		EXPECT_EQ(1, count_entries);
	}
	db.close();
}

TEST(set_retrieve_null, db_wrapper_tests) {
	test_db db;

	EXPECT_TRUE(db.open_or_create("test_c.db"));
	db.begin_transaction();
	db.clear_all_tables();

	{
		auto inserter = db.insert_into<db_second_table>();
		inserter.set_column<db_fval>();
		inserter.set_column<db_string_value>();
		inserter.set_column<db_string_value_b>();
		inserter.set_column<db_three_ints>();
		inserter.set_column<db_vint>();
		inserter.execute();
	}

	db.end_transaction();
	db.save_and_close("test_c.db");

	EXPECT_TRUE(db.open_or_create("test_c.db"));
	{
		auto selector = db.select_from<db_second_table>();

		int count_entries = 0;
		selector.forall([&count_entries](auto& s) {
			++count_entries;
			EXPECT_EQ(0.5 , s.template maybe_get_column<db_fval>(0.5));
			EXPECT_EQ(0 , std::string("test_string a").compare(s.template maybe_get_column<db_string_value>("test_string a")));
			EXPECT_EQ(0 , std::string("test_string b").compare(s.template maybe_get_column<db_string_value_b>("test_string b")));
			three_ints o{ 0, 0, 0 };
			s.template maybe_get_column<db_three_ints>(o, three_ints{ 5, 10, 20 });
			EXPECT_EQ(5, o.a);
			EXPECT_EQ(10, o.b);
			EXPECT_EQ(20, o.c);
			std::vector<int> vo;
			s.template get_column<db_vint>(vo);
			EXPECT_EQ(0ui64, vo.size());
		});
		EXPECT_EQ(1, count_entries);
	}
	db.close();
}

TEST(test_maybe_non_null, db_wrapper_tests) {
	test_db db;
	std::vector<int> vi = { 0, 6, 18, -1, 9 };
	std::string strb("string b");

	EXPECT_TRUE(db.open_or_create("test_d.db"));
	db.begin_transaction();
	db.clear_all_tables();

	{
		auto inserter = db.insert_into<db_second_table>();
		inserter.set_column<db_fval>(0.75);
		inserter.set_column<db_string_value>("STRINGA");

		inserter.set_column<db_string_value_b>(strb);
		three_ints k{ 10,-5, 17275 };
		inserter.set_column<db_three_ints>(k);

		inserter.set_column<db_vint>(vi);
		inserter.execute();
	}

	db.end_transaction();
	db.save_and_close("test_d.db");

	EXPECT_TRUE(db.open_or_create("test_d.db"));
	{
		auto selector = db.select_from<db_second_table>();

		int count_entries = 0;
		selector.forall([&count_entries](auto& s) {
			++count_entries;
			EXPECT_EQ(0.75 , s.template maybe_get_column<db_fval>(0.0));
			EXPECT_EQ(0 , std::string("STRINGA").compare(s.template maybe_get_column<db_string_value>("no match")));
			EXPECT_EQ(0 , std::string("string b").compare(s.template maybe_get_column<db_string_value_b>("no match")));
			three_ints o{ 0, 0, 0 };
			s.template maybe_get_column<db_three_ints>(o, three_ints{ 1,2,3 });
			EXPECT_EQ(10, o.a);
			EXPECT_EQ(-5, o.b);
			EXPECT_EQ(17275, o.c);
		});
		EXPECT_EQ(1, count_entries);
	}
	db.close();
}
