#include "simple_db.hpp"
#pragma  warning(push)
#pragma  warning(disable:4701)

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundef"
#endif
#include "sqlite3.h"
#ifdef __llvm__
#pragma clang diagnostic pop
#endif
#pragma  warning(pop)

#include <Windows.h>

bool file_exists(const wchar_t* szPath) {
	DWORD dwAttrib = GetFileAttributes(szPath);
	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool file_exists(const char* szPath) {
	DWORD dwAttrib = GetFileAttributesA(szPath);
	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, bool isSave) {
	int rc;                   /* Function return code */
	sqlite3 *pFile;           /* Database connection opened on zFilename */
	sqlite3_backup *pBackup;  /* Backup object used to copy data */
	sqlite3 *pTo;             /* Database to copy to (pFile or pInMemory) */
	sqlite3 *pFrom;           /* Database to copy from (pFile or pInMemory) */

	rc = sqlite3_open(zFilename, &pFile);
	if (rc == SQLITE_OK) {

		pFrom = (isSave ? pInMemory : pFile);
		pTo = (isSave ? pFile : pInMemory);

		pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
		if (pBackup) {
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
		}
		rc = sqlite3_errcode(pTo);
	}

	(void)sqlite3_close(pFile);
	return rc;
}
