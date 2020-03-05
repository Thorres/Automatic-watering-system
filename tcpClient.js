var net = require('net');

const data = "Led = LOW"

const clients = net.connect({port: 333, host: "192.168.0.148"}, () => {
  // 'connect' listener
  console.log('connected to server!');
  clients.write(data);
  clients.write("test");
});