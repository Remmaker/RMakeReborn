#include "include/rmake_cmd.h"
#include "include/rmake_sys.h"


char do_after[10][CONF_EXT_CMD_LEN] = {0};
int do_after_count = 0;

void handle_extras_after()
{
    for (int i = 0; i < do_after_count; i++)
    {
        printf("Rmake cmd: %s\n", do_after[i]);
        system(do_after[i]);
    }
    
    do_after_count = 0;
}

void handle_extras(Config* conf)
{
    Conf_build b = conf->build;
    for (int i = 0; i < 10; ++i)
    {
        if (b.usedExtras[i][0] == 0x00) break;
        if (str_cmp(b.usedExtras[i], conf->extras.name))
        {
            if (str_cmp(conf->extras.when, "before"))
            {
                printf("Rmake cmd: %s\n", conf->extras.cmd);
                system(conf->extras.cmd);
            } 
            else 
            {
                snprintf(do_after[do_after_count], CONF_EXT_CMD_LEN, "%s", conf->extras.cmd);
                do_after_count += 1;
            }
        }
        else 
        {
            Conf_extras* current = conf->extras.next;
            while(current != NULL)
            {
                if (str_cmp(current->name, b.usedExtras[i]))
                {
                    if (str_cmp(current->when, "before"))
                    {
                        printf("Rmake cmd: %s\n", current->cmd);
                        system(current->cmd);
                    } 
                    else
                    {
                        snprintf(do_after[do_after_count], CONF_EXT_CMD_LEN, "%s", current->cmd);
                        do_after_count += 1;
                    }    
                }
                current = current->next;
            }
        }
    }
}

void rmake_exec(Config* config, char* arg_cmd)
{
    Conf_build cb = config->build;
    Conf_run cr = config->run;

    str_lower(arg_cmd);
    if (str_cmp("run", arg_cmd))
    {
#if defined(_WIN32) || defined(WIN32)
        str_replace(cr.run, str_size(cr.run), '/', '\\');
#endif
        printf("Rmake cmd: %s\n", cr.run);
        system(cr.run);
        return;
    } 

    char cc[4] = {"gcc"};
    if (cb.cpp)
    {
        snprintf(cc, 4, "g++");
    }

    char tmpInclude[500];
    str_mset(tmpInclude, 500, 0);
    
    
    int occ = str_contain(cb.include, ' ');
    if (occ > 0)
    {
        char dst[2][100];
        for (int i = 0; i < occ; ++i)
        {
            str_split_at_occ(cb.include, ' ', i+1, 100, dst, false);
            char tmp[250];
            str_mset(tmp, 250, 0);

            snprintf(tmp, 250, "-I%s -I%s", dst[0], dst[1]);
            str_concat(tmpInclude, 500, tmp);
        }
        occ = 0;
        str_mset(cb.include, CONF_STR_LEN, 0);
        snprintf(cb.include, CONF_STR_LEN, tmpInclude);
    } 
    else
    {
        if (cb.include[0] != 0x00)
        {
            snprintf(tmpInclude, 500, "-I%s", cb.include);
            snprintf(cb.include, CONF_STR_LEN, tmpInclude);
        }
    }

    char tmpLpath[500];
    str_mset(tmpLpath, 500, 0);

    occ = str_contain(cb.lpath, ' ');
    if (occ > 0)
    {
        char dst[2][100];
        for (int i = 0; i < occ; ++i)
        {
            char tmp[250];
            str_mset(tmp, 250, 0);

            str_split_at_occ(cb.lpath, ' ', i+1, 100, dst, false);
            snprintf(tmp, 250, "-L%s -L%s ", dst[0], dst[1]);
            str_concat(tmpLpath, 500, tmp);
        }
        occ = 0;
        str_mset(cb.lpath, CONF_STR_LEN, 0);
        snprintf(cb.lpath, CONF_STR_LEN, tmpLpath);
    } 
    else
    {
        if (cb.lpath[0] != 0x00)
        {
            snprintf(tmpLpath, 500, "-L%s", cb.lpath);
            snprintf(cb.lpath, CONF_STR_LEN, tmpLpath);
        }
    }


    handle_extras(config);

    char rm[CONF_STR_LEN+5];
    str_mset(rm, CONF_STR_LEN+5, 0);
    snprintf(rm, CONF_STR_LEN+5, "rm %s", cb.target);

    printf("Rmake cmd: %s\n", rm);
    system(rm);

    if (config->indexer.used)
    {
        char cmd[2000];
        if (config->indexer.need_build)
        {
            for (int i = 0; i < FILE_LIST_SIZE; ++i)
            {
                if (config->indexer.files[i][0] == 0x00) break;
                str_mset(cmd, 2000, 0);

                char tmpObjpath[100];
                str_mset(tmpObjpath, 100, 0);

                snprintf(tmpObjpath, 100, "%s/%s/%s.o", SAVE_DIR, SAVE_DIR_OBJ, config->indexer.files[i]);

                snprintf(cmd, 2000, "%s %s -c %s %s/%s %s %s -o %s", 
                    cc, cb.flag, cb.include, config->indexer.folder, config->indexer.files[i], cb.lpath, cb.lflag, tmpObjpath);
                

                printf("Rmake cmd: %s\n", cmd);
                system(cmd);
            }
        } else printf("Rmake note: nothing to build, relink..\n");
        
        str_mset(cmd, 2000, 0);
        
        char objsrc[100];
        str_mset(objsrc, 100, 0);

        snprintf(objsrc, 100, "%s/%s/*.o", SAVE_DIR, SAVE_DIR_OBJ);

        snprintf(cmd, 2000, "%s %s %s %s %s %s -o %s", 
            cc, cb.flag, cb.include, objsrc, cb.lpath, cb.lflag, cb.target);

        printf("Rmake cmd: %s\n", cmd);
        system(cmd);
    }
    else
    {

        char cmd[2000];
        str_mset(cmd, 2000, 0);

        snprintf(cmd, 2000, "%s %s %s %s %s %s -o %s", 
            cc, cb.flag, cb.include, cb.src, cb.lpath, cb.lflag, cb.target);
        

        printf("Rmake cmd: %s\n", cmd);
        system(cmd);
    }

    handle_extras_after();
}
