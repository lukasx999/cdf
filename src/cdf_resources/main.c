#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFSIZE 50






typedef struct {
    size_t size;
    size_t _capacity;
    char *str;
} String;

String
string_new(void) {
    String new = { .size = 0, ._capacity = 5 };
    new.str = malloc(new._capacity * sizeof(char));
    return new;
}

void
string_append(String *s, char new) {

    if (s->size+1 == s->_capacity) {
        s->_capacity *= 2;
        s->str = realloc(s->str, s->_capacity * sizeof(char));
    }

    s->str[s->size++] = new;

}

void
string_delete(String *s, size_t index) {

    const void *src = s->str + index+1;
    void *dest = s->str + index;
    size_t n = (s->size - index-2) * sizeof(char);

    memmove(dest, src, n);
    --s->size;

}


void
string_destroy(String *s) {
    free(s->str);
}






// reads a file and returns a heap-allocated array of its contents
// the caller is expected to `free()` the returned memory themselves
static String
read_file(const char *filename) {

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    struct stat buf;
    int err = stat(filename, &buf);
    if (err != 0) {
        perror("Failed to read file");
        exit(1);
    }

    String contents = string_new();

    char c;
    while ((c = fgetc(f)) != EOF)
        string_append(&contents, c);

    fclose(f);
    return contents;

}


// creates a file named `filename`, and writes `content` into it
static void
create_file(const char *filename, const char *content) {
    FILE *f = fopen(filename, "wx");
    if (f == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    fprintf(f, "%s", content);
    fclose(f);
}



void
template_init(void) {
    String s = read_file("./templates/init_main.c");
    create_file("cdf_main.c", s.str);
    string_destroy(&s);
}


// returns the absolute position of `query`
// returns -1 if no matching query was found
static ssize_t
lookahead(size_t position, char query, const char *contents) {

    // lookahead
    for (size_t j = position; j < strlen(contents); ++j)
        if (contents[j] == query)
            return j;

    return -1;

}




// replace a variable
// eg: `var`
static void
replace(const char *query, const char *replacement, String *contents) {

    // for (size_t i = 0; i < strlen(contents); ++i) {
        // ssize_t skip = lookahead(i, '`', contents);

    char *offset = strstr(contents->str, query);
    ptrdiff_t skip = offset - contents->str;
    printf("skip: %lu\n", skip);



}


void
template_add(const char *name) {
    String contents_header = read_file("./templates/new_header.h");
    String contents_source = read_file("./templates/new_source.c");


    replace("name", name, &contents_header);
    char buf[BUFSIZE] = { 0 };
    snprintf(buf, BUFSIZE, "%s.h", name);
    create_file(buf, contents_header.str);

    string_destroy(&contents_header);
    string_destroy(&contents_source);

}




static void
print_usage(void) {
    puts("usage: ./cdf <build | add>");
    puts("build ... build the project according to the current config");
    puts("add   ... add a C module (<name>.c | <name>.h)");
    puts("init  ... initialize a new project in the current directory");
}



int main(int argc, char **argv) {

    if (argc == 1)
        exit(EXIT_SUCCESS);

    if (!strcmp(argv[1], "help"))
        print_usage();

    else if (!strcmp(argv[1], "add"))
        template_add(argv[2]);

    else if (!strcmp(argv[1], "init"))
        template_init();

    else
        puts("unknown subcommand");



    return EXIT_SUCCESS;

}
