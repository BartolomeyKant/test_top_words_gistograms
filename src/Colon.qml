import QtQuick 2.15

Item {
	id: root

	// значение отображаемое на гистограмме
	property int value
	property string lable
	property int max_value

	// цвет гистограммы
	property color color

	property color _back_color: root.color

	// расчет процента заполнения колонки
	property real _height_percent: {
		return value/max_value
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

		Text {
			anchors.centerIn: parent
			text: root.lable + "\n" + root.value
		}
	}
}
