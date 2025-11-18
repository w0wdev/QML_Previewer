import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    // Container for dynamically loaded QML
    Item {
        id: container
        anchors.fill: parent
    }

    Connections {
        target: QmlUi
    
        function onReloadQml(str) {
            for (var i = container.children.length - 1; i >= 0; i--) {
                container.children[i].destroy();
            }

            // Create new QML object dynamically
            Qt.createQmlObject(str, container, "myQmlCode");
        }
    }
}
