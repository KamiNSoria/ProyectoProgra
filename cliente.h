#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar a un cliente
typedef struct {
    char cedula[11]; // Cedula ecuatoriana (10 dígitos + '\0')
    char nombre[100]; // Nombre del cliente
} Cliente;

// Funciones para la gestión de clientes
void ingresarCliente(Cliente* cliente);
void editarCliente(Cliente* cliente, const char* cedula);
void eliminarCliente(const char* cedula);
void listarClientes();
Cliente* buscarClientePorCedula(const char* cedula);

#endif // CLIENTE_H