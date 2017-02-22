var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?zerolevelavg", false);
        xhttp.send();
        var zerolevelavg = xhttp.responseText;
        document.getElementById("zerolevelavg1").innerHTML = zerolevelavg;
        document.getElementById("zerolevelavg2").innerHTML = zerolevelavg;
        xhttp.open("GET", "getdata?celllevelavg", false);
        xhttp.send();
        var celllevelavg = xhttp.responseText;
        document.getElementById("celllevelavg").innerHTML = celllevelavg;
        xhttp.open("GET", "getdata?celltempavg", false);
        xhttp.send();
        var celltempavg = (((xhttp.responseText-186)*((3.283/1.603)/4095))-0.5)*100;
        document.getElementById("celltempavg").innerHTML = celltempavg.toFixed(2);
        xhttp.open("GET", "getsample", false);
        xhttp.send();
        var arava = JSON.parse(xhttp.responseText);
        var ololo = 0;
        for (i = 0; i < 64; i++){ololo += arava[i]}
        var kekus = ololo/64;
        document.getElementById("kekus").innerHTML = kekus.toFixed(2);
        var demo = (((ololo/64)-zerolevelavg)/(celllevelavg-zerolevelavg))*(1297.17*Math.exp(0.082*(celltempavg-25)));
        document.getElementById("demo").innerHTML = demo.toFixed(6);
}
