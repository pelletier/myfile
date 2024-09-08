#include <cosmo.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TEXT_FILE_NAME "x"
#define EDITOR_COMMAND "$EDITOR "
#define SEPARATOR      "/"
#define BUFSIZE        sizeof(EDITOR_COMMAND) + PATH_MAX + 1 + 1

// https://github.com/jart/cosmopolitan/blob/4754f200ee5082cf7710f62aead949ab4cff236b/libc/testlib/extract.c#L34
void copy_file(const char *from, const char *to, int mode) {
  int fdin, fdout;
  if ((fdin = open(from, O_RDONLY)) == -1) {
    perror(from);
    exit(1);
  }
  if ((fdout = creat(to, mode)) == -1) {
    perror(to);
    exit(1);
  }
  if (copyfd(fdin, fdout, -1) == -1) {
    perror(from);
    exit(1);
  }
  if (close(fdout)) {
    perror(to);
    exit(1);
  }
  if (close(fdin)) {
    perror(from);
    exit(1);
  }
}

bool has_text_file() {
  DIR *d = opendir("/zip");
  if (d == NULL) {
    fprintf(stderr, "cannot open /zip\n");
    return false;
  }

  bool found = false;
  struct dirent *dir;
  while ((dir = readdir(d)) != NULL) {
    if (strcmp(TEXT_FILE_NAME, dir->d_name) == 0) {
      found = true;
      break;
    }
  }

  closedir(d);
  return found;
}

int execute(char **environ, char **argv) {
  int status = systemvpe(argv[0], argv, environ);

  if (status == -1) {
    fprintf(stderr, "could not create process\n");
    return 1;
  }

  if (WIFSIGNALED(status)) {
    fprintf(stderr, "process signaled %G\n", WTERMSIG(status));
    return 1;
  }

  return WEXITSTATUS(status);
}

int run(char *temp_dir) {
  if (BUFSIZE < strlen(EDITOR_COMMAND) + strlen(temp_dir) + strlen(SEPARATOR) +
                    strlen(TEXT_FILE_NAME) + 1) {
    fprintf(stderr, "path too long\n");
    return 1;
  }

  char buf[BUFSIZE] = {0};

  strlcat(buf, EDITOR_COMMAND, BUFSIZE);
  strlcat(buf, temp_dir, BUFSIZE);
  strlcat(buf, SEPARATOR, BUFSIZE);
  strlcat(buf, TEXT_FILE_NAME, BUFSIZE);

  char *command = buf;
  char *file_path = buf + strlen(EDITOR_COMMAND);

  int mode = 0600;
  if (has_text_file()) {
    copy_file("/zip/" TEXT_FILE_NAME, file_path, mode);
  } else {
    if (creat(file_path, mode) == -1) {
      perror(file_path);
      exit(1);
    }
  }

  int status = system(command);
  if (status != 0) {
    fprintf(stderr, "editor command returned %d\n", status);
    return 1;
  }

  // TODO Problematic if multiple instances of myfile are running concurrently.
  copy_file("/zip/zip", "/tmp/myfile_zip", 0700);

  char *self_path = GetProgramExecutableName();
  status = execute(
      environ, (char *[]){"/tmp/myfile_zip", "-Aujq", self_path, file_path, 0});
  if (status != 0) {
    return 1;
  }

  return 0;
}

int main() {
  char template[] = "/tmp/myfile.XXXXXX";
  char *temp_dir = mkdtemp(template);
  if (temp_dir == NULL) {
    fprintf(stderr, "could not create temporary directory");
    return 1;
  }

  int status = run(temp_dir);

  rmdir(temp_dir);

  return status;
}
