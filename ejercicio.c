#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     // Para open()
#include <unistd.h>    // Para read(), write(), close()
#include <string.h>
#include <sys/stat.h>  // Para permisos de archivo

#define MAX_TEXTO 1000
#define NOMBRE_ARCHIVO "texto_guardado.txt"

int main() {
    char texto[MAX_TEXTO];
    char buffer_lectura[MAX_TEXTO];
    int fd;  // File descriptor
    ssize_t bytes_escritos, bytes_leidos;
    
    printf("=== PROGRAMA DE MANEJO DE ARCHIVOS ===\n\n");
    
    // Paso 1: Leer texto del usuario
    printf("Ingrese el texto que desea guardar:\n");
    printf("> ");
    
    // Leer línea completa incluyendo espacios
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        printf("Error al leer el texto\n");
        return 1;
    }
    
    // Remover el salto de línea si existe
    size_t longitud = strlen(texto);
    if (longitud > 0 && texto[longitud - 1] == '\n') {
        texto[longitud - 1] = '\0';
        longitud--;
    }
    
    printf("\nTexto ingresado: \"%s\"\n", texto);
    printf("Longitud: %zu caracteres\n\n", longitud);
    
    // Paso 2: Crear/abrir archivo para escritura
    printf("Guardando texto en archivo \"%s\"...\n", NOMBRE_ARCHIVO);
    
    // O_CREAT: crear si no existe
    // O_WRONLY: solo escritura  
    // O_TRUNC: truncar archivo existente
    // 0644: permisos (rw-r--r--)
    fd = open(NOMBRE_ARCHIVO, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Error al abrir archivo para escritura");
        return 1;
    }
    
    // Paso 3: Escribir texto al archivo
    bytes_escritos = write(fd, texto, longitud);
    
    if (bytes_escritos == -1) {
        perror("Error al escribir en el archivo");
        close(fd);
        return 1;
    }
    
    printf("Se escribieron %zd bytes al archivo\n", bytes_escritos);
    
    // Cerrar archivo después de escribir
    if (close(fd) == -1) {
        perror("Error al cerrar archivo después de escribir");
        return 1;
    }
    
    printf("Archivo cerrado exitosamente\n\n");
    
    // Paso 4: Abrir archivo para lectura
    printf("Leyendo contenido del archivo...\n");
    
    fd = open(NOMBRE_ARCHIVO, O_RDONLY);  // Solo lectura
    
    if (fd == -1) {
        perror("Error al abrir archivo para lectura");
        return 1;
    }
    
    // Limpiar buffer de lectura
    memset(buffer_lectura, 0, sizeof(buffer_lectura));
    
    // Paso 5: Leer contenido del archivo
    bytes_leidos = read(fd, buffer_lectura, sizeof(buffer_lectura) - 1);
    
    if (bytes_leidos == -1) {
        perror("Error al leer del archivo");
        close(fd);
        return 1;
    }
    
    // Asegurar terminación nula
    buffer_lectura[bytes_leidos] = '\0';
    
    printf("Se leyeron %zd bytes del archivo\n", bytes_leidos);
    
    // Cerrar archivo después de leer
    if (close(fd) == -1) {
        perror("Error al cerrar archivo después de leer");
        return 1;
    }
    
    // Paso 6: Mostrar resultado
    printf("\n=== CONTENIDO LEÍDO DEL ARCHIVO ===\n");
    printf("Texto: \"%s\"\n", buffer_lectura);
    printf("Longitud: %zd caracteres\n", bytes_leidos);
    
    // Verificar que el contenido coincide
    if (strcmp(texto, buffer_lectura) == 0) {
        printf("\n✓ El texto leído coincide exactamente con el texto original\n");
    } else {
        printf("\n✗ El texto leído NO coincide con el texto original\n");
    }
    
    printf("\nArchivo \"%s\" creado exitosamente\n", NOMBRE_ARCHIVO);
    printf("Programa terminado\n");
    
    return 0;
}
