function loadData()
{

    var request = new XMLHttpRequest();

    // Initialize a request
    request.open('get', 'https://dgordo03.github.io/beacon_scanner/files/default.txt');
    // Send it
    request.send();

loadData();
