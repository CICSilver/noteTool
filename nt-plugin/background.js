var webSocket = new WebSocket('ws://localhost:58585');
function sendMsg(socket, message) {
    // 连接打开时发送一个测试消息
    socket.addEventListener('open', function (event) {
        socket.send(message);
    });

    // 监听服务器返回的消息
    socket.addEventListener('message', function (event) {
        console.log('Message from server: ', event.data);
    });
}
document.addEventListener('mouseup', function() {
    var selectedText = window.getSelection().toString();
    if (selectedText) {
        console.log('Selected text: ' + selectedText);
    }
});
