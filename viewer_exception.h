#ifndef VIEWER_EXCEPTION_H
#define VIEWER_EXCEPTION_H

#include <iostream>
#include <QString>

enum ViewerExceptionCodes
{
    PROCESS_OK = 0
    , INPUT_FILE_ERROR
    , HYDROGRAPH_ERROR
    , PNG_ERROR
};

class viewer_exception : public std::exception
{
public:
    viewer_exception(int errorCode, QString evidence = "");
    virtual ~viewer_exception() throw(){};

    void init(int errorCode, QString evidence);
    QString getErrorMsgString();

    static QString getCodeAsString(int errorCode);

private:
    int m_ErrorCode;
    QString m_qsEvidence;
};

#endif // VIEWER_EXCEPTION_H
