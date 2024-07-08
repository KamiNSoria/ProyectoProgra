#include "cliente.h"
#include <ctype.h> // Para la función isdigit()

// Función para verificar si el archivo Clientes.txt existe y no está vacío
int archivoClientesExiste() {
    FILE *file = fopen("Clientes.txt", "r"); // Abrir el archivo en modo lectura
    if (file == NULL) {
        return 0; // El archivo no existe
    }
    fseek(file, 0, SEEK_END); // Mover el puntero al final del archivo
    if (ftell(file) == 0) {
        fclose(file);
        return 0; // El archivo está vacío
    }
    fclose(file);
    return 1; // El archivo existe y tiene información
}

// Función para verificar si un cliente con la misma cédula ya existe
int clienteExiste(const char* cedula) {
    if (!archivoClientesExiste()) {
        return 0; // El archivo no existe o está vacío
    }
    FILE *file = fopen("Clientes.txt", "r"); // Abrir el archivo en modo lectura
    if (file == NULL) {
        return 0; // Error al abrir el archivo
    }

    char buffer[150];
    while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
        Cliente temp;
        sscanf(buffer, "%[^,],%[^\n]", temp.cedula, temp.nombre); // Leer los datos del cliente
        if (strcmp(temp.cedula, cedula) == 0) {
            fclose(file);
            return 1; // El cliente ya existe
        }
    }

    fclose(file);
    return 0; // El cliente no existe
}

// Función para validar la cédula ecuatoriana
int validarCedulaEcuatoriana(const char* cedula) {
    if (strlen(cedula) != 10) { // Verificar que la cédula tenga exactamente 10 dígitos
        return 0; // Cédula inválida
    }

    // Verificar que cada carácter de la cédula sea un dígito numérico
    for (int i = 0; i < 10; i++) {
        if (!isdigit(cedula[i])) {
            return 0; // Cédula inválida si contiene caracteres no numéricos
        }
    }

    int tercerDigito = cedula[2] - '0'; // Obtener el tercer dígito
    if (tercerDigito >= 6) {
        return 0; // Cédula inválida si el tercer dígito es mayor o igual a 6
    }

    // Coeficientes de validación cédula
    int coefValCedula[] = {2, 1, 2, 1, 2, 1, 2, 1, 2};
    int verificador = cedula[9] - '0'; // Obtener el dígito verificador
    int suma = 0;
    int digito = 0;

    for (int i = 0; i < 9; i++) {
        digito = (cedula[i] - '0') * coefValCedula[i]; // Multiplicar cada dígito por su coeficiente
        suma += (digito % 10) + (digito / 10); // Sumar los dígitos de cada resultado
    }

    if ((suma % 10 == 0 && suma % 10 == verificador) || (10 - (suma % 10)) == verificador) {
        return 1; // Cédula válida
    } else {
        return 0; // Cédula inválida
    }
}

// Función para ingresar un cliente
void ingresarCliente(Cliente* cliente) {
    if (!validarCedulaEcuatoriana(cliente->cedula)) {
        printf("La cédula ingresada no es válida. Debe contener exactamente 10 dígitos y cumplir con las reglas de validación.\n"); // Mensaje de error si la cédula no es válida
        return;
    }
    if (clienteExiste(cliente->cedula)) {
        printf("El cliente con la cédula '%s' ya existe.\n", cliente->cedula); // Mensaje de error si el cliente ya existe
        return;
    }
    FILE *file = fopen("Clientes.txt", "a"); // Abrir el archivo en modo adición
    if (file == NULL) {
        printf("Error al abrir el archivo de clientes.\n"); // Mensaje de error si no se puede abrir el archivo
        return;
    }
    fprintf(file, "%s,%s\n", cliente->cedula, cliente->nombre); // Escribir el nuevo cliente en el archivo
    fclose(file); // Cerrar el archivo
    printf("Cliente ingresado correctamente.\n"); // Mensaje de confirmación
}

