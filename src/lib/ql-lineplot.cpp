#include "ql-lineplot.hpp"
#include <QDebug>
#include <math.h>
#include <QtGui>

QlLinePlotItem::QlLinePlotItem( QQuickItem* parent ) : QQuickPaintedItem( parent ) , m_CustomPlot( nullptr ) {
	setFlag( QQuickItem::ItemHasContents, true );
	// setRenderTarget(QQuickPaintedItem::FramebufferObject);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons( Qt::AllButtons );
	setAntialiasing(false);

	connect( this, &QQuickPaintedItem::widthChanged, this, &QlLinePlotItem::updateCustomPlotSize );
	connect( this, &QQuickPaintedItem::heightChanged, this, &QlLinePlotItem::updateCustomPlotSize );

	m_CustomPlot = new QCustomPlot();
	
	m_imageData = new QList<QVariant>;
}
 
QlLinePlotItem::~QlLinePlotItem(){
	delete m_CustomPlot;
	m_CustomPlot = nullptr;
}

void QlLinePlotItem::initPlot(){
	updateCustomPlotSize();
	connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &QlLinePlotItem::onCustomReplot );

	autoRange_ = false;
	autoRangeCur_ = true;

	connect( m_CustomPlot, SIGNAL( plottableClick( QCPAbstractPlottable*, QMouseEvent* ) ), this, SLOT( graphClicked( QCPAbstractPlottable* ) ) );
}

void QlLinePlotItem::replot(){ m_CustomPlot->replot(); }
void QlLinePlotItem::rescale(){ m_CustomPlot->rescaleAxes(); replot(); }

void QlLinePlotItem::showLegend(const bool on){ m_CustomPlot->legend->setVisible(on); }

bool QlLinePlotItem::autoRange(){ return autoRangeCur_; }
void QlLinePlotItem::setAutoRange(const bool on){ autoRange_ = on; }


void QlLinePlotItem::setLabelX(const QString &name){m_CustomPlot->xAxis->setLabel(name);}
void QlLinePlotItem::setLabelY(const QString &name){m_CustomPlot->yAxis->setLabel(name);}


void QlLinePlotItem::setDateFormat(const bool enable, const QString &format){
	if (enable){
		m_CustomPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
		m_CustomPlot->xAxis->setDateTimeFormat(format);
	} else m_CustomPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
}


void QlLinePlotItem::clearGraphs(){ m_CustomPlot->clearGraphs(); }

void QlLinePlotItem::clearData(){
	for (int i=0; i < m_CustomPlot->graphCount(); i++)
		m_CustomPlot->graph(i)->clearData();
}


void QlLinePlotItem::addGraph(const QString &name, const bool visible, const QString &color, const double width, const QString &style){
	m_CustomPlot->addGraph();
	int i = m_CustomPlot->graphCount() - 1;

	setGraphName(i, name );
	setGraphColor(i,color);
	setGraphWidth(i,width);
	setGraphStyle(i,style);
	setVisible(i, visible );
}


void QlLinePlotItem::setGraphName(const int &num, const QString &name){
	m_CustomPlot->graph(num)->setName( name );
}
void QlLinePlotItem::setGraphColor(const int &num, const QString &color){
	QPen pen = m_CustomPlot->graph(num)->pen();
	m_CustomPlot->graph(num)->setPen( *(new QPen( QColor( color ), pen.widthF(), pen.style() )));
}
void QlLinePlotItem::setGraphWidth(const int &num, const double width){
	QPen pen = m_CustomPlot->graph(num)->pen();
	m_CustomPlot->graph(num)->setPen( *(new QPen( pen.color(), width, pen.style() )));
}
void QlLinePlotItem::setGraphStyle(const int &num, const QString &style){
	QPen pen = m_CustomPlot->graph(num)->pen();
	m_CustomPlot->graph(num)->setPen( *(new QPen( pen.color(), pen.widthF(),
		(style == "dash")       ? Qt::DashLine :
		(style == "dot")        ? Qt::DotLine :
		(style == "dashdot")    ? Qt::DashDotLine :
		(style == "dashdotdot") ? Qt::DashDotDotLine : Qt::SolidLine
	)));
}


bool QlLinePlotItem::visible(const int g){ return ((g >= 0) && (g < m_CustomPlot->graphCount()) && m_CustomPlot->graph( g )->visible()); }
void QlLinePlotItem::setVisible(const int g, const bool on){
	if ((g >= 0)&&(g < m_CustomPlot->graphCount())) m_CustomPlot->graph( g )->setVisible( on );
	else if (g == -1) for (int i=0; i<m_CustomPlot->graphCount(); i++) setVisible(i,on);
}


void QlLinePlotItem::addPoint(const int g, const double x, const double y){
	if (g < m_CustomPlot->graphCount())
		m_CustomPlot->graph( g )->addData( x,y );
}
void QlLinePlotItem::addPoints(const qreal x, const QList<qreal> &y){
	if ((m_CustomPlot->graphCount() > 0) && (y.length() > 0))
		for (int i=0; i<m_CustomPlot->graphCount(); i++)
			addPoint( i, x, (i < y.length()) ? y[i] : qQNaN() );
	if (autoRange_ && autoRangeCur_) rescale();
}
void QlLinePlotItem::removePoint(const int g){
	if (g < m_CustomPlot->graphCount())
		if (m_CustomPlot->graph(g)->data()->size() > 0)
			m_CustomPlot->graph(g)->removeData(m_CustomPlot->graph(g)->data()->firstKey());
}
void QlLinePlotItem::removePoints(){ for (int i=0; i<m_CustomPlot->graphCount(); i++) removePoint(i); }


