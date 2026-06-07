#define _GNU_SOURCE

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>

// show CEF version

static int (*orig_cef_initialize)(void*, void*, void*, void*) = NULL;

int cef_initialize(void* args, void* settings, void* application, void* windows_sandbox_info) {

  if (!orig_cef_initialize) {
    orig_cef_initialize = dlsym(RTLD_NEXT, "cef_initialize");
    assert(orig_cef_initialize != NULL);
  }

  int (*cef_version_info)(int) = dlsym(RTLD_NEXT, "cef_version_info");
  assert(cef_version_info != NULL);

  int cef_major    = cef_version_info(0);
  int cef_minor    = cef_version_info(1);
  int cef_patch    = cef_version_info(2);
  int cef_commit   = cef_version_info(3);
  int chrome_major = cef_version_info(4);
  int chrome_minor = cef_version_info(5);
  int chrome_build = cef_version_info(6);
  int chrome_patch = cef_version_info(7);

  fprintf(stderr, "[[CEF version = %d.%d.%d.%d, Chrome version = %d.%d.%d.%d]]\n",
    cef_major, cef_minor, cef_patch, cef_commit, chrome_major, chrome_minor, chrome_build,chrome_patch);

  return orig_cef_initialize(args, settings, application, windows_sandbox_info);
}

// [xxxx/xxxxxx.xxxxxx:FATAL:proc_util.cc(97)] Check failed: . : No such file or directory (2)

#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int (*libc___fxstatat64)(int, int, const char*, struct stat64*, int) = NULL;

int __fxstatat64(int ver, int dirfd, const char* path, struct stat64* stat_buf, int flags) {

  if (!libc___fxstatat64) {
    libc___fxstatat64 = dlsym(RTLD_NEXT, "__fxstatat64");
  }

  char link_buf[1024];
  ssize_t count = readlinkat(dirfd, path, link_buf, sizeof(link_buf) - 1);
  if (count != -1) {
    link_buf[count] = '\0';
    //~ fprintf(stderr, "link: %s -> %s\n", path, link_buf);
    if (strcmp(link_buf, "anon_inode:[unknown]") == 0) {
      return libc___fxstatat64(ver, AT_FDCWD, "/dev/null", stat_buf, flags);
    }
  }

  return libc___fxstatat64(ver, dirfd, path, stat_buf, flags);
}
