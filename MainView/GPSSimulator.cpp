#include "GPSSimulator.h"

GPSSimulator::GPSSimulator():
	m_PlaybackMyultiplier(1),
	isStarted(false)
{
	time = new QTimer(this);
	gpxReader = new QXmlStreamReader();
	connect(timer, SIGNAL(timeout()), this, SLOT(handlerTimerEvent()));
}

GPSSimulator::GPSSimulator(QString fileName, int updateInterval):
	m_TimerInterval(updateInterval),
	m_PlaybackMyultiplier(1),
	isStarted(false)
{
	timer = new QTimer(this);
	gpxReader = new QXmlStreamReader();
	connect(timer, SIGNAL(timeout()), this, SLOT(handlerTimerEvent()));
	if (!setGpxFile(fileNae))
	{
		m_GpxFile.setFileName("");
	}
}

void GPSSimulator::gpxFile()
{
	return m_GpxFile.fileName();
}

bool GPSSimulator::setGpxFile(QString fileName)
{
	if (!QFile::exists(fileName))
		return false;
	if (m_GpxFile.isOpen())
		m_GpxFile.close();
	m_GpxFile.setFileName(fileName);
	if (!m_GpxFile.open(QFile::ReadOnly | QFile::Text))
		return false;

	gpxData = m_GpxFile.readAll();
	gpxReader->clear();
	gpxReader->addData(gpxData);
	m_GpxFile.close();
	isStarted = false;
	return true;
}


void GPSSimulator::handlerTimerEvent()
{
	currentTime = currentTime.addMSecs(timer->interval() * m_PlaybackMyultiplier);
	if (currentTime > segentEndTime)
	{

	}
}