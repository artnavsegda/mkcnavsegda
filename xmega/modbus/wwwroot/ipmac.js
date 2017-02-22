var xhttp = new XMLHttpRequest();

function fillform()
{
        xhttp.open("GET", "getopt", false);
        xhttp.send("ip");
        document.forms["settings"]["ip"].value = xhttp.responseText;
        return false;
}
