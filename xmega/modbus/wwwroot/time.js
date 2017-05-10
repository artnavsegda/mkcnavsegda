var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?unixtime", false);
        xhttp.send();
        document.getElementById("unixtime").innerHTML = new Date(xhttp.responseText*1000);
        document.getElementById("localtime").innerHTML = Date();
}
