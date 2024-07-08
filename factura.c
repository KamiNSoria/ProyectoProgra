#include "factura.h"

// Función para ingresar una factura
void ingresarFactura(Factura* factura) {
    FILE *file = fopen("Facturas.txt", "a");
    if (file == NULL) {
        printf("Error al abrir el archivo de facturas.\n");
        return;
    }
    fprintf(file, "%d,%s,%s,%s,%s,%d,%.2f,%.2f\n",
            factura->numeroFactura, factura->fecha, factura->cedulaCliente,
            factura->nombreCliente, factura->nombreProducto, factura->cantidadComprada,
            factura->valorUnitario, factura->valorPagado);
    fclose(file);
    printf("Factura ingresada correctamente.\n");
}

// Función para editar una factura
void editarFactura(Factura* factura, int numeroFactura) {
    FILE *file = fopen("Facturas_temp.txt", "w");
    FILE *original = fopen("Facturas.txt", "r");
    if (file == NULL || original == NULL) {
        printf("Error al abrir el archivo de facturas.\n");
        return;
    }

    char buffer[250];
    while (fgets(buffer, sizeof(buffer), original)) {
        Factura temp;
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%f",
               &temp.numeroFactura, temp.fecha, temp.cedulaCliente, temp.nombreCliente,
               temp.nombreProducto, &temp.cantidadComprada, &temp.valorUnitario, &temp.valorPagado);
        if (temp.numeroFactura == numeroFactura) {
            fprintf(file, "%d,%s,%s,%s,%s,%d,%.2f,%.2f\n",
                    factura->numeroFactura, factura->fecha, factura->cedulaCliente,
                    factura->nombreCliente, factura->nombreProducto, factura->cantidadComprada,
                    factura->valorUnitario, factura->valorPagado);
        } else {
            fprintf(file, "%s", buffer);
        }
    }

    fclose(file);
    fclose(original);
    remove("Facturas.txt");
    rename("Facturas_temp.txt", "Facturas.txt");
    printf("Factura editada correctamente.\n");
}

// Función para eliminar una factura
void eliminarFactura(int numeroFactura) {
    FILE *file = fopen("Facturas_temp.txt", "w");
    FILE *original = fopen("Facturas.txt", "r");
    if (file == NULL || original == NULL) {
        printf("Error al abrir el archivo de facturas.\n");
        return;
    }

    char buffer[250];
    while (fgets(buffer, sizeof(buffer), original)) {
        Factura temp;
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%f",
               &temp.numeroFactura, temp.fecha, temp.cedulaCliente, temp.nombreCliente,
               temp.nombreProducto, &temp.cantidadComprada, &temp.valorUnitario, &temp.valorPagado);
        if (temp.numeroFactura != numeroFactura) {
            fprintf(file, "%s", buffer);
        }
    }

    fclose(file);
    fclose(original);
    remove("Facturas.txt");
    rename("Facturas_temp.txt", "Facturas.txt");
    printf("Factura eliminada correctamente.\n");
}

// Función para listar todas las facturas
void listarFacturas() {
    FILE *file = fopen("Facturas.txt", "r");
    if (file == NULL) {
        printf("Error al abrir el archivo de facturas.\n");
        return;
    }

    char buffer[250];
    printf("Lista de facturas:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        Factura temp;
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%f",
               &temp.numeroFactura, temp.fecha, temp.cedulaCliente, temp.nombreCliente,
               temp.nombreProducto, &temp.cantidadComprada, &temp.valorUnitario, &temp.valorPagado);
        printf("Número: %d, Fecha: %s, Cédula Cliente: %s, Nombre Cliente: %s, Producto: %s, Cantidad: %d, Valor Unitario: %.2f, Valor Pagado: %.2f\n",
               temp.numeroFactura, temp.fecha, temp.cedulaCliente, temp.nombreCliente,
               temp.nombreProducto, temp.cantidadComprada, temp.valorUnitario, temp.valorPagado);
    }

    fclose(file);
}

// Función para buscar una factura por número
Factura* buscarFacturaPorNumero(int numeroFactura) {
    FILE *file = fopen("Facturas.txt", "r");
    if (file == NULL) {
        printf("Error al abrir el archivo de facturas.\n");
        return NULL;
    }

    char buffer[250];
    while (fgets(buffer, sizeof(buffer), file)) {
        Factura *temp = (Factura *)malloc(sizeof(Factura));
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%f",
               &temp->numeroFactura, temp->fecha, temp->cedulaCliente, temp->nombreCliente,
               temp->nombreProducto, &temp->cantidadComprada, &temp->valorUnitario, &temp->valorPagado);
        if (temp->numeroFactura == numeroFactura) {
            fclose(file);
            return temp;
        }
        free(temp);
    }

    fclose(file);
    printf("Factura no encontrada.\n");
    return NULL;
}
