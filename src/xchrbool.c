#include <xstring.h>

bool x_chrbool_digit(const char c)
{
    return (c >= '0' && c <= '9');
}

bool x_chrbool_upper_ascii(const char c)
{
    return (c >= 'A' && c <= 'Z');
}

bool x_chrbool_lower_ascii(const char c)
{
    return (c >= 'a' && c <= 'z');
}

bool x_chrbool_upper_utf(const char* restrict ch)
{
    return (x_strchr(x_strtable_upper_utf(), ch));
}

bool x_chrbool_lower_utf(const char* restrict ch)
{
    return (x_strchr(x_strtable_lower_utf(), ch));
}

bool x_chrbool_upper(const char* restrict ch)
{
    return (x_chrbool_upper_ascii(*ch) || x_chrbool_upper_utf(ch));
}

bool x_chrbool_lower(const char* restrict ch)
{
    return (x_chrbool_lower_ascii(*ch) || x_chrbool_lower_utf(ch));
}

bool x_chrbool_letter(const char* restrict ch)
{
    return (x_chrbool_lower(ch) || x_chrbool_upper(ch));
}

bool x_chrbool_alphanum(const char* restrict ch)
{
    return (x_chrbool_digit(*ch) || x_chrbool_letter(ch));
}

bool x_chrbool_alphaspace(const char* restrict ch)
{
    return (*ch == ' ' || x_chrbool_alphanum(ch));
}

bool x_chrbool_symbol(const char* restrict ch)
{
    return !x_chrbool_alphaspace(ch);
}