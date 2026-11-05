/* Programa de ejemplo: la palabra "while" dentro de este comentario no cuenta. */
#include <stdio.h>

typedef struct {
    int id;
    unsigned char activo;
} Item;

static const int LIMITE = 10;

int suma(int a, int b) {
    return a + b;
}

int main(void) {
    Item item = {1, 1};
    double total = 0.0;
    char *msg = "for if else dentro de un string no cuentan";

    for (int i = 0; i < LIMITE; i++) {
        if (i % 2 == 0) {
            continue;
        } else {
            total += suma(i, item.id);
        }
    }

    int j = 0;
    while (j < 3) {
        switch (j) {
            case 0: printf("cero\n"); break;
            default: printf("otro\n");
        }
        j++;
    }

    printf("%s %f %d\n", msg, total, (int)sizeof(item));
    return 0;
}
