#ifndef QL_CUSTOMPLOT
	#define QL_CUSTOMPLOT


#include <QtQuick>
#include "qcustomplot.hpp"

class LinePlotItem : public QQuickPaintedItem {
	Q_OBJECT

	public:
		LinePlotItem( QQuickItem* parent = 0 );
		virtual ~LinePlotItem();

		// initialize plot with X,Y labels, show/hide legend
		Q_INVOKABLE void initPlot(const QString &xLabel, const QString &yLabel, bool showLegend, bool date, const QString &dateFormat);

		// replot
		Q_INVOKABLE void replot();

		// remove all graphs
		Q_INVOKABLE void clearGraphs();
		// add graph with name, color, width, style (solid/dash/dot/dashdot/dashdotdot)
		Q_INVOKABLE void addGraph(const QString &name, const QString &color, const double width, const QString &style);

		// show/hide graph
		Q_INVOKABLE void showGraph(const int g, const bool on);
		// show/hide all graphs
		Q_INVOKABLE void showGraphs(const bool on);
		
		// clear all data
		Q_INVOKABLE void clearData();
		// add 1 point to single graph
		Q_INVOKABLE void addPoint(const int g, const double x, const double y);
		// add 1 point to all graphs
		Q_INVOKABLE void addPoints(const qreal x, const QList<qreal> &y);

		// remove single point from beginning of graph
		Q_INVOKABLE void removePoint(const int g);
		// remove point from beginning of all graphs
		Q_INVOKABLE void removePoints();

		Q_INVOKABLE void setRangeX(const double min, const double max);
		Q_INVOKABLE void setRangeY(const double min, const double max);
		Q_INVOKABLE void setRanges(const double minX,const double maxX, const double minY,const double maxY);

		void paint( QPainter* painter );

	protected:
		void routeMouseEvents( QMouseEvent* event );

		virtual void mousePressEvent( QMouseEvent* event );
		virtual void mouseReleaseEvent( QMouseEvent* event );
		virtual void mouseMoveEvent( QMouseEvent* event );
		virtual void mouseDoubleClickEvent( QMouseEvent* event );

		void setupQuadraticDemo( QCustomPlot* customPlot );

	private:
		QCustomPlot* m_CustomPlot;

	private slots:
		void graphClicked( QCPAbstractPlottable* plottable );
		void onCustomReplot();   
		void updateCustomPlotSize();
};

#endif

