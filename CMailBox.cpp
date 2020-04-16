#ifndef __PROGTEST__
#include <iostream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "OtherClasses.cpp"
#endif /* __PROGTEST */

using namespace std;

//=================================================================================================

class CMail
{
public:
    // constructor
    CMail ( const CTimeStamp & timeStamp,
            const string & from,
            const CMailBody  & body,
            const CAttach    * attach )
            : m_TimeStamp(timeStamp), m_Sender(from), m_Body(body)
    {
        if(attach) {
            m_Attach = attach;
            m_Attach->AddRef();
        } else
            m_Attach = nullptr;
    }

    // getters
    const string & From () const
    {
        return m_Sender;
    }
    const CMailBody  & Body () const
    {
        return m_Body;
    }
    const CTimeStamp & TimeStamp () const
    {
        return m_TimeStamp;
    }
    const CAttach* Attachment () const
    {
        return m_Attach;
    }

    // < > == compare mails by time
    bool operator < (const CMail &mail) const
    {
        return m_TimeStamp.Compare(mail.m_TimeStamp) < 0;
    }
    bool operator > (const CMail &mail) const
    {
        return m_TimeStamp.Compare(mail.m_TimeStamp) > 0;
    }
    bool operator == (const CMail &mail) const
    {
        return m_TimeStamp.Compare(mail.m_TimeStamp) == 0;
    }

    // output operator
    friend ostream & operator << (ostream & os, const CMail & x )
    {
        os << x.m_TimeStamp << ' ' << x.m_Sender << ' ' << x.m_Body;
        if (x.m_Attach)
            os << " + " << *x.m_Attach;
        return os;
    }
private:
    CTimeStamp m_TimeStamp;     // keeps time
    string m_Sender;            // keeps sender's name
    CMailBody m_Body;           // body of the mail
    const CAttach *m_Attach;    // pointer to attach to the mail
};

//=================================================================================================

/**
 * folder keeps mails sorted by time
 */
class CFolder
{
public:
    string m_Name;          // name
    vector<CMail> m_Mails;  // vector of mails

    // default constuctor
    CFolder()
    {
        m_Name = "";
    }

    // constructor creates folder with a name
    explicit CFolder(const string & name)
    {
        m_Name = name;
    }

    // copies content of folder dir into the current folder
    CFolder &operator = (const CFolder & dir)
    {
        if (this == &dir)
            return *this;

        m_Name = dir.m_Name;
        m_Mails.erase(m_Mails.begin(), m_Mails.end());

        for (const auto& m_Mail : dir.m_Mails)
        {
            m_Mails.push_back(m_Mail);
        }

        return *this;
    }

    // adds mail into the wright position (by time)
    void AddMessage(const CMail & mail)
    {
        m_Mails.insert(lower_bound(m_Mails.begin(), m_Mails.end(), mail), mail);
    }
};

//=================================================================================================

class CMailBox
{
public:
    // default constructor
    CMailBox ()
    {
        CFolder temp("inbox");
        m_Folders.insert(pair<string, CFolder>("inbox", temp));
    }

    // saves mail in "inbox" folder
    bool Delivery ( const CMail & mail )
    {
        m_Folders["inbox"].AddMessage(mail);

        return true;
    }

    // creates folder with name folderName
    bool NewFolder ( const string & folderName )
    {
        if (m_Folders[folderName].m_Name == folderName)
            return false;

        CFolder temp(folderName);
        m_Folders[folderName] = temp;

        return true;
    }

    // moves all mails from the directory fromFolder to the toFolder
    bool MoveMail ( const string & fromFolder, const string & toFolder )
    {
        CFolder *from, *to;
        try {
            from = &m_Folders.at(fromFolder);
            to = &m_Folders.at(toFolder);
        }
        catch (out_of_range &e)
        {
            return false;
        }

        vector<CMail>::iterator iter;
        for (iter = from->m_Mails.begin(); iter != from->m_Mails.end(); iter++)
        {
            to->AddMessage(*iter);
        }
        from->m_Mails.erase(from->m_Mails.begin(), from->m_Mails.end());

        return true;
    }

    /**
     * method returns all mails from folder folderName between from and to
     * @param folderName : listed folder
     * @param from : lower bound of time
     * @param to : upper bound of time
     * @return : list of mails
     */
    list<CMail> ListMail (const string & folderName, const CTimeStamp & from, const CTimeStamp & to ) const
    {
        list<CMail> result;
        CFolder * currentFolder;
        try {
            currentFolder = const_cast<CFolder *>(&m_Folders.at(folderName));
        }
        catch (out_of_range &e)
        {
            return result;
        }

        CMail firstMail(from, "", CMailBody(0, ""), nullptr);
        CMail lastMail(to, "", CMailBody(0, ""), nullptr);

        vector<CMail>::iterator iter;
        for (iter = lower_bound(currentFolder->m_Mails.begin(), currentFolder->m_Mails.end(), firstMail);
        iter != upper_bound(currentFolder->m_Mails.begin(), currentFolder->m_Mails.end(), lastMail); iter++)
        {
            result.push_back(*iter);
        }

        return result;
    }

    /**
     * method returns names of users, that sent mails to the mailbox between from and to
     * @param from : lower bound of time
     * @param to : upper bound of time
     * @return : list of users' names
     */
    set<string> ListAddr (const CTimeStamp & from, const CTimeStamp & to ) const
    {
        set<string> result;

        for (auto &i: m_Folders)
        {
            CMail firstMail(from, "", CMailBody(0, ""), nullptr);
            CMail lastMail(to, "", CMailBody(0, ""), nullptr);

            for (auto iter = lower_bound(i.second.m_Mails.begin(), i.second.m_Mails.end(), firstMail);
                 iter != upper_bound(i.second.m_Mails.begin(), i.second.m_Mails.end(), lastMail); iter++)
            {
                result.insert(iter->From());
            }
        }

        return result;
    }

private:
    map<string, CFolder> m_Folders; // map keeps folder's with mails
};

//=================================================================================================

