#define _GNU_SOURCE

#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static char pass[64] = {0};
static int hook_fgets = 0;

// read from file and return password
char *getpass(const char *prompt)
{
    FILE *fp = fopen("passwd.txt", "r");
    size_t ln;

    if(!fp)
    {
       printf("Failed to open passwd.txt file!\n");
       exit(0);
    }

    if(!fgets(pass, 63, fp))
    {
       printf("Failed to read passwd.txt file!\n");
       exit(0);
    }

    fclose(fp);

    // chop line terminator
    ln = strlen(pass) -1;
    if(ln > 0)
       pass[ln] = '\0';

    return pass;
}

// hooks text output, detect when next action is accept certificate
int puts(const char *str)
{
    int (*orig_puts)(const char *str) = dlsym(RTLD_NEXT, "puts");

    if(strstr(str, "The certificate for the SSLVPN server is invalid"))
        hook_fgets = 1;

    // libc call
    return orig_puts(str);
}

// hook fgets and check when it's time to accept certificate
char *fgets(char *str, int n, FILE *stream)
{
    char *(*orig_fgets)(char *, int, FILE *) = dlsym(RTLD_NEXT, "fgets");

    if(hook_fgets)
    {
        hook_fgets = 0;

        // accept invalid certificate blah blah
        strcpy(str, "Y");
        return str;
    }

    // libc call
    return orig_fgets(str, n, stream);
}
