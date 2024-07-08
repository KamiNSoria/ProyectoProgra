#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "producto.h"
#include "factura.h"

// Prototipos de funciones para los menús
void menuPrincipal();
void menuGestionarProductos();
void menuGestionarClientes();
void menuGestionarFacturas();

// Función principal
int main() {
    // Llamada al menú principal
    menuPrincipal();
    return 0;
}

// Menú principal
void menuPrincipal() {
    int opcion;

    do {
        // Mostrar opciones del menú principal
        printf("\n--- Menu Principal ---\n");
        printf("1. Gestionar Productos\n");
        printf("2. Gestionar Clientes\n");
        printf("3. Gestionar Facturas\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        // Manejar la opción seleccionada
        switch (opcion) {
            case 1:
                menuGestionarProductos();
                break;
            case 2:
                menuGestionarClientes();
                break;
            case 3:
                menuGestionarFacturas();
                break;
            case 4:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 4);
}

// Menú para gestionar productos
void menuGestionarProductos() {
    int opcion;
    char nombre[100];
    Producto producto;
    char* nombres[10]; // Array para almacenar nombres de productos a eliminar
    int cantidad;

    do {
// Mostrar opciones del menú de gestión de productos
        printf("\n--- Gestionar Productos ---\n");
        printf("1. Ingresar Producto\n");
        printf("2. Editar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Eliminar Varios Productos\n");
        printf("5. Listar Productos\n");
        printf("6. Regresar al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        // Manejar la opción seleccionada
        switch (opcion) {
            case 1:
                printf("Ingrese nombre del producto: ");
                scanf("%s", producto.nombre);
                printf("Ingrese cantidad: ");
                scanf("%d", &producto.cantidad);
                printf("Ingrese precio: ");
                scanf("%f", &producto.precio);
                ingresarProducto(&producto);
                break;
            case 2:
                printf("Ingrese nombre del producto a editar: ");
                scanf("%s", nombre);
                printf("Ingrese nuevo nombre del producto: ");
                scanf("%s", producto.nombre);
                printf("Ingrese nueva cantidad: ");
                scanf("%d", &producto.cantidad);
                printf("Ingrese nuevo precio: ");
                scanf("%f", &producto.precio);
                editarProducto(&producto, nombre);
                break;
            case 3:
                printf("Ingrese nombre del producto a eliminar: ");
                scanf("%s", nombre);
                eliminarProducto(nombre);
                break;
            case 4:
                printf("Ingrese la cantidad de productos a eliminar: ");
                scanf("%d", &cantidad);
                for (int i = 0; i < cantidad; i++) {
                    nombres[i] = (char*)malloc(100 * sizeof(char));
                    printf("Ingrese nombre del producto %d: ", i + 1);
                    scanf("%s", nombres[i]);
                }
                eliminarVariosProductos(nombres, cantidad);
                for (int i = 0; i < cantidad; i++) {
                    free(nombres[i]);
                }
                break;
            case 5:
                listarProductos();
                break;
            case 6:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}

// Menú para gestionar clientes
void menuGestionarClientes() {
    int opcion;
    char cedula[11];
    Cliente cliente;

    do {
        // Mostrar opciones del menú de gestión de clientes
        printf("\n--- Gestionar Clientes ---\n");
        printf("1. Ingresar Cliente\n");
        printf("2. Editar Cliente\n");
        printf("3. Eliminar Cliente\n");
        printf("4. Listar Clientes\n");
        printf("5. Regresar al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        // Manejar la opción seleccionada
        switch (opcion) {
            case 1:
                printf("Ingrese cedula del cliente: ");
                scanf("%s", cliente.cedula);
                printf("Ingrese nombre del cliente: ");
                scanf("%s", cliente.nombre);
                ingresarCliente(&cliente);
                break;
            case 2:
                printf("Ingrese cedula del cliente a editar: ");
                scanf("%s", cedula);
                printf("Ingrese nueva cedula del cliente: ");
                scanf("%s", cliente.cedula);
                printf("Ingrese nuevo nombre del cliente: ");
                scanf("%s", cliente.nombre);
                editarCliente(&cliente, cedula);
                break;
            case 3:
                printf("Ingrese cedula del cliente a eliminar: ");
                scanf("%s", cedula);
                eliminarCliente(cedula);
                break;
            case 4:
                listarClientes();
                break;
            case 5:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 5);
}

// Menú para gestionar facturas
void menuGestionarFacturas() {
    int opcion;
    int numeroFactura;
    Factura factura;

    do {
        // Mostrar opciones del menú de gestión de facturas
        printf("\n--- Gestionar Facturas ---\n");
        printf("1. Ingresar Factura\n");
        printf("2. Editar Factura\n");
        printf("3. Eliminar Factura\n");
        printf("4. Listar Facturas\n");
        printf("5. Regresar al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        // Manejar la opción seleccionada
        switch (opcion) {
            case 1:
                // Asignar número de factura (incremental)
                factura.numeroFactura = 1;
                FILE *file = fopen("Facturas.txt", "r");
                if (file != NULL) {
                    char buffer[250];
                    while (fgets(buffer, sizeof(buffer), file)) {
                        sscanf(buffer, "%d", &factura.numeroFactura);
                    }
                    factura.numeroFactura++;
                    fclose(file);
                }

                // Obtener fecha actual
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(factura.fecha, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

                // Obtener cédula y nombre del cliente
                printf("Ingrese cedula del cliente: ");
                scanf("%s", factura.cedulaCliente);
                Cliente* cliente = buscarClientePorCedula(factura.cedulaCliente);
                if (cliente != NULL) {
                    strcpy(factura.nombreCliente, cliente->nombre);
                    free(cliente);
                } else {
                    printf("Cliente no encontrado.\n");
                    break;
                }

                // Obtener nombre del producto y cantidad comprada
                printf("Ingrese nombre del producto: ");
                scanf("%s", factura.nombreProducto);
                Producto* producto = buscarProductoPorNombre(factura.nombreProducto);
                if (producto != NULL) {
                    printf("Ingrese cantidad comprada: ");
                    scanf("%d", &factura.cantidadComprada);
                    if (factura.cantidadComprada > producto->cantidad) {
                        printf("Cantidad solicitada no disponible.\n");
                        free(producto);
                        break;
                    }
                    factura.valorUnitario = producto->precio;
                    producto->cantidad -= factura.cantidadComprada;
                    editarProducto(producto, producto->nombre);
                    free(producto);
                } else {
                    printf("Producto no encontrado.\n");
                    break;
                }

                // Calcular valor pagado (incluyendo IVA)
                factura.valorPagado = factura.valorUnitario * factura.cantidadComprada * 1.14;
                ingresarFactura(&factura);
                break;
            case 2:
                printf("Ingrese numero de factura a editar: ");
                scanf("%d", &numeroFactura);
                // Aquí puede agregarse la lógica para editar la factura
                break;
            case 3:
                printf("Ingrese numero de factura a eliminar: ");
                scanf("%d", &numeroFactura);
                eliminarFactura(numeroFactura);
                break;
            case 4:
                listarFacturas();
                break;
            case 5:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 5);
}
