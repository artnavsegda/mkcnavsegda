var xhttp = new XMLHttpRequest();

setInterval(dropdata, 1000);

function dropdata()
{
        xhttp.open("LOL", "getdata", false);
        xhttp.send("x16");
        document.getElementById("demo").innerHTML = xhttp.responseText;
}
