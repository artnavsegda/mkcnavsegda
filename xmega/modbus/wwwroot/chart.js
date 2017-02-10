var xhttp = new XMLHttpRequest();
var line1 = new TimeSeries();
var line2 = new TimeSeries();

function dropdata()
{
        xhttp.open("LOL", "getdata", false);
        xhttp.send("x16");
        line1.append(new Date().getTime(), xhttp.responseText);
}

function dropdata2()
{
        xhttp.open("LOL", "getdata", false);
        xhttp.send("raw");
        line2.append(new Date().getTime(), xhttp.responseText);
}

function createTimeline()
{
        var smoothie = new SmoothieChart({millisPerPixel:100,maxValue:7000,minValue:5000,grid:{millisPerLine:5000,verticalSections:10,strokeStyle:'#202020',sharpLines:true},interpolation:'linear'});
        smoothie.addTimeSeries(line2,{strokeStyle:'rgb(200,0,0)'});
        smoothie.addTimeSeries(line1,{lineWidth:2});
        smoothie.streamTo(document.getElementById("mycanvas"));
        setInterval(dropdata, 500);
        setInterval(dropdata2, 100);
}
