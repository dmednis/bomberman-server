const net = require('net');

const buf = Buffer.from([1, 'r'.charCodeAt(), 'u'.charCodeAt(), 'k'.charCodeAt(), 0], 'ascii');

const client = net.createConnection({port: 8888}, () => {
    //'connect' listener
    console.log('connected to server!');
    client.write(buf);
});

client.on('data', (data) => {
    console.log('packet type:' + data[0]);
    switch (data[0]) {
        case 2: {

            break;
        }
        case 7: {
            printLobby(data);
            break;
        }
        default: {
            console.log('Unknown packet');
        }
    }
});

client.on('end', () => {
    console.log('disconnected from server');
});


function printLobby(data) {
    const playerOffset = 1;
    const playerCount = data[playerOffset];
    for (let i = 0; i < playerCount; i++) {
        const offset = playerOffset + (25 * i);
        const id = data[offset];
        const name = [];
        data.slice(offset + 1, offset + 23).forEach((char) => {
            if (char >= 48 && char <= 126) {
                name.push(String.fromCharCode(char))
            }
        });
        const ready = data[offset + 24];
        console.log(`ID: ${id} NAME: ${name.join('')} READY: ${ready}`)
    }
}