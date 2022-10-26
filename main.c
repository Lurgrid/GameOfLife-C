#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "gol.h"

matrice *n = NULL;
matrice *m = NULL;

void stop(void){
  clear_out();
  mc_dispose(&m);
  mc_dispose(&n);
  printf("Good Bye !\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Syntax: %s [FILE]\n", argv[0]);
    return EXIT_SUCCESS;
  }
  size_t w;
  size_t h;
  FILE *f = fopen(argv[1], "r");
  if (f == NULL){
    fprintf(stderr, "Cannot open the file: \"%s\".\n", argv[1]);
    goto error;
  }
  if (get_size(f, &w, &h, ALP) != 0){
    fprintf(stderr, "Invalid type file.\n");
    goto error;
  }
  m = mc_empty(w, h);
  n = mc_empty(w, h);
  if (m == NULL || n == NULL){
    goto error;
  }
  if (mc_init(f, m, ALP) != 0){
    fprintf(stderr, "Invalid type value.\n");
    goto error;
  }
  if (fclose(f) != 0){
    fprintf(stderr, "Cannot close the file: \"%s\".\n", argv[1]);
    goto error;
  }
  clear_out();
  signal(SIGINT, (void (*)(int))stop);
  while (1){
    print_mc(m);
    mc_next(m, n);
    matrice *t = m;
    m = n;
    n = t;
    sleep_gol(50);
    clear_out();
  }
  return EXIT_SUCCESS;

error:
  mc_dispose(&m);
  mc_dispose(&n);
  fprintf(stderr, "*** A fatal error occurs.\n");
  return EXIT_FAILURE;
}
