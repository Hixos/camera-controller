#!/bin/python3

from string import Template
import datetime
import re
import sys
import os

includes = []
events = []
topics = []

re_topic = re.compile(r"\s*(?P<topic>[A-Z_0-9]+)\s*\n")
re_include = re.compile(r"\s*(?P<include>\#include [\"\<][\w\d/\.]+[\"\>]\n)")

re_event_name_vars = re.compile(r"\s*((?P<name>\w+)\s*{)")
re_event_name_no_vars = re.compile(r"\s*((?P<name>\w+)\s*\n)")

re_vars = re.compile(r"\s*((?P<type>[\w:<>]+)[ \t]+(?P<name>\w+))+\s*")
re_end_vars = re.compile(r"}\s*")

json_case_template = Template(
    "case $event_name::id:\n"
    + "    return make_shared<$event_name>(\n"
    + "           j.get<$event_name>());\n"
    + "    break;\n"
)

kotlin_type_map = {
    "int8_t": "Byte",
    "int16_t": "Short",
    "int32_t": "Int",
    "int64_t": "Long",
    "uint8_t": "UByte",
    "uint16_t": "UShort",
    "uint32_t": "UInt",
    "uint64_t": "ULong",
    "float": "Float",
    "double": "Double",
    "bool": "Boolean",
    "string": "String",
    "vector<int8_t>": "ArrayList<Byte>",
    "vector<int16_t>": "ArrayList<Short>",
    "vector<int32_t>": "ArrayList<Int>",
    "vector<int64_t>": "ArrayList<Long>",
    "vector<uint8_t>": "ArrayList<UByte>",
    "vector<uint16_t>": "ArrayList<UShort>",
    "vector<uint32_t>": "ArrayList<UInt>",
    "vector<uint64_t>": "ArrayList<ULong>",
    "vector<float>": "ArrayList<Float>",
    "vector<double>": "ArrayList<Double>",
    "vector<bool>": "ArrayList<Boolean>",
    "vector<string>": "ArrayList<String>"
}

kotlin_var_template = Template(
    '@SerializedName("$var_name" ) var $kot_var_name : $kot_var_type? = null'
)


kotlin_event_switch_templ = Template(
    "        $id_val -> return gson.fromJson(json, $event_name::class.java)\n"
)


def kotlinVarName(varname: str):
    s = varname.lower().split("_")
    kot = ""
    for i, k in enumerate(s):
        if i == 0:
            kot += k
        else:
            kot += k.capitalize()
    return kot


with open("./events.txt", "r") as event_list:
    state = "parse_topics"
    file_str = event_list.read()

    while len(file_str) > 0:
        match state:
            case "parse_topics":
                m = re_topic.match(file_str)
                if m:
                    topics.append(m.group("topic"))
                    file_str = file_str[m.end() :]
                else:
                    state = "parse_include"

            case "parse_include":
                m = re_include.match(file_str)
                if m:
                    includes.append(m.group("include").strip().rstrip())
                    file_str = file_str[m.end() :]
                else:
                    state = "parse_event_name"
            case "parse_event_name":
                m_evt = re_event_name_vars.match(file_str)
                if m_evt:
                    file_str = file_str[m_evt.end() :]
                    name = m_evt.group("name")
                    events.append({"name": name, "vars": []})
                    state = "parse_vars"
                else:
                    m_evt = re_event_name_no_vars.match(file_str)
                    if m_evt:
                        file_str = file_str[m_evt.end() :]
                        name = m_evt.group("name")
                        events.append({"name": name, "vars": []})
                    else:
                        sys.exit(
                            "Error parsing event: expected event name. @\n{}...".format(
                                file_str[:100]
                            )
                        )
            case "parse_vars":
                m = re_vars.match(file_str)
                if m:
                    file_str = file_str[m.end() :]
                    events[-1]["vars"].append(
                        {"name": m.group("name"), "type": m.group("type")}
                    )
                else:
                    m = re_end_vars.match(file_str)
                    if m:
                        file_str = file_str[m.end() :]
                        state = "parse_event_name"
                    else:
                        sys.exit(
                            'Error parsing event vars: expected "}}" @\n{}...'.format(
                                file_str[:100]
                            )
                        )


