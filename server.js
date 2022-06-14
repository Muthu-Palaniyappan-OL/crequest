/*
    Sample javascript program which creates a simple webserver
    serves a normal webpage to all the requests it received
*/

const http = require('http');
const server = http.createServer((req, res) => {
    res.setHeader("Content-Type", "text/html")
    console.log('Received', req.url, req.connection.remoteAddress)
    res.end('<h1> Request: ' + req.url + ' from ' + req.connection.remoteAddress + '</h1>');
})
server.listen(8080)