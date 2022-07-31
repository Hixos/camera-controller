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

class EventHeartBeat : Event(10) 
{
}

class EventCmdRestart : Event(11) 
{
}

class EventCmdReboot : Event(12) 
{
}

class EventCmdShutdown : Event(13) 
{
}

class EventCameraCmdConnect : Event(14) 
{
}

class EventCameraCmdDisconnect : Event(15) 
{
}

class EventCameraCmdRecoverError : Event(16) 
{
}

class EventCameraCaptureStarted : Event(17) 
{
}

class EventCameraCmdCapture : Event(18) 
{
}

class EventCameraCmdCapture_Internal : Event(19) 
{
}

class EventCameraCmdDownload : Event(20) 
{
    @SerializedName("download" ) var download : Boolean? = null
}

class EventCameraCmdDownload_Internal : Event(21) 
{
}

class EventCameraConnected : Event(22) 
{
}

class EventCameraReady : Event(23) 
{
}

class EventCameraBusyOrError : Event(24) 
{
}

class EventCameraDisconnected : Event(25) 
{
}

class EventCameraConnectionError : Event(26) 
{
}

class EventCameraError : Event(27) 
{
}

class EventCameraIgnoreError : Event(28) 
{
}

class EventCameraCmdLowLatency : Event(29) 
{
    @SerializedName("low_latency" ) var lowLatency : Boolean? = null
}

class EventCameraCaptureDone : Event(30) 
{
    @SerializedName("downloaded" ) var downloaded : Boolean? = null
    @SerializedName("download_dir" ) var downloadDir : String? = null
    @SerializedName("file" ) var file : String? = null
}

class EventGetCameraControllerState : Event(31) 
{
}

class EventCameraControllerState : Event(32) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("camera_connected" ) var cameraConnected : Boolean? = null
    @SerializedName("download_enabled" ) var downloadEnabled : Boolean? = null
}

class EventConfigGetShutterSpeed : Event(33) 
{
}

class EventConfigGetChoicesShutterSpeed : Event(34) 
{
}

class EventConfigSetShutterSpeed : Event(35) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
}

class EventConfigValueShutterSpeed : Event(36) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
    @SerializedName("bulb" ) var bulb : Boolean? = null
}

class EventConfigChoicesShutterSpeed : Event(37) 
{
    @SerializedName("shutter_speed_choices" ) var shutterSpeedChoices : ArrayList<Int>? = null
}

class EventConfigGetAperture : Event(38) 
{
}

class EventConfigGetChoicesAperture : Event(39) 
{
}

class EventConfigSetAperture : Event(40) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigValueAperture : Event(41) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigChoicesAperture : Event(42) 
{
    @SerializedName("aperture_choices" ) var apertureChoices : ArrayList<Int>? = null
}

class EventConfigGetISO : Event(43) 
{
}

class EventConfigGetChoicesISO : Event(44) 
{
}

class EventConfigSetISO : Event(45) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigValueISO : Event(46) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigChoicesISO : Event(47) 
{
    @SerializedName("iso_choices" ) var isoChoices : ArrayList<Int>? = null
}

class EventConfigGetBattery : Event(48) 
{
}

class EventConfigValueBattery : Event(49) 
{
    @SerializedName("battery" ) var battery : Int? = null
}

class EventConfigGetFocalLength : Event(50) 
{
}

class EventConfigValueFocalLength : Event(51) 
{
    @SerializedName("focal_length" ) var focalLength : Int? = null
}

class EventConfigGetFocusMode : Event(52) 
{
}

class EventConfigNextFocusMode : Event(53) 
{
}

class EventConfigValueFocusMode : Event(54) 
{
    @SerializedName("focus_mode" ) var focusMode : String? = null
}

class EventConfigGetLongExpNR : Event(55) 
{
}

class EventConfigSetLongExpNR : Event(56) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigValueLongExpNR : Event(57) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigGetVibRed : Event(58) 
{
}

