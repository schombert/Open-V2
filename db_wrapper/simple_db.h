#pragma once
#pragma  warning(push)
#pragma  warning(disable:4701)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundef"
#include "sqlite3.h"
#pragma clang diagnostic pop
#pragma  warning(pop)
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>

#ifdef _DEBUG
#include<Windows.h>
#undef min
#undef max
#endif

class db_key {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "INTEGER PRIMARY KEY"; }

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, long long val) {
		sqlite3_bind_int64(stmt, index, val);
	}
	template<int index>
	static long long get_value(sqlite3_stmt* stmt) {
		return sqlite3_column_int64(stmt, index);
	}
};

#define DB_KEY(x) class db_ ## x : public db_key {public: static constexpr const char* column_name = #x; };

class db_hidden_key {
public:
	static constexpr int increment = 0;

	static constexpr const char* get_sql_type() { return "INTEGER PRIMARY KEY"; }

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, long long val) {
	}
};

#define DB_HIDDEN_KEY(x) class db_ ## x : public db_hidden_key {public: static constexpr const char* column_name = #x; };

class db_integer {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "INTEGER"; }
	using return_type = long long;

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, long long val) {
		if(val != 0)
			sqlite3_bind_int64(stmt, index, val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, int val) {
		if (val != 0)
			sqlite3_bind_int(stmt, index, val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, short val) {
		if (val != 0)
			sqlite3_bind_int(stmt, index, val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt) {
		return sqlite3_column_int64(stmt, index);
	}
	template<int index>
	static return_type maybe_get_value(sqlite3_stmt* stmt, int def) {
		if(sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return sqlite3_column_int64(stmt, index);
		return def;
	}
};

#define DB_INTEGER(x) class db_ ## x : public db_integer {public: static constexpr const char* column_name = #x; };

class db_unsigned_integer {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "INTEGER"; }
	using return_type = unsigned long long;

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, unsigned long long val) {
		if (val != 0)
			sqlite3_bind_int64(stmt, index, (long long)val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, unsigned int val) {
		if (val != 0)
			sqlite3_bind_int(stmt, index, (int)val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, unsigned short val) {
		if (val != 0)
			sqlite3_bind_int(stmt, index, (int)val);
		else
			sqlite3_bind_null(stmt, index);
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt) {
		return (return_type)sqlite3_column_int64(stmt, index);
	}
	template<int index>
	static unsigned long long maybe_get_value(sqlite3_stmt* stmt, unsigned long long def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return (unsigned long long)sqlite3_column_int64(stmt, index);
		return def;
	}
	template<int index>
	static unsigned int maybe_get_value(sqlite3_stmt* stmt, unsigned int def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return (unsigned int)sqlite3_column_int64(stmt, index);
		return def;
	}
	template<int index>
	static unsigned short maybe_get_value(sqlite3_stmt* stmt, unsigned short def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return (unsigned short)sqlite3_column_int(stmt, index);
		return def;
	}
};

#define DB_UINT(x) class db_ ## x : public db_unsigned_integer {public: static constexpr const char* column_name = #x; };

class db_float {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "REAL"; }
	using return_type = double;

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, double val) {
		sqlite3_bind_double(stmt, index, val);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, float val) {
		sqlite3_bind_double(stmt, index, static_cast<double>(val));
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt) {
		return sqlite3_column_double(stmt, index);
	}
	template<int index>
	static return_type maybe_get_value(sqlite3_stmt* stmt, double def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return sqlite3_column_double(stmt, index);
		return def;
	}
	template<int index>
	static float maybe_get_value(sqlite3_stmt* stmt, float def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return sqlite3_column_double(stmt, index);
		return def;
	}
};

#define DB_FLOAT(x) class db_ ## x : public db_float {public: static constexpr const char* column_name = #x; };

class db_text {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "TEXT"; }
	using return_type = const char*;

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, char* val) {
		sqlite3_bind_text(stmt, index, val, -1, SQLITE_TRANSIENT);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, char16_t* val) {
		sqlite3_bind_text16(stmt, index, val, -1, SQLITE_TRANSIENT);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, const std::string& val) {
		sqlite3_bind_text(stmt, index, val.c_str(), -1, SQLITE_TRANSIENT);
	}
	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, const std::wstring& val) {
		sqlite3_bind_text16(stmt, index, val.c_str(), -1, SQLITE_TRANSIENT);
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt) {
		return (const char*)sqlite3_column_text(stmt, index);
	}
	template<int index>
	static const char* maybe_get_value(sqlite3_stmt* stmt, const char* def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return (const char*)sqlite3_column_text(stmt, index);
		return def;
	}
	template<int index>
	static const char16_t* maybe_get_value(sqlite3_stmt* stmt, const char16_t* def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return (char16_t*)sqlite3_column_text16(stmt, index);
		return def;
	}
};

