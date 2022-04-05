#include <xstring.h>
#include <stdlib.h>
#include <string.h>

bool x_strbool_num(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_digit(str[i])) {
            return false;
        }
    }
    return true;
}

bool x_strbool_num_has(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_digit(str[i])) {
            return true;
        }
    }
    return false;
}

bool x_strbool_space_has(const char* restrict str)
{
    static const char* space = " ";
    return (x_strchr(str, space));
}

bool x_strbool_letter(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_letter(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_letter_has(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_letter(str + i)) {
            return true;
        }
    }
    return false;
}

bool x_strbool_alphanum(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_alphanum(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_alphanum_has(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_alphanum(str + i)) {
            return true;
        }
    }
    return false;
}

bool x_strbool_alphaspace(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (str[i] != ' ' && !x_chrbool_alphanum(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_symbol(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (!x_chrbool_symbol(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_symbol_has(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if (x_chrbool_symbol(str + i)) {
            return true;
        }
    }
    return false;
}

bool x_strbool_upper(const char* restrict str) 
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if  (x_chrbool_lower(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_upper_alphanum(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if  (!x_chrbool_upper(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_lower(const char* restrict str) 
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if  (x_chrbool_upper(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_lower_alphanum(const char* restrict str)
{
    size_t bytes;
    for (size_t i = 0; str[i] != 0; i += bytes) {
        bytes = x_utf_bytes(str[i]);
        if  (!x_chrbool_lower(str + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_match(const char* restrict str1, const char* restrict str2)
{
    if ((x_strbool_upper(str1) != x_strbool_upper(str2))            ||
        (x_strbool_symbol(str1) != x_strbool_symbol(str2))          ||
        (x_strbool_letter_has(str1) != x_strbool_letter_has(str2))  ||
        (x_strbool_num(str1) != x_strbool_num(str2))) {
        return false;
    }
    return true;
}

bool x_strbool_normal(const char* restrict str)
{
    const char* table = x_strtable_normal_utf();

    size_t bytes;
    for (size_t i = 0; table[i]; i += bytes) {
        bytes = x_utf_bytes(table[i]);
        if (x_strchr(str, table + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_capital_token(const char* restrict token)
{
    if (!x_chrbool_upper(token)) return false;
    
    size_t bytes;
    for (size_t i = 1; token[i]; i += bytes) {
        bytes = x_utf_bytes(token[i]);
        if (x_chrbool_upper(token + i)) {
            return false;
        }
    }
    return true;
}

bool x_strbool_capital_phrase(const char* restrict phrase)
{
    bool upper = true;
    size_t bytes;
    for (size_t i = 0; phrase[i]; i += bytes) {
        bytes = x_utf_bytes(phrase[i]);
        if (x_chrbool_letter(phrase + i)) {
            if (upper) {
                if (!x_chrbool_upper(phrase + i)) return false;
                upper = !upper;
            }
            else if (!x_chrbool_lower(phrase + i)) return false;
        }
        else upper = true;
    }
    return true;
}

bool x_strbool_match_pattern(const char* restrict str, const char* restrict ref)
{
    if (NULLSTR(str) || NULLSTR(ref)) return false;

    size_t strcount, refcount, symcount;
    
    char** symbs = x_strget_symbols(ref, &symcount);
    if (!symbs) return false;

    char** refs = x_strget_divide(ref, &refcount, symbs, symcount);
    if (!refs) {
        x_strget_free(symbs, symcount);
        return false;
    }

    char** strs = x_strget_divide(str, &strcount, symbs, symcount);
    if (!strs) {
        x_strget_free(symbs, symcount);
        x_strget_free(refs, refcount);
        return false;
    }
    
    if (strcount < refcount) {
        x_strget_free(symbs, symcount);
        x_strget_free(refs, refcount);
        x_strget_free(strs, strcount);
        return false;
    }

    const size_t rcount = refcount;
    const size_t scount = strcount;
    size_t mark = 0;

    bool match_pattern = true;
    for (size_t i = 0; i < rcount; ++i) {
        bool match = false;
        for (size_t j = mark; j < scount; ++j) {
            if (x_strbool_match(refs[i], strs[j])) {
                match = true;
                mark = j + 1;
                break;
            }
        }
        if (!match) {
            match_pattern = false;
            break;
        }
    }

    x_strget_free(strs, strcount);
    x_strget_free(refs, refcount);
    x_strget_free(symbs, symcount);
    
    return match_pattern;
}