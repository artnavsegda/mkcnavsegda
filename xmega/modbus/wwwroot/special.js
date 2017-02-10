var xhttp = new XMLHttpRequest();

function dropdata()
{
        xhttp.open("GET", "special", false);
        xhttp.send("special");
        document.getElementById("demo").innerHTML = xhttp.responseText;
}
