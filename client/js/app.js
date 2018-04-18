(function() {
  'use strict';
  // create a new websocket
  var socket = io.connect('http://localhost:8000'),
    // select all the DOM elements needed for this experiment
    $body = $('body'),
    onSocketNotification = function(data) {
      // print all the messages coming from the arduino board
      var $div = $('<center>');
      $div.html(data);
      $body.append($div);
      $div.delay(1000).fadeOut(function() {
        $div.remove();
      });
    };

  // Set listeners
  socket.on('notification', onSocketNotification);
  $btn.on('click', toggleLightStatus);

  // turn off the light by default on any new connection
  socket.emit('lightStatus', lightStatus);
}());
