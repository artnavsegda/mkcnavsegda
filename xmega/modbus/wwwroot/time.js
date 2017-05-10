var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?unixtime", false);
        xhttp.send();
        document.getElementById("unixtime").innerHTML = xhttp.responseText;
        document.getElementById("localtime").innerHTML = Date.now()/1000;
}
