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
	Q_PROPERTY(int stop READ getStopBits WRITE setStopBits)
	Q_PROPERTY(int data READ getDataBits WRITE setDataBits)
	Q_PROPERTY(int parity READ getParity WRITE setParity)
	Q_PROPERTY(bool echo READ getEcho WRITE setEcho)
	Q_PROPERTY(bool postpone READ getPostpone WRITE setPostpone)
	Q_PROPERTY(QString line READ getLineEnd WRITE setLineEnd)
	Q_PROPERTY(bool caps READ getCaps WRITE setCaps)
	
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
	void setDataBits(int value) { _dataBits = (QSerialPort::DataBits) value; }
	
	QSerialPort::StopBits getStopBits() { return _stopBits; }
	void setStopBits(QSerialPort::StopBits value) { _stopBits = value; }
	void setStopBits(int value) { _stopBits = (QSerialPort::StopBits) value; }
	
	QSerialPort::Parity getParity() { return _parity; }
	void setParity(QSerialPort::Parity value) { _parity = value; }
	void setParity(int value) { _parity = (QSerialPort::Parity) value; }
	
	QString getLineEnd() { return _lineEnd; }
	void setLineEnd(QString value) { _lineEnd = value; }
	
	bool getCaps() { return _caps; }
	void setCaps(bool value) { _caps = value; }
	
private:
	quint32 _id;
	bool _echo;
	bool _postpone;
	bool _caps;
	QString _name;
	int _baudRate;
	QSerialPort::DataBits _dataBits;
	QSerialPort::StopBits _stopBits;
	QSerialPort::Parity _parity;
	QString _lineEnd;

};

#endif // DEVICE_H
