#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST */

using namespace std;

#ifndef __PROGTEST__
class CTimeStamp
{
public:
    CTimeStamp ( int year, int month, int day, int hour, int minute, int sec )
    {
        m_time[0] = year;
        m_time[1] = month;
        m_time[2] = day;
        m_time[3] = hour;
        m_time[4] = minute;
        m_time[5] = sec;
    }

    int Compare ( const CTimeStamp & x ) const
    {
        for (int i = 0; i < 6; i++)
        {
            if (m_time[i] < x.m_time[i])
                return -1;
            else if (m_time[i] > x.m_time[i])
                return 1;
        }
        return 0;
    }

    friend ostream & operator << ( ostream & os, const CTimeStamp & x )
    {
        os << x.m_time[0] << '-' << (x.m_time[1] >= 10 ? to_string(x.m_time[1]) : "0" + to_string(x.m_time[1])) << '-'
           << (x.m_time[2] >= 10 ? to_string(x.m_time[2]) : "0" + to_string(x.m_time[2]))
           << ' ' << (x.m_time[3] >= 10 ? to_string(x.m_time[3]) : "0" + to_string(x.m_time[3]))
           << ':' << (x.m_time[4] >= 10 ? to_string(x.m_time[4]) : "0" + to_string(x.m_time[4]))
           << ':' << (x.m_time[5] >= 10 ? x.m_time[5] : '0' + x.m_time[5]);
        return os;
    }

private:
    int m_time[6]{};
};
//=================================================================================================
class CMailBody
{
public:
    CMailBody ( int size, const char * data )
    {
        m_Size = size;
        m_Data = new char[m_Size];
        for (int i = 0; i < m_Size; i++)
        {
            m_Data[i] = data[i];
        }
    }

    CMailBody(const CMailBody &x)
    {
        m_Size = x.m_Size;
        m_Data = new char[m_Size];
        for (int i = 0; i < m_Size; i++)
        {
            m_Data[i] = x.m_Data[i];
        }
    }

    CMailBody &operator = (const CMailBody &x)
    {
        if (this == &x)
            return *this;

        delete [] m_Data;
        m_Size = x.m_Size;
        m_Data = new char[m_Size];
        for (int i = 0; i < m_Size; i++)
        {
            m_Data[i] = x.m_Data[i];
        }
        return *this;
    }

    ~CMailBody()
    {
        delete [] m_Data;
    }

    friend ostream & operator << ( ostream & os, const CMailBody & x )
    {
        return os << "mail body: " << x . m_Size << " B";
    }

private:
    int m_Size;
    char * m_Data;
};
//=================================================================================================
class CAttach
{
public:
    CAttach ( int x )
            : m_X (x),
              m_RefCnt ( 1 )
    {
    }
    void AddRef () const
    {
        m_RefCnt ++;
    }
    void Release () const
    {
        if ( !--m_RefCnt )
            delete this;
    }
private:
    int m_X;
    mutable int m_RefCnt;
    CAttach ( const CAttach & x );
    CAttach & operator = ( const CAttach   & x );
    ~CAttach () = default;
    friend ostream & operator << ( ostream & os, const CAttach & x )
    {
        return os << "attachment: " << x . m_X << " B";
    }
};
#endif /* __PROGTEST__, DO NOT remove */
