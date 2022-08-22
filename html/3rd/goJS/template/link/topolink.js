;(function (NetBrain) {
    'use strict';

    var GoTopoLink = function () {
        go.Link.call(this);
    };
    NetBrain.Map.GoTopoLink = GoTopoLink;

    go.Diagram.inherit(GoTopoLink, go.Link);
    var $ = go.GraphObject.make;

    GoTopoLink.getMapTemplate = function () {
        // L3居中对齐
        var linkShape = GoTopoLink.getLinkShape();
        var srcText = GoTopoLink.getSrcText();
        var destText = GoTopoLink.getDestText();

        return $(GoTopoLink,
            {
                //curve: go.Link.Bezier,
                adjusting: go.Link.Stretch,
                //routing: go.Link.AvoidsNodes,
                adjusting: go.Link.None,
                reshapable: true, relinkableFrom: true, relinkableTo: true,
                toShortLength: 20
            },
            new go.Binding("points").makeTwoWay(),
            new go.Binding("curviness"),
            linkShape,
            srcText,
            destText
        );
    };

    GoTopoLink.getLinkShape = function () {
        return $(go.Shape,  // the link shape
            { strokeWidth: 1.5 },
            new go.Binding('stroke', 'color', function(color) {
                return color ? color /* green */ : 'black';
            }),
            new go.Binding('strokeWidth', 'color', function(color) {
                return color ? 2.5 : 1.5;
            })
        )
    }

    GoTopoLink.getSrcText = function () {
        return $(go.TextBlock, "src",
            {
                textAlign: "center",
                font: "9pt helvetica, arial, sans-serif",
                margin: 4,
                segmentIndex: 0,
                segmentOffset: new go.Point(NaN, NaN)
            },
            // editing the text automatically updates the model data
            new go.Binding("text", "src").makeTwoWay()
        )
    }

    GoTopoLink.getDestText = function () {
        return $(go.TextBlock, "dest",
            {
                textAlign: "center",
                font: "9pt helvetica, arial, sans-serif",
                margin: 4,
                segmentIndex: -1,
                segmentOffset: new go.Point(NaN, NaN)
            },
            // editing the text automatically updates the model data
            new go.Binding("text", "dest").makeTwoWay()
        )
    }
    
})(NetBrain);