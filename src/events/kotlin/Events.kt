package it.hixos.cameracontroller

import com.google.gson.Gson
import com.google.gson.annotations.SerializedName

open class Event(id: Int)
{
    @SerializedName("event_id" ) private var _id : Int = 0

    init {
        this._id = id
    }
    fun getId() : Int = _id

}

class EventCameraCmdConnect : Event(10) 
{
}

class EventCameraCmdDisconnect : Event(11) 
{
}

class EventCameraCmdRecoverError : Event(12) 
{
}

class EventCameraCmdCapture : Event(13) 
{
}

class EventCameraCmdCapture_Internal : Event(14) 
{
}

class EventCameraCmdDownload : Event(15) 
{
    @SerializedName("download" ) var download : Boolean? = null
}

class EventCameraCmdDownload_Internal : Event(16) 
{
}

class EventCameraConnected : Event(17) 
{
}

class EventCameraReady : Event(18) 
{
}

class EventCameraBusyOrError : Event(19) 
{
}

class EventCameraDisconnected : Event(20) 
{
}

class EventCameraConnectionError : Event(21) 
{
}

class EventCameraError : Event(22) 
{
}

class EventCameraIgnoreError : Event(23) 
{
}

class EventCameraCmdLowLatency : Event(24) 
{
    @SerializedName("low_latency" ) var lowLatency : Boolean? = null
}

class EventCameraCaptureDone : Event(25) 
{
    @SerializedName("downloaded" ) var downloaded : Boolean? = null
    @SerializedName("download_dir" ) var downloadDir : String? = null
    @SerializedName("file" ) var file : String? = null
}

class EventGetCameraControllerState : Event(26) 
{
}

class EventCameraControllerState : Event(27) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("camera_connected" ) var cameraConnected : Boolean? = null
    @SerializedName("download_enabled" ) var downloadEnabled : Boolean? = null
}

class EventConfigGetShutterSpeed : Event(28) 
{
}

class EventConfigGetChoicesShutterSpeed : Event(29) 
{
}

class EventConfigSetShutterSpeed : Event(30) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
}

class EventConfigValueShutterSpeed : Event(31) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
    @SerializedName("bulb" ) var bulb : Boolean? = null
}

class EventConfigChoicesShutterSpeed : Event(32) 
{
    @SerializedName("shutter_speed_choices" ) var shutterSpeedChoices : ArrayList<Int>? = null
}

class EventConfigGetAperture : Event(33) 
{
}

class EventConfigGetChoicesAperture : Event(34) 
{
}

class EventConfigSetAperture : Event(35) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigValueAperture : Event(36) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigChoicesAperture : Event(37) 
{
    @SerializedName("aperture_choices" ) var apertureChoices : ArrayList<Int>? = null
}

class EventConfigGetISO : Event(38) 
{
}

class EventConfigGetChoicesISO : Event(39) 
{
}

class EventConfigSetISO : Event(40) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigValueISO : Event(41) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigChoicesISO : Event(42) 
{
    @SerializedName("iso_choices" ) var isoChoices : ArrayList<Int>? = null
}

class EventConfigGetBattery : Event(43) 
{
}

class EventConfigValueBattery : Event(44) 
{
    @SerializedName("battery" ) var battery : Int? = null
}

class EventConfigGetFocalLength : Event(45) 
{
}

class EventConfigValueFocalLength : Event(46) 
{
    @SerializedName("focal_length" ) var focalLength : Int? = null
}

class EventConfigGetFocusMode : Event(47) 
{
}

class EventConfigNextFocusMode : Event(48) 
{
}

class EventConfigValueFocusMode : Event(49) 
{
    @SerializedName("focus_mode" ) var focusMode : String? = null
}

class EventConfigGetLongExpNR : Event(50) 
{
}

class EventConfigSetLongExpNR : Event(51) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigValueLongExpNR : Event(52) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigGetVibRed : Event(53) 
{
}

class EventConfigSetVibRed : Event(54) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigValueVibRed : Event(55) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigGetCaptureTarget : Event(56) 
{
}

class EventConfigSetCaptureTarget : Event(57) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigValueCaptureTarget : Event(58) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigGetExposureProgram : Event(59) 
{
}

class EventConfigValueExposureProgram : Event(60) 
{
    @SerializedName("exposure_program" ) var exposureProgram : String? = null
}

class EventConfigGetLightMeter : Event(61) 
{
}

class EventConfigValueLightMeter : Event(62) 
{
    @SerializedName("light_meter" ) var lightMeter : Float? = null
    @SerializedName("min" ) var min : Float? = null
    @SerializedName("max" ) var max : Float? = null
}

class EventConfigGetAutoISO : Event(63) 
{
}

class EventConfigSetAutoISO : Event(64) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigValueAutoISO : Event(65) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigGetAll : Event(66) 
{
}

class EventGetCurrentMode : Event(67) 
{
}

class EventValueCurrentMode : Event(68) 
{
    @SerializedName("mode" ) var mode : String? = null
}

class EventModeStopped : Event(69) 
{
}

class EventModeStop : Event(70) 
{
}

class EventModeIntervalometer : Event(71) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerStart : Event(72) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerDeadlineExpired : Event(73) 
{
}

class EventIntervalometerState : Event(74) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("num_captures" ) var numCaptures : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventEnableEventPassThrough : Event(75) 
{
}

class EventDisableEventPassThrough : Event(76) 
{
}



