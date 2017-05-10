var x = new XMLHttpRequest();
var temp = new TimeSeries();
var pmtc = new TimeSeries();
var pmtv = new TimeSeries();

function dropdata()
{
        x.open("GET", "getdata?temp", false);
        x.send();
        temp.append(new Date().getTime(), x.responseText);
        x.open("GET", "getdata?pmtc", false);
        x.send();
        pmtc.append(new Date().getTime(), x.responseText);
        x.open("GET", "getdata?pmtv", false);
        x.send();
        pmtv.append(new Date().getTime(), x.responseText);
}

function createTimeline()
{
        var g = {millisPerLine:5000,verticalSections:5,strokeStyle:'#202020',sharpLines:true};
        var tempc = new SmoothieChart({millisPerPixel:100,maxValue:1900,minValue:1500,grid:g});
        tempc.addTimeSeries(temp,{strokeStyle:'rgb(200,0,0)',lineWidth:2});
        tempc.streamTo(document.getElementById("temp"),1000);
        var pmtcc = new SmoothieChart({millisPerPixel:100,maxValue:2700,minValue:2500,grid:g});
        pmtcc.addTimeSeries(pmtc,{strokeStyle:'rgb(0,200,0)',lineWidth:2});
        pmtcc.streamTo(document.getElementById("pmtc"),1000);
        var pmtvc = new SmoothieChart({millisPerPixel:100,maxValue:3600,minValue:2000,grid:g});
        pmtvc.addTimeSeries(pmtv,{strokeStyle:'rgb(0,0,200)',lineWidth:2});
        pmtvc.streamTo(document.getElementById("pmtv"),1000);
        setInterval(dropdata, 1000);
}
