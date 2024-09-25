//SocketUnix Recebendo data perfeitamente!

const express = require("express");
const app = express();
const port = 4000;

app.get('/', (req, res) => {
    res.send("Nah, its express bruh")
});

app.get('/json', (req, res) => {
    res.send({
        server: "Express... bruh",
        creator: "Dont know, but is a cool guy i guess"
    });
});

app.listen(port, () => {
    console.log("Server running in port: "+ port);
})