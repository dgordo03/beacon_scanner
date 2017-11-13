function loadData()
{
    var xhr = new XMLHttpRequest; 

    xhr.open('GET', 'https://www.github.com/dgordo03/beacon_scanner/files/default.txt', true);
    xhr.withCredentials = true;
    xhr.send();
}

loadData();
