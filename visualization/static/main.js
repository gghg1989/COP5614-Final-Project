var c=document.getElementById("canvas");
var ctx=c.getContext("2d");
c.width = 800;
c.height = 600;

ctx.beginPath();
ctx.arc(400,300,200,0,2*Math.PI);
ctx.stroke();

var img = new Image();
img.onload = function () {
    ctx.drawImage(img, 100, 100, 64, 64);
}
img.src = "assets/com.png";