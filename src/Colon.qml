import QtQuick 2.15

Item {
	id: root

	// значение отображаемое на гистограмме
	property int value
	property int max_value

	// цвет гистограммы
	property color color

	property bool _hovered: m_area.containsMouse
	property bool _pressed: m_area.containsPress
	property color _back_color: root.color
	property color _hovered_color : {
		var c = root.color
		return Qt.hsva(c.hsvHue, c.hsvSaturation - 0.3, c.hsvValue - 0.3, 1)
	}
	property color _pressed_color : {
		var c = root.color
		return Qt.hsva(c.hsvHue, c.hsvSaturation - 0.7, c.hsvValue - 0.7, 1)
	}

	// расчет процента заполнения колонки
	property real _height_percent: {
		return value/max_value
	}

	signal clicked()

	// Зона для перехвата событий мыши
	MouseArea {
		id: m_area
		anchors.fill: parent
		hoverEnabled: true
		onClicked: root.clicked()
	}

	Rectangle {
		id: colon_rect

		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: parent.width * 0.01

		// высота отображаемой колонки определяется как процент от маскимальноый высоты
		height:  parent.height * root._height_percent
		
		color: root._back_color
		// для отображения интерактивности добавляем
		border.color: root._pressed ? root._pressed_color : root._hovered_color
		border.width: {
			if (root._pressed) {
				return 2
			}
			else if (root._hovered) {
				return 4
			}
			return 0
		}
	}
}
