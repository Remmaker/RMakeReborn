#include "include/rmake_sys.h"

#include "include/rmake.h"

void rmake_create_cache_dir();
void rmake_create_cache_files();

int rmake_mkdir(char* path)
{
    #if defined(__WIN32)
        return mkdir(path);
    #else
        return mkdir(path, 0777);
    #endif
}

void rmake_init_sys()
{
    rmake_create_cache_dir();
    rmake_create_cache_files();
}

bool rmake_dir_exist(char* path)
{
    bool ret = false;

    DIR* dir = opendir(path);
    ret = dir != NULL;
    closedir(dir);

    return ret;
}

bool rmake_file_exist(char* path)
{
    bool ret = false;

    FILE* f = fopen(path, "r");
    ret = f != NULL;

    if (f != NULL)
    {
        fclose(f);
    }
    
    return ret;

}

void rmake_create_cache_dir()
{
    if (!rmake_dir_exist(SAVE_DIR))
    {
        int ok = rmake_mkdir(SAVE_DIR);
        if (ok != 0)
        {
            printf("Rmake error: unexpected mkdir failed with code %d\n", ok);
        }
    }
    else
    {
        printf("Rmake note: %s already exist, reusing it ...\n", SAVE_DIR);
    }

    char tmp[100];
    snprintf(tmp, 100, "%s/%s", SAVE_DIR, SAVE_DIR_OBJ);
    if (!rmake_dir_exist(tmp))
    {
        int ok = rmake_mkdir(tmp);
        if (ok != 0)
        {
            printf("Rmake error: unexpected mkdir failed with code %d\n", ok);
        }
    }
}

void rmake_create_cache_files()
{
    char tmp[100];
    snprintf(tmp, 100, "%s/%s", SAVE_DIR, SAVE_FILE_INDEXER);
    if (!rmake_file_exist(tmp))
    {
        FILE* f = fopen(tmp, "w");
        fclose(f);
    }
}