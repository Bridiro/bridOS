#include "util.h"

int string_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = string_length(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void int_to_string(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void append(char s[], char n) {
    int len = string_length(s);
    s[len] = n;
    s[len+1] = '\0';
}

bool backspace(char s[]) {
    int len = string_length(s);
    if (len > 0) {
        s[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

/* K&R
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

char *string_to_lowercase(char *string) {
    for(int i=0; i<string_length(string); i++){
        if(string[i]=='A')
            string[i]='a';
        else if(string[i]=='B')
            string[i]='b';
        else if(string[i]=='C')
            string[i]='c';
        else if(string[i]=='D')
            string[i]='d';
        else if(string[i]=='E')
            string[i]='e';
        else if(string[i]=='F')
            string[i]='f';
        else if(string[i]=='G')
            string[i]='g';
        else if(string[i]=='H')
            string[i]='h';
        else if(string[i]=='I')
            string[i]='i';
        else if(string[i]=='J')
            string[i]='j';
        else if(string[i]=='K')
            string[i]='k';
        else if(string[i]=='L')
            string[i]='l';
        else if(string[i]=='M')
            string[i]='m';
        else if(string[i]=='N')
            string[i]='n';
        else if(string[i]=='O')
            string[i]='o';
        else if(string[i]=='P')
            string[i]='p';
        else if(string[i]=='Q')
            string[i]='q';
        else if(string[i]=='R')
            string[i]='r';
        else if(string[i]=='S')
            string[i]='s';
        else if(string[i]=='T')
            string[i]='t';
        else if(string[i]=='U')
            string[i]='u';
        else if(string[i]=='V')
            string[i]='v';
        else if(string[i]=='W')
            string[i]='w';
        else if(string[i]=='X')
            string[i]='x';
        else if(string[i]=='Y')
            string[i]='y';
        else if(string[i]=='Z')
            string[i]='z';
    }

    return string;
}