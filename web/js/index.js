function loadData()
{
    var xhr; 
    if (window.XMLHttpRequest) 
        xhr = new XMLHttpRequest(); 
    else if (window.ActiveXObject) 
        xhr = new ActiveXObject("Msxml2.XMLHTTP");
    else 
        throw new Error("Ajax is not supported by your browser");

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            console.log("done loading data...");
        }
    }

    xhr.open('GET', 'https://www.github.com/dgordo03/beacon_scanner/files/default.txt');
    xhr.send(null);
}

loadData();
