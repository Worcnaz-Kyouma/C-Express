//SocketUnix Recebendo data perfeitamente!

const express = require("express");
const app = express();
const port = 4000;

app.get('/', (req, res) => {
    console.log(req.headers);
    res.send({"success":"true"});
})

app.listen(port, () => {
    console.log("Server running in port: "+ port);
})