fun jsonToEvent(json: String) : Event?
{
    val gson = Gson()
    val e: Event = gson.fromJson(json, Event::class.java)
    when(e.getId())
    {
        10 -> return gson.fromJson(json, EventCameraCmdConnect::class.java)
        11 -> return gson.fromJson(json, EventCameraCmdDisconnect::class.java)
        12 -> return gson.fromJson(json, EventCameraCmdRecoverError::class.java)
        13 -> return gson.fromJson(json, EventCameraCmdCapture::class.java)
        14 -> return gson.fromJson(json, EventCameraCmdCapture_Internal::class.java)
        15 -> return gson.fromJson(json, EventCameraCmdDownload::class.java)
        16 -> return gson.fromJson(json, EventCameraCmdDownload_Internal::class.java)
        17 -> return gson.fromJson(json, EventCameraConnected::class.java)
        18 -> return gson.fromJson(json, EventCameraReady::class.java)
        19 -> return gson.fromJson(json, EventCameraBusyOrError::class.java)
        20 -> return gson.fromJson(json, EventCameraDisconnected::class.java)
        21 -> return gson.fromJson(json, EventCameraConnectionError::class.java)
        22 -> return gson.fromJson(json, EventCameraError::class.java)
        23 -> return gson.fromJson(json, EventCameraIgnoreError::class.java)
        24 -> return gson.fromJson(json, EventCameraCmdLowLatency::class.java)
        25 -> return gson.fromJson(json, EventCameraCaptureDone::class.java)
        26 -> return gson.fromJson(json, EventGetCameraControllerState::class.java)
        27 -> return gson.fromJson(json, EventCameraControllerState::class.java)
        28 -> return gson.fromJson(json, EventConfigGetShutterSpeed::class.java)
        29 -> return gson.fromJson(json, EventConfigGetChoicesShutterSpeed::class.java)
        30 -> return gson.fromJson(json, EventConfigSetShutterSpeed::class.java)
        31 -> return gson.fromJson(json, EventConfigValueShutterSpeed::class.java)
        32 -> return gson.fromJson(json, EventConfigChoicesShutterSpeed::class.java)
        33 -> return gson.fromJson(json, EventConfigGetAperture::class.java)
        34 -> return gson.fromJson(json, EventConfigGetChoicesAperture::class.java)
        35 -> return gson.fromJson(json, EventConfigSetAperture::class.java)
        36 -> return gson.fromJson(json, EventConfigValueAperture::class.java)
        37 -> return gson.fromJson(json, EventConfigChoicesAperture::class.java)
        38 -> return gson.fromJson(json, EventConfigGetISO::class.java)
        39 -> return gson.fromJson(json, EventConfigGetChoicesISO::class.java)
        40 -> return gson.fromJson(json, EventConfigSetISO::class.java)
        41 -> return gson.fromJson(json, EventConfigValueISO::class.java)
        42 -> return gson.fromJson(json, EventConfigChoicesISO::class.java)
        43 -> return gson.fromJson(json, EventConfigGetBattery::class.java)
        44 -> return gson.fromJson(json, EventConfigValueBattery::class.java)
        45 -> return gson.fromJson(json, EventConfigGetFocalLength::class.java)
        46 -> return gson.fromJson(json, EventConfigValueFocalLength::class.java)
        47 -> return gson.fromJson(json, EventConfigGetFocusMode::class.java)
        48 -> return gson.fromJson(json, EventConfigNextFocusMode::class.java)
        49 -> return gson.fromJson(json, EventConfigValueFocusMode::class.java)
        50 -> return gson.fromJson(json, EventConfigGetLongExpNR::class.java)
        51 -> return gson.fromJson(json, EventConfigSetLongExpNR::class.java)
        52 -> return gson.fromJson(json, EventConfigValueLongExpNR::class.java)
        53 -> return gson.fromJson(json, EventConfigGetVibRed::class.java)
        54 -> return gson.fromJson(json, EventConfigSetVibRed::class.java)
        55 -> return gson.fromJson(json, EventConfigValueVibRed::class.java)
        56 -> return gson.fromJson(json, EventConfigGetCaptureTarget::class.java)
        57 -> return gson.fromJson(json, EventConfigSetCaptureTarget::class.java)
        58 -> return gson.fromJson(json, EventConfigValueCaptureTarget::class.java)
        59 -> return gson.fromJson(json, EventConfigGetExposureProgram::class.java)
        60 -> return gson.fromJson(json, EventConfigValueExposureProgram::class.java)
        61 -> return gson.fromJson(json, EventConfigGetLightMeter::class.java)
        62 -> return gson.fromJson(json, EventConfigValueLightMeter::class.java)
        63 -> return gson.fromJson(json, EventConfigGetAutoISO::class.java)
        64 -> return gson.fromJson(json, EventConfigSetAutoISO::class.java)
        65 -> return gson.fromJson(json, EventConfigValueAutoISO::class.java)
        66 -> return gson.fromJson(json, EventConfigGetAll::class.java)
        67 -> return gson.fromJson(json, EventGetCurrentMode::class.java)
        68 -> return gson.fromJson(json, EventValueCurrentMode::class.java)
        69 -> return gson.fromJson(json, EventModeStopped::class.java)
        70 -> return gson.fromJson(json, EventModeStop::class.java)
        71 -> return gson.fromJson(json, EventModeIntervalometer::class.java)
        72 -> return gson.fromJson(json, EventIntervalometerStart::class.java)
        73 -> return gson.fromJson(json, EventIntervalometerDeadlineExpired::class.java)
        74 -> return gson.fromJson(json, EventIntervalometerState::class.java)
        75 -> return gson.fromJson(json, EventEnableEventPassThrough::class.java)
        76 -> return gson.fromJson(json, EventDisableEventPassThrough::class.java)

        
        else -> return null
    }
}