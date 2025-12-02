#include "include/rmake_sys.h"
#include "include/rmake_cmd.h"

void TODO(char* in)
{
    printf("Todo: %s Not Implemented Yet\n", in);
}

void printHelp()
{
    printf(""" \
Use: Rmake [FILE] | [CMD]\n \
\tFILE : optional config filename\n \
\t       if not specified rmake will search for the first .rm in working directory\n \
\tCMD : optional command build | run | help (show this)\n \
\t      if not specified rmake will execute build\n \
            """);
}

void printfConfig(Config config)
{
    char extra[10*CONF_VAR_NAME_LEN];
    str_mset(extra, 10*CONF_VAR_NAME_LEN, 0);

    for (int i = 0; i < 10; ++i)
    {
        if (config.build.usedExtras[i][0] == 0x00) break;
        str_concat(extra, 10*CONF_VAR_NAME_LEN, config.build.usedExtras[i]);
        str_concat(extra, 10*CONF_VAR_NAME_LEN, " ");
    }

    printf("\
Build { \n\
\tinclude=%s\n\
\tsrc=%s\n\
\tflag=%s \n\
\tlflag=%s  \n\
\tlpath=%s   \n\
\ttarget=%s  \n\
\textraUsed=%s\n\
}\n", config.build.include, config.build.src, config.build.flag, config.build.lflag,
config.build.lpath, config.build.target, extra);

    printf("\
Run {\n\
\trun=%s\n\
}\n", config.run.run);

    if (config.extras.name[0] != 0x00)
    {
        printf("%s {\n\
\tcmd=%s\n\
\trestart=%d\n\
}\n", config.extras.name,  config.extras.cmd, config.extras.restart);
    }

    if (config.extras.next != NULL)
    {
        Conf_extras* extra = config.extras.next;
        do {
            printf("%s {\n\
\tcmd=%s\n\
\trestart=%d\n\
}\n", extra->name, extra->cmd, extra->restart);
            extra = extra->next;
        } while(extra != NULL);
    }
}

typedef enum {
    FNO = 0,
    FCMD = 1,
    FFILE = 2,
    FBOTH = 3,
} FLAGS;

static char RMAKE_RUNTIME_EXEC[100];

#ifdef RM_VERSION
void printVersion()
{
    printf("Rmake v%s\n", RM_VERSION);
}
#endif



int main(int argc, char* argv[])
{

    str_mset(RMAKE_RUNTIME_EXEC, 100, 0);
    snprintf(RMAKE_RUNTIME_EXEC, 100, argv[0]);
    char flag = FNO;
    
    char cmd[6];
    str_mset(cmd, 5, 0);
    
    char filename[30];
    str_mset(filename, 30, 0);

    if (argc >= 2)
    {
        if (str_cmp("help", argv[1]))
        {
            printHelp();
            return 0;
        }

        #ifdef RM_VERSION
        if (str_cmp("version", argv[1]) || str_cmp("v", argv[1]))
        {
            printVersion();
            return 0;
        }
        #endif

        if (str_cmp("-c", argv[1]))
        {
            flag = FCMD;
            if (argc >= 3)
            {
                if (str_cmp("help", argv[2]))
                {
                    printHelp();
                    return 0;
                }
                
                #ifdef RM_VERSION
                if (str_cmp("version", argv[2]) || str_cmp("v", argv[2]))
                {
                    printVersion();
                    return 0;
                }
                #endif

                snprintf(cmd, 6, argv[2]);
            }
            else
            {
                printHelp();
                return 0;
            }
        }
        if (flag == FNO) 
        {
            flag = FFILE;
            snprintf(filename, 30, argv[1]);
        }
        if (argc >= 3)
        {
            flag = flag==FFILE ? FBOTH : FCMD;
            snprintf(cmd, 6, argv[2]);
        }
    }

    bool file_found = false;
    str_lower(cmd);
    
    DIR* cwd = opendir(".");
    if (!cwd)
    {
        printf("RMAKE FATAL: Cannot open working directory\n");
        exit(-1);
    }
    
    struct dirent* file;
    while((file = readdir(cwd)) && file != NULL)
    {
        if (str_cmp(".", file->d_name) || str_cmp("..", file->d_name))
        {
            continue;
        }

        if (flag != FBOTH && flag != FFILE)
        {
            if (rmake_checkExt(file->d_name, "rm"))
            {
                snprintf(filename, 30, file->d_name);
                file_found = true;
                break;
            }
        } 
        else 
        {
            if (str_cmp(filename, file->d_name))
            {
                file_found = true;
                break;
            }
        }
        
    }
    closedir(cwd);
   
    if (!file_found)
    {
        printf("Rmake error: No rmake conf file %s\n", filename);
        return 0;
    }
    
    FILE* Fconf = fopen(filename, "r");
    if (Fconf == NULL)
    {
        printf("Rmake error: Failed to open %s\n", filename);
        return 0;
    }


    rmake_init_sys();

    
    char conf[MAX_ROW][MAX_COL];

    int row = 0;
    while(!feof(Fconf))
    {
        if (row >= MAX_ROW) 
        {
            printf("Rmake note: read file return before end reached\n");
            break;
        }
        str_mset(conf[row], MAX_COL, 0);
        fgets(conf[row], MAX_COL, Fconf);
        row++;
    }

    rmake_mkConf(conf);
    
    Config config = rmake_getConfig(conf);

    rmake_indexer_fillSrc(&config);
    rmake_compute_file_to_build(&config);
    rmake_indexer_save(&config);

    if (config.err == ERR_NO)
    {
        rmake_exec(&config, cmd);
    }
    else
    {
        printf("Rmake error: config error code %d\n", config.err);
    }

    rmake_deleteConfig(&config);
    printf("Rmake exited successfull\n");
    return 0;
}