class EventConfigSetVibRed : Event(59) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigValueVibRed : Event(60) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigGetCaptureTarget : Event(61) 
{
}

class EventConfigSetCaptureTarget : Event(62) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigValueCaptureTarget : Event(63) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigGetExposureProgram : Event(64) 
{
}

class EventConfigValueExposureProgram : Event(65) 
{
    @SerializedName("exposure_program" ) var exposureProgram : String? = null
}

class EventConfigGetLightMeter : Event(66) 
{
}

class EventConfigValueLightMeter : Event(67) 
{
    @SerializedName("light_meter" ) var lightMeter : Float? = null
    @SerializedName("min" ) var min : Float? = null
    @SerializedName("max" ) var max : Float? = null
}

class EventConfigGetAutoISO : Event(68) 
{
}

class EventConfigSetAutoISO : Event(69) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigValueAutoISO : Event(70) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigGetAll : Event(71) 
{
}

class EventGetCurrentMode : Event(72) 
{
}

class EventValueCurrentMode : Event(73) 
{
    @SerializedName("mode" ) var mode : String? = null
}

class EventModeStopped : Event(74) 
{
}

class EventModeStop : Event(75) 
{
}

class EventModeIntervalometer : Event(76) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerStart : Event(77) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerDeadlineExpired : Event(78) 
{
}

class EventIntervalometerState : Event(79) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("num_captures" ) var numCaptures : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventEnableEventPassThrough : Event(80) 
{
}

class EventDisableEventPassThrough : Event(81) 
{
}