with open("./templates/event_dec.template", "r") as file:
    event_dec_templ = Template(file.read())

with open("./templates/event_def.template", "r") as file:
    event_def_templ = Template(file.read())

with open("./templates/event_def.kt.template", "r") as file:
    kotlin_event_template = Template(file.read())

starting_id = 10
dt = datetime.datetime.now()

file_templ_d = {
    "includes": "",
    "event_dec": "",
    "event_def": "",
    "json_to_event_cases": "",
    "kotlin_event_def": "",
    "kotlin_event_switch": "",
    "year": dt.year,
    "gen_date": str(dt),
}

for i, ev in enumerate(events):
    event_templ_d = {}
    event_templ_d["event_name"] = ev["name"]
    event_templ_d["id_val"] = starting_id + i

    if ev["vars"]:
        event_templ_d["members"] = "\n".join(
            [var["type"] + " " + var["name"] + ";" for var in ev["vars"]]
        )
        event_templ_d["c_params"] = ", ".join(
            [var["type"] + " " + var["name"] for var in ev["vars"]]
        )
        event_templ_d["c_params_init"] = ", " + ", ".join(
            ["{name}({name})".format(name=var["name"]) for var in ev["vars"]]
        )
        event_templ_d["to_string_format"] = ", ".join(
            [var["name"] + " = {}" for var in ev["vars"]]
        )
        event_templ_d["var_list"] = ", " + ", ".join(
            [var["name"] for var in ev["vars"]]
        )
        event_templ_d["def_constructor"] = "\n{}() : Event(id) {{}};".format(ev["name"])
        event_templ_d["no_macro_args"] = ""
        event_templ_d["kotlin_vars"] = "\n" + "\n".join(
            [
                "    "
                + kotlin_var_template.substitute(
                    var_name=var["name"],
                    kot_var_name=kotlinVarName(var["name"]),
                    kot_var_type=kotlin_type_map[var["type"]],
                )
                for var in ev["vars"]
            ] 
        )
    else:
        event_templ_d["kotlin_vars"] = ""
        event_templ_d["members"] = ""
        event_templ_d["c_params"] = ""
        event_templ_d["c_params_init"] = ""
        event_templ_d["var_list"] = ""
        event_templ_d["def_constructor"] = ""
        event_templ_d["no_macro_args"] = "_NOARGS"

    file_templ_d["event_dec"] += event_dec_templ.substitute(**event_templ_d)
    file_templ_d["event_def"] += event_def_templ.substitute(**event_templ_d)
    file_templ_d["kotlin_event_def"] += kotlin_event_template.substitute(
        **event_templ_d
    )
    file_templ_d["json_to_event_cases"] += json_case_template.substitute(
        **event_templ_d
    )
    file_templ_d["kotlin_event_switch"] += kotlin_event_switch_templ.substitute(**event_templ_d)


file_templ_d["includes"] = "\n".join(includes)
file_templ_d["topic_enum"] = ",\n".join(topics)
file_templ_d["topic_string_map"] = ",\n".join(
    ['{{{t},"{t}"}}'.format(t=t) for t in topics]
)
file_templ_d["topic_id_map"] = ",\n".join(['{{"{t}",{t}}}'.format(t=t) for t in topics])

with open("./templates/Events.h.template", "r") as file:
    events_h_templ = Template(file.read())

with open("./templates/Events.cpp.template", "r") as file:
    events_cpp_templ = Template(file.read())

with open("Events.h", "w") as file:
    file.write(events_h_templ.substitute(**file_templ_d))

with open("Events.cpp", "w") as file:
    file.write(events_cpp_templ.substitute(**file_templ_d))

os.system("clang-format -i --style=file Events.h")
os.system("clang-format -i --style=file Events.cpp")


# Kotlin
with open("./templates/Events.kt.template", "r") as file:
    events_kot_templ = Template(file.read())

with open("./kotlin/Events.kt", "w") as file:
    file.write(events_kot_templ.substitute(**file_templ_d))
