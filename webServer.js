const express = require('express')
const bodyParser = require('body-parser');
const net = require('net');
const app = express()
const cors = require('cors');


const ESP_TCP_PORT = 333
const ESP_TCP_SERVER = "192.168.0.148"

let dataWrite = "Led = HIGH"

/*let Plants = { Plants: [
    { 
        name : 'Palmier semi-mort',
        room : 'Salon',
        humidityLevel: 90
    },
    { 
        name: "Bébé nénuphare",
        room : 'Cuisine',
        humidityLevel: 20
    },
    { 
        name: "Ti cactus",
        room : 'Cuisine',
        humidityLevel: 12
    },
    { 
        name: "Grosse nenuphare",
        room : 'Cuisine',
        humidityLevel: 72
    },
    { 
        name: "Poilue pâle",
        room : 'Cuisine',
        humidityLevel: 72
    },
    { 
        name: "Poilue foncé",
        room : 'Cuisine',
        humidityLevel: 72
    },
    { 
        name: "Mini érable",
        room : 'Cuisine',
        humidityLevel: 72
    },
    { 
        name: "Potos",
        room : 'Cuisine',
        humidityLevel: 72
    },
  ]};
;*/
let Plants = { Plants: []};
var corsOptions = {
    origin: 'http://localhost:4201',
    optionsSuccessStatus: 200,
    allowedHeaders:['Content-Type', 'application/json']
  }
  
app.use(cors());
app.use(bodyParser.json(), (req, res, next) => {
    next();
});

function show(dataIn){
    data = dataIn;
}

function verifyPlantsQuery(query){
    let isPresent = false
    Plants.Plants.forEach(element => {
        if(element.name == query.PlantName){
            isPresent = true;
        }
    });
    return isPresent;
}

function updatePlants(body) {
    if(body.Plants != undefined){
        body.Plants.forEach(bodyPlant => {
            Plants.Plant.forEach(localPlant => {
                if(localPlant.name == bodyPlant.name){
                    localPlant.room = bodyPlant.room;
                    localPlant.humidityLevel = bodyPlant.humidityLevel;
                }
            })
        })
    }    
}


app.route('/')
    .get(function(req, res){
        res.send(data);
    })

    .post(function(req, res){
        console.log(req.body)
        show(req.body.data);
        res.send({"result": "Data correctly updated"});
    });

app.route('/plants')
    .get(function(req, res){
        res.send(Plants);
    })

    .post(function(req, res){
        updatePlants(req.body);
    });

app.route('/waterPlants')
    .post(function(req,res){
        if(req.query.PlantName == undefined){
            res.statusCode = 422;
            res.send({"result": "Missing PlantName parameter"})
        }
        if(verifyPlantsQuery(req.query)){
            let PlantWateringCommand = "";

            const client = net.connect({port: ESP_TCP_PORT, host: ESP_TCP_SERVER}, () => {           
                if(dataWrite == "Led = HIGH"){
                    dataWrite = "Led = LOW";
                } else if(dataWrite == "Led = LOW"){
                    dataWrite = "Led = HIGH";
                }
                client.write(dataWrite);

                PlantWateringCommand = req.query.PlantName;
                //client.write(PlantWateringCommand)
                client.end();
            });
            res.statusCode = 200;
            res.send({"result":"Your " + req.query.PlantName + " was watered."});
        }
        else{
            res.statusCode = 404;
            res.send({"result":"Invalid plant name in the query parameters"});
        }
    });

app.route('/addPlant')
    .post(function(req,res){
        if(req.body.Plant.name != undefined && req.body.Plant.room != undefined && req.body.Plant.humidityLevel != undefined){
            res.statusCode = 200;
            Plants.Plants.push(req.body.Plant);
            console.log("Your new plant named " + req.body.PlantName + " was added.")
            res.send("Your new plant named " + req.body.PlantName + " was added.");
        }
        else {
            res.statusCode = 400;
            res.send("There was an error, your plant was not added.");
        }
    });

app.listen(3000, function () {
    console.log('Web-Server listening on port 3000!')
})