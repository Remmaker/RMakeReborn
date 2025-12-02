#include "include/rmake.h"

#include "assert.h"

int min(int a, int b)
{
    return (a < b ? a : b); 
}

void rmake_init_conf_var(Conf_var* in)
{
    in->count = 0;
    for (int i = 0; i < 10; ++i)
    {
        str_mset(in->pool[i], CONF_VAR_POOL_LEN, 0);
        str_mset(in->names[i], CONF_VAR_NAME_LEN, 0);
    } 
}

void rmake_init_conf_build(Conf_build* in)
{
    in->cpp = false;
    in->run = false;
    str_mset(in->include, CONF_STR_LEN, 0);
    str_mset(in->src, CONF_STR_LEN, 0);
    str_mset(in->flag, CONF_STR_LEN, 0);
    str_mset(in->lflag, CONF_STR_LEN, 0);
    str_mset(in->lpath, CONF_STR_LEN, 0);
    str_mset(in->target, CONF_STR_LEN, 0);
}

void rmake_init_conf_extras(Conf_extras* in)
{
    str_mset(in->cmd, CONF_EXT_CMD_LEN, 0);
    str_mset(in->name, CONF_VAR_NAME_LEN, 0);
    str_mset(in->when, 7, 0);
    in->restart = false;
    in->next = NULL;
}

bool rmake_get_var_from_str(char* str, Conf_var* confvar)
{
    int idx = 0;
    if (str[0] == '$') idx = 1;

    for (unsigned int i = 0; i < confvar->count; ++i)
    {
        if (str_cmp(&str[idx], confvar->names[i]))
        {
            str_mset(str, 150, 0);
            snprintf(str, str_size(confvar->pool[i])+1, confvar->pool[i]);
            return true;
        }
    }
    return false;
}

void rmake_setBuild(char* line, Config* conf)
{
    if (str_contain(line, '='))
    {
        char tmp[2][150];

        str_split(line, '=', 150, tmp, false);
        str_lower(tmp[0]);
        
        if (tmp[1][0] == '$') str_lower(tmp[1]);

        if (str_cmp(tmp[0], "cpp"))
        {
            conf->build.cpp = false;
            
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }

            if (str_cmp(tmp[1], "true"))
            {
                conf->build.cpp = true;
            }
            return; 
        }

        if (str_cmp(tmp[0], "include"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.include, str_size(tmp[1])+1, tmp[1]);
        }

        if (str_cmp(tmp[0], "src"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.src, str_size(tmp[1])+1, tmp[1]);
            return; 
        }

        if (str_cmp(tmp[0], "flag"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.flag, str_size(tmp[1])+1, tmp[1]);
            return; 
        }

        if (str_cmp(tmp[0], "lflag"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.lflag, str_size(tmp[1])+1, tmp[1]);
            return; 
        }

        if (str_cmp(tmp[0], "lpath"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.lpath, str_size(tmp[1])+1, tmp[1]);
            return; 
        }

        if (str_cmp(tmp[0], "target"))
        {
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->build.target, str_size(tmp[1])+1, tmp[1]);
            return; 
        }

        if (str_cmp(tmp[0], "run"))
        {
            conf->build.run = false;
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }

            if (str_cmp(tmp[1], "true"))
            {
                conf->build.run = true;
            } 
            return; 
        }
    } 
    else 
    {
        printf("Rmake error: unhandled no '=' in %s\n", line);
    }        
}

