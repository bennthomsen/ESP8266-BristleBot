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
        if(obj.leftThr)    document.getElementById("leftProxThr").innerHTML = obj.leftThr;
        if(obj.righThr)   document.getElementById("rightProxThr").innerHTML = obj.rightThr;
        if(obj.leftCal)    document.getElementById("leftMotorCal").innerHTML = obj.leftThr;
        if(obj.righCal)   document.getElementById("rightMotorCal").innerHTML = obj.rightThr;
        
    };
    connection.onerror = function(evt) {
        console.log("ERROR: " + evt.data);
    };
}

function RedLEDBlink() {
    var toSend = "RED_LED_BLINK";
    WSSend(toSend);
}

function RedLEDOn() {
    var toSend = "RED_LED_ON";
    WSSend(toSend);
}

function RedLEDOff() {
    var toSend = "RED_LED_OFF";
    WSSend(toSend);
}

function BlueLEDBlink() {
    var toSend = "BLUE_LED_BLINK";
    WSSend(toSend);
}

function BlueLEDOn() {
    var toSend = "BLUE_LED_ON";
    WSSend(toSend);
}

function BlueLEDOff() {
    var toSend = "BLUE_LED_OFF";
    WSSend(toSend);
}

function FrontIRLEDBlink() {
    var toSend = "FRONT_LED_BLINK";
    WSSend(toSend);
}

function FrontIRLEDOn() {
    var toSend = "FRONT_LED_ON";
    WSSend(toSend);
}

function FrontIRLEDOff() {
    var toSend = "FRONT_LED_OFF";
    WSSend(toSend);
}

function battFn() {
    var toSend = "BATT";
    WSSend(toSend);
}

function proxSingle() {
    var toSend = "PROX_SINGLE";
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

function stopMotors() {
    console.log("Stopping");
    WSSend("p" + 0);
}

function startMotors() {
    console.log("Starting Motors");
    WSSend("s" + document.getElementById("inputSliderSteer").value);
    WSSend("p" + document.getElementById("inputSliderPower").value);
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
}

function updateSteer(newValue)
{
    console.log("Setting Steering to: " +newValue);
    WSSend("s"+newValue);
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