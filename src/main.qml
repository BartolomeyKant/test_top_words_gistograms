import QtQuick 2.0
import QtQuick.Window 2.0

Window {
	id: root_window

	visible: true
	minimumHeight: 200
	minimumWidth: 300

	color: "white"

	Rectangle {
		width: 30
		height: 20
		color: "blue"
		anchors.centerIn: parent
	}
}
