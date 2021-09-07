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

		Column {
			anchors.fill: parent
			spacing: 10

			Gistogramm {
				width: parent.width
				height: parent.height - progress.height
				max_value: TopWordList.max_value
				model: TopWordList
			}

			Progress {
				id: progress
				width: parent.width
				height: visible?20: 0
				percent: TopWordList.curr_progress
				visible: false

				Connections {
					target: TopWordList
					function onLoad_start() {
						progress.visible = true;
					}
					function onLoad_end() {
						progress.visible = false;
					}
				}
			}
		}

	}
}
