#ifndef CACHE_H
#define CACHE_H

#include <QtCore/QCoreApplication>
#include <fstream>
#include <string>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QStringList>
using namespace std;

class Cache
{
private:
    QStringList instructionsIn;
    QList<QString> m_Output;

public:
    Cache(QString);
    ~Cache();

    void Direct(int, int);
    void TwoWay(int, int);
    void FourWay(int, int);
    void FullyA(int, int);
    void toTextFile(QString);
};

#endif // CACHE_H
