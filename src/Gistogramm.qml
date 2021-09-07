import QtQuick 2.15

Item {
	id: root

	// максимально возможное значение в колонке гистограммы
	property int max_value
	property alias model: rep.model

	property real _colon_width: {
		return (width - row.spacing*rep.count - 1)/rep.count
	}

	Row {
		id: row

		anchors.fill: parent

		spacing: (parent.width/(rep.count - 1))*0.1

		Repeater {
			id: rep

			delegate: Colon {
				width: root._colon_width
				height: parent.height
				max_value: root.max_value
				value: 10
				color: "#00ff00"
			}
		}
	}
}
