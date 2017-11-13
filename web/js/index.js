function loadData()
{

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
           // Typical action to be performed when the document is ready:
           console.log(xhttp.responseText);
        }
    };
    // Initialize a request
    xhttp.open('get', 'https://dgordo03.github.io/beacon_scanner/files/default.txt');
    // Send it
    xhttp.send();
}
loadData();
