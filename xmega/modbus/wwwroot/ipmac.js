var xhttp = new XMLHttpRequest();

function fillform()
{
        xhttp.open("GET", "getopt?ip", false);
        xhttp.send();
        document.forms["settings"]["ip"].value = xhttp.responseText;
        return false;
}
