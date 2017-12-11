#ifndef COMMON_H
#define COMMON_H
#define IS_2D_ARRAY(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P))
#define IS_SCALAR(P)    (IS_2D_ARRAY(P) && mxGetNumberOfElements(P) == 1)
#include <QtNetwork/QTcpSocket>
#include "mex.h"
#include "mat.h"
#include <QtCore/QDataStream>

int waitForResponse(QTcpSocket *socket);
//int waitForResponse2(QTcpSocket *socket, QByteArray& response);
int getPid();
QTcpSocket *connect();
mxArray *createAnyWaveStruct();
void writeToHost(QTcpSocket *socket, int pid, const QByteArray& data);

#endif

