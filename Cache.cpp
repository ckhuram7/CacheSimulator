#include "Cache.h"

Cache::Cache(QString fileName)
{
    // Open File
    QFile instrFile(fileName);
    instrFile.open(QIODevice::ReadOnly);

    // Read File & Insert Instructions to List
    QTextStream inFile(&instrFile);
    while(!inFile.atEnd())
    {
        QString inputLine = inFile.readLine();
        instructionsIn.append(inputLine);
    }
    instrFile.close();
}

Cache::~Cache(){}

void Cache::Direct(int cacheSize, int blockSize)
{
    // Properties
    int blockCount = cacheSize/blockSize;
    int cache[blockCount];
    bool dirtyCheck[blockCount];
    float hit = 0;
    int MtoCByteTotal = 0;
    int CtoMByteTotal = 0;
    int cacheTag[blockCount];

    for (int x=0; x <= blockCount;x++)
        {
            dirtyCheck[x] = 0;
        }

    // Reset Cache
    for(int i = 0; i < blockCount; i++)
            cache[i] = 0;

    //for each instruction
    for(int i = 0; i < instructionsIn.length(); i++)
    {
        bool ok;
        QString textmemAddress = instructionsIn[i].split(" ")[1];
        int memAddress = textmemAddress.toInt(&ok, 16);
        int blockIndex = (memAddress/blockSize)%blockCount;
        int blockOffset = memAddress%blockSize;
        int tempTag = memAddress/cacheSize;


            // Hit
            if(tempTag == cacheTag[blockIndex])
                hit++;
            // Miss
            else
            {
                if (dirtyCheck[blockIndex] == true)
                    CtoMByteTotal += blockSize;
                cacheTag[blockIndex] = tempTag;
                MtoCByteTotal += blockSize;
            }
            if(instructionsIn[i].contains("write"))
                dirtyCheck[blockIndex] = true;
   }
    //write to output
   float hitRatio = hit/instructionsIn.length();
   QString output;
   QString cache_size = QString::number(cacheSize);
   QString block_size = QString::number(blockSize);
   QString hit_ratio = QString::number(hitRatio, 'g', 2);
   QString MtoC = QString::number(MtoCByteTotal);
   QString CtoM = QString::number(CtoMByteTotal);
   output.append(cache_size).append("   ").append(block_size).append("   DM   ")
           .append(hit_ratio).append("   ").append(MtoC).append("   ")
           .append(CtoM).append("   1");
   m_Output.append(output);
}

void Cache::TwoWay(int cacheSize, int blockSize)
{
    // Properties
    int blockCount = cacheSize/blockSize;
    int totalWays = 2;
    int totalSets = blockCount/totalWays;
    int cache[totalSets][totalWays+1];
    float hit = 0;
    int MtoCByteTotal = 0;
    int CtoMByteTotal = 0;

    // Reset Cache
    for(int i = 0; i < totalSets; i++)
    {
        cache[i][0] = i;
        for(int j = 0; j <= totalWays; j++)
        {
            cache[i][1] = -1;
            cache[i][2] = -1;
        }
    }
    //for each instruction
    for(int i = 0; i < instructionsIn.length(); i++)
    {
        bool ok;
        QString textmemAddress = instructionsIn[i].split(" ")[1];
        int memAddress = textmemAddress.toInt(&ok, 16);
        int set = (memAddress/blockSize)%totalSets;
        int tag = memAddress/blockSize/totalSets;
        int setter = 0;

        if(instructionsIn[i].contains("read"))
        {
            for(int i = 1; i <= totalWays; i++)
            {
                if(cache[set][i] == tag)
                {
                    hit++;
                    setter = 1;
                }
            }
            if(setter != 1)// checking if least recently used
            {
                MtoCByteTotal += blockSize;
                cache[set][1] = tag;
            }
        }

        if(instructionsIn[i].contains("write"))
        {
            for(int i = 1; i <= totalWays; i++)
            {
                if(cache[set][i] == tag)
                {
                    hit++;
                    setter = 1;
                    CtoMByteTotal += blockSize;
                }
            }
            if(setter != 1)// checking if least recently used
            {
                cache[set][2] = tag;
                MtoCByteTotal += blockSize;
                CtoMByteTotal += blockSize;
            }
        }
    }
    //output stats
    float hitRatio = hit/instructionsIn.length();
    QString output;
    QString cache_size = QString::number(cacheSize);
    QString block_size = QString::number(blockSize);
    QString hit_ratio = QString::number(hitRatio, 'g', 2);
    QString MtoC = QString::number(MtoCByteTotal);
    QString CtoM = QString::number(CtoMByteTotal);
    output.append(cache_size).append("   ").append(block_size).append("   2W   ")
            .append(hit_ratio).append("   ").append(MtoC).append("   ")
            .append(CtoM).append("   2");
    m_Output.append(output);
}

