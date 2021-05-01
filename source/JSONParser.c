//
// Created by lowrider on 2021-03-28.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../header/JSONParser.h"

unsigned int idx;
char *content = NULL;

void get_value(JSON_Data *);
void get_str(JSON_Data *entry, unsigned char type);
void get_array(JSON_Data *);
void get_object(JSON_Data *);
void get_number(JSON_Data *);
void get_bool(JSON_Data *);
void print_json_data(JSON_Data *root);
void print_recursive(JSON_Data *root, int indents);
void ff_to_start();
JSON_Data *malloc_json_entry();
char skip_irrelevant_chars();
char *get_numeric_str();
char *read_until(char end_char, unsigned char include_end_char);
char curr_ch();
char next_ch();
unsigned char is_num();

JSON_Data *parser_parse(char *file_content, unsigned char print_result) {
    // Calc length of content
    JSON_Data *root, *curr;
    int content_length = 0;

    STR_LEN(content_length, file_content);

    content = file_content;
    root = malloc_json_entry();
    curr = root;
    ff_to_start();
    while (idx < content_length) {
        skip_irrelevant_chars();

        get_str(curr, 'k');
        skip_irrelevant_chars();
        get_value(curr);
        skip_irrelevant_chars();

        // Go to next if we have not reached the end yet
        unsigned int nextIdx = idx + 1;
        char nextChar = content[nextIdx];
        if (nextIdx >= content_length || nextChar == '\0' || (nextChar == '}' && content[++nextIdx] == '\0')) {
            break;
        }
        curr->next = malloc_json_entry();
        curr = curr->next;
        skip_irrelevant_chars();
        idx++;
    }
    if (print_result) {
        print_json_data(root);
    }
    idx = 0;
    return root;
}

void print_json_data(JSON_Data *root) {
    printf("{\n");
    print_recursive(root, 2);
    printf("}");
}

void parser_free(JSON_Data *root) {
    JSON_Data *curr = root;
    unsigned char is_nested;
    while(curr != NULL) {
        JSON_Data* aux;
        is_nested = curr->type.arr | curr->type.obj;
        if (is_nested) {
            parser_free((JSON_Data *) curr->value);
        }
        aux = curr;
        curr = curr->next;
        if(is_nested) {
            aux->value = NULL;
        } else {
            FREE_AND_NULL(aux->value);
        }
        FREE_AND_NULL(aux->key);
        FREE_AND_NULL(aux);
    }
}

void print_recursive(JSON_Data *root, int indents) {
    int i;
    JSON_Data *curr;
    char padding[32];
    for (i = 0; i < indents; i++) {
        padding[i] = ' ';
    }
    padding[i] = '\0'; // Add null terminator

    for (curr = root; curr != NULL; curr = curr->next) {
        char *comma = curr->next == NULL ? "" : ",";
        printf("%s", padding);
        if (curr->key != NULL) {
            printf("\"%s\": ", curr->key);
        }
        // Check value
        if (curr->type.str) {
            printf("\"%s\"%s\n", (char *) curr->value, comma);
        } else if (curr->type.decimal) {
            printf("%.2f%s\n", *((float *) curr->value), comma);
        } else if (curr->type.integer) {
            printf("%i%s\n", *((int *) curr->value), comma);
        } else if (curr->type.bool) {
            printf("%s%s\n", *((unsigned char *) curr->value) == 1 ? "true" : "false", comma);
        } else if (curr->type.arr) {
            printf("[\n");
            print_recursive((JSON_Data *) curr->value, indents + 2);
            printf("%s]%s\n", padding, comma);
        } else if (curr->type.obj) {
            printf("{\n");
            print_recursive((JSON_Data *) curr->value, indents + 2);
            printf("%s}%s\n", padding, comma);
        }
    }
}

void get_value(JSON_Data *entry) {
    char c = curr_ch();
    if (c == '\"') {
        get_str(entry, 'v');
    } else if (c == '[') {
        idx++;
        skip_irrelevant_chars();
        get_array(entry);
    } else if (c == '{') {
        idx++;
        skip_irrelevant_chars();
        get_object(entry);
    } else if (is_num()) {
        get_number(entry);
    } else if (c == 't' || c == 'f') {
        get_bool(entry);
    } else {
        idx++;
        skip_irrelevant_chars();
        get_value(entry);
    }
}

