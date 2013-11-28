#ifndef SEAFILEEVENTS_H
#define SEAFILEEVENTS_H

#include <vector>
#include <QString>
#include <QMetaType>
#include <jansson.h>

class SeafileEvent {

public:
    SeafileEvent():event_type("non_type"),
        repo_id("undefined"),
        repo_name("undefined"),
        author("undefined"),
        nick_name("undefined"),
        description("undefined"),
        mtime(0),
        is_last(false){}
public:
    QString event_type;
    QString repo_id;
    QString repo_name;
    QString author;
    QString nick_name;
    QString description;
    qint64 mtime;
    bool is_last;

    QString descrip();

};

class SeafileEvents
{
public:
    SeafileEvents(){}
public:
    std::vector<SeafileEvent> events_list;
    bool more;
    int more_offset;

    static SeafileEvent eventFromJSON(const json_t *json, json_error_t *error);
    static SeafileEvents fromJSON(const json_t *json, json_error_t *error);
};

/**
 * Register with QMetaType so we can wrap it with QVariant::fromValue
 */
Q_DECLARE_METATYPE(SeafileEvent)


#endif // SEAFILEEVENTS_H
