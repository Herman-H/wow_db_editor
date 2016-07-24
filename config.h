#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>
#include <QString>
#include <QTextStream>
#include <QFile>

enum class config_dbms
{
    MYSQL
};

static QString config_dbms_name(config_dbms d)
{
    switch(d)
    {
    case config_dbms::MYSQL:
        return "QMYSQL";
    default:
        return "";
        break;
    }
    return "";
}

class config
{
private:
    QHostAddress    host;
    quint16         port;
    QString         user;
    QString         pass;
    QString         w_db;
    QString         r_db;
    QString         c_db;
    config_dbms     dbms;
    QString         prev_sess;
    QString         sess_dir;
    QFile           file;
    bool            file_open;
public:
    config(QString s) :
        file(s)
    {
        file_open = false;
        if(file.exists())
        {
            if((file_open = file.open(QIODevice::ReadOnly | QIODevice::Text)))
            {
                QTextStream s{&file};
                host = QHostAddress{s.readLine()};
                port = quint16(s.readLine().toInt());
                user = s.readLine();
                pass = s.readLine();
                w_db = s.readLine();
                r_db = s.readLine();
                c_db = s.readLine();
                dbms = config_dbms(s.readLine().toInt());
                prev_sess = s.readLine();
                sess_dir = s.readLine();
            }
            if(file_open)
            {
                file.close();
                file_open = false;
            }
        }
        else
        {
            host = QHostAddress{QHostAddress::LocalHost};
            port = 3306;
            user = "user";
            pass = "password";
            w_db = "mangos";
            r_db = "realmd";
            c_db = "characters";
            dbms = config_dbms::MYSQL;
            prev_sess = "";
            sess_dir = "Session/";
            save_config();
        }
    }
    void save_config()
    {
        if((file_open = file.open(QIODevice::Truncate | QIODevice::ReadWrite | QIODevice::Text)))
        {
            QTextStream s{&file};
            s << host.toString() << "\n" << QString::number(port) << "\n" << user << "\n" <<
                 pass << "\n" << w_db << "\n" << r_db << "\n" << c_db << "\n" <<
                 QString::number(static_cast<int>(dbms)) << "\n" << prev_sess << "\n" << sess_dir;
        }
        if(file_open)
        {
            file.close();
            file_open = false;
        }
    }

    void set_host(QString s){ host = QHostAddress{s}; }
    void set_port(QString s){ port = quint16(s.toInt()); }
    void set_user(QString s){ user = s; }
    void set_pass(QString s){ pass = s; }
    void set_world_db(QString s){ w_db = s; }
    void set_realm_db(QString s){ r_db = s; }
    void set_chars_db(QString s){ c_db = s; }
    void set_dbms(int i){ dbms = static_cast<config_dbms>(i); }
    void set_previous_session(QString s){ prev_sess = s; }
    void set_session_directory(QString s){ sess_dir = s; }

    QHostAddress get_host() const { return host; }
    quint16      get_port() const { return port; }
    QString      get_user() const { return user; }
    QString      get_pass() const { return pass; }
    QString      get_world_db() const { return w_db; }
    QString      get_realm_db() const { return r_db; }
    QString      get_chars_db() const { return c_db; }
    config_dbms  get_dbms() const { return dbms; }
    QString      get_previous_session() const { return prev_sess; }
    QString      get_sessions_directory() const { return sess_dir; }
};

class config_dbc
{
private:
    QString path;
    bool    file_open;
    QFile   file;
public:
    config_dbc(QString s) :
        file(s)
    {
        file_open = false;
        if(file.exists())
        {
            if((file_open = file.open(QIODevice::ReadOnly | QIODevice::Text)))
            {
                QTextStream s{&file};
                path = s.readLine();
            }
            if(file_open)
            {
                file.close();
                file_open = false;
            }
        }
        else
        {
            path = "dbc/";
            save_config();
        }
    }
    void save_config()
    {
        if((file_open = file.open(QIODevice::Truncate | QIODevice::ReadWrite | QIODevice::Text)))
        {
            QTextStream s{&file};
            s << path;
        }
        if(file_open)
        {
            file.close();
            file_open = false;
        }
    }

    void set_path(QString path){ this->path = path; }
    QString get_path(){ return path; }
};

class config_session_add_file
{
private:
    QString add;
    bool    file_open;
    QFile   file;
public:
    config_session_add_file(QString s) :
        file(s)
    {
        file_open = false;
        if(file.exists())
        {
            if((file_open = file.open(QIODevice::ReadOnly | QIODevice::Text)))
            {
                QTextStream s{&file};
                add = s.readAll();
            }
            if(file_open)
            {
                file.close();
                file_open = false;
            }
        }
        else
        {
            add = "";
            save_config();
        }
    }
    void save_config()
    {
        if((file_open = file.open(QIODevice::Truncate | QIODevice::ReadWrite | QIODevice::Text)))
        {
            QTextStream s{&file};
            s << add;
        }
        if(file_open)
        {
            file.close();
            file_open = false;
        }
    }

    void set_session_add_text(QString add){ this->add = add; }
    QString get_text(){ return add; }
};

#endif // CONFIG_H
