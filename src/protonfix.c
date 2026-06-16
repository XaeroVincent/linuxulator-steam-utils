#define _GNU_SOURCE

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* substitute wine with wine64, skip steam.so.exe helper */

#ifdef __i386__

extern char* __progname;

__attribute__((constructor))
static void redirect_wine_to_wine64(int argc, char** argv, char** env) {

  if (getenv("PROTONFIX_REDIRECT") != NULL) {
    exit(1);
  }

  if (strcmp(__progname, "wine") == 0) {

    char wine64_path[strlen(argv[0]) + 3];
    snprintf(wine64_path, sizeof(wine64_path), "%s64", argv[0]);

    if (strcmp(argv[1], "steam") == 0) {
      if (access("/compat/linux/bin/env", F_OK) == 0) {
        argv[0] = "/compat/linux/bin/env";
      } else {
        argv[0] = "/bin/env";
      }
      argv[1] = wine64_path;
    } else {
      argv[0] = wine64_path;
    }

    setenv("PROTONFIX_REDIRECT", "1", 1);

    execv(argv[0], argv);

    perror("execv");
    exit(1);
  }
}

unsigned short wine_ldt_alloc_fs() {
  assert(0);
}

#endif

/* we don't want to allow Proton 11 to launch wine-preloader */

static int (*libc_execv) (const char*, char* const []) = NULL;
static int (*libc_execvp)(const char*, char* const []) = NULL;

__attribute__((constructor))
static void init_libc_func_pointers() {
  libc_execv  = dlsym(RTLD_NEXT, "execv");
  libc_execvp = dlsym(RTLD_NEXT, "execvp");
}

static inline bool str_ends_with(const char* str, const char* suffix) {
  int str_len    = strlen(str);
  int suffix_len = strlen(suffix);
  return str_len >= suffix_len ? strcmp(str + str_len - suffix_len, suffix) == 0 : false;
}

int execv(const char* path, char* const argv[]) {

  //~ fprintf(stderr, "%s: path = %s\n", __func__, path);

  if (str_ends_with(path, "/wine64-preloader")) {
    errno = ENOENT;
    return -1;
  }

  return libc_execv(path, argv);
}

int execvp(const char* file, char* const argv[]) {

  //~ fprintf(stderr, "%s: file = %s\n", __func__, file);

  if (str_ends_with(file, "/wine-preloader") &&
    argv[0] != NULL && argv[1] != NULL && str_ends_with(argv[1], "/wine")) {
    return libc_execvp("env", argv);
  }

  return libc_execvp(file, argv);
}

/* ??? */

#ifdef __x86_64__

#define PTRACE_POKEDATA 5

static long (*libc_ptrace)(int, pid_t, void*, void*) = NULL;

long ptrace(int request, pid_t pid, void* addr, void* data) {

  if (!libc_ptrace) {
    libc_ptrace = dlsym(RTLD_NEXT, "ptrace");
  }

  if (request == PTRACE_POKEDATA) {
    fprintf(stderr, "PTRACE_POKEDATA: addr = %p, data = %p\n", addr, data);
    return -1;
  } else {
    return libc_ptrace(request, pid, addr, data);
  }
}

#endif

/* ??? */

int prctl() {
  return 0;
}
