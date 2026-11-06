import unittest
from unittest.mock import patch

from caballo import (
    CASOS_PRUEBA,
    MOVIMIENTOS_500,
    POSICION_INICIAL_500,
    generar_movimientos,
    medir_tiempo_500,
    validar_movimientos,
)


class PruebasCaballo(unittest.TestCase):
    def test_500_saltos_legales(self):
        self.assertEqual(len(MOVIMIENTOS_500), 500)
        origen = POSICION_INICIAL_500
        for numero, token in enumerate(MOVIMIENTOS_500, start=1):
            with self.subTest(movimiento=numero):
                self.assertEqual(len(token), 3)
                self.assertEqual(token[0], "K")
                self.assertIn(token[1], "abcdefgh")
                self.assertIn(token[2], "12345678")
                dc = abs(ord(token[1]) - ord(origen[0]))
                df = abs(int(token[2]) - int(origen[1]))
                self.assertEqual(sorted((dc, df)), [1, 2])
            origen = token[1:]
        self.assertTrue(validar_movimientos(POSICION_INICIAL_500, MOVIMIENTOS_500)[0])

    def test_casos_y_fallos_primero_intermedio_ultimo(self):
        for origen, cadena, esperado in CASOS_PRUEBA:
            with self.subTest(esperado=esperado):
                self.assertEqual(validar_movimientos(origen, cadena.split())[1], esperado)

    def test_generacion_reproducible(self):
        self.assertEqual(generar_movimientos(POSICION_INICIAL_500, 500), MOVIMIENTOS_500)
        self.assertEqual(generar_movimientos("a1", 0), [])

    def test_entradas_invalidas(self):
        for origen, cantidad in (("i1", 500), ("a1", -1)):
            with self.subTest(origen=origen, cantidad=cantidad):
                with self.assertRaises(ValueError):
                    generar_movimientos(origen, cantidad)
        with self.assertRaises(ValueError):
            medir_tiempo_500(0)

    def test_medicion_verifica_500_movimientos_diez_veces(self):
        with patch("caballo.validar_movimientos", wraps=validar_movimientos) as validar:
            total, promedio = medir_tiempo_500(10)
        self.assertEqual(validar.call_count, 11)
        for llamada in validar.call_args_list:
            self.assertEqual(llamada.args[0], POSICION_INICIAL_500)
            self.assertEqual(len(llamada.args[1]), 500)
        self.assertGreater(total, 0)
        self.assertEqual(promedio, total / 10)

    def test_se_detiene_en_el_primer_error(self):
        def movimientos():
            yield "Ka2"
            raise AssertionError("No debe intentar leer despues del primer fallo")

        self.assertEqual(validar_movimientos("a2", movimientos()),
                         (False, "INCORRECTO falla en el movimiento 1"))


if __name__ == "__main__":
    unittest.main()
