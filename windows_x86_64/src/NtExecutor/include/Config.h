#pragma once

#define SYSNAME_MAXLEN (128)
#define SYSARG_MAXNUM (12)
#define SYSNUM_MAXRANGE (4096)
#define JSON_MAXLEN (128*1024)
#define BUF_RESERVE_LEN (1048576)
#define ALLOC_PADDING_SIZE (128)
#define OUTPTR_MAXCOUNT (1024)

typedef int (*SyscallWrapper)(unsigned int* args);