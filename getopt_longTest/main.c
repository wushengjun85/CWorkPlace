#include <stdio.h>
#include <getopt.h>
char *l_opt_arg;
char* const short_options = "nbl:";
struct option long_options[] = {
    { "name", 0, NULL, 'n' },
    { "bf_name", 0, NULL, 'b' },
    { "love", 1, NULL, 'l' },
    { 0, 0, 0, 0},

{"help",                    no_argument,        NULL,       'h'},
{"uart",                    required_argument,  NULL,       'u'},
{"config",                  required_argument,  NULL,       'c'},
{"port",                    required_argument,  NULL,       'p'},
{"net",                     required_argument,  NULL,       'n'},
{"ip",                      required_argument,  NULL,       'i'},
{"reagent",                 required_argument,  NULL,       'r'},
{"sampler",                 required_argument,  NULL,       's'},
{"daemon",                  no_argument,        NULL,       'd'},
{ NULL, 0, NULL, 0}

};
int main(int argc, char *argv[])
{
    int c;
    while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch (c)
        {
            case 'n':
            printf("My name is XL.\n");
            break;
            case 'b':
            printf("His name is ST.\n");
            break;
            case 'l':
            l_opt_arg = optarg;
            printf("Our love is optarg  = %s,%s!\n", optarg,l_opt_arg);
            break;
        }
    }
    return 0;
}
