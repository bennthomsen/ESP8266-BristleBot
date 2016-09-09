function setupWebSocket() {
    var host = window.location.hostname;
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
        if (obj.battery) document.getElementById("outputTextStatus").innerHTML = obj.battery;
        if(obj.left)    document.getElementById("Left").innerHTML = obj.left;
        if(obj.right)   document.getElementById("Right").innerHTML = obj.right;
    };
    connection.onerror = function(evt) {
        console.log("ERROR: " + evt.data);
    };
}

function ledFn() {
    var toSend = "LED";
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
    WSSend(toSend);
}

function stop() {
    console.log("Stopping");
    WSSend("p" + 0);
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


function closeSocket()
{
    console.log("Closing Socket");
    connection.close();
    return null;
}

window.onload = setupWebSocket;

window.onbeforeunload = closeSocket;