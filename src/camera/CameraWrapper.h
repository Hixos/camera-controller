/**
 * Copyright (c) 2020 Luca Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <gphoto2/gphoto2.h>
#include <scn/scn.h>
#include <stdlib.h>

#include <chrono>
#include <cstring>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "CameraExceptions.h"
#include "CameraWidget.h"
#include "PrintLogger.h"
#include "camera_mappings/mappings.h"

using std::function;
using std::map;
using std::pair;
using std::string;
using std::vector;
using std::chrono::milliseconds;
using std::chrono::seconds;

namespace gphotow
{
static const string NOT_A_GOOD_SERIAL = "NOT_A_GOOD_SERIAL";

struct CameraPath
{
    string folder;
    string name;

    CameraPath() : folder(""), name("") {}

    CameraPath(CameraFilePath path) : folder(path.folder), name(path.name) {}

    string getPath() const { return folder + "/" + name; }

    CameraFilePath toCameraFilePath() const
    {
        CameraFilePath path;
        strcpy(path.name, name.c_str());
        strcpy(path.folder, folder.c_str());

        return path;
    }
};

using CameraEvent = std::pair<CameraEventType, std::optional<CameraPath>>;

class CameraWrapper
{
    friend class CameraWidgetBase;

public:
    struct ShutterSpeedConfig
    {
        int32_t shutter_speed;
        bool bulb;
    };

    CameraWrapper();
    ~CameraWrapper();

    CameraWrapper(CameraWrapper const&) = delete;
    void operator=(CameraWrapper const&) = delete;

    /**
     * @brief Connects to a camera and returns its serial.
     *
     * @throw GPhotoError
     */
    string connect();

    /**
     * @brief Disconnects from a connected camera.
     *
     */
    bool disconnect();

    /**
     * @brief True if connected to a camera.
     *
     * @return Wether a camera is connected or not.
     */
    bool isConnected();

    /**
     * @brief Pools the camera to see if it's responsive.
     *
     * @return Wether a camera is responsive or not.
     */
    bool isResponsive();

    /**
     * @brief Returns the serial number of the connected camera.
     * @throw GPhotoError
     * @return string Serial number
     */
    string getSerialNumber();

    /**
     * @brief Returns the manufacturer of the connected camera.
     * @throw GPhotoError
     * @return string Serial number
     */
    string getManufacturer();

    /**
     * @brief Returns the model of the connected camera.
     * @throw GPhotoError
     * @return string Serial number
     */
    string getCameraModel();

    /**
     * @brief Returns the device version of the connected camera.
     * @throw GPhotoError
     * @return string Serial number
     */
    string getDeviceVersion();

    /**
     * @brief Returns the information (manufacturer, model, version, SN) of the
     * connected camera.
     * @throw GPhotoError
     * @return string Serial number
     */
    string getCameraInfo();

    /**
     * @brief Returns the current exposure time in useconds.
     * @throw GPhotoError
     * @return exposure time in usec or 0 if error, -1 if BULB
     */
    ShutterSpeedConfig getShutterSpeed();

    /**
     * @brief Sets the exposure time
     * @throw GPhotoError
     * @param    exposuretime Exposure time in us. Must be one of the values
     * returned in listExposureTimes
     */
    void setShutterSpeed(int32_t shutter_speed);

    /**
     * @brief Lists all the available exposure times in the current exposure
     * mode.
     * @throw GPhotoError
     * @return list of exposure times, or empty list if error.
     */
    vector<int32_t> getShutterSpeedChoices(bool include_bulb = true);

    /**
     * @brief Lists all the available exposure times in the current exposure
     * mode.
     * @throw GPhotoError
     * @return list of exposure times, or empty list if error.
     */
    vector<int32_t> getShutterSpeedChoices(CameraWidgetRadio& widget,
                                  bool include_bulb = true);

    /**
     * @brief Returns the current aperture.
     * @throw GPhotoError
     * @return F Number * 100, 0 if error
     */
    int32_t getAperture();

    /**
     * @brief Sets the aperture
     * @throw GPhotoError
     * @param    aperture F-Number * 100. Must be one of the values
     * returned in listApertures()
     */
    void setAperture(int32_t aperture);

    /**
     * @brief Lists all the available apertures.
     * @throw GPhotoError
     * @return list of apertures (F-Number * 100), or empty list if error.
     */
    vector<int32_t> getApertureChoices();

    /**
     * @brief Lists all the available apertures.
     * @throw GPhotoError
     * @return list of apertures (F-Number * 100), or empty list if error.
     */
    vector<int32_t> getApertureChoices(CameraWidgetRadio& widget);
    /**
     * @brief Returns the current ISO.
     * @throw GPhotoError
     * @return ISO, 0 if error
     */
    int32_t getISO();

    /**
     * @brief Sets the aperture
     * @throw GPhotoError
     * @param    iso Must be one of the values returned in listISOs()
     */
    void setISO(int32_t iso);

    /**
     * @brief Lists all the available ISOs.
     * @throw GPhotoError
     * @return list of ISOs, or empty list if error.
     */
    vector<int32_t> getIsoChoices();

    /**
     * @brief Lists all the available ISOs.
     * @throw GPhotoError
     * @return list of ISOs, or empty list if error.
     */
    vector<int32_t> getIsoChoices(CameraWidgetRadio& widget);

    /**
     * @brief Returns the current battery percent of the camera
     * @throw GPhotoError
     * @return Battery percent, or -1 if error
     */
    int getBatteryPercent();

    /**
     * @brief Get the current Exposure Program of the camera
     * @throw GPhotoError
     */
    string getExposureProgram();

    /**
     * @brief Get focus mode
     * @throw GPhotoError
     */
    string getFocusMode();

    /**
     * @brief Get focus mode
     * @throw GPhotoError
     */
    void nextFocusMode();

    /**
     * @brief Returns the current trigger mode.
     * @throw GPhotoError
     */
    string getTriggerMode();

    /**
     * @brief Returns the current focal length.
     * @throw GPhotoError
     */
    int getFocalLength();

    bool getLongExpNR();
    void setLongExpNR(bool nr);

    bool getAutoISO();

    void setAutoISO(bool auto_iso);

    /**
     * @brief Sets the Capture Target
     * @throw GPhotoError
     */
    void setCaptureTarget(string capture_target);

    /**
     * @brief Gets the Capture Target
     * @throw GPhotoError
     */
    string getCaptureTarget();

    float getLightMeter();
    CameraWidgetRange::Range getLightMeterRange();

    CameraPath capture();
    CameraPath cameraCapture();
    CameraPath bulbCapture(int exposure_time,
                           milliseconds timeout = seconds(60));

    void triggerCapture();
    void downloadFile(CameraFilePath path, string destination);

    CameraEvent waitForEvent(int timeout);

