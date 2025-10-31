// ISC License
//
// Copyright 2025 Juan Milkah <juanmilkah@proton.me>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef JUAN_H
#define JUAN_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_VEC_CAP 64

struct Vec {
        size_t cap;
        size_t len;
        void **items;
};

// File IO operations
long size_of_file(const char *path);
int read_file_to_buffer(const char *path, void *__restrict__ buf,
                        size_t file_size);
int write_buffer_to_file(const char *path, void *__restrict__ buf,
                         size_t buf_size);
int append_buffer_to_file(const char *path, void *__restrict__ buf,
                          size_t buf_size);

// Dynamic Data structures
struct Vec *init_vec(void);
void vec_insert(struct Vec *__restrict__ vec, void *item);
int realloc_vec(struct Vec *__restrict__ vec);
void drop_vec(struct Vec *a);

// Quality of life
void exit_with_error(const char *message);

#ifdef JUAN_IMPLEMENTATION

long size_of_file(const char *path) {
        FILE *f = fopen(path, "r");
        if (!f) {
                return -1;
        }
        if (fseek(f, 0, SEEK_END) < 0) {
                return -1;
        }
        long size = ftell(f);
        if (size < -1) {
                return -1;
        }
        rewind(f);
        fclose(f);
        return size;
}

int read_file_to_buffer(const char *path, void *__restrict__ buf,
                        size_t file_size) {
        FILE *f = fopen(path, "r");
        if (!f) {
                return -1;
        }

        size_t read = fread(buf, 1, file_size, f);
        fclose(f);
        return read == file_size ? read : -1;
}

int append_buffer_to_file(const char *path, void *__restrict__ buf,
                          size_t buf_size) {

        FILE *f = fopen(path, "a");
        if (!f) {
                return -1;
        }

        size_t written = fwrite(buf, 1, buf_size, f);
        fclose(f);
        return written;
}

int write_buffer_to_file(const char *path, void *__restrict__ buf,
                         size_t buf_size) {

        FILE *f = fopen(path, "w");
        if (!f) {
                return -1;
        }

        size_t written = fwrite(buf, 1, buf_size, f);
        fclose(f);
        return written;
}

struct Vec *init_vec(void) {
        void **items = (void **)malloc(INIT_VEC_CAP * sizeof(void *));
        if (!items) {
                return NULL;
        }

        struct Vec *v = (struct Vec *)malloc(sizeof(struct Vec));
        if (!v) {
                free(items);
                return NULL;
        }

        v->cap = INIT_VEC_CAP;
        v->items = (void **)items;
        v->len = 0;

        return v;
}

void drop_vec(struct Vec *a) {
        free(a->items);
        a->len = 0;
        a->cap = 0;
        free(a);

        return;
}

int realloc_vec(struct Vec *__restrict__ vec) {
        size_t cap = vec->cap * 2;
        void *new_ = reallocarray(vec->items, cap, sizeof(vec->items[0]));
        if (!new_) {
                return -1;
        }

        vec->items = (void **)new_;
        vec->cap = cap;

        return 0;
}

void vec_insert(struct Vec *__restrict__ vec, void *item) {
        if (!vec) {
                vec = init_vec();
        }
        if (vec->len == vec->cap) {
                if (realloc_vec(vec) < 0) {
                        return;
                }
        }

        vec->items[vec->len++] = item;
        return;
}

void exit_with_error(const char *message) {
        perror(message);
        exit(1);
        return;
}

#endif

#endif
