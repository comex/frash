#pragma once
#include <stdbool.h>
typedef char *str_t;
struct _NPStream;
typedef struct _NPStream *npstream_t;
typedef struct idp_t {
    unsigned int id;
    bool is_string;
} idp_t;
