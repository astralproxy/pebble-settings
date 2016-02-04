Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://cdn.rawgit.com/jsternfield/mgscodec/6ea29eb2ec3ee67ef8b41f31c5af16dc1e3c996b/config/index.html';
  //var url = 'https://cdn.rawgit.com/jsternfield/mgscodec/02c71fcbf2c7feee68874a33e629125938d6c0a2/config/index.html'; // with invert
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var mainColor = configData['main_color'];
  var secColor = configData['sec_color'];

  var dict = {};
  dict['KEY_COLOR_RED1'] = parseInt(mainColor.substring(2, 4), 16);
  dict['KEY_COLOR_GREEN1'] = parseInt(mainColor.substring(4, 6), 16);
  dict['KEY_COLOR_BLUE1'] = parseInt(mainColor.substring(6), 16);
  dict['KEY_COLOR_RED2'] = parseInt(secColor.substring(2, 4), 16);
  dict['KEY_COLOR_GREEN2'] = parseInt(secColor.substring(4, 6), 16);
  dict['KEY_COLOR_BLUE2'] = parseInt(secColor.substring(6), 16);
  dict['KEY_TWENTY_FOUR'] = configData['twenty_four'];

  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});