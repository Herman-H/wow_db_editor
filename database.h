#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include "config.h"

#include "creaturetemplatequery.h"

class database
{
public:
    struct status
    {
        bool connection_error;
        bool unknown_error;
        bool didnttry;
        bool connected;
    };

private:
    QSqlDatabase world;
    QSqlDatabase realm;
    QSqlDatabase chars;
    status world_status;
    status realm_status;
    status chars_status;
public:
    CreatureTemplateQuery creature_template_query;
public:
    database(config &cfg) :
        world(QSqlDatabase::addDatabase(config_dbms_name(cfg.get_dbms()), cfg.get_world_db())),
        realm(QSqlDatabase::addDatabase(config_dbms_name(cfg.get_dbms()), cfg.get_realm_db())),
        chars(QSqlDatabase::addDatabase(config_dbms_name(cfg.get_dbms()), cfg.get_chars_db()))
    {
        reconfigure(cfg);

        world_status = { false,false,true,false };
        realm_status = { false,false,true,false };
        chars_status = { false,false,true,false };

        try_open_world();
        try_open_realm();
        try_open_chars();
    }
    ~database()
    {
        if(world_status.connected)
            world.close();
        if(realm_status.connected)
            realm.close();
        if(chars_status.connected)
            chars.close();
    }

    QSqlDatabase& World(){ return world; }

    void reconfigure(config &cfg)
    {
        world.setHostName(cfg.get_host().toString());
        world.setDatabaseName(cfg.get_world_db());
        world.setPort(cfg.get_port());
        world.setUserName(cfg.get_user());
        world.setPassword(cfg.get_pass());

        realm.setHostName(cfg.get_host().toString());
        realm.setDatabaseName(cfg.get_realm_db());
        realm.setPort(cfg.get_port());
        realm.setUserName(cfg.get_user());
        realm.setPassword(cfg.get_pass());

        chars.setHostName(cfg.get_host().toString());
        chars.setDatabaseName(cfg.get_chars_db());
        chars.setPort(cfg.get_port());
        chars.setUserName(cfg.get_user());
        chars.setPassword(cfg.get_pass());
    }

    status& try_open_world()
    {
        if(world_status.connected)
        {
            world.close();
            world_status.connected = false;
        }
        world_status.didnttry = false;
        if(!(world_status.connected = world.open()))
        {
            if(world.lastError().type() == QSqlError::ConnectionError)
            {
                world_status.connection_error = true;
            }
            else
            {
                world_status.unknown_error = true;
            }
        }
        else
        {
            world_status.connection_error = false;
            world_status.unknown_error = false;
        }
        return world_status;
    }

    status& try_open_realm()
    {
        if(realm_status.connected)
        {
            realm.close();
            realm_status.connected = false;
        }
        realm_status.didnttry = false;
        if(!(realm_status.connected = realm.open()))
        {
            if(realm.lastError().type() == QSqlError::ConnectionError)
            {
                realm_status.connection_error = true;
            }
            else
            {
                realm_status.unknown_error = true;
            }
        }
        else
        {
            realm_status.connection_error = false;
            realm_status.unknown_error = false;
        }
        return realm_status;
    }

    status& try_open_chars()
    {
        if(chars_status.connected)
        {
            chars.close();
            chars_status.connected = false;
        }
        chars_status.didnttry = false;
        if(!(chars_status.connected = chars.open()))
        {
            if(chars.lastError().type() == QSqlError::ConnectionError)
            {
                chars_status.connection_error = true;
            }
            else
            {
                chars_status.unknown_error = true;
            }
        }
        else
        {
            chars_status.connection_error = false;
            chars_status.unknown_error = false;
        }
        return chars_status;
    }

    QSqlQuery queryCreatureTemplate()
    {
        return world.exec(creature_template_query.make_query());
    }
};

#endif // DATABASE_H
