var express = require('express');
var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://localhost:1883');
var router = express.Router();

console.log('Enter to pub.js...');
console.log('Enter to router.get...in pub');
client.on('connect', function () {
  console.log('Enter to connect...in pub');
  client.publish('test', 'hello mqtt');
  console.log('Successfully Publishing...');
  res.send('Publish Success!');
})

client.on('message', function (topic, message) {
  console.log(message.toString());
  client.end();
});

module.exports = router;
