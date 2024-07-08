#include "producto.h"

// Función para verificar si el archivo Productos.txt existe y no está vacío
int archivoProductosExiste() {
  FILE *file = fopen("Productos.txt", "r"); // Abrir el archivo en modo lectura
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

// Función para verificar si un producto con el mismo nombre ya existe
int productoExiste(const char *nombre) {
  if (!archivoProductosExiste()) {
    return 0; // El archivo no existe o está vacío
  }
  FILE *file = fopen("Productos.txt", "r"); // Abrir el archivo en modo lectura
  if (file == NULL) {
    return 0; // Error al abrir el archivo
  }

  char buffer[150];
  while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
    Producto temp;
    sscanf(buffer, "%[^,],%d,%f", temp.nombre, &temp.cantidad,
           &temp.precio); // Leer los datos del producto
    if (strcmp(temp.nombre, nombre) == 0) {
      fclose(file);
      return 1; // El producto ya existe
    }
  }

  fclose(file);
  return 0; // El producto no existe
}

// Función para verificar si un producto está presente en Facturas.txt
int productoEnFacturas(const char *nombre) {
  FILE *file = fopen("Facturas.txt", "r"); // Abrir el archivo en modo lectura
  if (file == NULL) {
    return 0; // El archivo no existe
  }

  char buffer[250];
  while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
    char nombreProducto[100];
    sscanf(buffer, "%*d,%*[^,],%*[^,],%*[^,],%[^,]",
           nombreProducto); // Leer el nombre del producto de la factura
    if (strcmp(nombreProducto, nombre) == 0) {
      fclose(file);
      return 1; // El producto está en una factura
    }
  }

  fclose(file);
  return 0; // El producto no está en ninguna factura
}

// Función para ingresar un producto
void ingresarProducto(Producto *producto) {
  if (productoExiste(producto->nombre)) {
    printf("El producto con el nombre '%s' ya existe.\n",
           producto->nombre); // Mensaje de error si el producto ya existe
    return;
  }
  FILE *file = fopen("Productos.txt", "a"); // Abrir el archivo en modo adición
  if (file == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si no se puede abrir el archivo
    return;
  }
  fprintf(file, "%s,%d,%.2f\n", producto->nombre, producto->cantidad,
          producto->precio); // Escribir el nuevo producto en el archivo
  fclose(file);              // Cerrar el archivo
  printf("Producto ingresado correctamente.\n"); // Mensaje de confirmación
}

