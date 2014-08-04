

#ifndef GPSSIMULATOR_H
#define GPSSIMULATOR_H


#include <QDomDocument>
#include <QXmlStreamReader>
#include <QDomElement>
#include <QTimer>
#include <QFile>
#include <QLineF>
#include <QTime>
#include <QPointF>

class GPSSimulator : public QObject
{
	Q_OBJECT
public:
	GPSSimulator();
	GPSSimulator(QString fileName, int updateInterval = 20);
	~GPSSimulator();

	void startSimulation();
//	void pauseSimulation();
//	void resumeSimulation();

private:
	QFile m_GpxFile;
	QByteArray gpxData;
	QXmlStreamReader* gpxReader;
	QTimer* timer;
	int m_TimerInterval;
	int m_PlaybackMultipier;
	QLineF currentSegment;
	QLineF nextSegment;
	QPointF latestPoint;

	bool isStarted;

	private slots:
	void handleTimerEvent();

	signals:
	void positionUpdateAvailable(QPointF pos, double origentation);

};

#endif // GPSSIMULATOR_H
