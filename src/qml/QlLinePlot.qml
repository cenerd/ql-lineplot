import QtQuick        2.3
import QlLinePlotItem 1.0

Item {
	// X/Y labels
	property string labelX:         'X'
	property string labelY:         'Y'
	
	// show/hide legend
	property bool   legend:         true
	
	// X axis time/date format
	property bool   xDate:          false
	property string xDateFormat:    'hh.mm.ss'
	
	// enable/disable X/Y mouse drag
	property bool   dragX:          false
	property bool   dragY:          false

	// enable/disable X/Y mouse zoom
	property bool   zoomX:          false
	property bool   zoomY:          false

	// arrays of X/Y data
	property var    dataXY:         []
	property var    dataLog:        []

	// previous X/Y values
	property real   xPrev:         -Infinity
	property var    yPrev:          []
	
	// calculated min/max X/Y values
	property real   xMin:           Infinity
	property real   xMax:          -Infinity
	property real   yMin:           Infinity
	property real   yMax:          -Infinity

	// mouse hover update interval (ms), and coordinates
	property int    hoverInterval:  100
	property real   hoverX:         NaN
	property real   hoverXprev:     NaN
	property real   hoverY:         NaN
	property real   hoverYprev:     NaN
	
	property int    hoverXWidth:    8 // hover coordinate width
	property int    hoverYWidth:    8 // hover coordinate width
	property int    hoverXPrecision:3 // hover coordinate precision
	property int    hoverYPrecision:3 // hover coordinate precision

	// enable/disable autoranging on appending of new data
	property bool   autoRange:      false

	QlLinePlotItem { anchors.fill:parent; }
	property var    plot: children[0]

	Component.onCompleted: {
		// initialize plot
		plot.initPlot();
		setDateFormat(xDate,xDateFormat);
		setAutoRange(autoRange);
		setLabelX(labelX);
		setLabelY(labelY);
		showLegend(legend);
		plot.setDrag(dragX,dragY);
		plot.setZoom(zoomX,zoomY);
	}

	// set hover slot
	Connections {
		target: children[0]
		onHoverHandler: { hoverX = x; hoverY = y; }
	}

	// redraw chart
	function replot(){ plot.replot(); }

	// set X axis label
	function setLabelX(name){ plot.setLabelX(labelX=name); }
	// set Y axis label
	function setLabelY(name){ plot.setLabelY(labelY=name); }
	// set date format for X axis
	function setDateFormat(enable,format){ plot.setDateFormat(enable,format); }

	// show/hide legend
	function showLegend(on){ plot.showLegend(on); }

	// rescale chart
	function rescale(){ plot.rescale(); }
	// get current auto range flag
	function isAutoRange(){ return plot.autoRange(); }
	// set auto range flag
	function setAutoRange(on){ plot.setAutoRange(on); }
	// set X axis range
	function setRangeX(xMin,xMax){ plot.setRangeX(xMin,xMax); }
	// set Y axis range
	function setRangeY(yMin,yMax){ plot.setRangeY(yMin,yMax); }
	// set X/Y axes ranges
	function setRanges(xMin,xMax, yMin,yMax){ setRangeX(xMin,xMax); setRangeY(yMin,yMax); }

	// remove all graphs
	function clearGraphs(){ plot.clearGraphs(); }
	// add new graph
	function addGraph(name,visible,color,width,style){ plot.addGraph(name,visible,color,width,style); }
	// set graph name
	function setGraphName( n,name,replot_ ){ plot.setGraphName(n,name); if (replot_) replot(); }
	// set graph color
	function setGraphColor( n,color,replot_ ){ plot.setGraphColor(n,color); if (replot_) replot(); }
	// set graph width
	function setGraphWidth( n,width,replot_ ){ plot.setGraphWidth(n,width); if (replot_) replot(); }
	// set graph style
	function setGraphStyle( n,style,replot_ ){ plot.setGraphStyle(n,style); if (replot_) replot(); }

	// check if graph is visible
	function visible(num){ return plot.visible(num); }
	// show/hide single graph
	function setVisible(num, on){ plot.setVisible(num,on); }

	// clear all data
	function clearData(){
		if (plot) plot.clearData();
		xMin = yMin = Infinity;
		xMax = yMax = -Infinity;
		xPrev = -Infinity;
		yPrev= [];
		dataXY = [];
	}
	// append single point to graphs
	function addPoints(x,y){
		plot.addPoints(x,y);
		dataXY.push({'x':x, 'y':y,
			'yMin':y.reduceRight(function(prev,cur){ return isNaN(cur) ? prev : (cur < prev) ? cur : prev; }, Infinity),
			'yMax':y.reduceRight(function(prev,cur){ return isNaN(cur) ? prev : (cur > prev) ? cur : prev; }, -Infinity),
		});
		xPrev = x; // remember previous X,Y
		yPrev = y;
	}
	function updateRangeX(){
		xMin = dataXY.length ? dataXY[0].x : Infinity;
		xMax = dataXY.length ? dataXY[dataXY.length-1].x : -Infinity;
	}
	function updateRangeY(){
		yMin = dataXY.reduceRight(function(prev,cur){ return (cur.yMin < prev) ? cur.yMin : prev; }, Infinity);
		yMax = dataXY.reduceRight(function(prev,cur){ return (cur.yMax > prev) ? cur.yMax : prev; }, -Infinity);
	}
	function updateRanges(){ updateRangeX(); updateRangeY(); }
	// remove single point from beginning of graphs
	function removePoints(){
		plot.removePoints();
		dataXY.shift();
	}
	
	// get PNG screenshot as byte array
	function toPng(){ return plot.toPng(); }
}