#define DB_TEXT(x) class db_ ## x : public db_text {public: static constexpr const char* column_name = #x; };

class db_blob {
public:
	static constexpr int increment = 1;

	static constexpr const char* get_sql_type() { return "BLOB"; }
	using return_type = const void*;

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, void* data, size_t size) {
		sqlite3_bind_blob(stmt, index, data, static_cast<int>(size), SQLITE_TRANSIENT);
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt) {
		return sqlite3_column_blob(stmt, index);
	}
	template<int index>
	static return_type get_value(sqlite3_stmt* stmt, void* def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			return sqlite3_column_blob(stmt, index);
		else
			return def;
	}
};

#define DB_BLOB(x) class db_ ## x : public db_blob {public: static constexpr const char* column_name = #x; };

template <typename O>
class db_obj {
public:
	static constexpr int increment = 1;
	static constexpr const char* get_sql_type() { return "BLOB"; }

	template<int index>
	static void bind_operation(sqlite3_stmt* stmt, const O& obj) {
		sqlite3_bind_blob(stmt, index, &obj, static_cast<int>(sizeof(O)), SQLITE_TRANSIENT);
	}
	template<int index>
	static void get_value(sqlite3_stmt* stmt, O& obj_out) {
		memcpy(&obj_out, sqlite3_column_blob(stmt, index), std::min(sizeof(O), (size_t)sqlite3_column_bytes(stmt, index)));
	}
	template<int index>
	static void maybe_get_value(sqlite3_stmt* stmt, O& obj_out, O def) {
		if (sqlite3_column_type(stmt, index) != SQLITE_NULL)
			memcpy(&obj_out, sqlite3_column_blob(stmt, index), std::min(sizeof(O), (size_t)sqlite3_column_bytes(stmt, index)));
		else
			obj_out = def;
	}
};

#define DB_OBJ(x) class db_ ## x : public db_obj< x > {public: static constexpr const char* column_name = #x; };

template <typename O>
class db_vector {
public:
	static constexpr int increment = 1;
	static constexpr const char* get_sql_type() { return "BLOB"; }

	template<int index, typename A>
	static void bind_operation(sqlite3_stmt* stmt, const std::vector<O, A>& vec) {
		sqlite3_bind_blob(stmt, index, vec.data(), static_cast<int>(vec.size() * sizeof(O)), SQLITE_TRANSIENT);
	}
	template<int index, typename A>
	static void get_value(sqlite3_stmt* stmt, std::vector<O, A>& vec) {
		vec.clear();
		const auto bytes = sqlite3_column_bytes(stmt, index);
		const auto optr = (const O*)sqlite3_column_blob(stmt, index);
		vec.insert(vec.end(), optr, optr + static_cast<size_t>(bytes) / sizeof(O));
	}
};

#define DB_VECTOR(x, y) class db_ ## x : public db_vector< y > {public: static constexpr const char* column_name = #x; };

template<typename ... LIST>
class table_columns;

