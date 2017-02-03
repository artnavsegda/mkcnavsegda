var xhttp = new XMLHttpRequest();

function fillform()
{
        var something = ["sll","cdl","cel","zdl","ztl","prg","tdl","tml","edl","eml","rcl","cal","pcl"];
        for (var i = 0; i < something.length; i++)
        {
                xhttp.open("LOL", "getopt", false);
                xhttp.send(something[i]);
                document.forms["settings"][something[i]].value = xhttp.responseText;
        }
}
