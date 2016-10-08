var JSONObj = {"led":{"red":{"state":0},"blue":{"state":0},"ir":{"state":0}},
    "proximity":{"leftThreshold":200,"rightThreshold":200,"cycles":7500,"level":1,"rate":1000},
    "drive":{"leftMax":255,"rightMax":255,"delayLeft":500,"delayRight":500,"delay180":500}
};

var Cmd = ["RED", "BLU", "IRF", "IRR"];
var Action = ["BLI", "ENA", "DIS"];

function LEDBlink() {
    var e = document.getElementById("led");
    WSSend("SET:" + Cmd[e.selectedIndex] +":BLI");
}

function LEDOn() {
    var e = document.getElementById("led");
    WSSend("SET:" + Cmd[e.selectedIndex] +":ENA");
}

function LEDOff() {
    var e = document.getElementById("led");
    WSSend("SET:" + Cmd[e.selectedIndex] +":DIS");
}

function battFn() {
    WSSend("GET:BAT");
}

function proxEnableFn() {
    WSSend("SET:PRO:ENA1");
}

function proxDisableFn() {
    WSSend("SET:PRO:ENA0");
}

function startAuto() {
    WSSend("SET:DRI:AUT1");
}

function stopAuto() {
    WSSend("SET:DRI:AUT0");
}

function stopMotors() {
    console.log("Stopping");
    WSSend("SET:DRI:ENA0");
}

function startMotors() {
    console.log("Starting Motors");
    WSSend("SET:DRI:ENA1");
}

function updatePower(newValue)
{
    console.log("Setting Power to: " +newValue);
    WSSend("SET:DRI:POW"+newValue);
    WSSend("SET:DRI:ENA1");
}

function updateAutoPower(newValue)
{
    console.log("Setting Power to: " +newValue);
    WSSend("SET:DRI:POW"+newValue);
}

function updateSteer(newValue)
{
    console.log("Setting Steering to: " +newValue);
    WSSend("SET:DRI:STE"+newValue);
    WSSend("SET:DRI:ENA1");
}

function center(newValue)
{
    document.getElementById("inputSliderSteer").value = 0;
    WSSend("SET:DRI:STE0");
    WSSend("SET:DRI:ENA1");
}

function setLeftMotor(newValue)
{
    console.log("Setting Left Motor Power to: " +newValue);
    document.getElementById("outputTextLeft").innerHTML = newValue;
    WSSend("SET:DRI:LEF"+newValue);
}

function setRightMotor(newValue)
{
    console.log("Setting Right Motor Power to: " +newValue);
    document.getElementById("outputTextRight").innerHTML = newValue;
    WSSend("SET:DRI:RIG"+newValue);
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