template<typename T, typename ... REST>
class table_columns<T, REST...> {
public:
	static std::string get_column_definition() {
		if (const auto tail = table_columns<REST ...>::get_column_definition(); tail.length() != 0)
			return std::string(T::column_name) + " " + T::get_sql_type() + ", " + tail;
		else
			return std::string(T::column_name) + " " + T::get_sql_type();
	}
	static std::string get_column_names() {
		if (const auto tail = table_columns<REST ...>::get_column_names(); tail.length() != 0)
			return T::increment == 1 ? std::string(T::column_name) + ", " + tail : tail;
		else
			return T::increment == 1 ? std::string(T::column_name) : "";
	}
	template<int n>
	static std::string get_value_place_names() {
		if (const auto tail = table_columns<REST ...>::template get_value_place_names<n + T::increment>(); tail.length() != 0)
			return T::increment == 1 ? std::string("?") + std::to_string(n) + ", " + tail : tail;
		else
			return T::increment == 1 ? std::string("?") + std::to_string(n) : "";
	}
	template<typename cname, int n, typename V>
	static void bind_to_column(sqlite3_stmt* stmt, const V& val) {
		if constexpr(std::is_same_v<T, cname>)
			T::template bind_operation<n>(stmt, val);
		else
			table_columns<REST ...>::template bind_to_column<cname, n + T::increment>(stmt, val);
	}
	template<typename cname, int n>
	static void bind_to_column(sqlite3_stmt* stmt, void* data, size_t size) {
		if constexpr(std::is_same_v<T, cname>)
			T::template bind_operation<n>(stmt, data, size)
		else
			table_columns<REST ...>::template bind_to_column<cname, n + T::increment>(stmt, data, size);
	}
	template<typename cname, int n>
	static void bind_to_column(sqlite3_stmt* stmt) {
		if constexpr(std::is_same_v<T, cname>)
			sqlite3_bind_null(stmt, n);
		else
			table_columns<REST ...>::template bind_to_column<cname, n + T::increment>(stmt);
	}

	template<typename cname, int n>
	static auto get_value(sqlite3_stmt* stmt) {
		if constexpr(std::is_same_v<T, cname>)
			return T::template get_value<n>(stmt);
		else
			return table_columns<REST ...>::template get_value<cname, n + T::increment>(stmt);
	}

	template<typename cname, int n, typename O>
	static auto get_value(sqlite3_stmt* stmt, O& out) {
		if constexpr(std::is_same_v<T, cname>)
			return T::template get_value<n>(stmt, out);
		else
			return table_columns<REST ...>::template get_value<cname, n + T::increment>(stmt, out);
	}

	template<typename cname, int n, typename O>
	static auto maybe_get_value(sqlite3_stmt* stmt, O def) {
		if constexpr(std::is_same_v<T, cname>)
			return T::template maybe_get_value<n>(stmt, def);
		else
			return table_columns<REST ...>::template maybe_get_value<cname, n + T::increment>(stmt, def);
	}

	template<typename cname, int n, typename O>
	static auto maybe_get_value(sqlite3_stmt* stmt, O& out, const O& def) {
		if constexpr(std::is_same_v<T, cname>)
			return T::template maybe_get_value<n>(stmt, out, def);
		else
			return table_columns<REST ...>::template maybe_get_value<cname, n + T::increment>(stmt, out, def);
	}

};

template<>
class table_columns<> {
public:
	static std::string get_column_definition() {
		return std::string();
	}
	static std::string get_column_names() {
		return std::string();
	}
	template<int n>
	static std::string get_value_place_names() {
		return std::string();
	}
	template<typename cname, int n>
	static void bind_to_column(sqlite3_stmt* stmt) {
	}
	template<typename cname, int n, typename V>
	static void bind_to_column(sqlite3_stmt* stmt, const V& val) {
	}
	template<typename cname, int n>
	static void bind_to_column(sqlite3_stmt* stmt, void* data, size_t size) {
	}
	template<typename cname, int n>
	static void* get_value(sqlite3_stmt* stmt) {
		return nullptr;
	}
	template<typename cname, int n, typename V>
	static void* get_value(sqlite3_stmt* stmt, V& vout) {
		return nullptr;
	}
	template<typename cname, int n, typename T>
	static void* maybe_get_value(sqlite3_stmt* stmt, T def) {
		return nullptr;
	}
	template<typename cname, int n, typename O>
	static void* maybe_get_value(sqlite3_stmt* stmt, O& out, const O& def) {
		return nullptr;
	}
};

