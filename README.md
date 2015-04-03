# ql-lineplot
QML wrapper for QCustomPlot

Registration:

```c++
#include "lib/ql-lineplot.hpp"

qmlRegisterType<LinePlotItem>("LinePlot", 1,0, "LinePlotItem");
```


Usage:

```Javascript

import LinePlot 1.0

Item { anchors.fill:parent
	LinePlotItem { id: linePlot; anchors.fill: parent
		Component.onCompleted: {
			// plot setup
		}
	}
}

// initialization
linePlot.initPlot(xName,yName, showLegend, xDateTime, dateTimeFormat);

// add new graph. Available styles: solid/dot/dash/dashdot/dashdotdot
linePlot.addGraph(name, color, width, style);

// redraw
linePlot.replot();

// remove all graphs
linePlot.clearGraphs();

// remove all points from all graphs
linePlot.clearData();

// show/hide graph
linePlot.showGraph(graphNum, enable);

// show/hide all graphs
linePlot.showGraphs(enable);

// add single point to graph
linePlot.addPoint(graphNum, x,y);

// add single point to all graphs
linePlot.addPoints(x, [y0,y1,...,yN]);

// remove single point from beginning of graph
linePlot.removePoint(graphNum);

// remove single point from beginning of all graphs
linePlot.removePoints();

// set range for X axis
linePlot.setRangeX(min, max);

// set range for Y axis
linePlot.setRangeY(min, max);

// set ranges for X/Y axes
linePlot.setRanges(minX,maxX, minY,maxY);
```

Example:

```Javascript
import LinePlot 1.0

ApplicationWindow { id:app; visible:true

	// plot graphs parameters
	property var plot: { 'x':0, 'f':[
		{ y:0, name:'plot1', color:'red',   width:1,   style:'solid'   },
		{ y:0, name:'plot2', color:'green', width:1.5, style:'dot'     },
		{ y:0, name:'plot3', color:'blue',  width:2,   style:'dash'    },
		{ y:0, name:'plot4', color:'black', width:3,   style:'dashdot' }
	]}

	Item { anchors.fill:parent

		LinePlotItem { id: linePlot; anchors.fill: parent
			Component.onCompleted: {
				// initialize plot with parameters: Axes names: t/T, show legend: true, x axis as date/time: true, time format: 'mm.ss'
				initPlot('t','T', true, true,'mm.ss');
				// create graphs, set name,color,with,style
				for (var i=0; i < app.plot.f.length; i++)
					addGraph(app.plot.f[i].name, app.plot.f[i].color, app.plot.f[i].width, app.plot.f[i].style);
				// set ranges for X/Y axes
				setRanges(0,30, -100,100);
				// show all graphs
				showGraphs(true);
			}
		}
	}
	
	// update graphs every 500ms
	Timer { interval: 500; running: true; repeat: true
		onTriggered: {
			// shift graphs
			if (app.plot.x > 30){
				linePlot.removePoints();
				linePlot.setRangeX(app.plot.x-30, app.plot.x);
			}
			// add new points
			linePlot.addPoints( app.plot.x, [app.plot.f[0].y, app.plot.f[1].y, app.plot.f[2].y, app.plot.f[3].y] );
			// redraw
			linePlot.replot();
			// update coordinates
			app.plot.x += 1;
			for (var i=0; i < app.plot.f.length; i++)
				app.plot.f[i].y += Math.random() * 10 - 5;
		}
	}
}
```

Result:

![4graphs](https://raw.github.com/ncp1402/ql-lineplot/master/repo/doc/pics/pic01.png)

