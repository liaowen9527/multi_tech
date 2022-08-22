;(function (NetBrain) {
    'use strict';

    var GoNodeImage = function () {
        go.Node.call(this);
    };
    NetBrain.Map.GoNodeImage = GoNodeImage;

    go.Diagram.inherit(GoNodeImage, go.Node);
    var $ = go.GraphObject.make;

    GoNodeImage.getMapTemplate = function () {
        // L3居中对齐
        var iconPanel = GoNodeImage.getIconPanel()
        var namePanel = GoNodeImage.getNamePanel();

        namePanel.alignment = go.Spot.Center;

        return $(GoNodeImage, 'Vertical',
            {
                locationObjectName: 'IMAGE',
                locationSpot: go.Spot.Center,
                selectionObjectName: 'PICTURE', // "picturePanel",
                shadowOffset: new go.Point(1, 2),
                shadowBlur: 20,
                shadowColor: '#2999E3',
                copyable: false
            },
            new go.Binding('isShadowed').makeTwoWay(),
            new go.Binding('selectable', 'opacity', function(opacity) { return (opacity >= 1); }).ofObject(), // make node unselectable if it is not in current editing dataview
            new go.Binding('visible').makeTwoWay(),
            iconPanel,
            namePanel
        );
    };

    GoNodeImage.getIconPanel = function () {
        return $(
            go.Picture, 
            {width: 50, height: 50},
            new go.Binding("source", "icon")
        )
    }

    GoNodeImage.getNamePanel = function () {
        return $(
            go.TextBlock, 
            { stroke: "gray" },
            new go.Binding("text", "name")
        )
    }
    
})(NetBrain);