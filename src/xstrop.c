#include <xstring.h>
#include <string.h>

void x_strop_upper(char* str)
{
    size_t index, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        if (x_chrbool_lower_ascii(str[i])) {
            str[i] -= 32;
        } 
        else if ((index = x_strchr(x_strtable_lower_utf(), str + i))) {
            x_strop_replace_at(str + i, x_strtable_upper_utf() + index - 1);
        }
        bytes = x_utf_bytes(str[i]);
    }
}

void x_strop_lower(char* str)
{
    size_t index, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        if (x_chrbool_upper_ascii(str[i])) {
            str[i] += 32;
        } 
        else if ((index = x_strchr(x_strtable_upper_utf(), str + i))) {
            x_strop_replace_at(str + i, x_strtable_lower_utf() + index - 1);
        }
        bytes = x_utf_bytes(str[i]);
    }
}

/* assumes lowercase string */
void x_strop_capitalize(char* str)
{
    if (x_chrbool_lower_utf(str)) {
        size_t index = x_strchr(x_strtable_lower_utf(), str);
        if (index) {
            x_strop_replace_at(str, x_strtable_upper_utf() + index - 1);
        }
    } else if (x_chrbool_lower_ascii(*str)) {
        *str -= 32;
    }
}

void x_strop_capitalize_phrase(char* str)
{
    x_strop_lower(str);

    while (*str) {
        x_strop_capitalize(str);
        
        while (*str && x_chrbool_letter(str)) {
            str += x_utf_bytes(*str);
        }

        while (*str && !x_chrbool_letter(str)) {
            str += x_utf_bytes(*str);
        }
    }
}

void x_strop_normalize(char* str)
{
    const char* utf = x_strtable_normal_utf();
    const char* ascii = x_strtable_normal_ascii();

    size_t bytes, index = 0;
    for (size_t i = 0; utf[i]; i += bytes, ++index) {
        bytes = x_utf_bytes(utf[i]);
        x_strop_replace(str, utf + i, ascii + index);
    }
}

void x_strop_replace_at(char* str, const char* restrict dest)
{   
    const size_t destbytes = x_utf_bytes(dest[0]);
    const size_t srcbytes = x_utf_bytes(str[0]);
    if (destbytes != srcbytes) {
        char* cpy = str + srcbytes;
        memmove(str + destbytes, cpy, strlen(cpy) + 1);
    }
    memcpy(str, dest, destbytes);
}

void x_strop_replace(char* str, const char* restrict src, const char* restrict dest)
{
    size_t index;
    while ((index = x_strchr(str, src))) {
        x_strop_replace_at(str + index - 1, dest);
    }
}

void x_strop_remove_at(char* str, const char* restrict remove)
{
    const char* end = str + strlen(remove);
    memmove(str, end, strlen(end) + 1);
}

void x_strop_remove(char* str, const char* restrict remove)
{
    while ((str = strstr(str, remove))) {
        x_strop_remove_at(str, remove);
    }
}

void x_strop_remove_num(char* str)
{
    size_t bytes;
    for (size_t i = 0; str[i]; i += bytes) {
        if (x_chrbool_digit(str[i])) {
            char* ch = str + i;
            memmove(ch, ch + 1, strlen(ch));
        }
        bytes = x_utf_bytes(str[i]);
    }
}

void x_strop_strip(char* str)
{   
    const size_t size = strlen(str);
    char trimmed[size + 1];

    size_t index = 0, bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (str[i] == ' ') {
            eatchar(str, ' ', i);
            if (index && i < size - 1) trimmed[index++] = ' ';
        }
        else if (str[i] == '\n') {
            eatchar(str, '\n', i);
            if (index && i < size - 1) trimmed[index++] = '\n';
        }
        else if (str[i] != '\t') {
            memcpy(trimmed + index, str + i, bytes);
            index += bytes;
        }
    }
    
    trimmed[index] = 0;
    memcpy(str, trimmed, size);
}