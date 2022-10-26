#include "gol.h"
#include <string.h>
#include <stdint.h>

#define STRACK printf("**%s: %d\n", __func__, __LINE__)

enum {
  death,
  alive
};

typedef char cell;

struct matrice {
  cell *value;
  size_t width;
  size_t height;
};

matrice *mc_empty(size_t w, size_t h) {
  if (w == 0 || h == 0) {
    return NULL;
  }
  matrice *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  if (w > SIZE_MAX / h || sizeof(cell) > SIZE_MAX / (h * w)) {
    free(p);
    return NULL;
  }
  p->value = malloc(sizeof(cell) * w * h);
  if (p->value == NULL) {
    free(p);
    return NULL;
  }
  size_t k = 0;
  while (k < w * h) {
    *(p->value + k) = death;
    ++k;
  }
  p->width = w;
  p->height = h;
  return p;
}

// lenght_line_in : renvoit la longeur de la premier ligne lu du fichier pointé
//    par f si tout les carractères lu sont dans l'alphabet alp sinon renvois
//    une valeur inferieur a zero.
static int lenght_line_in(FILE *f, const char *alp) {
  int c = fgetc(f);
  int a = 0;
  while (c != '\n' && c != '\r') {
    if (strchr((char *) alp, c) == NULL) {
      return -1;
    }
    ++a;
    c = fgetc(f);
  }
  if (c == '\n') {
    return a;
  }
  c = fgetc(f);
  if (c != '\n') {
    return -1;
  }
  return a;
}

int get_size(FILE *f, size_t *w, size_t *h, const char *alp) {
  int b = lenght_line_in(f, alp);
  if (b == -1) {
    return -1;
  }
  int a;
  int x = 0;
  while ((a = lenght_line_in(f, alp)) == b) {
    ++x;
  }
  if (feof(f) == 0) {
    return -1;
  }
  *w = (size_t) b;
  *h = (size_t) x + 1;
  return 0;
}

int mc_init(FILE *f, matrice *m, const char *alp) {
  int c;
  size_t k = 0;
  rewind(f);
  while ((c = fgetc(f)) != EOF && k <= m->width * m->height) {
    if ((char) c != '\n' && (char) c != '\r' && strchr(alp, c) != NULL) {
      *(m->value + k) = *alp == (char) c ? death : alive;
      ++k;
    }
  }
  if (feof(f) == 0) {
    return -1;
  }
  return 0;
}

void mc_dispose(matrice **m) {
  if (*m == NULL) {
    return;
  }
  free((*m)->value);
  free((*m));
  *m = NULL;
  return;
}

int print_mc(const matrice *m) {
  size_t b = 1;
  if (printf("╔═") == EOF) {
    return EOF;
  }
  while (b < (m->width + 1)) {
    if (printf("══") == EOF) {
      return EOF;
    }
    ++b;
  }
  if (printf("═╗") == EOF) {
    return EOF;
  }
  if (printf("\n") == EOF) {
    return EOF;
  }
  size_t l = 0;
  while (l < m->height) {
    if (printf("║ ") == EOF) {
      return EOF;
    }
    size_t c = 0;
    while (c < m->width) {
      if (*(m->value + l * m->width + c) == death) {
        if (printf("  ") == EOF) {
          return EOF;
        }
      } else {
        if (printf("\033[105m  \033[0m") == EOF) {
          return EOF;
        }
      }
    }
      ++c;
    if (printf(" ║\n") == EOF) {
      return EOF;
    }
    ++l;
  }
  b = 1;
  if (printf("╚═") == EOF) {
    return EOF;
  }
  while (b < m->width + 1) {
    if (printf("══") == EOF) {
      return EOF;
    }
    ++b;
  }
  if (printf("═╝") == EOF) {
    return EOF;
  }
  if (printf("\n") == EOF) {
    return EOF;
  }
  return 0;
}

// cell_update : A partir de la cell d'offset k de la matrice m, calcule sont
//    état selon les règles du jeu de la vie. Renvois l'état suivant de la cell
static cell cell_update(const matrice *m, const size_t k) {
  long int y = (long int) (k / m->width) - 1;
  size_t c = 0;
  while (y >= 0 && y <= (long int) (k / m->width) + 1
      && y < (long int) m->height) {
    long int x = (long int) (k % m->width) - 1;
    while (x <= (long int) (k % m->width) + 1) {
      if (x >= 0 && x < (long int) m->width
          && !(x == (long int) (k % m->width)
          && y == (long int) (k / m->width))) {
        c += *(m->value + y * (long int) m->width + x) == alive ? 1 : 0;
      }
      ++x;
    }
    ++y;
  }
  if (*(m->value + k) == alive){
      return (cell) c > 3 || c < 2 ? death : alive;
  }
  return (cell) c == 3 ? alive : death;
}

int mc_next(const matrice *src, matrice *next) {
  if (src->width != next->width || src->height != next->height) {
    return -1;
  }
  size_t k = 0;
  while (k <= src->height * src->width) {
  *(next->value + k) = cell_update(src, k);
  ++k;
  }
  return 0;
}

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

int clear_out(void) {
  return system("cls");
}

void sleep_gol(int ms) {
  Sleep(ms);
}

#elif defined(__unix__)

#include <time.h>

int nanosleep(const struct timespec *req, struct timespec *rem);

void sleep_gol(int ms) {
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&ts, &ts);
}

int clear_out(void) {
  return system("clear");
}

#else

#error Unsupported Operating System

#endif