// Función para editar un producto
void editarProducto(Producto *producto, const char *nombre) {
  if (!archivoProductosExiste()) {
    printf("El archivo Productos.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
    return;
  }
  if (productoEnFacturas(nombre)) {
    printf("El producto '%s' no se puede editar el nombre ya que está presente "
           "en una factura.\n",
           nombre); // Mensaje de error si el producto está en una factura
    return;
  }
  FILE *file = fopen("Productos_temp.txt",
                     "w"); // Abrir un archivo temporal en modo escritura
  FILE *original =
      fopen("Productos.txt", "r"); // Abrir el archivo original en modo lectura
  if (file == NULL || original == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si no se puede abrir el archivo
    return;
  }

  char buffer[150];
  int encontrado = 0; //bandera 
  while (fgets(buffer, sizeof(buffer), original)) { // Leer línea por línea
    Producto temp;
    sscanf(buffer, "%[^,],%d,%f", temp.nombre, &temp.cantidad,
           &temp.precio); // Leer los datos del producto
    if (strcmp(temp.nombre, nombre) == 0) {
      if (productoEnFacturas(nombre)) {
        fprintf(file, "%s,%d,%.2f\n", temp.nombre, producto->cantidad,
                producto->precio); // Escribir solo cantidad y precio
      } else {
        fprintf(file, "%s,%d,%.2f\n", producto->nombre, producto->cantidad,
                producto->precio); // Escribir nombre, cantidad y precio
      }
      encontrado = 1;
    } else {
      fprintf(file, "%s", buffer); // Escribir el producto no editado en el archivo temporal
    }
  }

  fclose(file);     // Cerrar el archivo temporal
  fclose(original); // Cerrar el archivo original
  if (encontrado) {
    remove("Productos.txt"); // Eliminar el archivo original
    rename("Productos_temp.txt",
           "Productos.txt"); // Renombrar el archivo temporal
    printf("Producto editado correctamente.\n"); // Mensaje de confirmación
  } else {
    remove("Productos_temp.txt"); // Eliminar el archivo temporal si no se
                                  // encontró el producto
    printf("Producto no encontrado.\n"); // Mensaje de error si no se encontró el producto
  }
}

// Función para eliminar un producto
void eliminarProducto(const char *nombre) {
  if (!archivoProductosExiste()) {
    printf("El archivo Productos.txt no existe o está vacío.\n"); // Mensaje de error si el archivo no existe o está vacío
    return;
  }
  if (productoEnFacturas(nombre)) {
    printf("El producto '%s' no se puede eliminar ya que está presente en una "
           "factura.\n",
           nombre); // Mensaje de error si el producto está en una factura
    return;
  }
  FILE *file = fopen("Productos_temp.txt",
                     "w"); // Abrir un archivo temporal en modo escritura
  FILE *original =
      fopen("Productos.txt", "r"); // Abrir el archivo original en modo lectura
  if (file == NULL || original == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si
                                                         // no se puede abrir el
                                                         // archivo
    return;
  }

  char buffer[150];
  int encontrado = 0;
  while (fgets(buffer, sizeof(buffer), original)) { // Leer línea por línea
    Producto temp;
    sscanf(buffer, "%[^,],%d,%f", temp.nombre, &temp.cantidad,
           &temp.precio); // Leer los datos del producto
    if (strcmp(temp.nombre, nombre) != 0) {
      fprintf(
          file, "%s",
          buffer); // Escribir el producto no eliminado en el archivo temporal
    } else {
      encontrado = 1;
    }
  }

  fclose(file);     // Cerrar el archivo temporal
  fclose(original); // Cerrar el archivo original
  if (encontrado) {
    remove("Productos.txt"); // Eliminar el archivo original
    rename("Productos_temp.txt",
           "Productos.txt"); // Renombrar el archivo temporal
    printf("Producto eliminado correctamente.\n"); // Mensaje de confirmación
  } else {
    remove("Productos_temp.txt"); // Eliminar el archivo temporal si no se
                                  // encontró el producto
    printf("Producto no encontrado.\n"); // Mensaje de error si no se encontró
                                         // el producto
  }
}

// Función para eliminar varios productos
void eliminarVariosProductos(const char *nombres[], int cantidad) {
  if (!archivoProductosExiste()) {
    printf("El archivo Productos.txt no existe o está vacío.\n"); // Mensaje deerror si el archivo no existe oestá vacío
    return;
  }
  for (int i = 0; i < cantidad; i++) {
    if (productoEnFacturas(nombres[i])) {
      printf("El producto '%s' no se puede eliminar ya que está presente en "
             "una factura.\n",
             nombres[i]); // Mensaje de error si el producto está en una factura
      return;
    }
  }
  FILE *file = fopen("Productos_temp.txt",
                     "w"); // Abrir un archivo temporal en modo escritura
  FILE *original =
      fopen("Productos.txt", "r"); // Abrir el archivo original en modo lectura
  if (file == NULL || original == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si
                                                         // no se puede abrir el
                                                         // archivo
    return;
  }

  char buffer[150];
  int encontrado = 0;
  while (fgets(buffer, sizeof(buffer), original)) { // Leer línea por línea
    Producto temp;
    int eliminar = 0;
    sscanf(buffer, "%[^,],%d,%f", temp.nombre, &temp.cantidad,
           &temp.precio); // Leer los datos del producto
    for (int i = 0; i < cantidad; i++) {
      if (strcmp(temp.nombre, nombres[i]) == 0) {
        eliminar = 1;
        encontrado = 1;
        break;
      }
    }
    if (!eliminar) {
      fprintf(
          file, "%s",
          buffer); // Escribir el producto no eliminado en el archivo temporal
    }
  }

  fclose(file);     // Cerrar el archivo temporal
  fclose(original); // Cerrar el archivo original
  if (encontrado) {
    remove("Productos.txt"); // Eliminar el archivo original
    rename("Productos_temp.txt",
           "Productos.txt"); // Renombrar el archivo temporal
    printf("Productos eliminados correctamente.\n"); // Mensaje de confirmación
  } else {
    remove("Productos_temp.txt"); // Eliminar el archivo temporal si no se
                                  // encontró ningún producto
    printf("Ningún producto encontrado para eliminar.\n"); // Mensaje de error
                                                           // si no se encontró
                                                           // ningún producto
  }
}

// Función para listar todos los productos
void listarProductos() {
  if (!archivoProductosExiste()) {
    printf("El archivo Productos.txt no existe o está vacío.\n"); // Mensaje de
                                                                  // error si el
                                                                  // archivo no
                                                                  // existe o
                                                                  // está vacío
    return;
  }
  FILE *file = fopen("Productos.txt", "r"); // Abrir el archivo en modo lectura
  if (file == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si
                                                         // no se puede abrir el
                                                         // archivo
    return;
  }

  char buffer[150];
  printf("Lista de productos:\n");
  while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
    Producto temp;
    sscanf(buffer, "%[^,],%d,%f", temp.nombre, &temp.cantidad,
           &temp.precio); // Leer los datos del producto
    printf("Nombre: %s, Cantidad: %d, Precio: %.2f\n", temp.nombre,
           temp.cantidad, temp.precio); // Imprimir los datos del producto
  }

  fclose(file); // Cerrar el archivo
}

// Función para buscar un producto por nombre
Producto *buscarProductoPorNombre(const char *nombre) {
  if (!archivoProductosExiste()) {
    printf("El archivo Productos.txt no existe o está vacío.\n"); // Mensaje de
                                                                  // error si el
                                                                  // archivo no
                                                                  // existe o
                                                                  // está vacío
    return NULL;
  }
  FILE *file = fopen("Productos.txt", "r"); // Abrir el archivo en modo lectura
  if (file == NULL) {
    printf("Error al abrir el archivo de productos.\n"); // Mensaje de error si
                                                         // no se puede abrir el
                                                         // archivo
    return NULL;
  }

  char buffer[150];
  while (fgets(buffer, sizeof(buffer), file)) { // Leer línea por línea
    Producto *temp = (Producto *)malloc(
        sizeof(Producto)); // Asignar memoria para un nuevo producto
    sscanf(buffer, "%[^,],%d,%f", temp->nombre, &temp->cantidad,
           &temp->precio); // Leer los datos del producto
    if (strcmp(temp->nombre, nombre) == 0) {
      fclose(file); // Cerrar el archivo
      return temp;  // Devolver el producto encontrado
    }
    free(temp); // Liberar la memoria si no es el producto buscado
  }

  fclose(file);                        // Cerrar el archivo
  printf("Producto no encontrado.\n"); // Mensaje de error si no se encontró el
                                       // producto
  return NULL;
}
