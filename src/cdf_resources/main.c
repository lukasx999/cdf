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

    const void *src = s->str + index + 1;
    void *dest = s->str + index;
    size_t n = (s->size - index - 1) * sizeof(char);

    memmove(dest, src, n);
    --s->size;

}

void
string_insert_before(String *s, size_t index, char new) {

    ++s->size;
    ++s->_capacity;
    s->str = realloc(s->str, s->_capacity * sizeof(char));

    const void *src = s->str + index;
    void *dest = s->str + index + 1;
    size_t n = (s->size - index - 1) * sizeof(char);

    memmove(dest, src, n);

    s->str[index] = new;

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



// replace all occurances of `query` with `replacement` inside of `contents`
// eg: `var`
static void
replace(const char *query, const char *replacement, String *contents) {

    // add backticks to beginning and end of query
    char query_buf[BUFSIZE] = { 0 };
    snprintf(query_buf, BUFSIZE, "`%s`", query);

    for (;;) {

        char *offset = strstr(contents->str, query_buf);

        if (offset == NULL)
            break;

        ptrdiff_t skip = offset - contents->str;

        for (size_t _ = 0; _ < strlen(query_buf); ++_)
            string_delete(contents, skip);

        for (ssize_t i = strlen(replacement) - 1; i >= 0; --i)
            string_insert_before(contents, skip, replacement[i]);

    }

}


void
template_add(const char *name) {

    String contents_header = read_file("./templates/new_header.h");
    String contents_source = read_file("./templates/new_source.c");

    replace("name", name, &contents_header);
    char buf[BUFSIZE] = { 0 };
    snprintf(buf, BUFSIZE, "%s.h", name);
    create_file(buf, contents_header.str);

    replace("header", name, &contents_source);
    snprintf(buf, BUFSIZE, "%s.c", name);
    create_file(buf, contents_source.str);

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
