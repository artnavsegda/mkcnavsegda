var xhttp = new XMLHttpRequest();

function dropdata()
{
        xhttp.open("LOL", "getdata", false);
        xhttp.send("raw");
        document.getElementById("demo").innerHTML = xhttp.responseText;
}
