import QtQuick 2.0
import Local 1.0

Rectangle
{
    id: page
    width: 360
    height: 360

    Text
    {
        id: errorLabel
        text: qsTr("Last Error: None")
        font.pointSize: 8
        anchors.top : parent.top
    }

    SatelliteModel
    {
        id:satModel
        running: true;
        onErrorFound: errorLabel.text = qsTr("Last Error: %1", "%1=error number").arg(a_iCode)
    }

    ListView
    {
        y: 30
        width: 200;
        height: 200;
        model: satModel

        delegate: Rectangle
        {
            height: 25
            width: 100

            Text
            {
                font.pointSize: 8
                color: isInUse ? "#FF00CC00" : "#FF990000"
                text: qsTr("Id: %1, Str: %2, Elev: %3, Azi: %4")
                    .arg(satelliteIdentifier)
                    .arg(signalStrength)
                    .arg(elevation)
                    .arg(azimuth)
            }
        }
    }
}

