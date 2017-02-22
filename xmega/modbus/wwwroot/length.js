var xhttp = new XMLHttpRequest();

function fillform()
{
        var something = ["sll","cdl","cel","zdl","ztl","prg","tdl","tml","edl","eml","rcl","cal","pcl"];
        for (var i = 0; i < something.length; i++)
        {
                xhttp.open("GET", "getopt?" + something[i], false);
                xhttp.send();
                document.forms["settings"][something[i]].value = xhttp.responseText;
        }
}
