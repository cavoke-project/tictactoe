import QtQuick 2.0

Rectangle {
    // BEGIN cavoke section
    Connections {
        target: cavoke

        function onReceiveUpdate(jsonUpdate) {
            console.log("Received: " + jsonUpdate);
            // TODO: Implement your state update here
        }
    }
    // END cavoke section

    onClicked: {
        // TODO: You can send a move using the globally available object
        cavoke.sendMove("<my move>");
    }
}
