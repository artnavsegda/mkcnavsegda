var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getdata?currentmode", false);
        xhttp.send();
        document.getElementById("currentmode").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?nextmode", false);
        xhttp.send();
        document.getElementById("nextmode").innerHTML = xhttp.responseText;
        xhttp.open("GET", "getdata?timetoexitmode", false);
        xhttp.send();
        document.getElementById("timetoexitmode").innerHTML = xhttp.responseText;
}
