const net = require('net');
const axios = require('axios');
const c_timeout = 2000;

const WebServerURL = 'http://192.168.0.102:3000'

var clients = [];

net.createServer(function (socket) {
  socket.name = socket.remoteAddress + ":" + socket.remotePort 

  clients.push(socket);

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
    if(request.command == "Init"){
      initPlantsValues(socket);
    }
    else {
      doHTTRequest(request);
    }
  });

  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
  });
}).listen(5000);

async function initPlantsValues(){
  let response = await axios.post(WebServerURL + "/plants",{timeout: c_timeout});
  socket.write(response);
}


async function doHTTRequest(request) {
  try{
    let promiseHTTP = await axios.post(WebServerURL, { data: request.Data}, {timeout: c_timeout})
    console.log(promiseHTTP.data);
  } catch(err) {
    console.error(err);
  } 
}

console.log("TCP transfer server running at port 5000\n");