var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?zerolevelavg", false);
        xhttp.send();
        var zerolevelavg = xhttp.responseText;
        xhttp.open("GET", "getdata?celllevelavg", false);
        xhttp.send();
        var celllevelavg = xhttp.responseText;
        xhttp.open("GET", "getdata?celltempavg", false);
        xhttp.send();
        var celltempavg = (((xhttp.responseText-186)*((3.283/1.603)/4095))-0.5)*100;
        xhttp.open("GET", "getdata?x16", false);
        xhttp.send();
        var demo = ((xhttp.responseText-zerolevelavg)/(celllevelavg-zerolevelavg))*(1297.17*Math.exp(0.082*(celltempavg-25)));
        document.getElementById("demo").innerHTML = demo.toFixed(6);
}
