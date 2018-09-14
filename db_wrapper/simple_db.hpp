#pragma once
#include "simple_db.h"
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
