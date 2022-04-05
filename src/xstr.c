#include <xstring.h>
#include <string.h>
#include <stdlib.h>

static inline int ipow(int base, int exp)
{
    int result = 1;
    while (1) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }

    return result;
}

static inline size_t x_strfnd(const char* restrict big, const char* restrict little, const size_t cmplen)
{
    size_t bytes;
    for (size_t i = 0; big[i] != 0; i += bytes) {
        bytes = x_utf_bytes(big[i]);
        if (!memcmp(big + i, little, cmplen)) {
            return i + 1;
        }
    }
    return 0;
}

bool x_strcmp(const char* restrict str1, const char* restrict str2)
{
    size_t i = 0;
    while (str1[i] != 0) {
        if (str1[i] != str2[i]) return false;
        ++i;
    }
    return (str2[i] == 0);
}

char* x_strdup_range(const char* restrict start, const char* restrict end)
{
    if (!start || !end || start >= end) return NULL; 
    const size_t size = end - start;
    char* str = calloc(size + 1, sizeof(char));
    memcpy(str, start, size);
    return str;
}

char* x_strdup_size(const char* restrict str, const size_t size)
{
    if (NULLSTR(str) || !size || strlen(str) < size) return NULL;
    char* ret = calloc(size + 1, sizeof(char));
    memcpy(ret, str, size);
    return ret;
}

char* x_strdup(const char* restrict str)
{
    if (NULLSTR(str)) return NULL;
    const size_t size = strlen(str);
    char* ret = calloc(size + 1, sizeof(char));
    memcpy(ret, str, size);
    return ret;
}

char* x_strdup_num(size_t num)
{
    char buff[64];
    size_t index = 0;

    while (num > 0) {
        int digit = (int)num % 10;
        buff[index++] = (char)digit + '0';
        num /= 10;
    }
    buff[index] = 0;

    return x_strget_reverse(buff);
}

char* x_strend(const char* restrict str)
{
    size_t i = 0, bytes = 0;
    for (i = 0; str[i]; i += bytes) {
        bytes = x_utf_bytes(str[i]);
    }
    return (char*)(size_t)str + i - bytes;
}

char* x_strcat(const char* restrict str1, const char* restrict str2)
{
    if (NULLSTR(str1)) return x_strdup(str2);
    if (NULLSTR(str2)) return x_strdup(str1);

    const size_t len = strlen(str1) + strlen(str2);
    char* str = calloc(len + 1, sizeof(char));
    
    strcpy(str, str1);
    strcat(str, str2);
    
    return str;
}

char* x_strcat_spaced(const char* restrict str1, const char* restrict str2)
{
    if (NULLSTR(str1)) return x_strdup(str2);
    if (NULLSTR(str2)) return x_strdup(str1);

    const size_t len = strlen(str1) + strlen(str2) + 1;
    char* str = calloc(len + 1, sizeof(char));
    
    strcpy(str, str1);
    strcat(str, " ");
    strcat(str, str2);
    
    return str;
}

size_t x_strnum(const char* restrict str)
{
    const size_t len = strlen(str);
    char buff[len + 1];
    memset(buff, 0, len + 1);

    size_t bytes, index = 0;
    for (size_t i = 0; str[i]; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_digit(str[i])) {
            buff[index++] = str[i];
        }
    }

    const size_t buff_len = strlen(buff);
    size_t num = !!buff_len;
    for (size_t i = 0; i < buff_len; ++i) {
        num += (size_t)(int)(buff[buff_len - 1 - i] - '0') * ipow(10, i); 
    }
    return num;
}

size_t x_strnum_first(const char* restrict str)
{
    const size_t len = strlen(str);
    char buff[len + 1];
    memset(buff, 0, len + 1);

    bool found = false;
    size_t bytes, index = 0;
    for (size_t i = 0; str[i]; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        
        if (!found && x_chrbool_digit(str[i])) {
            found = true;
        } 
        
        if (found) {
            if (!x_chrbool_digit(str[i])) {
                break;
            }
            buff[index++] = str[i];
        }
    }

    const size_t buff_len = strlen(buff);
    size_t num = !!buff_len;
    for (size_t i = 0; i < buff_len; ++i) {
        num += (size_t)(int)(buff[buff_len - 1 - i] - '0') * ipow(10, i); 
    }
    return num;
}

size_t x_strchr(const char* restrict str, const char* restrict ch)
{
    return x_strfnd(str, ch, x_utf_bytes(*ch));
}

size_t x_strstr(const char* restrict big, const char* restrict little)
{
    return x_strfnd(big, little, strlen(little));
}

size_t x_strcnt(const char* restrict str, const char* restrict find)
{
    const size_t len = strlen(find);
    size_t count = 0, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        count += !memcmp(str + i, find, len);
    }
    return count;
}

size_t x_strmatch(const char* restrict str, char** restrict keywords, const size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        if (!strcmp(str, keywords[i])) return i + 1;
    }
    return 0;
}

size_t x_strmatch_sub(const char* restrict str, char** restrict keywords, const size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        if (strstr(str, keywords[i])) return i + 1;
    }
    return 0;
}