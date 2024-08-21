#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <winsock2.h>

#define PORT 8080

// Función para generar un nombre de usuario
void generarNombreUsuario(int longitud, char *resultado);

// Función para generar una contraseña
void generarContrasena(int longitud, char *resultado);

// Función para manejar la conexión con el cliente
void manejarConexion(SOCKET new_socket);

// Función para iniciar el servidor
void iniciarServidor();

#endif // SERVIDOR_H
