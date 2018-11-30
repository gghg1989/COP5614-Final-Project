var express = require('express');
var http = require('http');
var path = require('path');
var fs = require('fs');

var app = express();
var server = http.Server(app);

var data = {};

app.set('port', 5000);
app.use('/static', express.static(__dirname + '/static'));
// app.use('/assets', express.static(__dirname + '/assets'));

app.get('/', function(req, res) {
	res.sendFile(path.join(__dirname, 'index.html'));	
});

server.listen(5000, function() {
	console.log('Starting server on port 5000...');
});