void rmake_setRun(char* line, Config* conf)
{

    if (str_contain(line, '='))
    {
        char tmp[2][150];
        str_split(line, '=', 150, tmp, false);

        str_lower(tmp[0]);
        str_lower(tmp[1]);

        if (str_cmp(tmp[0], "run"))
        {                
            if (tmp[1][0] == '$')
            {
                if (!rmake_get_var_from_str(tmp[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", tmp[1]);
                    exit(1);
                }
            }
            snprintf(conf->run.run, 50, tmp[1]);
        }
    }

}

void rmake_getVars(char* line, Conf_var* var)
{
    if (str_contain(line, '='))
    {
        char split[2][CONF_VAR_NAME_LEN];
        str_split(line, '=', CONF_VAR_NAME_LEN, split, false);
                        
        str_trim(split[0]);
        str_trim(split[1]);

        str_lower(split[0]);
        
        if (var->count != 0)
        {
            for (unsigned int j = 0; j < var->count; ++j)
            {
                if (str_cmp(var->names[j], split[0]))
                {
                    printf("Error: Var \"%s\" already defined\n", split[0]);
                    exit(1);
                }
            }
        }

        snprintf(var->names[var->count], min(CONF_VAR_NAME_LEN, str_size(split[0])+1), "%s", split[0]);
        snprintf(var->pool[var->count], min(CONF_VAR_NAME_LEN, str_size(split[1])+1), "%s", split[1]);
        
        var->count++;
    }
}

void rmake_extras_set_ptr(Config* conf, Conf_extras* current)
{
    Conf_extras* previous = NULL;
    Conf_extras* next = conf->extras.next;
    do 
    {
        if (next == NULL)
        {
            if (previous == NULL)
            {
                conf->extras.next = current;
                break;
            }

            previous->next = current;
            break;
        }
        previous = next;
        next = next->next;
    } while(1);
}

Conf_extras* rmake_extras_get_create(char* scope, Config* conf)
{
    if (conf->extras.name[0] == 0x00)
    {
        snprintf(conf->extras.name, CONF_VAR_NAME_LEN, scope);
        return &conf->extras;
    }

    if (str_cmp(conf->extras.name, scope))
    {
        return &conf->extras;
    }

    Conf_extras* ret = conf->extras.next;

    do {
        if (ret == NULL)
        {
            ret = malloc(sizeof(Conf_extras));

            assert(ret == NULL && "RMAKE FATAL ERROR: Memory allocation failed.");
            //_STATIC_ASSERT(ret == NULL && "RMAKE FATAL ERROR: Memory allocation failed.");
            rmake_init_conf_extras(ret);
            snprintf(ret->name, CONF_VAR_NAME_LEN, scope);
            rmake_extras_set_ptr(conf, ret);
            break;
        }

        if (str_cmp(ret->name, scope))
        {
            break;
        }

        ret = ret->next;

    } while (1);
    
    return ret;    
}

bool check_extras(char* extraname, Config* conf)
{
    if (str_cmp(conf->extras.name, extraname)) return true;
    Conf_extras* current = conf->extras.next;
    while (current != NULL)
    {
        if (str_cmp(current->name, extraname)) return true;
        current = current->next;
    }
    return false;
}

void rmake_setExtra(char* scope, char* line, Config* conf)
{
    Conf_extras* currentModule = rmake_extras_get_create(scope, conf);
    if (str_contain(line, '='))
    {
        str_lower(line);
        char dst[2][CONF_VAR_NAME_LEN];
        str_split(line, '=', CONF_VAR_NAME_LEN, dst, false);

        if (str_cmp(dst[0], "cmd"))
        {
            if (dst[1][0] == '$')
            {
                if (!rmake_get_var_from_str(dst[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", dst[1]);
                    exit(1);
                }
            }
            snprintf(currentModule->cmd, CONF_EXT_CMD_LEN, dst[1]);
        }


        // Todo: impl variable $
        if (str_cmp(dst[0], "when"))
        {
            if (!str_cmp(dst[1], "before") && !str_cmp(dst[1], "after"))
            {
                printf("Error: only 'before' and 'after' keyword are supported for 'when'\n");
                exit(1);
            }
            snprintf(currentModule->when, 7, dst[1]);
        }

        if (str_cmp(dst[0], "restart"))
        {
            currentModule->restart = false;
            if (dst[1][0] == '$')
            {
                if (!rmake_get_var_from_str(dst[1], &conf->var)) 
                {
                    printf("Error: Undefined %s\n", dst[1]);
                    exit(1);
                }
            }
            if (str_cmp(dst[1], "true"))
            {
                currentModule->restart = true;
            }
        }
    }
}

void parse_line_for_section(char* scope, char* line, Config* conf)
{
    if (str_cmp(scope, "build"))
    {
        rmake_setBuild(line, conf);
        return;
    }

    if (str_cmp(scope, "run"))
    {
        rmake_setRun(line, conf);
        return;
    }

    rmake_setExtra(scope, line, conf);
}

void set_extra_to_scope(char* scope, char* extraName, Config* conf)
{
    if (str_cmp(scope, "build"))
    {
        if (check_extras(extraName, conf))
        {
            for (int i = 0; i < 10; ++i)
            {
                if (conf->build.usedExtras[i][0] == 0x00)
                {
                    snprintf(conf->build.usedExtras[i], CONF_VAR_NAME_LEN, extraName);
                    break;
                }
            }
        } else printf("Warning: trying to add extra '%s' but not found in conf file\n", extraName);
        return;
    }

    printf("Warning: extras currently only supported in build section\n");
    // if(extra_exist(scope)){getExtra(scope)->usedExtra(extraName)}
}

bool rmake_checkExt(char* str, char* ext)
{
    if (str == NULL || str[0] == 0x00) return false;
    if (ext == NULL || ext[0] == 0x00) return false;
    
    unsigned size = str_size(str);
    if (size == 0) return false;

    for (int i = size; i >= 0; --i)
    {
        if (str[i] == '.')
        {
            if (str_cmp(ext, &str[i+1])) return true;
        }
    }

    return false;
}

void rmake_mkConf(char conf[MAX_ROW][MAX_COL])
{    
    char tmp[MAX_ROW][MAX_COL];
    int c = 0;

    for (unsigned int i = 0; i < MAX_ROW; ++i)
    {
        if (conf[i][0] == 0x00) break; 
        str_trim(conf[i]);
        // TODO: remove second loop, add condition here
        if (conf[i][0] != 0x00) 
        {
            snprintf(tmp[c], MAX_COL, conf[i]);
            c++;
        }
    }

    c = 0;
    for (unsigned int i = 0; i < MAX_ROW; ++i)
    {
        if (tmp[i][0] == CONF_COMMENT_TOKEN) continue;
        snprintf(conf[c], MAX_COL, tmp[i]);
        c++;
    }
    conf[c][0] = 0x00; 
}

Config rmake_getConfig(char conf[MAX_ROW][MAX_COL])
{
    Config ret;
    rmake_init_conf_var(&ret.var);
    rmake_init_conf_build(&ret.build);
    rmake_init_conf_extras(&ret.extras);
    str_mset(ret.run.run, CONF_STR_LEN, 0);
    ret.err = ERR_NO;
    ret.indexer.used = false;
    ret.indexer.need_build = true;
    
    if (conf[0][0] == 0x00) return ret;

    bool globalScope = true;
    
    char currentScope[CONF_VAR_NAME_LEN];
    str_mset(currentScope, CONF_VAR_NAME_LEN, 0);

    int scopeState = 0;

    // Todo: rethink parser
    for (unsigned int i = 0; i < MAX_ROW; ++i)
    {
        if (conf[i][0] == 0x00) break;
        
        char* line = conf[i];
        unsigned int clen = str_size(line);
        
        
        if ((!str_contain(line, '{') || (str_contain(line, '{') && clen > 1)) 
            && 
            !str_contain(line, '=') && !str_contain(line, '('))
        {
            globalScope = false;
            snprintf(currentScope, CONF_VAR_NAME_LEN, line);
            str_lower(currentScope);
            
            scopeState = 0;
            if (str_contain(line, '{')) 
            {
                if (!str_contain(line, ' '))
                {
                    globalScope = true;
                    str_mset(currentScope, CONF_VAR_NAME_LEN, 0);
                    continue;
                }

                char dst[2][CONF_VAR_NAME_LEN/2];
                str_split(line, ' ', CONF_VAR_NAME_LEN/2, dst, false);
                snprintf(currentScope, CONF_VAR_NAME_LEN, dst[0]);
                str_lower(currentScope);
                scopeState = 1;
            }
            continue;
        }

        if (!globalScope)
        {
            switch (scopeState)
            {
                case 0:
                    if (str_contain(line, '{'))
                    {
                        scopeState = 1;
                        continue;
                    }
                    printf("Rmake conf: Missing '{' to start %s section\n", currentScope);
                    exit(1);
                break; 
                
                case 1:
                    if (str_contain(line, '('))
                    {
                        if (str_contain(line, ')'))
                        {
                            char tmp[CONF_VAR_NAME_LEN];
                            str_mset(tmp, sizeof(tmp), 0);

                            char tmp2[2][CONF_VAR_NAME_LEN];
                            str_split(line, '(', CONF_VAR_NAME_LEN, tmp2, false);
                        
                            char tmp3[CONF_VAR_NAME_LEN];
                            snprintf(tmp3, CONF_VAR_NAME_LEN, tmp2[1]);
                            str_split(tmp3, ')', CONF_VAR_NAME_LEN, tmp2, false);
                            
                            snprintf(tmp, CONF_VAR_NAME_LEN, tmp2[0]);

                            set_extra_to_scope(currentScope, tmp, &ret);
                            continue;
                        }
                        else
                        {
                            printf("Missing ')' %s\n", line);
                            exit(1);
                        }
                    }                        
                    
                    if (str_contain(line, '}'))
                    {
                        scopeState = 0;
                        globalScope = true;
                        continue;
                    }

                    parse_line_for_section(currentScope, line, &ret);
                break;

            }
            
        }

        if (globalScope)
        {
            rmake_getVars(line, &ret.var);
        }
    }

    return ret;
}

void rmake_deleteConfig(Config* conf)
{
    int count = 0;
    Conf_extras* Counter = conf->extras.next;
    while(Counter != NULL)
    {
        count++;
        Counter = Counter->next;
    }

    if (count == 0) return;    

    for (int i = 0; i < count; --count)
    {
        Conf_extras* toFree = conf->extras.next;

        for (int j = count-1; j > 0; --j)
        {
            toFree = toFree->next;
        }

        free(toFree);
    }
}

void rmake_indexer_fillSrc(Config* conf)
{
    int ci = 0;

    if (str_contain(conf->build.src, ' ') == 0)
    {
        if (str_contain(conf->build.src, '/'))
        {
            char split[2][FILE_NAME_LEN];
            str_split(conf->build.src, '/', FILE_NAME_LEN, split, false);
            
            snprintf(conf->indexer.folder, FILE_NAME_LEN, "%s", split[0]);
            char* src = split[1];
            if (str_contain(src, '*'))
            {
                char* ext = conf->build.cpp ? "cpp" : "c";
                DIR* dir = opendir(split[0]);
                if (dir == NULL)
                {
                    printf("Rmake error: cannot open %s directory\n", split[0]);
                    conf->err = ERR_INDEXER;
                    return;
                }

                struct dirent* file;
                while((file = readdir(dir)) && file != NULL)
                {
                    if (str_cmp(".", file->d_name) || str_cmp("..", file->d_name))
                    {
                        continue;
                    }
                    if (rmake_checkExt(file->d_name, ext))
                    {
                        if (ci >= FILE_LIST_SIZE)
                        {
                            printf("Rmake error: more src files than allocated size %d\n", ci);
                        } 
                        else
                        {
                            snprintf(conf->indexer.files[ci], FILE_NAME_LEN, "%s", file->d_name);
                            ci++;
                        }
                    }                  
                }

                conf->indexer.used = true;
                closedir(dir);
            } else printf("Rmake note: unsuported indexer without * yet\n");
        } else printf("Rmake note: unsuported indexer without / yet\n");
    }
    else
    {
        printf("Rmake note: unsuported indexer on mutli source yet\n");
    }
}

void rmake_compute_file_to_build(Config* conf)
{
    char existing_files[FILE_LIST_SIZE][FILE_NAME_LEN];
    char filepath[100];

    snprintf(filepath, 100, "%s/%s", SAVE_DIR, SAVE_FILE_INDEXER);
    
    FILE* f = fopen(filepath, "r");
    if (f == NULL)
    {
        printf("Rmake error: failed to open save file %s\n", filepath);
        conf->err = ERR_INDEXER;
        fclose(f);
        return;
    }

    unsigned short nb_file_listed = 0;
    while(!feof(f))
    {
        if (nb_file_listed >= FILE_LIST_SIZE) 
        {
            printf("Rmake note: read file return before end reached\n");
            break;
        }
        str_mset(existing_files[nb_file_listed], FILE_NAME_LEN, 0);
        fgets(existing_files[nb_file_listed], FILE_NAME_LEN, f);
        str_trim(existing_files[nb_file_listed]);
        nb_file_listed++;
    }
    fclose(f);
    existing_files[nb_file_listed][0] = 0x00;

    int nbfilesrc = 0;
    int filesave_in_src = 0;
    for (; nbfilesrc < FILE_LIST_SIZE; ++nbfilesrc)
    {
        if (conf->indexer.files[nbfilesrc][0] == 0x00) break;
        for (int j = 0; j < nb_file_listed; ++j)
        {
            if (str_cmp(conf->indexer.files[nbfilesrc], existing_files[j]))
            {
                filesave_in_src++;
                break;
            }
        }    
    }

    // Add md5 sum
    
   // TODO find way to compare changed file and so on

}    

void rmake_indexer_save(Config* conf)
{
    char tmp[100];
    snprintf(tmp, 100, "%s/%s", SAVE_DIR, SAVE_FILE_INDEXER);
    FILE* f = fopen(tmp, "w");
    for (int i = 0; i < FILE_LIST_SIZE; ++i)
    {
        if (conf->indexer.files[i][0] == 0x00) break;
        fprintf(f, "%s\n", conf->indexer.files[i]);
    }

    fclose(f);
}
