#include <_stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_select.h>
#include <sys/select.h>
#include <unistd.h>
#include <util.h>

static int32_t masterfd;

int32_t utf8decode(const char *s, uint32_t *out_cp) {
  unsigned char c = s[0];
  if (c < 0x80) {
    *out_cp = c;
    return 1;
  } else if ((c >> 5) == 0x6) {
    *out_cp = ((c & 0x1F) << 6) | ((s[1] & 0x3F));
    return 2;
  } else if ((c >> 4) == 0xE) {
    *out_cp = ((c & 0x0F) << 12) | ((s[1] & 0x03F) << 6) | (s[2] & 0x3F);
    return 3;
  } else if ((c >> 3) == 0x1E) {
    *out_cp = ((c & 0x07) << 18) | ((s[1] & 0x03F) << 12) |
              ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
    return 4;
  }

  return -1;
}

size_t readfrompty(void) {

  static char buf[SHRT_MAX];
  static uint32_t buflen = 0;

  int32_t nbytes = read(masterfd, buf + buflen, sizeof(buf) - buflen);
  buflen += nbytes;

  uint32_t iter = 0;

  while (iter < buflen) {
    uint32_t codepoint;
    int32_t len = utf8decode(&buf[iter], &codepoint);
  }
}

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

    select(masterfd + 1, &fdset, NULL, NULL, NULL);

    if (FD_ISSET(&masterfd, &fdset)) {
      // read from pty()
    }
  }

  printf("HelloWorld\n");
  return EXIT_SUCCESS;
}
