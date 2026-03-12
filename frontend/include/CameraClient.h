#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonObject>

class CameraClient : public QObject
{
  Q_OBJECT

public:
  explicit CameraClient(QObject* parent = nullptr);

  bool connectToEndpoint(const QString& endpoint); // tcp:HOST:PORT
  void disconnect();

  bool isConnected() const;

  void requestStatus();

  void captureStream(int exposureUs,
                     int gain,
                     const QString& outdir,
                     const QString& prefix,
                     const QString& format,   /* tiff */
                     bool hasLensMetadata = false,
                     int focusRaw = -1,
                     int apertureRaw = -1,
                     double fNumber = -1.0);

signals:
  void connectedChanged(bool connected);
  void statusMessage(const QString& msg);
  void captureSaved(const QString& path);
  void previewSaved(const QString& path);

private slots:
  void onReadyRead();
  void onErrorOccurred(QAbstractSocket::SocketError);
  void onDisconnected();

private:
  void sendJsonLine(const QJsonObject& obj);

  // stream receive state
  bool mExpectingPayload = false;
  qint64 mExpectedBytes = 0;
  QByteArray mPayload;
  int mW = 0;
  int mH = 0;
  QString mPendingSavePath;
  QString mPendingFormat; // keep "tiff" for now

  // pending metadata for TIFF/preview save
  int mPendingExposureUs = 0;
  int mPendingGain = 0;

  bool mPendingHasLensMetadata = false;
  int mPendingFocusRaw = -1;
  int mPendingApertureRaw = -1;
  double mPendingFNumber = -1.0;

  QByteArray mRxBuf;

  QTcpSocket* mSock = nullptr;
};