template<typename TABLE>
class table_bound_statement {
private:
	sqlite3_stmt* stmt;
public:
	table_bound_statement() : stmt(nullptr) {}
	table_bound_statement(sqlite3_stmt* source) : stmt(source) {}
	table_bound_statement(sqlite3* db, const char* def) : stmt(nullptr){
#ifdef _DEBUG
		int res =
#endif
		sqlite3_prepare_v2(db, def, -1, &stmt, nullptr);
#ifdef _DEBUG
		if (res != SQLITE_OK) {
			OutputDebugStringA((std::string(def) + " SQL PERP ERR: ").c_str());
			OutputDebugStringA(sqlite3_errstr(res));
		}
#endif
	}
	table_bound_statement(const table_bound_statement&) = delete;
	table_bound_statement(table_bound_statement&& i) noexcept {
		std::swap(stmt, i.stmt);
	}
	~table_bound_statement() {
		if(stmt)
			sqlite3_finalize(stmt);
		stmt = nullptr;
	}

	template<typename cname>
	void set_column() {
		TABLE::template bind_to_column<cname, 1>(stmt);
	}
	template<typename cname, typename V>
	void set_column(V val) {
		TABLE::template bind_to_column<cname, 1>(stmt, val);
	}
	template<typename cname>
	void set_column(void* data, size_t size) {
		TABLE::template bind_to_column<cname, 1>(stmt, data, size);
	}

	template<typename cname>
	auto get_column() {
		return TABLE::template get_value<cname, 0>(stmt);
	}
	template<typename cname, typename V>
	auto get_column(V& val) {
		return TABLE::template get_value<cname, 0>(stmt, val);
	}
	template<typename cname, typename V>
	auto maybe_get_column(V def) {
		return TABLE::template maybe_get_value<cname, 0>(stmt, def);
	}
	template<typename cname, typename V>
	auto maybe_get_column(V& valout, const V& def) {
		return TABLE::template maybe_get_value<cname, 0>(stmt, valout, def);
	}

	bool execute() {
		int res = sqlite3_step(stmt);
		if (res == SQLITE_ROW)
			return true;
#ifndef _DEBUG
		sqlite3_reset(stmt);
		return false;
#else
		if (res == SQLITE_OK || res == SQLITE_DONE) {
			sqlite3_reset(stmt);
			return false;
		}

		OutputDebugStringA((std::string(sqlite3_sql(stmt)) + " SQL EXEC ERR: ").c_str());
		OutputDebugStringA(sqlite3_errstr(res));
		return false;
#endif
	}
	template<typename T>
	void forall(const T& function) {
		while (execute()) {
			function(*this);
		}
	}
};

template<typename ... REST>
class table : public table_columns<REST ...> {
public:
	static std::string get_creation_string(const char* table_name) {
		return std::string("CREATE TABLE ") + table_name + " (" + table_columns<REST ...>::get_column_definition() + ")";
	}
	static std::string get_select_string(const char* table_name) {
		return std::string("SELECT ") + table_columns<REST ...>::get_column_names() + " FROM " + table_name;
	}
	static std::string get_delete_string(const char* table_name) {
		return std::string("DELETE FROM ") + table_name;
	}
	static std::string get_insert_string(const char* table_name) {
		return std::string("INSERT INTO ") + table_name + " (" + table_columns<REST ...>::get_column_names() + ") VALUES (" + table_columns<REST ...>::template get_value_place_names<1>() + ")";
	}

};

#define DB_TABLE(x, ...) class db_ ## x : public table< __VA_ARGS__ > {public: static constexpr const char* table_name = #x; };

bool file_exists(const wchar_t* szPath);
bool file_exists(const char* szPath);
int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, bool isSave);

template<typename ... TABLES>
class simple_db;

