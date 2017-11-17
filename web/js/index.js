(function() {

    // Initialize Firebase
    var config = {
        apiKey: "AIzaSyA-sKN3yMLlrP_IUDulFAc69OwquLIRVN0",
        authDomain: "beacon-scanner-c41dc.firebaseapp.com",
        databaseURL: "https://beacon-scanner-c41dc.firebaseio.com",
        projectId: "beacon-scanner-c41dc",
        storageBucket: "beacon-scanner-c41dc.appspot.com",
        messagingSenderId: "481630920238"
    };
    firebase.initializeApp(config);
    
    //create reference for entire database
    const dbRefObject = firebase.database().ref();

    //sync changes to any entry in the database
    dbRefObject.on('value', snap => {
        //do stuff with snap.val()
        console.log(snap.val());
    });
}());
