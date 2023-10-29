import QtQuick 2.7
import QtQuick.Window 2.2
import QtLocation 5.8
import QtPositioning 5.8

Item {
    id: win
    objectName: "window"
    visible: true
    width: 300
    height: 200

    Map {
        id: map
        anchors.fill: parent
        zoomLevel: 8
        //maximumZoomLevel:12
        minimumZoomLevel: 3
        copyrightsVisible:false
        //gesture.enabled: false
        //activeMapType: MapType.SatelliteMapDay//map.supportedMapTypes[10]
        center:QtPositioning.coordinate(tracker.getPosLat,tracker.getPosLon)


        plugin: Plugin {
            name: 'osm';
             PluginParameter {
                 name: "osm.mapping.providersrepository.disabled";
                 value: true
             }
        }

        MapQuickItem{
            id: pos_marker
            coordinate:QtPositioning.coordinate(tracker.getPosLat,tracker.getPosLon)
            sourceItem: Grid {
                Rectangle{
                    width: 10
                    height:10
                    radius:10
                    color: 'red'
                }
            }
        }
        Item {
            Timer {
                interval: 500; running: true; repeat: true
                onTriggered: {
                    map.zoomLevel = map.zoomLevel+1
                    map.zoomLevel = map.zoomLevel-1
                }
            }

        }


    }
}
