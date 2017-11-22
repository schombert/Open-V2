#include "performance.h"
#include "parsers.hpp"
#include <windows.h>
#include <iostream>

int parse_artisan(char* start, char* end) {
	std::vector<token_group> results;
	parse_pdx_file(results, start, end);
	return (int)results.size();
}

class file_read_tester {
public:
	HANDLE fin;
	char* fixed_copy = nullptr;
	char* temp_copy = nullptr;
	unsigned long long filesize = 0;

	file_read_tester(const wchar_t* filename) {
		HANDLE fin = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fin == INVALID_HANDLE_VALUE) {
			std::cout << "Could not open file: " << filename << std::endl;
		} else {
			LARGE_INTEGER file_size;
			GetFileSizeEx(fin, &file_size);

			fixed_copy = new char[file_size.QuadPart];
			temp_copy = new char[file_size.QuadPart];
			filesize = file_size.QuadPart;

			ReadFile(fin, fixed_copy, (DWORD)filesize, NULL, NULL);
		}
	}
	~file_read_tester() {
		delete[] fixed_copy;
		delete[] temp_copy;
		CloseHandle(fin);
	}
	int test_function() {
		memcpy(temp_copy, fixed_copy, filesize);
		return (int)filesize;
	}
};

class pdx_read_tester : public file_read_tester {
public:
	pdx_read_tester() : file_read_tester(TEXT("artisans.txt")) {
	}
	int test_function() {
		file_read_tester::test_function();

		std::vector<token_group> results;
		parse_pdx_file(results, temp_copy, temp_copy + filesize);
		return (int)results.size();
	}
};

class csv_read_tester : public file_read_tester {
public:
	csv_read_tester() : file_read_tester(TEXT("text.csv")) {
	}
	int test_function() {
		file_read_tester::test_function();
		char* start = temp_copy;
		int total;
		while (start != temp_copy + filesize) {
			start = parse_first_and_nth_csv_values(2, start, temp_copy + filesize, ';', [&total](std::pair<char*, char*> tag, std::pair<char*, char*> value) {
				total += (int)(tag.second - tag.first) + (int)(value.second - value.first);
			});
		}
		return total;
	}
};

int __cdecl main() {
	
	logging_object log;
	{
		test_object<50, 20, pdx_read_tester> pdx_to;
		std::cout << pdx_to.log_function(log, "pdx parse artisans.txt") << std::endl;
	}
	{
		test_object<50, 5, csv_read_tester> csv_to;
		std::cout << csv_to.log_function(log, "csv parse text.csv") << std::endl;
	}
	return 0;
}

