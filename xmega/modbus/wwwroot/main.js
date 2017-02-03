var xhttp = new XMLHttpRequest();

function fillform()
{
        xhttp.open("LOL", "getopt", false);
        xhttp.send("ip");
        document.forms["settings"]["ip"].value = xhttp.responseText;
        xhttp.open("LOL", "getopt", false);
        xhttp.send("mac");
        document.forms["settings"]["mac"].value = xhttp.responseText;
        return false;
}
