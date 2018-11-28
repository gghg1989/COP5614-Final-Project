var express = require('express');
var http = require('http');
var path = require('path');
var fs = require('fs');

var app = express();
var server = http.Server(app);

var data = {};

app.set('port', 5000);
app.use('/static', express.static(__dirname + '/static'));
app.use('/assets', express.static(__dirname + '/assets'));

app.get('/', function(req, res) {
	res.sendFile(path.join(__dirname, 'index.html'));	
});


fs.readFile('assets/network.csv', function(err, data) {
	if (err) {
		console.error(err);
		response.send(500);
	}
	else {
		var str = data.toString()
		console.log(str);
		data = str.split('\r\n');
		console.log(data);
		// response.json(data);
	}
});

// app.get('/data', function(req, res) {
// 		request(data, function(err, res, body) {  
// 		var json = JSON.parse(body);
// 		console.log(json); // Logging the output within the request function
// 	});
// 	// res.send(request.json)
// });

server.listen(5000, function() {
	console.log('Starting server on port 5000...');
});