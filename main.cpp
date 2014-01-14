#include <QtCore/QCoreApplication>
#include "Cache.h"

int main(int argc, char *argv[])
{
    // Compose File Name for Input
    QString inFileName = argv[1];
    inFileName = inFileName.append(".trace");
    Cache myCache(inFileName);

    // Parse Command Line Argument
    if(argc < 2)
        cout << "ERROR: Too many arguments." << endl;
    else
    {
        for(int a = 1024; a <= 131072; a = a*2)
        {
            if(a == 2048)
                a = a*2;
            if(a == 8192)
                a = a*8;
            for(int b = 8; b <= 128; b= b*2)
            {
                myCache.Direct(a,b);
                myCache.TwoWay(a,b);
                myCache.FourWay(a,b);
                myCache.FullyA(a,b);
                if (b==32)
                    b=b*2;
            }
        }
    }

    // Compose File for Output
    QString outFile = argv[1];
    outFile = outFile.append(".result");
    myCache.toTextFile(outFile);

    return 0;
}
