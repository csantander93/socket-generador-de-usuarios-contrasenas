#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generarNombreUsuario(int longitud, char *resultado) {
    const char *vocales = "aeiou"; // Conjunto de vocales
    const char *consonantes = "bcdfghjklmnpqrstvwxyz"; // Conjunto de consonantes
    int esVocal = rand() % 2; // Decidir aleatoriamente si empezar con vocal o consonante

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            resultado[i] = vocales[rand() % 5]; // Seleccionar una vocal aleatoria
        } else {
            resultado[i] = consonantes[rand() % 21]; // Seleccionar una consonante aleatoria
        }
        esVocal = !esVocal; // Alternar entre vocal y consonante
    }

    resultado[longitud] = '\0'; // Terminar el string
}

void generarContrasena(int longitud, char *resultado) {
    const char *caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // Conjunto de caracteres para la contraseña

    for (int i = 0; i < longitud; i++) {
        resultado[i] = caracteres[rand() % 62]; // Seleccionar un carácter aleatorio
    }

    resultado[longitud] = '\0'; // Terminar el string
}

void manejarConexion(SOCKET new_socket) {
    int opcion;
    int longitud;
    char resultado[1024] = {0}; // Asegura que el buffer esté limpio

    while (1) {
        // Recibir opción desde el cliente
        int bytes_received = recv(new_socket, (char*)&opcion, sizeof(opcion), 0);
        if (bytes_received <= 0) {
            perror("Error al recibir la opción o conexión cerrada");
            break; // Salir del bucle si hay error en la recepción
        }

        // Recibir longitud desde el cliente
        bytes_received = recv(new_socket, (char*)&longitud, sizeof(longitud), 0);
        if (bytes_received <= 0) {
            perror("Error al recibir la longitud");
            break; // Salir del bucle si hay error en la recepción
        }

        // Validar la longitud
        if (opcion == 1 && (longitud < 5 || longitud > 15)) {
            strcpy(resultado, "Error: La longitud debe ser entre 5 y 15.");
        } else if (opcion == 2 && (longitud < 8 || longitud > 50)) {
            strcpy(resultado, "Error: La longitud debe ser entre 8 y 50.");
        } else {
            // Procesar la opción recibida
            if (opcion == 1) {
                generarNombreUsuario(longitud, resultado);
                printf("Se ha generado un nombre de usuario.\n");
            } else if (opcion == 2) {
                generarContrasena(longitud, resultado);
                printf("Se ha generado una contraseña.\n");
            } else {
                strcpy(resultado, "Opción no válida.");
            }
        }

        // Mostrar en consola lo que se envía al cliente
        printf("\n--- Respuesta al cliente ---\n");
        printf("%s\n", resultado);

        // Enviar respuesta al cliente
        send(new_socket, resultado, strlen(resultado) + 1, 0); // +1 para incluir '\0'
    }

    // Cerrar conexión
    closesocket(new_socket);
}

void iniciarServidor() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Inicializar Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Crear socket para el servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces
    address.sin_port = htons(PORT); // Puerto en el que el servidor escuchará

    // Asociar el socket con la dirección
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("\nEsperando conexiones...\n");

        // Aceptar una conexión entrante
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("Error en accept");
            exit(EXIT_FAILURE);
        }

        printf("Cliente conectado exitosamente\n");

        // Manejar la conexión con el cliente
        manejarConexion(new_socket);
    }

    // Cerrar el socket del servidor
    closesocket(server_fd);
    WSACleanup(); // Limpiar Winsock
}
