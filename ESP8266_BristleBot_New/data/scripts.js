var JSONObj = {"led":{"red":{"state":0},"blue":{"state":0},"ir":{"state":0}},
    "proximity":{"leftThreshold":200,"rightThreshold":200,"cycles":7500,"level":1,"rate":1000},
    "drive":{"leftMax":255,"rightMax":255,"delayLeft":500,"delayRight":500,"delay180":500}
};



function setupWebSocket() {
    var host = window.location.hostname;
    var host = "192.168.4.1"
    console.log("Host: " + host);
    connection = new WebSocket("ws://" + host + ":81/");
    connection.onopen = function(evt) {
        console.log("CONNECTED");
        document.getElementById("outputTextStatus").innerHTML = "CONNECTED";
    };
    connection.onclose = function(evt) {
        console.log("DISCONNECTED");
        document.getElementById("outputTextStatus").innerHTML = "DISCONNECTED";
    };
    connection.onmessage = function(evt) {
        console.log('Server: ', evt.data);
        var obj = JSON.parse(evt.data);
        if (obj.led) document.getElementById("outputTextStatus").innerHTML = obj.led;
        if (obj.battery) document.getElementById("outputTextBattery").innerHTML = obj.battery;
        if(obj.left) {
            document.getElementById("leftProx").innerHTML = obj.left;
            if (obj.left > 200) document.getElementById("leftLimit").className = "indicator_off";
            else document.getElementById("leftLimit").className = "indicator_on";
        }
        if(obj.right) {
            document.getElementById("rightProx").innerHTML = obj.right;
            if (obj.right > 200) document.getElementById("rightLimit").className = "indicator_off";
            else document.getElementById("rightLimit").className = "indicator_on";
        }
        if(obj.proximity.leftThr) document.getElementById("leftProxThr").value = obj.proximity.leftThr;
        if(obj.proximity.rightThr) document.getElementById("rightProxThr").value = obj.proximity.rightThr;
        if(obj.motor.leftMax) document.getElementById("leftMotorCal").value = obj.motor.leftMax;
        if(obj.motor.rightMax) document.getElementById("rightMotorCal").value = obj.motor.rightMax;
        
    };
    connection.onerror = function(evt) {
        console.log("ERROR: " + evt.data);
    };
}

function getSettings() {
    var toSend = "SETTINGS";
    WSSend(toSend);
}

function sendSettings() {
    JSONObj.led.red.state = 1;
    console.log(JSONObj.led.red.state);
    WSSend(JSON.stringify(JSONObj));
}

function RedLEDBlink() {
    var toSend = "SET:RED:BLI";
    WSSend(toSend);
}

function RedLEDOn() {
    var toSend = "SET:RED:ENA";
    WSSend(toSend);
}

function RedLEDOff() {
    var toSend = "SET:RED:DIS";
    WSSend(toSend);
}


function BlueLEDBlink() {
    var toSend = "SET:BLU:BLI";
    WSSend(toSend);
}

function BlueLEDOn() {
    var toSend = "SET:BLU:ENA";
    WSSend(toSend);
}

function BlueLEDOff() {
    var toSend = "SET:BLU:DIS";
    WSSend(toSend);
}

function FrontIRLEDBlink() {
    var toSend = "SET:IRF:BLI";
    WSSend(toSend);
}

function FrontIRLEDOn() {
    var toSend = "SET:IRF:ENA";
    WSSend(toSend);
}

function FrontIRLEDOff() {
    var toSend = "SET:IRF:DIS";
    WSSend(toSend);
}

function battFn() {
    var toSend = "BATT";
    WSSend(toSend);
}

function proxEnableFn() {
    var toSend = "PROX_EN";
    WSSend(toSend);
}

function proxDisableFn() {
    var toSend = "PROX_DIS";
    setIndicator();
    WSSend(toSend);
}

function startAuto() {
    var toSend = "AUTO_EN";
    WSSend(toSend);
}

function stopAuto() {
    WSSend("AUTO_DIS");
}

function stopMotors() {
    console.log("Stopping");
    WSSend("STOP");
}

function startMotors() {
    console.log("Starting Motors");
    WSSend("START");
}

function center(newValue)
{
    document.getElementById("inputSliderSteer").value = 0;
    WSSend("s" + 0);
}

function updatePower(newValue)
{
    console.log("Setting Power to: " +newValue);
    WSSend("p"+newValue);
 //   document.getElementById("outputTextPower").innerHTML = Math.round(100*newValue/256);

}

function updateAutoPower(newValue)
{
    console.log("Setting Power to: " +newValue);
    WSSend("q"+newValue);
    //   document.getElementById("outputTextPower").innerHTML = Math.round(100*newValue/256);
    
}

function updateSteer(newValue)
{
    console.log("Setting Steering to: " +newValue);
    WSSend("s"+newValue);
 //   document.getElementById("outputTextSteer").innerHTML = Math.round(100*newValue/256);
}

function setLeftMotor(newValue)
{
    console.log("Setting Left Motor Power to: " +newValue);
    document.getElementById("outputTextLeft").innerHTML = newValue;
    WSSend("e"+newValue);
}

function setRightMotor(newValue)
{
    console.log("Setting Right Motor Power to: " +newValue);
    document.getElementById("outputTextRight").innerHTML = newValue;
    WSSend("f"+newValue);
}


function WSSend(data){
    try {
        console.log("Sending: " + data);
        connection.send(data);
    }
    catch(err) {
        console.log("ERROR: " + err.message);
        console.log("Trying to reestablish connection");
        setupWebSocket();
    }
}

function setIndicator() {
    document.getElementById("rightLimit").className = "indicator_on";
}

function resetIndicator() {
    document.getElementById("leftLimit").className = "indicator_off";
}



function closeSocket()
{
    console.log("Closing Socket");
    proxDisableFn();
    connection.close();
    return null;
}

window.onload = setupWebSocket;

window.onbeforeunload = closeSocket;