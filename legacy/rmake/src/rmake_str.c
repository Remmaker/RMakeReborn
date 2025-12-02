#include "include/rmake_str.h"

void str_trim(char* str)
{
    if (str == NULL || str[0] == 0x00) return;
    unsigned int size = str_size(str);
    if (size <= 0) return;

    char tmp[size+1];
    str_mset(tmp, size+1, 0);
    
    bool hitbeg = false;
    int tmpc = 0;

    for (unsigned int i = 0; i < size; ++i)
    {
        if ( str[i] <= 0x20 && !hitbeg )
        {
            continue;
        }
        hitbeg=true;
        tmp[tmpc] = str[i];
        if (str[i] <= 0x20)
        {
            // TODO: Opti?
            for (unsigned int j = i; j < size; ++j)
            {
                if (str[j] <= 0x20) 
                {
                    continue;
                } 
                goto _continue;
            }
            tmp[tmpc] = 0x00;
            break;
        }
        
    
    _continue:
        tmpc++;
    }
    
    str_mset(str, size, 0);
    snprintf(str, size+1, tmp);
}

void str_replace(char* str, unsigned int size, char old, char new)
{
    if (size <= 0) return;
    if (str == NULL || str[0] == 0x00) return;

    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] == old)
        {
            str[i] = new;
        }
    }
}

void str_split(char* str, char at, unsigned int col, char dst[2][col], bool isc)
{
    str_mset(dst[0], col, 0);
    str_mset(dst[1], col, 0);

    if (str == NULL) return;
    if (str[0] == 0x00) return;
    if (!str_contain(str, at)) return;

    int split = 0;
    unsigned int size = str_size(str);

    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] == at)
        {
            split = i;
            break;
        }
    }
    
    snprintf(dst[0], split+1, str); 
    snprintf(dst[1], (size - split), &str[split+1]);

    if (!isc)
    {
        str_remove_char(dst[0], at);
    }
}

void str_split_at_occ(char* str, char at, int occ, unsigned int col, char dst[2][col], bool isc)
{
    str_mset(dst[0], col, 0);
    str_mset(dst[1], col, 0);

    if (str == NULL) return;
    if (str[0] == 0x00) return;
    if (!str_contain(str, at)) return;

    int split = 0;
    unsigned int size = str_size(str);
    
    int o = 0;

    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] == at)
        {
            o++;
            if (o == occ)
            {
                split = i;
                break;
            }
        }
    }
    
    snprintf(dst[0], split+1, str); 
    snprintf(dst[1], (size - split), &str[split+1]);

    if (!isc)
    {
        str_remove_char(dst[0], at);
    }
}


unsigned short str_contain(char* str, char c)
{
    if (str == NULL || str[0] == 0x00) return 0;
    unsigned int size = str_size(str);
    if (size <= 0) return 0;

    unsigned short ret_count = 0;
    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] == 0x00) break;
        if (str[i] == c)
        {
            ret_count++;
        }
    }

    return ret_count;
}

unsigned short str_contain_str(char* str, char* seq)
{
    if (str == NULL || str[0] == 0x00) return 0;
    unsigned int size = str_size(str);
    if (size <= 0) return 0;
        
    unsigned int sizeSeq = str_size(seq);
    if (size < sizeSeq) return 0;
    
    unsigned short ret_count = 0;
    for (unsigned int i = 0; i < size; ++i)
    {
        if (i + sizeSeq <= size)
        {
            char tmp[sizeSeq+1];
            str_mset(tmp, sizeSeq+1, 0);
            snprintf(tmp, sizeSeq, &str[i]);
            if (str_cmp(seq, tmp))
            {
                ret_count++;           
            }
        } else break;
    }
    return ret_count;
}

unsigned int str_size(char* str)
{
    if (str == NULL) return 0;
    unsigned int ret_count = 0;
    
    while(str[ret_count] != 0x00) 
    {
        ret_count++;
    }
    
    return ret_count;
}

bool str_cmp(char* str, char* rts)
{
    if (str == NULL || rts == NULL) return false;
    
    unsigned int size = str_size(str);
    if (size != str_size(rts)) return false;
    
    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] != rts[i]) return false;
    }
    return true;
}

void str_mset(char* str, unsigned int size, unsigned char value)
{
    if (size == 0) return;
    if (str == NULL || str[0] == 0x00) return;
    
    for (unsigned int i = 0; i < size; ++i)
    {
        str[i] = value;
    }
}

void str_lower(char* str)
{
    if (str == NULL || str[0] == 0x00) return;
    
    unsigned int size = str_size(str);
    if (size <= 0) return;
    
    for (unsigned int i = 0; i < size; ++i)
    {
        if (str[i] >= 0x41 && str[i] <= 0x5A)
        {
            str[i] += 0x20;
        }
    }
}

void str_concat(char* dst, unsigned int dst_size, char* src)
{
    if (dst == NULL || src == NULL) return;
    
    unsigned int src_size = str_size(src);
    unsigned int current_dst_size = str_size(dst);

    if ( src_size > dst_size-current_dst_size)
    {
        printf("str_concat: error size of src is greater than destination\n");
        return;
    }

    if (current_dst_size+1 == dst_size)
    {
        printf("str_concat: error destination already full\n");
        return;
    }
    
    unsigned int i;
    for (i = current_dst_size; i < current_dst_size+src_size; ++i)
    {
        dst[i] = src[i-current_dst_size];
    }
    dst[i] = 0x00;
}

void str_remove_char(char* str, char rm)
{
    if (str == NULL) return;
    if (!str_contain(str, rm)) return;
    
    int sstr = str_size(str);
    for (int i = 0; i < sstr; ++i)
    {
        if (str[i] == rm)
        {
            for (int j = i; j < sstr; ++j)
            {
                if (j+1 >= sstr)
                {
                    str[j] = 0x00;
                    break;
                }
                str[j] = str[j]+1;
            }
            sstr -= 1;
        }
    }
}