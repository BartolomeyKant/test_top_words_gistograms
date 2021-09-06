import QtQuick 2.0
import QtQuick.Window 2.0

Window {
	id: root_window

	visible: true
	minimumHeight: 200
	minimumWidth: 300

	color: "white"

	Rectangle {
		width: parent.width - 10
		height: parent.height - 10
		color: "#fcf5e0"
		anchors.centerIn: parent

		Gistogramm {
			anchors.fill: parent
			max_value: 15
			model: 15
		}
	}
}
