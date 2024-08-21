const net = require('net');
const readline = require('readline');

// Configuración del cliente
const PORT = 8080;
const HOST = '127.0.0.1';

// Crear la interfaz para la entrada del usuario
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Crear el cliente y conectarse al servidor
const client = new net.Socket();
client.connect(PORT, HOST, () => {
    console.log('Cliente conectado exitosamente');
    mostrarMenu();
});

// Función para enviar la opción y longitud al servidor
function enviarOpcion(opcion, longitud) {
    const bufferOpcion = Buffer.alloc(4);
    const bufferLongitud = Buffer.alloc(4);
    bufferOpcion.writeInt32LE(opcion);
    bufferLongitud.writeInt32LE(longitud);
    client.write(bufferOpcion);  // Enviar la opción
    client.write(bufferLongitud); // Enviar la longitud
}

// Función para mostrar el menú y manejar la entrada del usuario
function mostrarMenu() {
    console.log('\n--- Menú ---');
    console.log('1: Generar nombre de usuario');
    console.log('2: Generar contraseña');
    console.log('0: Salir');

    rl.question('Ingrese opción: ', (opcion) => {
        opcion = parseInt(opcion);

        if (isNaN(opcion)) {
            console.log('Error: Ingrese solo números.');
            return mostrarMenu();
        }

        if (opcion === 0) {
            client.end();  // Cerrar conexión y salir
            rl.close();
            return;
        }

        rl.question('Ingrese longitud: ', (longitud) => {
            longitud = parseInt(longitud);

            if (isNaN(longitud) || longitud < 1) {
                console.log('Error: Ingrese solo números positivos.');
                return mostrarMenu();
            }

            // Enviar opción y longitud al servidor
            enviarOpcion(opcion, longitud);

            // Recibir respuesta del servidor
            client.once('data', (data) => {
                console.log('\n--- Respuesta del servidor ---');
                console.log(data.toString());
                mostrarMenu();  // Volver a mostrar el menú después de recibir la respuesta
            });
        });
    });
}

// Manejo de errores
client.on('error', (err) => {
    console.error('Error en la conexión:', err.message);
});

client.on('close', () => {
    console.log('Conexión cerrada');
});
