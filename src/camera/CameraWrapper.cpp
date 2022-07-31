
#include "CameraWrapper.h"

#include <gphoto2/gphoto2.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <optional>
#include <sstream>
#include <system_error>
#include <thread>
#include <utility>

#include "PrintLogger.h"

using namespace std::this_thread;
using std::bind;
using std::max;
using std::min;
using std::optional;
using std::pair;
using std::stringstream;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

typedef system_clock Clock;

namespace gphotow
{

bool endsWith (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

CameraWrapper::CameraWrapper() : context(gp_context_new()) {}

CameraWrapper::~CameraWrapper() { disconnect(); }

void CameraWrapper::freeCamera()
{
    if (camera != nullptr)
    {
        gp_camera_exit(camera, context);
        gp_camera_free(camera);
        camera = nullptr;
    }
}

string CameraWrapper::connect()
{
    if (!connected)
    {
        int result = gp_camera_new(&camera);

        if (result != GP_OK)
        {
            throw GPhotoError(result);
        }

        result = gp_camera_init(camera, context);

        if (result == GP_OK)
        {
            serial = getSerialNumber();
            updateBulbConfig();
            // Sleep for 2 seconds to avoid errors if capturing too early
            // std::this_thread::sleep_for(seconds(2));
            connected = true;
            return serial;
        }
        else
        {
            gp_camera_free(camera);
            throw GPhotoError(result);
        }
    }

    return serial;
}

bool CameraWrapper::disconnect()
{
    if (connected)
    {
        freeCamera();
        connected = false;

        return true;
    }
    return false;
}

bool CameraWrapper::isConnected() { return connected; }

bool CameraWrapper::isResponsive()
{
    try
    {
        return connected && getSerialNumber() == serial;
    }
    catch (CameraException& e)
    {
        return false;
    }
}

string CameraWrapper::getSerialNumber()
{
    CameraWidgetText serial{*this, CONFIG_SERIAL_NUMBER};
    return serial.getValue();
}

string CameraWrapper::getManufacturer()
{
    CameraWidgetText serial{*this, CONFIG_MANUFACTURER};
    return serial.getValue();
}

string CameraWrapper::getCameraModel()
{
    CameraWidgetText serial{*this, CONFIG_MODEL};
    return serial.getValue();
}

string CameraWrapper::getDeviceVersion()
{
    CameraWidgetText serial{*this, CONFIG_VERSION};
    return serial.getValue();
}

string CameraWrapper::getCameraInfo()
{
    string info = "";
    info += getManufacturer();
    info += " " + getCameraModel();
    info += " (" + getDeviceVersion() + ")";
    info += " SN:" + getSerialNumber();
    return info;
}

CameraWrapper::ShutterSpeedConfig CameraWrapper::getShutterSpeed()
{
    CameraWidgetRadio widget{*this, CONFIG_SHUTTER_SPEED};

    vector<int32_t> choices = getShutterSpeedChoices(widget);
    unsigned int choice     = widget.getID();
    if (choice == bulb_choice)
    {
        return {.shutter_speed = bulb_shutter_speed, .bulb = true};
    }
    else
    {
        return {.shutter_speed = choices.at(choice), .bulb = false};
    }
}

void CameraWrapper::setShutterSpeed(int32_t shutter_speed)
{
    CameraWidgetRadio widget{*this, CONFIG_SHUTTER_SPEED};

    if (shutter_speed > max_shutter_speed)
    {
        widget.setValue(bulb_choice);
        bulb_shutter_speed = shutter_speed;
    }
    else
    {
        vector<int32_t> choices = getShutterSpeedChoices(widget, false);

        unsigned int id =
            CameraStringConversion::findNearest(choices, shutter_speed);
        widget.setValue(id);
    }

    widget.apply();
}

vector<int32_t> CameraWrapper::getShutterSpeedChoices(bool include_bulb)
{
    CameraWidgetRadio widget{*this, CONFIG_SHUTTER_SPEED};
    return getShutterSpeedChoices(widget, include_bulb);
}

vector<int32_t> CameraWrapper::getShutterSpeedChoices(CameraWidgetRadio& widget,
                                                      bool include_bulb)
{
    return choicesStringToInt(
        widget, &CameraStringConversion::exposureToMicroseconds, include_bulb);
}

int32_t CameraWrapper::getAperture()
{
    CameraWidgetRadio widget{*this, CONFIG_APERTURE};

    vector<int32_t> choices = getApertureChoices(widget);
    unsigned int id         = widget.getID();

    return choices.at(id);
}

void CameraWrapper::setAperture(int aperture)
{
    CameraWidgetRadio widget{*this, CONFIG_APERTURE};
    vector<int32_t> choices = getApertureChoices(widget);

    unsigned int id = CameraStringConversion::findNearest(choices, aperture);
    widget.setValue(id);
    widget.apply();
}

vector<int32_t> CameraWrapper::getApertureChoices()
{
    CameraWidgetRadio widget{*this, CONFIG_APERTURE};
    return getApertureChoices(widget);
}

vector<int32_t> CameraWrapper::getApertureChoices(CameraWidgetRadio& widget)
{
    return choicesStringToInt(widget, &CameraStringConversion::apertureToInt);
}

int32_t CameraWrapper::getISO()
{
    CameraWidgetRadio widget{*this, CONFIG_ISO};

    vector<int32_t> choices = getIsoChoices(widget);
    unsigned int id         = widget.getID();

    return choices.at(id);
}

void CameraWrapper::setISO(int32_t iso)
{
    CameraWidgetRadio widget{*this, CONFIG_ISO};
    vector<int32_t> choices = getIsoChoices(widget);

    unsigned int id = CameraStringConversion::findNearest(choices, iso);
    widget.setValue(id);
    widget.apply();
}

vector<int32_t> CameraWrapper::getIsoChoices()
{
    CameraWidgetRadio widget{*this, CONFIG_ISO};
    return getIsoChoices(widget);
}

vector<int32_t> CameraWrapper::getIsoChoices(CameraWidgetRadio& widget)
{
    return choicesStringToInt(widget, &CameraStringConversion::isoToInt);
}

int CameraWrapper::getBatteryPercent()
{
    CameraWidgetText widget{*this, CONFIG_BATTERY_LEVEL};
    return CameraStringConversion::batteryLevelToInt(widget.getValue());
}

string CameraWrapper::getExposureProgram()
{

    CameraWidgetRadio widget{*this, CONFIG_EXPOSURE_PROGRAM};
    return widget.getValue();
}

string CameraWrapper::getTriggerMode()
{

    CameraWidgetText widget{*this, CONFIG_TRIGGER_MODE};
    return widget.getValue();
}

int CameraWrapper::getFocalLength()
{

    CameraWidgetRange widget{*this, CONFIG_FOCAL_LENGHT};
    return int(widget.getValue());
}

bool CameraWrapper::getLongExpNR()
{
    CameraWidgetRadio widget{*this, CONFIG_LONG_EXP_NR};
    return widget.getValue() == "On";
}

void CameraWrapper::setLongExpNR(bool nr){
    CameraWidgetRadio widget{*this, CONFIG_LONG_EXP_NR};
    if(nr)
        widget.setValue("On");
    else
        widget.setValue("Off");
    widget.apply();
}   



// void CameraWrapper::setTriggerMode(string mode)
// {

//     CameraWidgetText widget{*this, CONFIG_TRIGGER_MODE};
//     widget.setValue(mode);
//     widget.apply();
// }

// vector<string> CameraWrapper::listTriggerModes()
// {

//     CameraWidgetText widget{*this, CONFIG_TRIGGER_MODE};
//     vector<string> choices = widget.getAvailableValues();
//     return choices;
// }

void CameraWrapper::setCaptureTarget(string capture_target)
{
    CameraWidgetRadio widget{*this, CONFIG_CAPTURE_TARGET};
    widget.setValue(capture_target);
    widget.apply();
}

string CameraWrapper::getCaptureTarget()
{
    CameraWidgetRadio widget{*this, CONFIG_CAPTURE_TARGET};
    return widget.getValue();
}

float CameraWrapper::getLightMeter()
{
    CameraWidgetRange widget{*this, CONFIG_LIGHT_METER};
    return widget.getValue();
}

CameraWidgetRange::Range CameraWrapper::getLightMeterRange()
{
    CameraWidgetRange widget{*this, CONFIG_LIGHT_METER};
    return widget.getRange();
}

string CameraWrapper::getFocusMode()
{
    CameraWidgetRadio widget{*this, CONFIG_FOCUS_MODE};
    return widget.getValue() == "Manual" ? "MF" : widget.getValue();
}

void CameraWrapper::nextFocusMode()
{
    CameraWidgetRadio widget{*this, CONFIG_FOCUS_MODE};
    size_t num_choices =  widget.getChoices().size();

    unsigned int id = (widget.getID() + 1) % num_choices;
    
    widget.setValue(id);
    widget.apply();
}

bool CameraWrapper::getAutoISO()
{
    CameraWidgetRadio widget{*this, CONFIG_AUTO_ISO};
    return widget.getValue() == "On";
}

void CameraWrapper::setAutoISO(bool auto_iso){
    CameraWidgetRadio widget{*this, CONFIG_AUTO_ISO};
    if(auto_iso)
        widget.setValue("On");
    else
        widget.setValue("Off");
    widget.apply();
}   


void CameraWrapper::bulb(int value)
{
    CameraWidgetToggle widget{*this, CONFIG_BULB};
    widget.setValue(value);
    widget.apply();
}

CameraPath CameraWrapper::capture()
{
    auto ss = getShutterSpeed();
    if (ss.bulb == true)
    {
        LOG_DEBUG(log, "Chosen bulb capture.");
        return bulbCapture(bulb_shutter_speed);
    }
    else
    {
        LOG_DEBUG(log, "Chosen camera capture.");
        return cameraCapture();
    }
}

CameraPath CameraWrapper::cameraCapture()
{
    CameraFilePath path;
    int result = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &path, context);

    if (result == GP_OK)
    {
        return CameraPath(path);
    }
    else
    {
        throw GPhotoError(result);
    }
}

CameraPath CameraWrapper::bulbCapture(int exposure_time, milliseconds timeout)
{
    auto my_log = log.getChild("bulbCapture");

    // If long exposuire NR is on camera will wait additional exposure_time to
    // caputere noise data
    if (getLongExpNR())
    {
        timeout += duration_cast<milliseconds>(microseconds(exposure_time));
        LOG_DEBUG(log,
                  "Long exposure noise reduction, timeout is increased by {} "
                  "s. Total timeout: {} s",
                  exposure_time / 1000000.0f, timeout.count() / 1000.0f);
    }

    bulb(1);
    std::this_thread::sleep_for(microseconds(exposure_time));
    bulb(0);

    CameraPath p{};

    bool file_event = false;
    bool done_event = false;
    do
    {
        auto start     = system_clock::now();
        CameraEvent ev = waitForEvent(timeout.count());

        if (ev.first == GP_EVENT_FILE_ADDED && p.name.length() == 0)
        {
            if (ev.second.has_value())
            {
                p = ev.second.value();

                if(endsWith(p.name, "JPG"))
                    file_event = true;
            }
        }

        if (ev.first == GP_EVENT_CAPTURE_COMPLETE)
        {
            done_event = true;
        }

        timeout =
            timeout - duration_cast<milliseconds>(system_clock::now() - start);
    } while (!(file_event && done_event) && timeout >= milliseconds(0));

    if (timeout < milliseconds(0))
    {
        LOG_DEBUG(my_log, "Timeout expired");
    }
    return p;
}

CameraEvent CameraWrapper::waitForEvent(int timeout)
{
    auto my_log = log.getChild("waitForEvent");

    void* eventdata = nullptr;
    CameraEventType event_type;
    gp_camera_wait_for_event(camera, timeout, &event_type, &eventdata, context);

    optional<CameraPath> opt = std::nullopt;

    switch (event_type)
    {
        case GP_EVENT_UNKNOWN:
        {
            if (eventdata == nullptr)
            {
                LOG_DEBUG(my_log, "GP_EVENT_UNKNOWN");
            }
            else
            {
                LOG_DEBUG(my_log, "GP_EVENT_UNKNOWN: {}", (char*)eventdata);
            }
            break;
        }
        case GP_EVENT_TIMEOUT:
        {
            LOG_DEBUG(my_log, "GP_EVENT_TIMEOUT ({} ms)", timeout);
            break;
        }
        case GP_EVENT_CAPTURE_COMPLETE:
        {
            LOG_INFO(my_log, "GP_EVENT_CAPTURE_COMPLETE {}",
                     eventdata == nullptr);
            break;
        }
        case GP_EVENT_FILE_ADDED:
        {
            CameraPath p{*(CameraFilePath*)eventdata};
            LOG_INFO(my_log, "GP_EVENT_FILE_ADDED: {}", p.getPath());
            opt = p;
            break;
        }
        case GP_EVENT_FILE_CHANGED:
        {
            CameraPath p{*(CameraFilePath*)eventdata};
            LOG_INFO(my_log, "GP_EVENT_FILE_CHANGED: {}", p.getPath());
            opt = p;
            break;
        }
        case GP_EVENT_FOLDER_ADDED:
        {
            CameraPath p{*(CameraFilePath*)eventdata};
            LOG_INFO(my_log, "GP_EVENT_FOLDER_ADDED: {}", p.getPath());
            opt = p;
            break;
        }
    }

    free(eventdata);

    return {event_type, opt};
}

void CameraWrapper::downloadFile(CameraFilePath path, string dest_file_path)
{
    CameraFile* file;
    int result, fd;
    bool gp_file_created = false;

    FILE* f = fopen(dest_file_path.c_str(), "w");

    if (f == NULL)
    {
        goto out;
    }

    fd = fileno(f);

    if (fd < 0)
    {
        goto out;
    }

    result = gp_file_new_from_fd(&file, fd);
    if (result != GP_OK)
    {
        goto out;
    }
    else
    {
        gp_file_created = true;
    }

    result = gp_camera_file_get(camera, path.folder, path.name,
                                GP_FILE_TYPE_NORMAL, file, context);

    if (result != GP_OK)
    {
        goto out;
    }

out:
    if (f != NULL)
    {
        fclose(f);
    }
    if (f == NULL || fd < 0)
    {
        throw std::filesystem::filesystem_error(
            "Cannot open file", dest_file_path,
            std::error_code(errno, std::system_category()));
    }

    if (gp_file_created)
    {
        gp_file_free(file);
    }

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

vector<int32_t> CameraWrapper::choicesStringToInt(
    CameraWidgetRadio& widget, function<int(string)> conv_func,
    bool include_invalid)
{
    vector<string> choices = widget.getChoices();

    vector<int32_t> int_choices;
    int string_choice_num = 0;

    for (unsigned int i = 0; i < choices.size(); ++i)
    {
        string choice = choices.at(i);

        int val = conv_func(choice);
        if (string_choice_num == 0 && val > 0)
        {
            int_choices.push_back(val);
        }
        else if (string_choice_num < 0 && val > 0)
        {
            throw CameraException(
                fmt::format("Invalid choice list ({})", widget.getName()));
        }
        else if (include_invalid)
        {
            int_choices.push_back(--string_choice_num);
        }
    }

    return int_choices;
}

void CameraWrapper::updateBulbConfig()
{
    CameraWidgetRadio widget{*this, CONFIG_SHUTTER_SPEED};
    vector<string> strings = widget.getChoices();
    vector<int32_t> values = getShutterSpeedChoices(widget);

    max_shutter_speed = *std::max_element(values.begin(), values.end());
    for (unsigned int i = 0; i < strings.size(); ++i)
    {
        if (strings.at(i) == "Bulb")
        {
            bulb_choice = i;
            break;
        }
    }
}

int CameraStringConversion::exposureToMicroseconds(string str)
{
    int num, den;
    if (scn::scan(str, "{}/{}", num, den))
    {
        return num * 1000 * 1000 / den;
    }
    float ss;
    if (scn::scan(str, "{}", ss))
    {
        return ss * 1000 * 1000;
    }
    return -1;
}

int CameraStringConversion::apertureToInt(string str)
{
    float aperture;
    if (scn::scan(str, "f/{}", aperture))
    {
        return aperture * 100;
    }

    return -1;
}

int CameraStringConversion::isoToInt(string str)
{
    int iso;
    if (scn::scan(str, "{}", iso))
    {
        return iso;
    }

    return -1;
}

int CameraStringConversion::batteryLevelToInt(string str)
{
    int battery;
    if (scn::scan(str, "{}", battery))
    {
        return battery;
    }

    return -1;
}

}  // namespace gphotow