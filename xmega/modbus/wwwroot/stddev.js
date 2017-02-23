var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getsample", false);
        xhttp.send();
        document.getElementById("stddev").innerHTML = math.std(JSON.parse(xhttp.responseText)).toFixed(1);
        document.getElementById("variance").innerHTML = math.var(JSON.parse(xhttp.responseText)).toFixed(1);
        document.getElementById("mean").innerHTML = math.mean(JSON.parse(xhttp.responseText)).toFixed(3);
        document.getElementById("median").innerHTML = math.median(JSON.parse(xhttp.responseText)).toFixed(1);
}
