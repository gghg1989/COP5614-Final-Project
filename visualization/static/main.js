

var data = {
	"1": {
		"from": "192.168.12.21",
		"to": "192.168.12.22",
		"pakets": 20
	},
	"2": {
		"from": "192.168.12.23",
		"to": "192.168.12.22",
		"pakets": 50
	},
	"3": {
		"from": "192.168.13.21",
		"to": "192.168.13.22",
		"pakets": 25
	},
	"4": {
		"from": "192.168.13.23",
		"to": "192.168.13.22",
		"pakets": 62
	},
	"5": {
		"from": "192.168.13.22",
		"to": "192.168.13.23",
		"pakets": 77
	},
	"6": {
		"from": "192.168.13.22",
		"to": "192.168.13.21",
		"pakets": 114
	},
	"7": {
		"from": "192.168.13.24",
		"to": "192.168.13.22",
		"pakets": 777
	}
};


var img = new Image();
img.onload = function () {
	init();
}
img.src = "assets/com.png";

function init() {
	setInterval(function() {
		$.getJSON( "static/data.json", function(data) {
		  // console.log(d);
			render(data);
		});
	}, 1000/60);
}


function render(data) {
	var dataObj = JSON.parse(JSON.stringify(data));
	var c=document.getElementById("canvas");
	var ctx=c.getContext("2d");
	c.width = 800;
	c.height = 600;

	var containers = [];
	var containersInfo = {};

	$.each(dataObj, function() {
		if (!containers.includes(this.from)) {
			containers.push(this.from);
		}
		if (!containers.includes(this.to)) {
			containers.push(this.to);
		}
	})

	var iconSize = 32;
	var containersLength = containers.length;
	var r = 200,
		cx = 400,
		cy = 300;
	var unitAngle = (2*Math.PI)/containersLength;
	// Generate container position information
	for (var i = 0; i < containersLength; i++) {
		tempContainerInfo = {};
		tempContainerInfo.x = Math.floor((r*Math.cos(i*unitAngle)) + cx);
		tempContainerInfo.y = Math.floor((r*Math.sin(i*unitAngle)) + cy);
		containersInfo[containers[i]] = tempContainerInfo;
	}

	
		for (var i = 0; i < containersLength; i++) {
			// Get current container informaions
			var currentContainer = containersInfo[containers[i]];
			// Draw container ip
			ctx.font = "14px Arial";
			var containerCenterTextOffset = ctx.measureText(containers[i]).width / 3;
			ctx.fillText(containers[i], 
						currentContainer.x - (iconSize / 2) - containerCenterTextOffset, 
						currentContainer.y - (iconSize / 2));
			// Draw container icon
	    	ctx.drawImage(img, 
	    				currentContainer.x - (iconSize / 2), 
	    				currentContainer.y - (iconSize / 2), 
	    				iconSize, 
	    				iconSize);
		}
	

	$.each(dataObj, function() {
		var shrinkRatio = 0.2;
		var lineOffset = 3;
		var TextOffset = 15;
		var arrowSize = 3;

		var startX = containersInfo[this.from].x;
		var startY = containersInfo[this.from].y;
		var endX = containersInfo[this.to].x;
		var endY = containersInfo[this.to].y;
		var vX = startX - endX;
		var vY = startY - endY;
		var packetTextX = startX - (vX / 2);
		var packetTextY = startY - (vY / 2);
		// Direction to right go up, direction to left go down
		if (vX < 0) {
			startY = startY - (vY * shrinkRatio) + lineOffset;
			endY = endY + (vY * shrinkRatio) + lineOffset;	
			packetTextY += TextOffset;
		}
		else {
			startY = startY - (vY * shrinkRatio) - lineOffset;
			endY = endY + (vY * shrinkRatio) - lineOffset;
			packetTextY -= TextOffset;
		}
		if (vY < 0) {
			startX = startX - vX * shrinkRatio - lineOffset; 
			endX = endX + vX * shrinkRatio - lineOffset;
			packetTextX -= TextOffset;
		}
		else {
			startX = startX - vX * shrinkRatio + lineOffset; 
			endX = endX + vX * shrinkRatio + lineOffset;
			packetTextX += TextOffset;
		}

		ctx.beginPath();
		ctx.moveTo(startX,startY);
		ctx.lineTo(endX,endY);
		ctx.stroke();
		canvas_arrow(ctx, startX, startY, endX, endY, arrowSize);

		var packetCenterTextOffset = ctx.measureText(this.pakets).width / 2;
		ctx.font = "12px Arial";
		ctx.fillText(this.pakets, 
					packetTextX - packetCenterTextOffset, 
					packetTextY);

	})
}

function canvas_arrow(context, fromx, fromy, tox, toy, r){
    var x_center = tox;
    var y_center = toy;

    var angle;
    var x;
    var y;

    context.beginPath();

    angle = Math.atan2(toy-fromy,tox-fromx)
    x = r*Math.cos(angle) + x_center;
    y = r*Math.sin(angle) + y_center;

    context.moveTo(x, y);

    angle += (1/3)*(2*Math.PI)
    x = r*Math.cos(angle) + x_center;
    y = r*Math.sin(angle) + y_center;

    context.lineTo(x, y);

    angle += (1 / 3) * (2 * Math.PI);
    x = (r * Math.cos(angle)) + x_center;
    y = (r * Math.sin(angle)) + y_center;

    context.lineTo(x, y);

    context.closePath();

    context.fill();
}