template<typename FIRST_TABLE, typename ... TABLES>
class simple_db<FIRST_TABLE, TABLES ...> : public simple_db<TABLES ...> {
public:
	bool open_or_create(const char* filename) {
		if (file_exists(filename)) {
			int rc = sqlite3_open(":memory:", &(simple_db<TABLES ...>::db));
			if (rc) {
#ifdef _DEBUG
				OutputDebugStringA("FAILED TO OPEN DB\r\n");
#endif
				sqlite3_close(simple_db<TABLES ...>::db);
				simple_db<TABLES ...>::db = nullptr;
				return false;
			}

			if (loadOrSaveDb(simple_db<TABLES ...>::db, filename, false) != SQLITE_OK) {
#ifdef _DEBUG
				OutputDebugStringA("LOAD ERROR\r\n");
#endif
				sqlite3_close(simple_db<TABLES ...>::db);
				simple_db<TABLES ...>::db = nullptr;
				return false;
			}
			return true;
		} else {
			int rc = sqlite3_open(":memory:", &(simple_db<TABLES ...>::db));
			if (rc) {
#ifdef _DEBUG
				OutputDebugStringA("FAILED TO OPEN DB\r\n");
#endif
				sqlite3_close(simple_db<TABLES ...>::db);
				simple_db<TABLES ...>::db = nullptr;
				return false;
			}

			create_tables();
			return true;
		}
	}

	void save_and_close(const char* filename) {
		if (simple_db<TABLES ...>::db) {
			loadOrSaveDb(simple_db<TABLES ...>::db, filename, true);
			simple_db<TABLES ...>::db = nullptr;
		}
	}

	void create_tables() {
		const auto command = FIRST_TABLE::get_creation_string(FIRST_TABLE::table_name);
		sqlite3_exec(simple_db<TABLES ...>::db, command.c_str(), nullptr, nullptr, nullptr);
		simple_db<TABLES...>::create_tables();
	}

	template<typename t_name>
	void clear_table() {
		if constexpr(std::is_same_v(FIRST_TABLE, t_name)) {
			const auto command = FIRST_TABLE::get_delete_string(FIRST_TABLE::table_name);
			sqlite3_exec(simple_db<TABLES ...>::db, command.c_str(), nullptr, nullptr, nullptr);
		} else {
			simple_db<TABLES ...>::clear_table();
		}
	}

	void clear_all_tables() {
		const auto command = FIRST_TABLE::get_delete_string(FIRST_TABLE::table_name);
		sqlite3_exec(simple_db<TABLES ...>::db, command.c_str(), nullptr, nullptr, nullptr);
		simple_db<TABLES ...>::clear_all_tables();
	}

	template<typename TI>
	auto insert_into() {
		if constexpr(std::is_same_v<FIRST_TABLE, TI>) {
			return table_bound_statement<FIRST_TABLE>(simple_db<TABLES ...>::db, FIRST_TABLE::get_insert_string(FIRST_TABLE::table_name).c_str());
		} else {
			return simple_db<TABLES ...>::template insert_into<TI>();
		}
	}
	template<typename TI>
	auto select_from() {
		if constexpr(std::is_same_v<FIRST_TABLE, TI>) {
			return table_bound_statement<FIRST_TABLE>(simple_db<TABLES ...>::db, FIRST_TABLE::get_select_string(FIRST_TABLE::table_name).c_str());
		} else {
			return simple_db<TABLES ...>::template select_from<TI>();
		}
	}
};

template<>
class simple_db<> {
public:
	sqlite3* db = nullptr;

	void begin_transaction() {
		sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
	}
	void end_transaction() {
		sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
	}
	void close() {
		sqlite3_exec(db, "VACUUM", nullptr, nullptr, nullptr);
		db = nullptr;
	}

	~simple_db() {
		if (db) {
			close();
		}
	}

	static void create_tables() {
	}
	void clear_all_tables() {
	}
	template<typename T>
	void clear_table() {
	}
	template<typename T>
	void* insert_into() {
		return nullptr;
	}
	template<typename T>
	void* select_from() {
		return nullptr;
	}
};
