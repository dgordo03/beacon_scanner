var dataLoaded;
function loadData()
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
           dataLoaded = xhttp.responseText;
           jsonParse(xhttp.responseText);
        }
    };
    xhttp.open('get', 'https://dgordo03.github.io/beacon_scanner/files/beacon.txt');
    xhttp.send();
}

function jsonParse(toParse)
{
   var obj = JSON.parse(toParse);
   console.log(obj);
}

setInterval(function () {
  loadData();
}, 1000);
