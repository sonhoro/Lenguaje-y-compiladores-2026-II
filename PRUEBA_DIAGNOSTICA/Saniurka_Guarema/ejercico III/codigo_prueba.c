#include <stdio.h>

int main() {
    int contador = 0;
    float limite = 10.5;

    while (contador < 5) {
        if (contador % 2 == 0) {
            printf("Numero par: %d\n", contador);
        } else {
            printf("Numero impar: %d\n", contador);
        }
        contador++;
    }

    for (int i = 0; i < 3; i++) {
        contador += i;
    }

    return 0;
}