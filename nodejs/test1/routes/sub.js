var express = require('express');
var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://192.168.0.1:1883');
var router = express.Router();

console.log('Enter to sub.js...');

/* GET home page. */
router.get('/', function(req, res, next) {
    client.subscribe('node/test');
});

module.exports = router;
