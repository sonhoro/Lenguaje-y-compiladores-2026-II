#include <stdio.h>

entero main() {
    entero contador = 0;
    flotante limite = 10.5;

    mientras (contador < 5) {
        si (contador % 2 == 0) {
            printf("Numero par: %d\n", contador);
        } sino {
            printf("Numero impar: %d\n", contador);
        }
        contador++;
    }

    para (entero i = 0; i < 3; i++) {
        contador += i;
    }

    retornar 0;
}