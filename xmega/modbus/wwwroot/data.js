var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("GET", "getx16", false);
        xhttp.send();
        document.getElementById("demo").innerHTML = xhttp.responseText;
}
