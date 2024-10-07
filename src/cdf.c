//bin/true;             \
echo "building cdf..."; \
cc $0 -o cdf;           \
exit 0

// Yeah, I know, this is the most ridiculous thing you've ever seen in a C file.



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>



#define SETUP() \
    const char *configname = __FILE_NAME__;



#ifndef CDF_HEADER


#include "cdfconfig.h"

#define GET_SIZE(array) (size_t) (sizeof((array)) / sizeof((*array)))
#define BUFSIZE_BIG 3000
#define BUFSIZE 100



char**
construct_cmdline(size_t *out_len,     const char *cc,
                  const char **units,  size_t units_len,
                  const char **cflags, size_t cflags_len,
                  const char **libs,   size_t libs_len) {

    // unused args are initialized with NULL, because empty arrays are an extension
    if (*units  == NULL) units_len  = 0;
    if (*cflags == NULL) cflags_len = 0;
    if (*libs   == NULL) libs_len   = 0;


    size_t length = 1 + units_len + cflags_len + libs_len;

    char **cmdline = malloc(length * sizeof(char*));

    for (size_t i = 0; i < length; ++i) {
        cmdline[i] = malloc(BUFSIZE * sizeof(char));
        memset(cmdline[i], 0, BUFSIZE);
    }

    size_t offset = 0;

    cmdline[offset] = (char*) cc; ++offset;
    memcpy(cmdline+offset, units,  units_len  * sizeof(char*)); offset += units_len;
    memcpy(cmdline+offset, cflags, cflags_len * sizeof(char*)); offset += cflags_len;
    memcpy(cmdline+offset, libs,   libs_len   * sizeof(char*));

    *out_len = length;
    return cmdline;

}




void
build(void) {

    size_t cmdline_len;
    char **cmdline = construct_cmdline(&cmdline_len, cc,
                                       translation_units, GET_SIZE(translation_units),
                                       cflags,            GET_SIZE(cflags),
                                       libs,              GET_SIZE(libs));

    // puts("cmdline:");
    // for (size_t i = 0; i < cmdline_len; ++i)
    //     printf("%s ", cmdline[i]);
    // puts("");


    if (!fork()) {
        int err = execvp(cc, cmdline);
        if (err == -1) {
            perror("Failed to build");
            exit(1);
        }
    }
    wait(NULL);

}



void
print_usage(void) {
    puts("usage: ./cdf < build | add >");
    puts("build ... build the project according to the current config");
    puts("add   ... add a C module (mod.c | mod.h)");
    puts("init  ... initialize a new project in the current directory");
}





static void
create_file(char *filename, char *content) {
    FILE *f = fopen(filename, "w");

    if (f == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    fprintf(f, "%s", content);
    fclose(f);
}




void
add_module(const char *name) {

    char header_filename[BUFSIZE] = { 0 };
    sprintf(header_filename, "%s.h", name);

    char *namebuf = strdup(name); // TODO: implement strdup for cross platform
    for (size_t i = 0; i<BUFSIZE; ++i)
        namebuf[i] = toupper(namebuf[i]);

    char content[BUFSIZE_BIG] = { 0 };
    sprintf(content, "#pragma once\n"
                     "#ifndef _%s_H\n"
                     "#define _%s_H\n"
                     "\n#include <stdint.h>\n"
                     "#include <stdbool.h>\n"
                     "#include <stddef.h>\n\n\n\n\n\n\n"
                     "#endif // _%s_H\n" , namebuf, namebuf, namebuf);

    create_file(header_filename, content);



    char impl_filename[BUFSIZE] = { 0 };
    sprintf(impl_filename, "%s.c", name);

    sprintf(content, "#include <stdio.h>\n"
                     "#include <stdlib.h>\n"
                     "#include <unistd.h>\n"
                     "#include <stdbool.h>\n"
                     "#include <stdint.h>\n"
                     "#include <stddef.h>\n"
                     "#include <assert.h>\n"
                     "#include <errno.h>\n\n"
                     "#include \"%s\"\n\n\n\n\n\n\n" , header_filename);


    create_file(impl_filename, content);

}



void
cdf_init(void) {

    char content[] = "#include <stdio.h>\n"
                     "#include <stdlib.h>\n"
                     "#include <stdint.h>\n"
                     "#include <stdbool.h>\n"
                     "#include <unistd.h>\n"
                     "#include <string.h>\n"
                     "#include <assert.h>\n\n\n\n"
                     "int main(int argc, char **argv) {\n\n\n\n"
                     "    return EXIT_SUCCESS;\n\n"
                     "}\n";

    create_file("main.c", content);

}



// returns true if file1 is newer than file2
bool
compare_filemodtime(const char *file1, const char *file2) {

    struct stat buf;

    int err = stat(file1, &buf);
    if (err != 0) {
        fprintf(stderr, "Failed to check for changes in %s: %s", file1, strerror(errno));
        exit(1);
    }

    long time1 = buf.st_mtime;

    err = stat(file2, &buf);
    if (err != 0) {
        fprintf(stderr, "Failed to check for changes in %s: %s", file2, strerror(errno));
        exit(1);
    }

    long time2 = buf.st_mtime;

    return time1 > time2;

}



void
rebuild(char **argv) {

    puts("rebuilding cdf...");

    if (!fork()) {
        int err = execlp("gcc", "gcc", __FILE_NAME__, "-o", "cdf", NULL);
        if (err == -1) {
            perror("Failed to rebuild");
            exit(1);
        }
    }
    wait(NULL);

    execvp(*argv, argv); // replace the current process with a new instance of cdf

}



int
main(int argc, char **argv) {

    // rebuild this file, if the config header has been modified
    if (compare_filemodtime(configname, *argv)) rebuild(argv);

    printf("compiler: %s\n", cc); // TODO: remove this

    if (argc == 1)
        puts("no subcommand specified");

    else if (argc == 2 && !strcmp(argv[1], "help"))
        print_usage();

    else if (argc == 3 && !strcmp(argv[1], "add"))
        add_module(argv[2]);

    else if (argc == 2 && !strcmp(argv[1], "init"))
        cdf_init();

    else if (argc == 2 && !strcmp(argv[1], "new"))
        assert(!"tbd"); // TODO: this

    else if (argc == 2 && !strcmp(argv[1], "clean"))
        assert(!"tbd"); // TODO: this

    else if (!strcmp(argv[1], "build"))
        build();

    else
        puts("unknown subcommand");





    return EXIT_SUCCESS;
}

#endif // CDF_HEADER
