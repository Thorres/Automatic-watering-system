const net = require('net');
const axios = require('axios');
const c_timeout = 1000;

const WebServerURL = 'http://192.168.0.102:3000'

// Keep track of the chat clients
var clients = [];

// Start a TCP Server
net.createServer(function (socket) {

  // Identify this client
  socket.name = socket.remoteAddress + ":" + socket.remotePort 

  // Put this new client in the list
  clients.push(socket);

  // Send a nice welcome message and announce
  socket.write("Connection successful, Welcome!\n");
   
  socket.on('data', function (data) {
    let request;
    
    try{
      request = JSON.parse(data.toString());
      socket.write("Data received correctly");
    } catch (err) {
      socket.write("Invalid JSON format for the data.");
      console.error(err);
    }
    doHTTRequest(request);
  });

  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
  });
}).listen(5000);


async function doHTTRequest(request) {
  try{
    let promiseHTTP = await axios.post(WebServerURL, { data: request.Data}, {timeout: c_timeout})
    console.log(promiseHTTP.data);
  } catch(err) {
    console.error(err);
  } 
}

// Put a friendly message on the terminal of the server.
console.log("TCP transfer server running at port 5000\n");