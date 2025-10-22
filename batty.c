#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <util.h>

static int32_t masterfd;
int main(void) {

  if (forkpty(&masterfd, NULL, NULL, NULL) == 0) {
    execlp("/usr/bin/bash", "bash", NULL);
    perror("execlp");
    exit(1);
  }

  bool running = true;

  fd_set fdset;
  while (running) {
    FD_ZERO(&fdset);
    FD_SET(masterfd, &fdset);
  }

  printf("HelloWorld\n");
  return EXIT_SUCCESS;
}
