#ifndef DEVICE_H
#define DEVICE_H

#include "IModel.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class Device : public IModel
{
	Q_OBJECT
	
	Q_PROPERTY(quint32 id READ getId WRITE setId)
	Q_PROPERTY(QString name READ getName WRITE setName)
	Q_PROPERTY(int baud READ getBaudRate WRITE setBaudRate)
	Q_PROPERTY(QSerialPort::StopBits stop READ getStopBits WRITE setStopBits)
	Q_PROPERTY(QSerialPort::DataBits data READ getDataBits WRITE setDataBits)
	Q_PROPERTY(QSerialPort::Parity parity READ getParity WRITE setParity)
	Q_PROPERTY(bool echo READ getEcho WRITE setEcho)
	Q_PROPERTY(bool postpone READ getPostpone WRITE setPostpone)
	Q_PROPERTY(QString line READ getLineEnd WRITE setLineEnd)
	
public:
	Device();
	
	quint32 getId() { return _id; }
	void setId(qint32 value) { _id = value; }
	
	bool getEcho() { return _echo; }
	void setEcho(bool value) { _echo = value; }
	
	bool getPostpone() { return _postpone; }
	void setPostpone(bool value) { _postpone = value; }
	
	QString getName() { return _name; }
	void setName(QString value) { _name = value; }
	
	int getBaudRate() { return _baudRate; }
	void setBaudRate(int value) { _baudRate = value; }
	
	QSerialPort::DataBits getDataBits() { return _dataBits; }
	void setDataBits(QSerialPort::DataBits value) { _dataBits = value; }
	
	QSerialPort::StopBits getStopBits() { return _stopBits; }
	void setStopBits(QSerialPort::StopBits value) { _stopBits = value; }
	
	QSerialPort::Parity getParity() { return _parity; }
	void setParity(QSerialPort::Parity value) { _parity = value; }
	
	QString getLineEnd() { return _lineEnd; }
	void setLineEnd(QString value) { _lineEnd = value; }
	
private:
	quint32 _id;
	bool _echo;
	bool _postpone;
	QString _name;
	int _baudRate;
	QSerialPort::DataBits _dataBits;
	QSerialPort::StopBits _stopBits;
	QSerialPort::Parity _parity;
	QString _lineEnd;

};

#endif // DEVICE_H
