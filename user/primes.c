#include "kernel/types.h"
#include "user/user.h"

void pipeline(int fd) {
  int prime;
  if (read(fd, &prime, sizeof(int)) <= 0) {
     close(fd);
     exit(1); 
  }
  printf("prime %d\n", prime);

  int p[2];
  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    pipeline(p[0]);
    exit(0);
  }

  close(p[0]);
  int val;
  while (read(fd, &val, sizeof(int))) {
    if (val % prime == 0)
      continue;
    write(p[1], &val, sizeof(int));
  }
  close(fd);
  close(p[1]);
  wait(0);
  exit(0);
}

int main(int argc, char **argv) {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    pipeline(p[0]);
    exit(0);
  }

  close(p[0]);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, sizeof(int));
  }
  close(p[1]);
  wait(0);
  exit(0);
}