void get_str(JSON_Data *entry, unsigned char type) {
    if (curr_ch() == '\"') {
        next_ch();
    }
    char *str = read_until('\"', 0);
    if (type == 'k') {
        entry->key = str;
    } else if (type == 'v') {
        entry->value = str;
        entry->type.str = 1;
    } else {
        printf("ERR, value=%c not one of 'k', 'v'\n", type);
        exit(1);
    }
}

void get_number(JSON_Data *entry) {
    char *num_str = get_numeric_str();
    float *f_ptr;
    int *i_ptr;
    if (strchr(num_str, '.')) {
        f_ptr = malloc(sizeof(float));
        *f_ptr = strtof(num_str, NULL);
        entry->value = f_ptr;
        entry->type.decimal = 1;
    } else {
        i_ptr = malloc(sizeof(int));
        *i_ptr = (int) strtol(num_str, NULL, 10);
        entry->value = i_ptr;
        entry->type.integer = 1;
    }
    free(num_str);
}

void get_bool(JSON_Data *entry) {
    char *str_bool = read_until('e', 1);   // true and false both end at 'e'
    unsigned char *bool_ptr = malloc(sizeof(unsigned char));
    *bool_ptr = strcmp(str_bool, "true") == 0 ? 1 : 0;
    free(str_bool);
    str_bool = NULL;
    entry->value = bool_ptr;
    entry->type.bool = 1;
}

void get_array(JSON_Data *entry) {
    char c = curr_ch();
    JSON_Data *root = malloc_json_entry();
    JSON_Data *curr = root;
    entry->type.arr = 1;
    while (c != ']') {
        JSON_Data *next = NULL;
        skip_irrelevant_chars();
        get_value(curr);
        c = skip_irrelevant_chars();

        if (c == ']') {
            break;
        }

        next = malloc_json_entry();
        curr->next = next;
        curr = curr->next;
    }
    entry->value = root;
    next_ch(); // So that we iterate past the last ] char
}

void get_object(JSON_Data *entry) {
    char c = curr_ch();
    JSON_Data *root = malloc_json_entry();
    JSON_Data *curr = root;
    entry->type.obj = 1;
    while (c != '}') {
        JSON_Data *next = NULL;
        skip_irrelevant_chars();
        get_str(curr, 'k');
        skip_irrelevant_chars();
        get_value(curr);
        c = skip_irrelevant_chars();

        if (c == '}') {
            break;
        }

        next = malloc_json_entry();
        curr->next = next;
        curr = curr->next;
    }
    entry->value = root;
    next_ch(); // So that we iterate past the last } char
}

JSON_Data *malloc_json_entry() {
    Type zeroType = {0, 0, 0, 0, 0, 0};
    JSON_Data *data = malloc(sizeof(JSON_Data));
    data->key = NULL;
    data->value = NULL;
    data->next = NULL;
    data->type = zeroType;
    return data;
}

char *read_until(char end_char, unsigned char include_end_char) {
    const unsigned char size = 100;
    char *str = calloc(size, sizeof(char));

    unsigned char i = 0;
    char c = curr_ch();
    while (c != end_char) {
        str[i] = c;
        c = next_ch();

        if (i >= size) {
            printf("ERROR: String exceeded max length of %i, accumulated string=%s\n", size, str);
            exit(1);
        }

        i++;
    }
    if (include_end_char) {
        str[i] = end_char;
        i++;
    }
    str[i] = '\0';
    next_ch(); // So that we iterate past the last " char
    return str;
}

char *get_numeric_str() {
    const unsigned char size = 15;
    unsigned char i = 0;
    char *str = calloc(size, sizeof(char));
    while (is_num()) {
        str[i] = curr_ch();
        next_ch();
        if (i >= size) {
            printf("ERROR: Numeric string exceeded max length of %i, accumulated string=%s\n", size, str);
            exit(1);
        }
        i++;
    }
    str[i] = '\0';
    return str;
}

unsigned char is_num() {
    char c = curr_ch();
    unsigned char i;
    char numbers[] = "0123456789.\0";
    for (i = 0; numbers[i] != '\0'; i++) {
        if (numbers[i] == c) {
            return 1;
        }
    }
    return 0;
}

void ff_to_start() {
    while (content[idx] != '\0') {
        if (content[idx++] == '{') {
            return;
        }
    }
}

char skip_irrelevant_chars() {
    char c = curr_ch();
    while (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == ',') {
        c = next_ch();
    }
    return c;
}

char curr_ch() {
    return content[idx];
}

char next_ch() {
    if (curr_ch() == '\0') {
        return curr_ch();
    }
    return content[++idx];
}