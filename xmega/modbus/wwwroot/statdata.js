var xhttp = new XMLHttpRequest();

setInterval(dropdata, 60000);

function dropdata()
{
        xhttp.open("GET", "getdata?zerolevelavg", false);
        xhttp.send();
        var zerolevelavg = xhttp.responseText;
        document.getElementById("zerolevelavg").innerHTML = zerolevelavg;
        xhttp.open("GET", "getdata?celllevelavg", false);
        xhttp.send();
        var celllevelavg = xhttp.responseText;
        document.getElementById("celllevelavg").innerHTML = celllevelavg;
        xhttp.open("GET", "getdata?celltempavg", false);
        xhttp.send();
        var celltempavg = (((xhttp.responseText-186)*((3.283/1.603)/4095))-0.5)*100;
        document.getElementById("celltempavg").innerHTML = celltempavg.toFixed(2);
        var coefficent = (1297.17*Math.exp(0.082*(celltempavg-25)))/(celllevelavg-zerolevelavg)
        document.getElementById("coefficent").innerHTML = coefficent.toFixed(6);
}
