#ifndef QL_CUSTOMPLOT
	#define QL_CUSTOMPLOT


#include <QtQuick>
#include "qcustomplot.hpp"

class QlLinePlotItem : public QQuickPaintedItem {
	Q_OBJECT

	public:
		QlLinePlotItem( QQuickItem* parent = 0 );
		virtual ~QlLinePlotItem();

		// initialize plot with X,Y labels, show/hide legend
		Q_INVOKABLE void initPlot();

		// replot
		Q_INVOKABLE void replot();
		// rescale axes
		Q_INVOKABLE void rescale();

		// show/hide legend
		Q_INVOKABLE void showLegend(const bool on);

		// get current autorange flag
		Q_INVOKABLE bool autoRange();
		Q_INVOKABLE void setAutoRange(const bool on);

		// set label for X axis
		Q_INVOKABLE void setLabelX(const QString &name);
		// set label for Y axis
		Q_INVOKABLE void setLabelY(const QString &name);

		// set date format for X axis
		Q_INVOKABLE void setDateFormat(const bool enable, const QString &format=QString(""));

		// remove all graphs
		Q_INVOKABLE void clearGraphs();
		// add graph with name, color, width, style (solid/dash/dot/dashdot/dashdotdot)
		Q_INVOKABLE void addGraph(const QString &name, const bool visible, const QString &color, const double width, const QString &style);

		// set graph name
		Q_INVOKABLE void setGraphName(const int &num, const QString &name);
		// set graph color
		Q_INVOKABLE void setGraphColor(const int &num, const QString &color);
		// set graph width
		Q_INVOKABLE void setGraphWidth(const int &num, const double width);
		// set graph style
		Q_INVOKABLE void setGraphStyle(const int &num, const QString &style);

		// check if graph is visible
		Q_INVOKABLE bool visible(const int g);
		// show/hide graph
		Q_INVOKABLE void setVisible(const int g, const bool on);
		
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

		Q_INVOKABLE void setTickX(const bool tickShow, const bool tickAuto, const double tickStep, const bool subAuto, const int subCount);
		Q_INVOKABLE void setTickY(const bool tickShow, const bool tickAuto, const double tickStep, const bool subAuto, const int subCount);

		Q_INVOKABLE void setDrag(const bool dragX, const bool dragY);
		Q_INVOKABLE void setZoom(const bool zoomX, const bool zoomY);

		Q_INVOKABLE QList<QVariant> toPng();
		
		void paint( QPainter* painter );


	signals:
		void hoverHandler(const double x, const double y);
		void clickHandler(const double x, const double y);

	protected:
		void routeMouseEvents( QMouseEvent* event );

		virtual void mousePressEvent( QMouseEvent* event );
		virtual void mouseReleaseEvent( QMouseEvent* event );
		virtual void mouseMoveEvent( QMouseEvent* event );
		virtual void mouseDoubleClickEvent( QMouseEvent* event );
		
		virtual void wheelEvent(QWheelEvent* event);
		
		virtual void hoverEnterEvent(QHoverEvent * event);
		virtual void hoverLeaveEvent(QHoverEvent * event);
		virtual void hoverMoveEvent(QHoverEvent * event);

	private:
		QCustomPlot* m_CustomPlot;
		bool autoRange_, autoRangeCur_;
		QList<QVariant> *m_imageData;

	private slots:
		void graphClicked( QCPAbstractPlottable* plottable );
		void onCustomReplot();   
		void updateCustomPlotSize();
};

#endif

