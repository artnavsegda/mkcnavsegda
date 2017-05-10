var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?unixtime", false);
        xhttp.send();
        document.getElementById("unixtime").innerHTML = new Date(xhttp.responseText*1000);
        document.getElementById("localtime").innerHTML = Date();
}

function myFunction()
{
        xhttp.open("GET", "setopt?unixtime=" + Math.floor(Date.now()/1000), false);
        xhttp.send();
}