// Función para editar un cliente
void editarCliente(Cliente* cliente, const char* cedula) {
    if (!validarCedulaEcuatoriana(cliente->cedula)) {
        printf("La nueva cédula ingresada no es válida. Debe contener exactamente 10 dígitos y cumplir con las reglas de validación.\n"); // Mensaje de error si la nueva cédula no es válida
        return;
    }
    if (!archivoClientesExiste()) {
        printf("El archivo Clientes.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
        return;
    }
    FILE *file = fopen("Clientes_temp.txt", "w"); // Abrir un archivo temporal en modo escritura
    FILE *original = fopen("Clientes.txt", "r"); // Abrir el archivo original en modo lectura
    if (file == NULL || original == NULL) {
        printf("Error al abrir el archivo de clientes.\n"); // Mensaje de error si no se puede abrir el archivo
        return;
    }

    char buffer[150];
    int encontrado = 0;
    while (fgets(buffer, sizeof(buffer), original)) { // Leer línea por línea
        Cliente temp;
        sscanf(buffer, "%[^,],%[^\n]", temp.cedula, temp.nombre); // Leer los datos del cliente
        if (strcmp(temp.cedula, cedula) == 0) {
            fprintf(file, "%s,%s\n", cliente->cedula, cliente->nombre); // Escribir el cliente editado en el archivo temporal
            encontrado = 1;
        } else {
            fprintf(file, "%s", buffer); // Escribir el cliente no editado en el archivo temporal
        }
    }

    fclose(file); // Cerrar el archivo temporal
    fclose(original); // Cerrar el archivo original
    if (encontrado) {
        remove("Clientes.txt"); // Eliminar el archivo original
        rename("Clientes_temp.txt", "Clientes.txt"); // Renombrar el archivo temporal
        printf("Cliente editado correctamente.\n"); // Mensaje de confirmación
    } else {
        remove("Clientes_temp.txt"); // Eliminar el archivo temporal si no se encontró el cliente
        printf("Cliente no encontrado.\n"); // Mensaje de error si no se encontró el cliente
    }
}

// Función para eliminar un cliente
void eliminarCliente(const char* cedula) {
    if (!archivoClientesExiste()) {
        printf("El archivo Clientes.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
        return;
    }
    FILE *file = fopen("Clientes_temp.txt", "w"); // Abrir un archivo temporal en modo escritura
    FILE *original = fopen("Clientes.txt", "r"); // Abrir el archivo original en modo lectura
    if (file == NULL || original == NULL) {
        printf("Error al abrir el archivo de clientes.\n"); // Mensaje de error si no se puede abrir el archivo
        return;
    }

    char buffer[150];
    int encontrado = 0;
    while (fgets(buffer, sizeof(buffer), original)) { // Leer línea por línea
        Cliente temp;
        sscanf(buffer, "%[^,],%[^\n]", temp.cedula, temp.nombre); // Leer los datos del cliente
        if (strcmp(temp.cedula, cedula) != 0) {
            fprintf(file, "%s", buffer); // Escribir el cliente no eliminado en el archivo temporal
        } else {
            encontrado = 1;
        }
    }

    fclose(file); // Cerrar el archivo temporal
    fclose(original); // Cerrar el archivo original
    if (encontrado) {
        remove("Clientes.txt"); // Eliminar el archivo original
        rename("Clientes_temp.txt", "Clientes.txt"); // Renombrar el archivo temporal
        printf("Cliente eliminado correctamente.\n"); // Mensaje de confirmación
    } else {
        remove("Clientes_temp.txt"); // Eliminar el archivo temporal si no se encontró el cliente
        printf("Cliente no encontrado.\n"); // Mensaje de error si no se encontró el cliente
    }
}

// Función para listar todos los clientes
void listarClientes() {
    if (!archivoClientesExiste()) {
        printf("El archivo Clientes.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
        return;
    }
    FILE *file = fopen("Clientes.txt", "r"); // Abrir el archivo en modo lectura
    if (file == NULL) {
        printf("Error al abrir el archivo de clientes.\n"); // Mensaje de error si no se puede abrir el archivo
        return;
    }

    char buffer[150];
    printf("Lista de clientes:\n");
    while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
        Cliente temp;
        sscanf(buffer, "%[^,],%[^\n]", temp.cedula, temp.nombre); // Leer los datos del cliente
        printf("Cédula: %s, Nombre: %s\n", temp.cedula, temp.nombre); // Imprimir los datos del cliente
    }

    fclose(file); // Cerrar el archivo
}

// Función para buscar un cliente por cédula
Cliente* buscarClientePorCedula(const char* cedula) {
    if (!archivoClientesExiste()) {
        printf("El archivo Clientes.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
        return NULL;
    }
    FILE *file = fopen("Clientes.txt", "r"); // Abrir el archivo en modo lectura
    if (file == NULL) {
        printf("Error al abrir el archivo de clientes.\n"); // Mensaje de error si no se puede abrir el archivo
        return NULL;
    }

    char buffer[150];
    while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
        Cliente *temp = (Cliente *)malloc(sizeof(Cliente)); // Asignar memoria para un nuevo cliente
        sscanf(buffer, "%[^,],%[^\n]", temp->cedula, temp->nombre); // Leer los datos del cliente
        if (strcmp(temp->cedula, cedula) == 0) {
            fclose(file); // Cerrar el archivo
            return temp; // Devolver el cliente encontrado
        }
        free(temp); // Liberar la memoria si no es el cliente buscado
    }

    fclose(file); // Cerrar el archivo
    printf("Cliente no encontrado.\n"); // Mensaje de error si no se encontró el cliente
    return NULL;
}
