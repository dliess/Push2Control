import QtQuick 2.0

Item {
    id: filter
    implicitWidth: 240
    implicitHeight: 140

    property real cutoffFreq: 20
    property real resonance: 0.0
    property real zoomIn: 50

    property string type: "onePoleLowPassFilter"

    onCutoffFreqChanged: canvas.requestPaint()
    onResonanceChanged: canvas.requestPaint()
    onZoomInChanged: canvas.requestPaint()
    onTypeChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true

        function log10Axes() {
            var theFunction;
            var args = [];
            for(var i = 0; i < arguments.length; ++i) {
                if(0 == i) {
                    theFunction = arguments[i];
                }
                else {
                    args.push(Math.pow(10, arguments[i] / filter.zoomIn));
                }
            }
            Math.log10 = Math.log10 || function(x) { // on some systems Math.log10() is not available
              return Math.log(x) * Math.LOG10E;
            };
            return Math.log10(theFunction.apply(this, args)) * filter.zoomIn; // expand array as function argument list
        }

        function onePoleLowPassFilter(freq, cutoff) {
            var A = 1.0;
            return A / Math.sqrt(1 + ( (freq * freq) / (cutoff * cutoff) ));
        }

        function onePoleHighPassFilter(freq, cutoff) {
            var A = freq / cutoff;
            return A / Math.sqrt(1 + ( (freq * freq) / (cutoff * cutoff) ));
        }

        function twoPoleLowPassFilter(freq, cutoff) {
            var X = freq / cutoff;
            var A = 1.0;
            var damping =  (100.0 - filter.resonance) / 100.0;
            return A / Math.sqrt(  (1 - X * X) * (1 - X * X) + 4 * X * X * damping * damping);
        }

        function twoPoleHighPassFilter(freq, cutoff) {
            var X = freq / cutoff;
            var A = X * X;
            var damping =  (100.0 - filter.resonance) / 100.0;
            return A / Math.sqrt(  (1 - X * X) * (1 - X * X) + 4 * X * X * damping * damping);
        }

        function twoPoleBandPassFilter(freq, cutoff) {
            var X = freq / cutoff;
            var damping =  (100.0 - filter.resonance) / 100.0;
            var A = 2 * X * damping;
            return A / Math.sqrt(  (1 - X * X) * (1 - X * X) + 4 * X * X * damping * damping);
        }


        onPaint: {
            var funct;
            switch(type){
                case "onePoleLowPassFilter":
                    funct = onePoleLowPassFilter;
                break;
                case "onePoleHighPassFilter":
                    funct = onePoleHighPassFilter;
                break;
                case "twoPoleLowPassFilter":
                    funct = twoPoleLowPassFilter;
                break;
                case "twoPoleHighPassFilter":
                    funct = twoPoleHighPassFilter;
                break;
                case "twoPoleBandPassFilter":
                    funct = twoPoleBandPassFilter;
                break;
            }
            var yOffset = 50;


            var ctx = getContext("2d")
            ctx.setTransform(1, 0, 0, -1, 0, canvas.height);

            // Clear Canvas
            ctx.clearRect(0, 0, canvas.width, canvas.height);


            // Draw coordinate system
            ctx.lineWidth = 1
            ctx.strokeStyle = "blue"
            ctx.beginPath()
            ctx.moveTo(5, 1);
            ctx.lineTo(5, canvas.height - 1);
            ctx.moveTo(1, yOffset);
            ctx.lineTo(canvas.width - 1, yOffset);
            ctx.stroke()


            // Draw Filter Curve
            ctx.lineWidth = 3
            ctx.beginPath()
            for (var x = 0; x < canvas.width; ++x){
                ctx.lineTo(x, log10Axes( funct, x, filter.cutoffFreq) + yOffset );
            }

            // Draw Cutoff Sign
            ctx.lineWidth = 3
            ctx.strokeStyle = "red"
            ctx.moveTo(filter.cutoffFreq, yOffset - 4);
            ctx.lineTo(filter.cutoffFreq, yOffset + 4);

            ctx.stroke()
            ctx.resetTransform();
        }
    }

}

