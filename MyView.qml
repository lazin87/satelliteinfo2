import QtQuick 2.0
import Local 1.0
import GpsPosProv 1.0

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

    Text
    {
        id: positionLabel
        y: errorLabel.height + errorLabel.y + 5
        font.pointSize: 8
        text: qsTr("Long: %1, Lat: %2, Alt: %3").arg(positionProvider.longitude)
                                                .arg(positionProvider.latitude)
                                                .arg(positionProvider.altitude)
    }

    SatelliteModel
    {
        id:satModel
        running: true;
        onErrorFound: errorLabel.text = qsTr("Last Error: %1", "%1=error number").arg(a_iCode)
    }

    CGpsPositionProvider
    {
        id: positionProvider
    }

    ListView
    {
        y: positionLabel.height + positionLabel.y + 5
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

