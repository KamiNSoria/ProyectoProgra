#ifndef FACTURA_H
#define FACTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cliente.h"
#include "producto.h"

// Estructura para representar una factura
typedef struct {
    int numeroFactura; // Número de factura (autoincremental)
    char fecha[11]; // Fecha de la factura (dd/mm/aaaa)
    char cedulaCliente[11]; // Cédula del cliente
    char nombreCliente[100]; // Nombre del cliente
    char nombreProducto[100]; // Nombre del producto
    int cantidadComprada; // Cantidad comprada del producto
    float valorUnitario; // Valor unitario del producto
    float valorPagado; // Valor total pagado (incluyendo IVA)
} Factura;

// Funciones para la gestión de facturas
void ingresarFactura(Factura* factura);
void editarFactura(Factura* factura, int numeroFactura);
void eliminarFactura(int numeroFactura);
void listarFacturas();
Factura* buscarFacturaPorNumero(int numeroFactura);

#endif // FACTURA_H
