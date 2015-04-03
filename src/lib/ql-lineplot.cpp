#include "ql-lineplot.hpp"
#include <QDebug>
#include <math.h>
 
LinePlotItem::LinePlotItem( QQuickItem* parent ) : QQuickPaintedItem( parent ) , m_CustomPlot( nullptr ) {
	setFlag( QQuickItem::ItemHasContents, true );
	// setRenderTarget(QQuickPaintedItem::FramebufferObject);
	// setAcceptHoverEvents(true);
	setAcceptedMouseButtons( Qt::AllButtons );
	setAntialiasing(false);

	connect( this, &QQuickPaintedItem::widthChanged, this, &LinePlotItem::updateCustomPlotSize );
	connect( this, &QQuickPaintedItem::heightChanged, this, &LinePlotItem::updateCustomPlotSize );
	
	m_CustomPlot = new QCustomPlot();
}
 
LinePlotItem::~LinePlotItem(){
	delete m_CustomPlot;
	m_CustomPlot = nullptr;
}

void LinePlotItem::initPlot(const QString &xLabel, const QString &yLabel, bool showLegend, bool date, const QString &dateFormat){
	updateCustomPlotSize();
	connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &LinePlotItem::onCustomReplot );
	m_CustomPlot->xAxis->setLabel(xLabel);
	m_CustomPlot->yAxis->setLabel(yLabel);
	m_CustomPlot->legend->setVisible(showLegend);
	if (date){
		m_CustomPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
		m_CustomPlot->xAxis->setDateTimeFormat(dateFormat);
	}

	m_CustomPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
	connect( m_CustomPlot, SIGNAL( plottableClick( QCPAbstractPlottable*, QMouseEvent* ) ), this, SLOT( graphClicked( QCPAbstractPlottable* ) ) );

	replot();
}

void LinePlotItem::replot(){ m_CustomPlot->replot(); }

void LinePlotItem::clearGraphs(){
	m_CustomPlot->clearGraphs();
	replot();
}

void LinePlotItem::clearData(){
	for (int i=0; i<m_CustomPlot->graphCount(); i++)
		m_CustomPlot->graph(i)->clearData();
	replot();
}

void LinePlotItem::addGraph(const QString &name, const QString &color, const double width, const QString &style){
	m_CustomPlot->addGraph();
	int i = m_CustomPlot->graphCount() - 1;
	m_CustomPlot->graph(i)->setPen( QPen( QColor( color ), width,
		(style == "dash") ? Qt::DashLine :
		(style == "dot") ? Qt::DotLine :
		(style == "dashdot") ? Qt::DashDotLine :
		(style == "dashdotdot") ? Qt::DashDotDotLine : Qt::SolidLine
	));
	m_CustomPlot->graph(i)->setName( name );
	m_CustomPlot->graph(i)->setVisible( false );
}

void LinePlotItem::showGraph(const int g, const bool on){
	if (g < m_CustomPlot->graphCount()) m_CustomPlot->graph( g )->setVisible( on );
}
void LinePlotItem::showGraphs(const bool on){ for (int i=0; i<m_CustomPlot->graphCount(); i++) showGraph(i,on); }

void LinePlotItem::addPoint(const int g, const double x, const double y){
	if (g < m_CustomPlot->graphCount())
		m_CustomPlot->graph( g )->addData( x,y );
}
void LinePlotItem::addPoints(const qreal x, const QList<qreal> &y){
	if ((m_CustomPlot->graphCount() > 0) && (y.length() > 0))
		for (int i=0; i<m_CustomPlot->graphCount(); i++)
			addPoint( i, x, (i < y.length()) ? y[i] : std::nan("") );
}
void LinePlotItem::removePoint(const int g){
	if (g < m_CustomPlot->graphCount())
		if (m_CustomPlot->graph(g)->data()->size() > 0)
			m_CustomPlot->graph(g)->removeData(m_CustomPlot->graph(g)->data()->firstKey());
}
void LinePlotItem::removePoints(){ for (int i=0; i<m_CustomPlot->graphCount(); i++) removePoint(i); }

void LinePlotItem::setRangeX(const double min, const double max){ m_CustomPlot->xAxis->setRange( min, max ); }
void LinePlotItem::setRangeY(const double min, const double max){ m_CustomPlot->yAxis->setRange( min, max ); }
void LinePlotItem::setRanges(const double minX,const double maxX, const double minY,const double maxY){
	setRangeX(minX,maxX);
	setRangeY(minY,maxY);
}

void LinePlotItem::paint( QPainter* painter ){
	if (m_CustomPlot){
		QPixmap    picture( boundingRect().size().toSize() );
		QCPPainter qcpPainter( &picture );
		//m_CustomPlot->replot();
		m_CustomPlot->toPainter( &qcpPainter );
		painter->drawPixmap( QPoint(), picture );
	}
}

void LinePlotItem::mousePressEvent( QMouseEvent* event ){
//	qDebug() << Q_FUNC_INFO;
	routeMouseEvents( event );
}

void LinePlotItem::mouseReleaseEvent( QMouseEvent* event ){
//	qDebug() << Q_FUNC_INFO;
	routeMouseEvents( event );
}

void LinePlotItem::mouseMoveEvent( QMouseEvent* event ){
	routeMouseEvents( event );
}

void LinePlotItem::mouseDoubleClickEvent( QMouseEvent* event ){
//	qDebug() << Q_FUNC_INFO;
	routeMouseEvents( event );   
}

void LinePlotItem::graphClicked( QCPAbstractPlottable* plottable ){
//	qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
}

void LinePlotItem::routeMouseEvents( QMouseEvent* event ){
	if (m_CustomPlot){
		QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
		//QCoreApplication::sendEvent( m_CustomPlot, newEvent );
		QCoreApplication::postEvent( m_CustomPlot, newEvent );
	}
}

void LinePlotItem::updateCustomPlotSize(){
	if (m_CustomPlot){
		m_CustomPlot->setGeometry( 0, 0, width(), height() );
	}
}

void LinePlotItem::onCustomReplot(){
//	qDebug() << Q_FUNC_INFO;
	update();
}

