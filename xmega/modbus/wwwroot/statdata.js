var xhttp = new XMLHttpRequest();

setInterval(dropdata, 60000);

function dropdata()
{
        xhttp.open("GET", "getdata?zerolevelavg", false);
        xhttp.send();
        document.getElementById("zerolevelavg").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?celllevelavg", false);
        xhttp.send();
        document.getElementById("celllevelavg").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?celltempavg", false);
        xhttp.send();
        document.getElementById("celltempavg").innerHTML = xhttp.responseText;
}
