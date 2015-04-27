# ql-lineplot
QML wrapper for QCustomPlot.

Supports auto ranging and mouse drag/zoom (with Shift - X axis, with Control - Y axis), screenshot to PNG.

Registration:

```c++
#include "lib/ql-lineplot.hpp"

qmlRegisterType<QlLinePlotItem>("QlLinePlotItem", 1,0, "QlLinePlotItem");
```


Usage:

```Javascript

import 'qml'

QlLinePlot { id:plot; Layout.fillWidth:true; Layout.fillHeight:true;

	// X/Y labels
	labelX: 't'
	labelY: 'T'
	
	// show legend
	legend: true

	// enable/disable autorange
	autoRange: false
	
	// enable/disable X/Y mouse drag
	dragX: true
	dragY: true
	
	// enable/disable X/Y mouse zoom (with Shift - only X, with Ctrl - only Y)
	zoomX: true
	zoomY: true
	
	// enable/disable time/date format for X axis
	xDate: false
	// time/date format
	xDateFormat: 'hh.mm.ss'
	
	property var colors: ['red','orange','magenta','green','navy','olive','blue','cyan','brown','black']
	property var names: ['T1','T2','T3','T4','T5','T6','T7','T8','T9','T10']

	// initialize plot
	Component.onCompleted: {
		// remove all graphs
		//clearGraphs();
		// add new graphs
		for (var i=0; i < colors.length; i++)
			addGraph(names[i], true, colors[i], 1, 'solid');
		// clear data
		//clearAll();
	}
}
```

Example:

![4graphs](https://raw.github.com/ncp1402/ql-lineplot/master/doc/pics/pic01.png)

