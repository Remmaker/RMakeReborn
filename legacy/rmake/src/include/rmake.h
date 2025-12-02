#ifndef RMAKE_H
#define RMAKE_H

#define CONF_COMMENT_TOKEN '#'

#define CONF_STR_LEN 300
#define CONF_VAR_POOL_LEN 100
#define CONF_VAR_NAME_LEN 100
#define CONF_EXT_CMD_LEN 500
#define MAX_ROW 100
#define MAX_COL 500

#define SAVE_DIR ".rm-cache"
#define SAVE_FILE_INDEXER "src_list.rmc"
#define SAVE_DIR_OBJ "obj"

#define FILE_LIST_SIZE 15
#define FILE_NAME_LEN 21

#include <stdlib.h>
#include <dirent.h>

#include "rmake_str.h"

typedef enum {
    ERR_NO = 0,
    ERR_PARSER,
    ERR_STR,
    ERR_CMD,
    ERR_SYS,
    ERR_INDEXER,
} RMAKE_ERROR;

typedef struct {
    char pool[10][CONF_VAR_POOL_LEN];
    char names[10][CONF_VAR_NAME_LEN];
    unsigned int count;
} Conf_var;

typedef struct {
    bool cpp;
    bool run;

    char include[CONF_STR_LEN];
    char src[CONF_STR_LEN];
    
    char flag[CONF_STR_LEN];
    char lflag[CONF_STR_LEN];
    char lpath[CONF_STR_LEN];
    
    char target[CONF_STR_LEN];

    char usedExtras[10][CONF_VAR_NAME_LEN];

} Conf_build;

typedef struct {
    char run[CONF_STR_LEN];
} Conf_run;


typedef struct Conf_extras {
    char name[CONF_VAR_NAME_LEN];
    char cmd[CONF_EXT_CMD_LEN];
    char when[7];
    bool restart;

    struct Conf_extras* next;
} Conf_extras;

typedef struct {
    char files[FILE_LIST_SIZE][FILE_NAME_LEN];
    char files_need_build[FILE_LIST_SIZE][FILE_NAME_LEN];
    char folder[FILE_NAME_LEN];
    bool used;
    bool need_build;
} Conf_indexer;

typedef struct {
    Conf_var var;
    Conf_build build;
    Conf_run run;
    Conf_extras extras;
    Conf_indexer indexer;
    unsigned short err;
} Config;

bool rmake_checkExt(char* str, char* ext);
void rmake_mkConf(char conf[MAX_ROW][MAX_COL]);
Config rmake_getConfig(char conf[MAX_ROW][MAX_COL]);
void rmake_deleteConfig(Config* conf);
void rmake_indexer_fillSrc(Config* conf);
void rmake_compute_file_to_build(Config* conf);
void rmake_indexer_save(Config* conf);
#endif
