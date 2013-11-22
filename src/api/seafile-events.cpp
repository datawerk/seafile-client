#include "seafile-events.h"
#include "utils/utils.h"
namespace {

QString getStringFromJson(const json_t *json, const char* key)
{
    return QString::fromUtf8(json_string_value(json_object_get(json, key)));
}

}


SeafileEvent SeafileEvents::eventFromJSON(const json_t *json, json_error_t *error)
{
    SeafileEvent event;
    event.event_type = getStringFromJson(json, "etype");
    event.repo_id = getStringFromJson(json, "repo_id");
    event.repo_name = getStringFromJson(json, "repo_name");
    event.author = getStringFromJson(json, "author");
    event.nick_name = getStringFromJson(json, "nick");
    event.description = getStringFromJson(json, "desc");

    event.mtime = json_integer_value(json_object_get(json, "time"));

    return event;
}

SeafileEvents SeafileEvents::fromJSON(const json_t *json, json_error_t *error)
{
    SeafileEvents events;
    std::vector<SeafileEvent> events_list;
    json_t *events_array = json_object_get(json, "events");
    SeafileEvent event;
    for (size_t i = 0; i < json_array_size(events_array); ++i) {
        event = eventFromJSON(json_array_get(events_array, i), error);
        events_list.push_back(event);
    }
    events.events_list = events_list;
    events.more = json_is_true(json_object_get(json, "more"));
    events.more_offset = json_integer_value(json_object_get(json, "more_offset"));
    return events;
}

QString SeafileEvent::descrip()
{
    if (event_type == "repo-create") {
        description = QString("Created") + " \"" + repo_name + "\"";
    }

    if (event_type == "repo-delete") {
        description = QString("Deleted") + " \"" + repo_name + "\"";
    }

    return description;
}
