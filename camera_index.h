#ifndef CAMERA_INDEX_H
#define CAMERA_INDEX_H

#include <pgmspace.h>

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Camera Pro Control</title>

<style>

body{
margin:0;
font-family:Segoe UI;
background:linear-gradient(135deg,#141e30,#243b55);
color:white;
}

.container{display:flex;height:100vh;}

.stream{
flex:2;
display:flex;
flex-direction:column;
justify-content:center;
align-items:center;
background:#0f2027;
}

.stream img{
width:90%;
border-radius:15px;
box-shadow:0 0 25px black;
}

.warning{
margin-top:10px;
color:#ff4d4d;
font-weight:bold;
font-size:18px;
}

.controls{
flex:1;
overflow-y:auto;
padding:20px;
background:#1c2b36;
}

button{
padding:10px 20px;
margin:5px;
border:none;
border-radius:20px;
background:#00c6ff;
font-weight:bold;
cursor:pointer;
}

label{margin-top:15px;display:block;}

.rangeRow{
display:flex;
align-items:center;
}

.rangeRow span{
width:40px;
opacity:.6;
text-align:center;
}

input[type=range]{flex:1;margin:0 10px;}

.toggle{
margin-top:10px;
display:flex;
justify-content:space-between;
align-items:center;
}

</style>
</head>

<body>

<div class="container">

<div class="stream">

<img id="stream">

<div id="warning" class="warning"></div>

</div>

<div class="controls">

<h2>Camera Controls</h2>

<button onclick="startStream()">Start</button>
<button onclick="stopStream()">Stop</button>
<button onclick="capture()">Snapshot</button>

<hr>

<label>Resolution</label>
<select onchange="updateValue('framesize',this.value)">
<option value="10">UXGA</option>
<option value="9">SXGA</option>
<option value="8">XGA</option>
<option value="7">SVGA</option>
<option value="6">VGA</option>
<option value="5">CIF</option>
<option value="4">QVGA</option>
<option value="3">HQVGA</option>
<option value="0">QQVGA</option>
</select>

<script>

function makeSlider(name,min,max){
document.write(`
<label id="${name}Label">${name}:50</label>
<div class="rangeRow">
<span>${min}</span>
<input type="range" min="0" max="100" value="50"
oninput="updateSlider('${name}',this.value)">
<span>${max}</span>
</div>
`);
}

</script>

<script>
makeSlider("quality",10,63);
makeSlider("brightness",-2,2);
makeSlider("contrast",-2,2);
makeSlider("saturation",-2,2);
makeSlider("sharpness",-2,2);
makeSlider("aec_value",0,1200);
makeSlider("gainceiling",0,6);
makeSlider("xclk",10,20);
</script>

<hr>

<div class="toggle">AWB <input type="checkbox" onchange="updateValue('awb',this.checked?1:0)"></div>
<div class="toggle">AWB Gain <input type="checkbox" onchange="updateValue('awb_gain',this.checked?1:0)"></div>
<div class="toggle">Auto Exposure Sensor <input type="checkbox" onchange="updateValue('aec',this.checked?1:0)"></div>
<div class="toggle">Auto Exposure DSP <input type="checkbox" onchange="updateValue('aec2',this.checked?1:0)"></div>
<div class="toggle">Auto Gain Control <input type="checkbox" onchange="updateValue('agc',this.checked?1:0)"></div>
<div class="toggle">Lens Correction <input type="checkbox" onchange="updateValue('lenc',this.checked?1:0)"></div>
<div class="toggle">Horizontal Mirror <input type="checkbox" onchange="updateValue('hmirror',this.checked?1:0)"></div>
<div class="toggle">Vertical Flip <input type="checkbox" onchange="updateValue('vflip',this.checked?1:0)"></div>
<div class="toggle">Color Bar <input type="checkbox" onchange="updateValue('colorbar',this.checked?1:0)"></div>
<div class="toggle">BPC <input type="checkbox" onchange="updateValue('bpc',this.checked?1:0)"></div>
<div class="toggle">WPC <input type="checkbox" onchange="updateValue('wpc',this.checked?1:0)"></div>
<div class="toggle">Raw Gamma <input type="checkbox" onchange="updateValue('raw_gma',this.checked?1:0)"></div>

</div>
</div>

<script>

let lastFrameTime=0;

function startStream(){

let base=document.location.origin.replace(/:\d+$/,'');
let img=document.getElementById("stream");

img.src=base+":81/stream";

img.onload=function(){
lastFrameTime=Date.now();
document.getElementById("warning").innerText="";
};

}

function stopStream(){
document.getElementById("stream").src="";
document.getElementById("warning").innerText="";
}

function capture(){window.open("/capture");}

function updateValue(v,val){
fetch(`/control?var=${v}&val=${val}`);
}

function updateSlider(variable,value){

let label=document.getElementById(variable+"Label");
label.innerText=variable+": "+value;

let espValue=value;

switch(variable){

case "brightness":
case "contrast":
case "saturation":
case "sharpness":
espValue=Math.round((value-50)/25);
break;

case "quality":
espValue=Math.round(63-(value*0.53));
break;

case "aec_value":
espValue=Math.round(value*12);
break;

case "gainceiling":
espValue=Math.round(value*0.06);
break;

case "xclk":
espValue=Math.round(10+(value*0.1));
break;
}

fetch(`/control?var=${variable}&val=${espValue}`);
}

setInterval(function(){

if(lastFrameTime===0)return;

let diff=Date.now()-lastFrameTime;

if(diff>5000){

document.getElementById("warning").innerText=
"⚠ Camera connection lost or device powered off";

}

},2000);

</script>

</body>
</html>
)rawliteral";

#endif
