#include "string.h"

// https://www.asciitable.com/

char tolower(char s1){

    if (s1 >= 65 && s1 <= 90){ //ASCII capital letters, 65 - 90

        s1 += 32; // plus 32 will convert to lower case (i.e. 65 + 32 = 97 (a))
    }

    return s1;
}

int strlen(const char* ptr){

    int i = 0;
    while(*ptr != 0)
    {
        i++;
        ptr += 1;
    }

    return i;
}

int strnlen(const char* ptr, int max){

    int i = 0;
    for (i = 0; i < max; i++)
    {
        if (ptr[i] == 0)
            break;
    }

    return i;
}

int strnlen_terminator(const char* str, int max, char terminator){

    int i = 0;
    for (i = 0; i < max; i++){

        if (str[i] == '\0' || str[i] == terminator){

            break;
        }
    }

    return i;
}

int istrncmp(const char* s1, const char* s2, int n)
{
    unsigned char u1, u2;
    while(n-- > 0)
    {
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;
        if (u1 != u2 && tolower(u1) != tolower(u2))
            return u1 - u2;
        if (u1 == '\0')
            return 0;
    }

    return 0;
}

int strncmp(const char* str1, const char* str2, int n){

    unsigned char u1, u2;

    while(n-- > 0){

        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2){

            return u1 - u2;
        }
        if (u1 == '\0'){

            return 0;
        }
    }
    return 0;
}

char* strcpy(char* dest, const char* src){

    char* tmp = dest;
    while(*src != 0){

        *dest = *src;
        src += 1;
        dest += 1;
    }

    *dest = 0x00;

    return tmp;
}

bool isdigit(char c){

    return c >= 48 && c <= 57;
}

int tonumericdigit(char c){
    
    return c - 48;
}