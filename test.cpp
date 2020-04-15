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
                   CTimeStamp                              ( int               year,
                                                             int               month,
                                                             int               day,
                                                             int               hour,
                                                             int               minute,
                                                             int               sec );
    int            Compare                                 ( const CTimeStamp & x ) const;
    friend ostream & operator <<                           ( ostream          & os,
                                                             const CTimeStamp & x );
};
//=================================================================================================
class CMailBody
{
  public:
                   CMailBody                               ( int               size,
                                                             const char      * data );
     // copy cons/op=/destructor is correctly implemented in the testing environment
    friend ostream & operator <<                           ( ostream         & os,
                                                             const CMailBody & x )
    {
      return os << "mail body: " << x . m_Size << " B";
    }
  private:
    int            m_Size;
    char         * m_Data;
};
//=================================================================================================
class CAttach
{
  public:
                   CAttach                                 ( int               x )
      : m_X (x),
        m_RefCnt ( 1 )
    {
    }
    void           AddRef                                  ( void ) const 
    { 
      m_RefCnt ++; 
    }
    void           Release                                 ( void ) const 
    { 
      if ( !--m_RefCnt ) 
        delete this; 
    }
  private:
    int            m_X;
    mutable int    m_RefCnt;
                   CAttach                                 ( const CAttach   & x );
    CAttach      & operator =                              ( const CAttach   & x );
                  ~CAttach                                 ( void ) = default;
    friend ostream & operator <<                           ( ostream         & os,
                                                             const CAttach   & x )
    {
      return os << "attachment: " << x . m_X << " B";
    }
};
//=================================================================================================
#endif /* __PROGTEST__, DO NOT remove */


class CMail
{
  public:
                   CMail                                   ( const CTimeStamp & timeStamp,
                                                             const string     & from,
                                                             const CMailBody  & body,
                                                             const CAttach    * attach );
    const string     & From                                ( void ) const;
    const CMailBody  & Body                                ( void ) const;
    const CTimeStamp & TimeStamp                           ( void ) const;
    const CAttach* Attachment                              ( void ) const;

    friend ostream & operator <<                           ( ostream          & os,
                                                             const CMail      & x );
  private:
    // todo
};
//=================================================================================================
class CMailBox
{
  public:
                   CMailBox                                ( void );
    bool           Delivery                                ( const CMail      & mail );
    bool           NewFolder                               ( const string     & folderName );
    bool           MoveMail                                ( const string     & fromFolder,
                                                             const string     & toFolder );
    list<CMail>    ListMail                                ( const string     & folderName,
                                                             const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;
    set<string>    ListAddr                                ( const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;
  private:
    // todo
};
//=================================================================================================
#ifndef __PROGTEST__
static string showMail ( const list<CMail> & l )
{
  ostringstream oss;
  for ( const auto & x : l )
    oss << x << endl;
  return oss . str ();
}
static string showUsers ( const set<string> & s )
{
  ostringstream oss;
  for ( const auto & x : s )
    oss << x << endl;
  return oss . str ();
}
int main ( void )
{
  list<CMail> mailList;
  set<string> users;
  CAttach   * att;

  CMailBox m0;
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), nullptr ) ) );
  att = new CAttach ( 200 );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), att ) ) );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> Release ();
  att = new CAttach ( 97 );
  assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), att ) ) );
  att -> Release ();
  assert ( showMail ( m0 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n" );
  assert ( showMail ( m0 . ListMail ( "inbox",
                      CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                      CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n" );
  assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                       CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );
  assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                       CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "boss1@fit.cvut.cz\n"
                        "user2@fit.cvut.cz\n" );

  CMailBox m1;
  assert ( m1 . NewFolder ( "work" ) );
  assert ( m1 . NewFolder ( "spam" ) );
  assert ( !m1 . NewFolder ( "spam" ) );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
  att = new CAttach ( 500 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), att ) ) );
  att -> Release ();
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), nullptr ) ) );
  att = new CAttach ( 468 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
  att -> Release ();
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), nullptr ) ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( m1 . MoveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 19, 24, 13 ), "user2@fit.cvut.cz", CMailBody ( 14, "mail content 4" ), nullptr ) ) );
  att = new CAttach ( 234 );
  assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 13, 26, 23 ), "user3@fit.cvut.cz", CMailBody ( 9, "complains" ), att ) ) );
  att -> Release ();
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
  assert ( m1 . MoveMail ( "inbox", "work" ) );
  assert ( showMail ( m1 . ListMail ( "inbox",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
  assert ( showMail ( m1 . ListMail ( "work",
                      CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                      CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                        "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                        "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                        "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                        "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
                        "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );

  return 0;
}
#endif /* __PROGTEST__ */
