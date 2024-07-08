#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar a un producto
typedef struct {
    char nombre[100]; // Nombre del producto
    int cantidad; // Cantidad del producto
    float precio; // Precio del producto
} Producto;

// Funciones para la gestión de productos
void ingresarProducto(Producto* producto);
void editarProducto(Producto* producto, const char* nombre);
void eliminarProducto(const char* nombre);
void eliminarVariosProductos(const char* nombres[], int cantidad);
void listarProductos();
Producto* buscarProductoPorNombre(const char* nombre);

#endif // PRODUCTO_H
