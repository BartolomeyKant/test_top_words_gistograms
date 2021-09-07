import QtQuick 2.0
import QtQuick.Window 2.0

import org.bulsandhoofs.topwordlist 0.1

Window {
	id: root_window

	visible: true
	minimumHeight: 600
	minimumWidth: 800

	color: "white"

	Rectangle {
		width: parent.width - 10
		height: parent.height - 10
		color: "#fcf5e0"
		anchors.centerIn: parent

		Gistogramm {
			anchors.fill: parent
			max_value: TopWordList.max_value
			model: TopWordList
		}
	}
}
