#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void enviarOpcion(SOCKET sock, int opcion, int longitud) {
    // Enviar la opción seleccionada por el usuario
    send(sock, (char*)&opcion, sizeof(opcion), 0);
    // Enviar la longitud requerida para el nombre de usuario o contraseña
    send(sock, (char*)&longitud, sizeof(longitud), 0);
}

void iniciarCliente() {

    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    int opcion = 0, longitud = 0;
    int res;

    // Inicializar Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Crear socket para la conexión
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor
    server.sin_port = htons(PORT); // Puerto en el que se conectará

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Error al conectar");
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado exitosamente\n");

    while (1) {
        printf("\n----- Menú -----\n\n");
        printf("1: Generar nombre de usuario\n");
        printf("2: Generar contraseña\n");
        printf("0: Salir\n");
        printf("\nIngrese opción: ");

        // Validar la entrada de opción
        res = scanf("%d", &opcion);
        if (res != 1) {
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            printf("Error: Ingrese solo números.\n");
            continue; // Volver a mostrar el menú
        }

        if (opcion == 0) {
            break; // Salir del bucle y cerrar el cliente
        }

        printf("Ingrese longitud: ");

        // Validar la entrada de longitud
        res = scanf("%d", &longitud);
        if (res != 1 || longitud < 1) {
            while (getchar() != '\n'); // Limpiar el buffer de entrada
            printf("Error: Ingrese solo números positivos.\n");
            continue; // Volver a mostrar el menú
        }

        // Enviar opción y longitud al servidor
        enviarOpcion(sock, opcion, longitud);

        // Recibir respuesta del servidor
        char respuesta[1024] = {0};
        recv(sock, respuesta, sizeof(respuesta), 0);

        // Mostrar respuesta del servidor
        printf("\n--- Respuesta del servidor ---\n");
        if (opcion == 1) {
            printf("Se solicitó un nombre de usuario.\n");
        } else if (opcion == 2) {
            printf("Se solicitó una contraseña.\n");
        }
        printf("%s\n", respuesta);
    }

    // Cerrar conexión
    closesocket(sock);
    WSACleanup();
}
