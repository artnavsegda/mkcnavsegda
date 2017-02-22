var xhttp = new XMLHttpRequest();

setInterval(dropdata, 60000);

function dropdata()
{
        xhttp.open("GET", "getdata?x16", false);
        xhttp.send();
        document.getElementById("x16").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?temp", false);
        xhttp.send();
        document.getElementById("temp").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?pmtc", false);
        xhttp.send();
        document.getElementById("pmtc").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?pmtv", false);
        xhttp.send();
        document.getElementById("pmtv").innerHTML = xhttp.responseText;
}
