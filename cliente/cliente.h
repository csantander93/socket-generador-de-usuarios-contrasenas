#ifndef CLIENTE_H
#define CLIENTE_H

#include <winsock2.h>

#define PORT 8080

// Función para enviar la opción y longitud al servidor
void enviarOpcion(SOCKET sock, int opcion, int longitud);

// Función para iniciar el cliente
void iniciarCliente();

#endif // CLIENTE_H