fun jsonToEvent(json: String) : Event?
{
    val gson = Gson()
    val e: Event = gson.fromJson(json, Event::class.java)
    when(e.getId())
    {
        10 -> return gson.fromJson(json, EventHeartBeat::class.java)
        11 -> return gson.fromJson(json, EventCmdRestart::class.java)
        12 -> return gson.fromJson(json, EventCmdReboot::class.java)
        13 -> return gson.fromJson(json, EventCmdShutdown::class.java)
        14 -> return gson.fromJson(json, EventCameraCmdConnect::class.java)
        15 -> return gson.fromJson(json, EventCameraCmdDisconnect::class.java)
        16 -> return gson.fromJson(json, EventCameraCmdRecoverError::class.java)
        17 -> return gson.fromJson(json, EventCameraCaptureStarted::class.java)
        18 -> return gson.fromJson(json, EventCameraCmdCapture::class.java)
        19 -> return gson.fromJson(json, EventCameraCmdCapture_Internal::class.java)
        20 -> return gson.fromJson(json, EventCameraCmdDownload::class.java)
        21 -> return gson.fromJson(json, EventCameraCmdDownload_Internal::class.java)
        22 -> return gson.fromJson(json, EventCameraConnected::class.java)
        23 -> return gson.fromJson(json, EventCameraReady::class.java)
        24 -> return gson.fromJson(json, EventCameraBusyOrError::class.java)
        25 -> return gson.fromJson(json, EventCameraDisconnected::class.java)
        26 -> return gson.fromJson(json, EventCameraConnectionError::class.java)
        27 -> return gson.fromJson(json, EventCameraError::class.java)
        28 -> return gson.fromJson(json, EventCameraIgnoreError::class.java)
        29 -> return gson.fromJson(json, EventCameraCmdLowLatency::class.java)
        30 -> return gson.fromJson(json, EventCameraCaptureDone::class.java)
        31 -> return gson.fromJson(json, EventGetCameraControllerState::class.java)
        32 -> return gson.fromJson(json, EventCameraControllerState::class.java)
        33 -> return gson.fromJson(json, EventConfigGetShutterSpeed::class.java)
        34 -> return gson.fromJson(json, EventConfigGetChoicesShutterSpeed::class.java)
        35 -> return gson.fromJson(json, EventConfigSetShutterSpeed::class.java)
        36 -> return gson.fromJson(json, EventConfigValueShutterSpeed::class.java)
        37 -> return gson.fromJson(json, EventConfigChoicesShutterSpeed::class.java)
        38 -> return gson.fromJson(json, EventConfigGetAperture::class.java)
        39 -> return gson.fromJson(json, EventConfigGetChoicesAperture::class.java)
        40 -> return gson.fromJson(json, EventConfigSetAperture::class.java)
        41 -> return gson.fromJson(json, EventConfigValueAperture::class.java)
        42 -> return gson.fromJson(json, EventConfigChoicesAperture::class.java)
        43 -> return gson.fromJson(json, EventConfigGetISO::class.java)
        44 -> return gson.fromJson(json, EventConfigGetChoicesISO::class.java)
        45 -> return gson.fromJson(json, EventConfigSetISO::class.java)
        46 -> return gson.fromJson(json, EventConfigValueISO::class.java)
        47 -> return gson.fromJson(json, EventConfigChoicesISO::class.java)
        48 -> return gson.fromJson(json, EventConfigGetBattery::class.java)
        49 -> return gson.fromJson(json, EventConfigValueBattery::class.java)
        50 -> return gson.fromJson(json, EventConfigGetFocalLength::class.java)
        51 -> return gson.fromJson(json, EventConfigValueFocalLength::class.java)
        52 -> return gson.fromJson(json, EventConfigGetFocusMode::class.java)
        53 -> return gson.fromJson(json, EventConfigNextFocusMode::class.java)
        54 -> return gson.fromJson(json, EventConfigValueFocusMode::class.java)
        55 -> return gson.fromJson(json, EventConfigGetLongExpNR::class.java)
        56 -> return gson.fromJson(json, EventConfigSetLongExpNR::class.java)
        57 -> return gson.fromJson(json, EventConfigValueLongExpNR::class.java)
        58 -> return gson.fromJson(json, EventConfigGetVibRed::class.java)
        59 -> return gson.fromJson(json, EventConfigSetVibRed::class.java)
        60 -> return gson.fromJson(json, EventConfigValueVibRed::class.java)
        61 -> return gson.fromJson(json, EventConfigGetCaptureTarget::class.java)
        62 -> return gson.fromJson(json, EventConfigSetCaptureTarget::class.java)
        63 -> return gson.fromJson(json, EventConfigValueCaptureTarget::class.java)
        64 -> return gson.fromJson(json, EventConfigGetExposureProgram::class.java)
        65 -> return gson.fromJson(json, EventConfigValueExposureProgram::class.java)
        66 -> return gson.fromJson(json, EventConfigGetLightMeter::class.java)
        67 -> return gson.fromJson(json, EventConfigValueLightMeter::class.java)
        68 -> return gson.fromJson(json, EventConfigGetAutoISO::class.java)
        69 -> return gson.fromJson(json, EventConfigSetAutoISO::class.java)
        70 -> return gson.fromJson(json, EventConfigValueAutoISO::class.java)
        71 -> return gson.fromJson(json, EventConfigGetAll::class.java)
        72 -> return gson.fromJson(json, EventGetCurrentMode::class.java)
        73 -> return gson.fromJson(json, EventValueCurrentMode::class.java)
        74 -> return gson.fromJson(json, EventModeStopped::class.java)
        75 -> return gson.fromJson(json, EventModeStop::class.java)
        76 -> return gson.fromJson(json, EventModeIntervalometer::class.java)
        77 -> return gson.fromJson(json, EventIntervalometerStart::class.java)
        78 -> return gson.fromJson(json, EventIntervalometerDeadlineExpired::class.java)
        79 -> return gson.fromJson(json, EventIntervalometerState::class.java)
        80 -> return gson.fromJson(json, EventEnableEventPassThrough::class.java)
        81 -> return gson.fromJson(json, EventDisableEventPassThrough::class.java)

        
        else -> return null
    }
}