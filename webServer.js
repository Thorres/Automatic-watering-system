const express = require('express')
const bodyParser = require('body-parser');
const app = express()

let data = {
    "Plant1": "Test"
};

app.use(bodyParser.json(), (req, res, next) => {
    next();
});

function show(dataIn){
    data = dataIn;
}

app.route('/')
    .get(function(req, res){
        res.send(data);
    })

    .post(function(req, res){
        console.log(req.body)
        show(req.body.data);
        res.send("Data correctly updated");
    });

app.listen(3000, function () {
    console.log('Web-Server listening on port 3000!')
})