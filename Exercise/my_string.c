//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (2018 Fall)
//
// Skeleton code for PA #0
// September 12, 2018
//
// Somm Kim, Sooyun Lee
// Embedded Software Laboratory
// Sungkyunkwan University
//
// Student Name	: Ryu SungHee
// Student ID	: 2017314895
//
//-----------------------------------------------------------

#include "my_string.h"
#include <stdlib.h>

#define isspace(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || \
					 (ch) == '\v' || (ch) == '\f' || (ch) == '\r')
#define isdigit(ch) ((unsigned)((ch) - '0') < 10)
/* Conversion of string to numeric format */
int my_atoi (const char *str) {
    return (int)my_atol(str);
}

long my_atol (const char *str) {
    register long val = 0;
    while(isdigit(*str)){
		if (isspace(*str)) continue;
    	val = val * 10 + *(str++) - '0';
    }
    return val;
}

/* Conversion of numeric format to string */
char *int2str (char *dest, int num) {
    if(!(dest = (dest ? dest : malloc(12)))) return NULL;

	char befRev[12] = { 0 };
    int i, j;
    if(!num) {
        dest[0] = '0';
        dest[1] = 0;
        return dest;
    }
    for(i = 0; num; befRev[i++] = num % 10 + '0', num /= 10);
	
	for (j = 0; i > 0; dest[j++] = befRev[--i]);
    dest[j] = '\0';

    return dest;
}

/* String manipulation */
char *strcpy (char *dest, const char *src) {
    int i = -1;
    while(*(src + ++i) != '\0')
	*(dest+i) = *(src+i);

	*(dest + i) = '\0';
    return dest;
}

char *strncpy (char *dest, const char *src, size_t n) {
    size_t i = -1;
    while(*(src + ++i) != '\0' && i < n)
	*(dest+i) = *(src+i);

    for(;i <= n; *(dest + i++) = '\0');

    return dest;
}

char *strcat (char *dest, const char *src) {
    int i = 0, j = 0;
    for(; *(dest + i) != '\0'; i++);
    for(; *(src + j) != '\0'; j++, i++)
	*(dest + i) = *(src + j);
	
    return dest;
}

char *strncat (char *dest, const char *src, size_t n) {
    int i = 0, j = 0;
    for(; *(dest + i) != '\0'; i++);
    for(; j < n && *(src+j); j++, i++)
    	*(dest + i) = *(src + j);
    for(; j++ <= n; *(dest + i++) = '\0');
	
    return dest;
}

char *strdup (const char *str) {
    char* dest = (char*)malloc(sizeof(str));
    return dest ? strcpy(dest, str) :  NULL;
}

/* String examination */
size_t strlen (const char *str) {
    size_t i;
    for(i = 0; *(str+i) != '\0'; i++);
    return i;
}

int strcmp(const char *str1, const char *str2) {
	register signed char rslt = 0;

	while (!(rslt = *str1 - *(str2++)) && *(str1++));
	return rslt ? (rslt < 0 ? -1 : 1) : 0;
}

int strncmp (const char *str1, const char *str2, size_t n) {
    register signed char rslt = 0;

    while(n--){
		if((rslt = *str1 - *(str2++)) || !*(str1++)) break;
    }

	return rslt ? (rslt < 0 ? -1 : 1) : 0;
}

char *strchr (const char *str, int c) {
    register char a = c;

    while(*str){
    	if(*(str++) == a) return str - 1;
    }

    return NULL;
}

char *strrchr (const char *str, int c) {
    register const char* ptr = NULL;
    register char a = c;

    while(*str){
	if(*(str++) == a) ptr = str - 1;
    }

    return (char*)ptr;

}
char *strpbrk (const char *str1, const char *str2) {
    char* ptr1 = str1, *ptr2;
    while(*ptr1 != '\0'){
    	ptr2 = str2;
    	while(*ptr2){
    		if(*ptr1 == *(ptr2++)) return ptr1;
    	}
		ptr1++;
    }

    return NULL;
}

char *strstr (const char *haystack, const char *needle) {
    char* ptr1, *ptr2;
    while(*haystack){
	ptr2 = needle;
	ptr1 = haystack++;
		while(*(ptr1 ++) == *(ptr2 ++)){
			if(*ptr2 == '\0') return haystack - 1;
		}

    }

    return NULL;
        
}

char *strtok (char *str, const char *delim) {
    static char* svptr;
    return strtok_r(str, delim, &svptr);
}

char *strtok_r (char *str, const char *delim, char **save_ptr) {
	char* rslt = 0;

    if(!str) str = *save_ptr;
    str += strspn(str, delim);
	
    if(*str){
		rslt = str;
		str += strcspn(str, delim);

		if(*str) 
			*(str++) = 0;
    }
	
    *save_ptr = str;
    return rslt;
}

/* Character array manipulation */
void *memcpy (void *dest, const void *src, size_t n) {
    size_t i;
    for(i = 0; i < n; i++) *((char*)dest+i) = *((char*)src+i);

    return dest;
}

void *memset (void *str, int c, size_t n) {
    char* tmp = str;
    while(n--) 	*(tmp++) = c;

    return str;
}


/* Additional functions */

size_t strspn(const char* str1, const char* str2){
    size_t rslt = 0;
    int isOk = 1;
    char* ptr;
    while(isOk && *str1){
	for(isOk = 0, ptr = str2; (!isOk)&&(*ptr);)
	    if(*str1 == *(ptr ++)) isOk = 1;
	
	if(isOk) rslt++;
	str1++;
    }

    return rslt;
}


size_t strcspn(const char* str1, const char* str2){
    size_t rslt = 0;
    int isOk = 1;
    char* ptr;
    while(isOk && *str1){
		for(ptr = str2; isOk && *ptr;)
			if(*str1 == *(ptr ++)) isOk = 0;
	
		if(isOk) rslt++;
		str1++;
    }

    return rslt;
}
