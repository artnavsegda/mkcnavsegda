var xhttp = new XMLHttpRequest();

function fillform()
{
        xhttp.open("LOL", "getopt", false);
        xhttp.send("ip");
        document.forms["settings"]["ip"].value = xhttp.responseText;
        return false;
}