private:
    /**
     * @brief Set the bulb toggle value
     * @throw GPhotoError
     */
    void bulb(int bulb);

    void freeCamera();

    void updateBulbConfig();

    vector<int32_t> choicesStringToInt(CameraWidgetRadio& widget,
                                   function<int(string)> conv_func,
                                   bool include_invalid = true);

    bool connected = false;

    string serial = "";

    unsigned int bulb_choice = 0;
    int max_shutter_speed    = 0;  // Maximum shutter speed without Bulb
    int bulb_shutter_speed   = 0;

    Camera* camera     = nullptr;
    GPContext* context = nullptr;
    PrintLogger log    = Logging::getLogger("CameraWrapper");
};

class CameraStringConversion
{
public:
    static int exposureToMicroseconds(string str);
    static int apertureToInt(string str);
    static int isoToInt(string str);
    static int batteryLevelToInt(string str);

    /**
     * @brief Finds the index of @p choices whose element is nearest to
     * @p value. Assumes @p choices is in ascending order
     *
     * @tparam T Numeric type
     * @param choices
     * @param value
     * @return int index of element closest to to @p value
     */
    template <typename T>
    static unsigned int findNearest(const vector<T>& choices, T value)
    {
        static_assert(std::is_arithmetic_v<T>);
        T prev_dist = std::abs(value - choices[0]);
        for (unsigned int i = 1; i < choices.size(); ++i)
        {
            T dist = std::abs(value - choices[i]);
            if (dist > prev_dist)
            {
                return i - 1;
            }
            prev_dist = dist;
        }
        return choices.size() - 1;
    }

private:
    static string toString(float value, int precision);
    CameraStringConversion() {}
};

}  // namespace gphotow