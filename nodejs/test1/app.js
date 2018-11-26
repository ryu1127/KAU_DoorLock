/***************************************** 
2018.11.19 ++ Dongheon for MQTT

Smart Home IoT Project

name : Dongheon Ryu
Version : 1.0.0
******************************************/


/***************************************** 
** Load Module
******************************************/

var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var app = express();
//++ add firebase module
var firebase = require('firebase');


/*****/
console.log('Initialize variable...\n');

/***************************************** 
** 
******************************************/


//for body parser 
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({extended:true}));
app.use(bodyParser.json());

/***************************************** 
** Configure MQTT
******************************************/
var mqtt = require('mqtt');
//Broker's IP Address
var mqttClient = mqtt.connect('mqtt://192.168.0.9:1883');

/**log**/
console.log('mqtt Successfully Connect...');

/***************************************** 
** MQTT Handler Settings !
******************************************/

mqttClient.subscribe('lockers_info/custom_uid/password');
mqttClient.subscribe('users/custom_uid/name');
// Subscribe event
mqttClient.on('message', (topic,message) => {
  console.log('Subscribe MQTT *******');
  topic = topic.split('/');
  console.log('topic : '+topic);
  console.log(message.toString());

  // //set firebase database
  // console.log('set firebase database...');
  // var lockerRef = firebase.database().ref().child(topic[0]+'/'+topic[1]);
  // lockerRef.set({
  //   password : message.toString()
  // });
  // console.log('set complete...');

  //update firebase database
  console.log('update firebase database...');
  var usersRef = firebase.database().ref().child(topic[0]+'/'+topic[1]);
  usersRef.update({
    name : message.toString()
  });
  console.log('update complete...');
  console.log('firebase Completely update !!');
});

//Publish Event
mqttClient.on('connect', () => {
  console.log('Connected to MQTT for Publishing...');
});

console.log('MQTT Handler Settings Complete...');


/***************************************** 
** Configure Firebase
******************************************/

  // Configuration for firebase 
  var config = {
    apiKey: "AIzaSyCXejis-IpYqTa4eO0jxUGZBFjATvOJofc",
    authDomain: "smart-doorlock-9fe12.firebaseapp.com",
    databaseURL: "https://smart-doorlock-9fe12.firebaseio.com/",
  };
  firebase.initializeApp(config);

  /**log**/
console.log('Firebase Initialize App Complete...');

  // Get a reference to the database service
  var database = firebase.database();

  /**log**/
console.log('Firebase Database Connection Settings Complete...');

/***************************************** 
** Firebase listener Settings  !
******************************************/
console.log('Firebase listener settings start...');

// State Change Listener
var doorStateRef = firebase.database().ref('lockers_info/custom_uid/password');
doorStateRef.on('value',function(snapshot){
  console.log('password update!');
  console.log(snapshot.val());
})






/* Define Port */
var port = process.env.PORT || 3000;

/**log**/
console.log('Port Settings Complete...');


/***************************************** 
** Route
******************************************/

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

/**log**/
console.log('Router settings complete...');



// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

/**log**/
console.log('App Engine settings complete...');


/***************************************** 
** Setting Routes
******************************************/
app.use('/', indexRouter);
app.use('/users', usersRouter);

/**log**/
console.log('Router set complete...');


// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

/**log**/
console.log('Handler setting complete...');


var server = app.listen(port, () => {
  /**log**/
  console.log('Ready to open server...');

  console.log("Express server has started on port" + port);
});

module.exports = app;
