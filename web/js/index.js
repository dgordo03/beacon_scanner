function loadData()
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
           jsonParse(xhttp.responseText);
        }
    };
    xhttp.open('get', 'https://dgordo03.github.io/beacon_scanner/files/default.txt');
    xhttp.send();
}

function jsonParse(toParse)
{
   console.log(toParse);
   var obj = JSON.parse(toParse);
   console.log(obj); 
}
