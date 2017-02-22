var xhttp = new XMLHttpRequest();

function fillform()
{
        var something = ["lowlight","lowflow"];
        for (var i = 0; i < something.length; i++)
        {
                xhttp.open("GET", "getopt", false);
                xhttp.send(something[i]);
                document.forms["settings"][something[i]].value = xhttp.responseText;
        }
}