void QlLinePlotItem::setRangeX(const double min, const double max){ m_CustomPlot->xAxis->setRange( min, max ); }
void QlLinePlotItem::setRangeY(const double min, const double max){ m_CustomPlot->yAxis->setRange( min, max ); }
void QlLinePlotItem::setRanges(const double minX,const double maxX, const double minY,const double maxY){
	setRangeX(minX,maxX);
	setRangeY(minY,maxY);
}


void QlLinePlotItem::setTickX(const bool tickShow, const bool tickAuto, const double tickStep, const bool subAuto, const int subCount){
	m_CustomPlot->xAxis->setTicks( tickShow );
	m_CustomPlot->xAxis->setAutoTicks( tickAuto );
	m_CustomPlot->xAxis->setTickStep( tickStep );
	m_CustomPlot->xAxis->setAutoSubTicks( subAuto );
	m_CustomPlot->xAxis->setSubTickCount( subCount );
}
void QlLinePlotItem::setTickY(const bool tickShow, const bool tickAuto, const double tickStep, const bool subAuto, const int subCount){
	m_CustomPlot->yAxis->setTicks( tickShow );
	m_CustomPlot->yAxis->setAutoTicks( tickAuto );
	m_CustomPlot->yAxis->setTickStep( tickStep );
	m_CustomPlot->yAxis->setAutoSubTicks( subAuto );
	m_CustomPlot->yAxis->setSubTickCount( subCount );
}


void QlLinePlotItem::setDrag(const bool dragX, const bool dragY){
	m_CustomPlot->setInteraction(QCP::iRangeDrag, dragX || dragY);
	if (dragX && !dragY) m_CustomPlot->axisRect()->setRangeDrag( Qt::Horizontal );
	else if (!dragX && dragY) m_CustomPlot->axisRect()->setRangeDrag( Qt::Vertical );
	else if (dragX && !dragY) m_CustomPlot->axisRect()->setRangeDrag( Qt::Horizontal | Qt::Vertical );
}
void QlLinePlotItem::setZoom(const bool zoomX, const bool zoomY){
	m_CustomPlot->setInteraction(QCP::iRangeZoom, zoomX || zoomY);
	if (zoomX && !zoomY) m_CustomPlot->axisRect()->setRangeZoom( Qt::Horizontal );
	else if (!zoomX && zoomY) m_CustomPlot->axisRect()->setRangeZoom( Qt::Vertical );
	else if (zoomX && !zoomY) m_CustomPlot->axisRect()->setRangeZoom( Qt::Horizontal | Qt::Vertical );
}


QList<QVariant> QlLinePlotItem::toPng(){
	QByteArray arr;
	QBuffer buf(&arr);
	QPixmap picture( boundingRect().size().toSize() );
	QCPPainter qcpPainter( &picture );
	m_CustomPlot->render(&qcpPainter, QPoint());
	picture.save(&buf, "PNG");
	m_imageData->clear();
	for (int i=0; i<arr.size(); i++) m_imageData->append(QVariant(arr[i] & 0xFF));
	arr.clear();
	return *m_imageData;
}


void QlLinePlotItem::paint( QPainter* painter ){
	QPixmap    picture( boundingRect().size().toSize() );
	QCPPainter qcpPainter( &picture );
	m_CustomPlot->toPainter( &qcpPainter );
	painter->drawPixmap( QPoint(), picture );
}

void QlLinePlotItem::mousePressEvent( QMouseEvent* event ){
	emit clickHandler(m_CustomPlot->xAxis->pixelToCoord(event->pos().x()), m_CustomPlot->yAxis->pixelToCoord(event->pos().y()));
	routeMouseEvents( event );
}
void QlLinePlotItem::mouseReleaseEvent( QMouseEvent* event ){ routeMouseEvents( event ); }
void QlLinePlotItem::mouseMoveEvent( QMouseEvent* event ){
	autoRangeCur_ = false;
	routeMouseEvents( event );
}
void QlLinePlotItem::mouseDoubleClickEvent( QMouseEvent* event ){
	autoRangeCur_ = true;
	m_CustomPlot->rescaleAxes();
	m_CustomPlot->replot();
}
void QlLinePlotItem::graphClicked( QCPAbstractPlottable* plottable ){
//	qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
}

void QlLinePlotItem::routeMouseEvents( QMouseEvent* event ){
	QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
	QCoreApplication::postEvent( m_CustomPlot, newEvent );
}

void QlLinePlotItem::wheelEvent(QWheelEvent* event){
	autoRangeCur_ = false;
	Qt::Orientations zoom = m_CustomPlot->axisRect()->rangeZoom();
	if (QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) m_CustomPlot->axisRect()->setRangeZoom( zoom & ~Qt::Vertical );
	else if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier) m_CustomPlot->axisRect()->setRangeZoom( zoom & ~Qt::Horizontal );
	QCoreApplication::sendEvent( m_CustomPlot, event );
	m_CustomPlot->axisRect()->setRangeZoom( zoom );
}

void QlLinePlotItem::hoverEnterEvent(QHoverEvent * event){}
void QlLinePlotItem::hoverLeaveEvent(QHoverEvent * event){}
void QlLinePlotItem::hoverMoveEvent(QHoverEvent * event){
//	m_CustomPlot->setToolTip(QString("%1 ,%2").arg(m_CustomPlot->xAxis->pixelToCoord(event->pos().x())).arg(m_CustomPlot->yAxis->pixelToCoord(event->pos().y())));
	emit hoverHandler(m_CustomPlot->xAxis->pixelToCoord(event->pos().x()), m_CustomPlot->yAxis->pixelToCoord(event->pos().y()));
}


void QlLinePlotItem::updateCustomPlotSize(){ m_CustomPlot->setGeometry( 0, 0, width(), height() ); }
void QlLinePlotItem::onCustomReplot(){ update(); }

