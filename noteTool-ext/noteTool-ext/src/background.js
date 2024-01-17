'use strict';

// With background scripts you can communicate with popup
// and contentScript files.
// For more information on background script,
// See https://developer.chrome.com/extensions/background_pages

var webSocket;
var url = 'ws://127.0.0.1:58585';
var isConnected = false;
var isConncting = false;
// webSocket = new WebSocket(url);
function connect() {
  webSocket = new WebSocket(url);

  webSocket.onopen = function (event) {
    console.log('Connected to server');
  }
};

webSocket.onopen = function (event) {
  isConnected = true;
  isConncting = false;
  // this.send('[BG] on Connect Hello Server!');
}

webSocket.onMessage = function (event) {
  console.log('Server', event.data);
};

webSocket.onclose = function (event) {
  isConnected = false;
  console.log('socket is closed. Reconnect will be attempted in 1 second.')
};
chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
  // greetings
  if (request.type === 'GREETINGS') {
    const message = `Hi ${sender.tab ? 'Con' : 'Pop'
      }, my name is Bac. I am from Background. It's great to hear from you.`;

    // Log message coming from the `request` parameter
    console.log(request.payload.message);
    // Send a response message
    sendResponse({
      message,
    });
  };

  // select text
  if (request.type === 'SELECT') {
    if (!isConnected && !isConncting) {
      connect();
      isConncting = true;
    };
    if(isConnected){
      webSocket.send(request.payload.message);
    }
    console.log('[BG] Selected text: ', request.payload.message);
  };
}
);
