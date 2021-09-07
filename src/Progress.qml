import QtQuick 2.15

Item {
	id: root

	property real percent

	Rectangle {
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		color: "blue"
		anchors.margins: 5
		width: (parent.width - 10)*(percent)
	}
}
