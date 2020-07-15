#include"headers.h"
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define ARGS_BUFF_SIZE 100
struct cli_args parse_args( char *, char*);