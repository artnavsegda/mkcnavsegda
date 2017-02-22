var xhttp = new XMLHttpRequest();
var line1 = new TimeSeries();
var line2 = new TimeSeries();

function dropdata()
{
        xhttp.open("GET", "getx16", false);
        xhttp.send();
        line1.append(new Date().getTime(), xhttp.responseText);
}

function dropdata2()
{
        xhttp.open("GET", "getrawrun", false);
        xhttp.send();
        var arava = JSON.parse(xhttp.responseText);
        for (i = 0; i < 10; i++)
        {
                line2.append(new Date().getTime()-(i*100), arava[i]);
        }
}

function dropdata3()
{
        xhttp.open("GET", "getrun", false);
        xhttp.send();
        var arava = JSON.parse(xhttp.responseText);
        for (i = 0; i < 100; i++)
        {
                line1.append(new Date().getTime()-(i*1000), arava[i]);
        }
}

function createTimeline()
{
        var smoothie = new SmoothieChart({millisPerPixel:100,maxValue:6500,minValue:6000,grid:{millisPerLine:5000,verticalSections:10,strokeStyle:'#202020',sharpLines:true},interpolation:'linear'});
        smoothie.addTimeSeries(line2,{strokeStyle:'rgb(200,0,0)'});
        smoothie.addTimeSeries(line1,{lineWidth:2});
        smoothie.streamTo(document.getElementById("mycanvas"),1000);
        dropdata3();
        setInterval(dropdata, 501);
        setInterval(dropdata2, 1000);
}
