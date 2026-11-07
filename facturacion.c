/*
 * Proyecto Unidad II - Sistema de facturacion basica para una tienda
 * Lenguaje: C
 *
 * Descripcion:
 *   Permite registrar productos (nombre, precio y cantidad), muestra un
 *   resumen parcial y al finalizar calcula subtotal, descuento por monto,
 *   impuesto (ITBMS 7%), total a pagar y el cambio del cliente.
 */

#include <stdio.h>

#define ITBMS 0.07            /* Impuesto sobre la venta: 7% */
#define LIMITE_DESC_ALTO 100.0 /* Compras desde B/.100 -> 10% de descuento */
#define LIMITE_DESC_BAJO 50.0  /* Compras desde B/.50  -> 5% de descuento  */

/* Limpia el buffer de entrada cuando el usuario escribe algo invalido */
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres */
    }
}

/* Pide un numero decimal mayor que 0. Repite hasta que sea valido. */
double leerPrecio(void) {
    double valor;
    int ok;
    do {
        printf("  Precio unitario (B/.): ");
        ok = scanf("%lf", &valor);
        if (ok != 1) {
            printf("  Error: debe escribir un numero.\n");
            limpiarBuffer();
        } else if (valor <= 0) {
            printf("  Error: el precio debe ser mayor que 0.\n");
            ok = 0;
        }
    } while (ok != 1);
    return valor;
}

/* Pide un entero mayor que 0. Repite hasta que sea valido. */
int leerCantidad(void) {
    int valor;
    int ok;
    do {
        printf("  Cantidad: ");
        ok = scanf("%d", &valor);
        if (ok != 1) {
            printf("  Error: debe escribir un numero entero.\n");
            limpiarBuffer();
        } else if (valor <= 0) {
            printf("  Error: la cantidad debe ser mayor que 0.\n");
            ok = 0;
        }
    } while (ok != 1);
    return valor;
}

/* Decide el porcentaje de descuento segun el subtotal */
double calcularPorcentajeDescuento(double subtotal) {
    if (subtotal >= LIMITE_DESC_ALTO) {
        return 0.10;
    } else if (subtotal >= LIMITE_DESC_BAJO) {
        return 0.05;
    } else {
        return 0.0;
    }
}

int main(void) {
    int opcion;
    int ok;
    int cantidad;
    int totalArticulos = 0;
    int numProductos = 0;
    char nombre[30];
    double precio;
    double subtotalLinea;
    double subtotal = 0.0;
    double porcentaje, descuento, base, impuesto, total, pago, cambio;

    printf("=====================================\n");
    printf("   TIENDA - SISTEMA DE FACTURACION\n");
    printf("=====================================\n");

    do {
        printf("\n--- MENU ---\n");
        printf("1. Agregar producto\n");
        printf("2. Ver resumen parcial\n");
        printf("3. Finalizar y facturar\n");
        printf("0. Cancelar compra\n");
        printf("Seleccione una opcion: ");

        ok = scanf("%d", &opcion);
        if (ok != 1) {
            printf("Error: escriba un numero del menu.\n");
            limpiarBuffer();
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1:
                printf("  Nombre del producto (sin espacios): ");
                scanf(" %29s", nombre);
                precio = leerPrecio();
                cantidad = leerCantidad();

                subtotalLinea = precio * cantidad;
                subtotal += subtotalLinea;
                totalArticulos += cantidad;
                numProductos++;

                printf("  Agregado: %s x%d = B/.%.2f\n", nombre, cantidad, subtotalLinea);
                break;

            case 2:
                if (numProductos == 0) {
                    printf("Aun no hay productos registrados.\n");
                } else {
                    printf("Productos: %d | Articulos: %d | Subtotal: B/.%.2f\n",
                           numProductos, totalArticulos, subtotal);
                }
                break;

            case 3:
                if (numProductos == 0) {
                    printf("No puede facturar sin productos. Agregue al menos uno.\n");
                    opcion = -1; /* evita salir del ciclo */
                } else {
                    porcentaje = calcularPorcentajeDescuento(subtotal);
                    descuento = subtotal * porcentaje;
                    base = subtotal - descuento;
                    impuesto = base * ITBMS;
                    total = base + impuesto;

                    printf("\n=========== FACTURA ===========\n");
                    printf("Articulos:        %d\n", totalArticulos);
                    printf("Subtotal:         B/.%.2f\n", subtotal);
                    printf("Descuento (%.0f%%): -B/.%.2f\n", porcentaje * 100, descuento);
                    printf("ITBMS (7%%):       B/.%.2f\n", impuesto);
                    printf("TOTAL A PAGAR:    B/.%.2f\n", total);
                    printf("===============================\n");

                    /* Validar que el pago cubra el total */
                    do {
                        printf("Monto recibido (B/.): ");
                        ok = scanf("%lf", &pago);
                        if (ok != 1) {
                            printf("Error: escriba un numero.\n");
                            limpiarBuffer();
                        } else if (pago < total) {
                            printf("Pago insuficiente. Faltan B/.%.2f\n", total - pago);
                            ok = 0;
                        }
                    } while (ok != 1);

                    cambio = pago - total;
                    printf("Cambio:           B/.%.2f\n", cambio);
                    printf("Gracias por su compra!\n");
                }
                break;

            case 0:
                printf("Compra cancelada.\n");
                break;

            default:
                printf("Opcion invalida. Elija 0, 1, 2 o 3.\n");
                break;
        }
    } while (opcion != 0 && opcion != 3);

    return 0;
}