void Cache::FourWay(int cacheSize, int blockSize)
{
    // Properties
    int blockCount = cacheSize/blockSize;
    int totalWays = 4;
    int totalSets = blockCount/totalWays;
    int cache[totalSets][totalWays+1];
    float hit = 0;
    int MtoCByteTotal = 0;
    int CtoMByteTotal = 0;

    // Reset Cache_Memory for New Cache Specs
    for(int i = 0; i < totalSets; i++)
    {
        cache[i][0] = i;
        for(int j = 0; j <= totalWays; j++)
        {
            cache[i][4] = -1;
            cache[i][3] = -1;
            cache[i][2] = -1;
            cache[i][1] = -1;
        }
    }
    //for each instruction
    for(int i = 0; i < instructionsIn.length(); i++)
    {
        bool ok;
        QString textmemAddress = instructionsIn[i].split(" ")[1];
        int memAddress = textmemAddress.toInt(&ok, 16);
        int set = (memAddress/blockSize)%totalSets;
        int tag = memAddress/blockSize/totalSets;

        if(instructionsIn[i].contains("write"))
        {
            int setter = 0;
            for(int i = 1; i <= totalWays; i++)
            {
                if(cache[set][i] == tag)
                {
                    hit++;
                    setter = 1;
                    CtoMByteTotal += blockSize;
                }
            }
            if(setter != 1)//checking if least recently used
            {
                cache[set][4] = tag;
                MtoCByteTotal += blockSize;
                CtoMByteTotal += blockSize;
            }
        }
        if(instructionsIn[i].contains("read"))
        {
            int setter = 0;
            for(int i = 1; i <= totalWays; i++)
            {
                if(cache[set][i] == tag)
                {
                    hit++;
                    setter = 1;
                }
            }
            if(setter != 1)//checking if least recently used
            {
                MtoCByteTotal += blockSize;
                cache[set][3] = tag;
            }
        }        
    }
    //output stats
    float hitRatio = hit/instructionsIn.length();
    QString output;
    QString cache_size = QString::number(cacheSize);
    QString block_size = QString::number(blockSize);
    QString hit_ratio = QString::number(hitRatio, 'g', 2);
    QString MtoC = QString::number(MtoCByteTotal);
    QString CtoM = QString::number(CtoMByteTotal);
    output.append(cache_size).append("   ").append(block_size).append("   4W   ")
            .append(hit_ratio).append("   ").append(MtoC).append("   ")
            .append(CtoM).append("   4");
    m_Output.append(output);
}

void Cache::FullyA(int cacheSize, int blockSize)
{
    // Properties
    int blockCount = cacheSize/blockSize;
    int cache[blockCount];
    bool dirty[blockCount];

    // Reset Cache
    for(int i = 0; i < blockCount; i++)
        cache[i] = 0;

    float hit = 0;
    int MtoCByteTotal = 0;
    int CtoMByteTotal = 0;

    for(int i = 0; i < instructionsIn.length(); i++)
    {
        QString textmemAddress = instructionsIn[i].split(" ")[1];

        bool ok;
        int memAddress = textmemAddress.toInt(&ok, 16);
        int tag = memAddress/blockSize;

        if(instructionsIn[i].contains("read"))
        {
            int setter = 0;
            for(int i = 0; i < blockCount; i++)
            {
                if(cache[i] == tag)
                {
                    hit++;
                    setter = 1;
                }
            }
            if(setter != 1)//checking if least recently used
            {
                MtoCByteTotal += blockSize;
                for(int i = 0; i < blockCount; i++)
                {
                    if(cache[i] == 0)
                    {
                        cache[i] = tag;
                        break;
                    }
                }
            }
        }
        if(instructionsIn[i].contains("write"))
        {
            int setter = 0;
            for(int i = 0; i < blockCount; i++)
            {
                if(cache[i] == tag)
                {
                    hit++;
                    setter = 1;
                    if(dirty[blockCount] == true)
                        CtoMByteTotal += blockSize;
                }
            }
            if(setter != 1)//checking if least recently used
            {
                MtoCByteTotal += blockSize;
                if(dirty[blockCount] == true)
                    CtoMByteTotal += blockSize;
                for(int i = 0; i < blockCount; i++)
                {
                    if(cache[i] == 0)
                    {
                        cache[i] = tag;
                        break;
                    }
                    dirty[blockCount] = true;
                }

            }
        }
    }
    float hitRatio = hit/instructionsIn.length();

    QString output;
    QString cache_size = QString::number(cacheSize);
    QString block_size = QString::number(blockSize);
    QString hit_ratio = QString::number(hitRatio, 'g', 2);
    QString MtoC = QString::number(MtoCByteTotal);
    QString CtoM = QString::number(CtoMByteTotal);
    QString comparisons = QString::number(blockCount);
    output.append(cache_size).append("   ").append(block_size).append("   FA   ")
            .append(hit_ratio).append("   ").append(MtoC).append("   ")
            .append(CtoM).append("   ").append(comparisons);
    m_Output.append(output);
}

void Cache::toTextFile(QString fileName)
{
    // Open New File
    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

    // Read File & Insert Instructions to List
    QTextStream out(&outputFile);
    while(!m_Output.isEmpty())
        out << m_Output.takeFirst() << endl;
}
