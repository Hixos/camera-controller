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

class EventCameraCmdConnect : Event(13) 
{
}

class EventCameraCmdDisconnect : Event(14) 
{
}

class EventCameraCmdRecoverError : Event(15) 
{
}

class EventCameraCmdCapture : Event(16) 
{
}

class EventCameraCmdCapture_Internal : Event(17) 
{
}

class EventCameraCmdDownload : Event(18) 
{
    @SerializedName("download" ) var download : Boolean? = null
}

class EventCameraCmdDownload_Internal : Event(19) 
{
}

class EventCameraConnected : Event(20) 
{
}

class EventCameraReady : Event(21) 
{
}

class EventCameraBusyOrError : Event(22) 
{
}

class EventCameraDisconnected : Event(23) 
{
}

class EventCameraConnectionError : Event(24) 
{
}

class EventCameraError : Event(25) 
{
}

class EventCameraIgnoreError : Event(26) 
{
}

class EventCameraCmdLowLatency : Event(27) 
{
    @SerializedName("low_latency" ) var lowLatency : Boolean? = null
}

class EventCameraCaptureDone : Event(28) 
{
    @SerializedName("downloaded" ) var downloaded : Boolean? = null
    @SerializedName("download_dir" ) var downloadDir : String? = null
    @SerializedName("file" ) var file : String? = null
}

class EventGetCameraControllerState : Event(29) 
{
}

class EventCameraControllerState : Event(30) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("camera_connected" ) var cameraConnected : Boolean? = null
    @SerializedName("download_enabled" ) var downloadEnabled : Boolean? = null
}

class EventConfigGetShutterSpeed : Event(31) 
{
}

class EventConfigGetChoicesShutterSpeed : Event(32) 
{
}

class EventConfigSetShutterSpeed : Event(33) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
}

class EventConfigValueShutterSpeed : Event(34) 
{
    @SerializedName("shutter_speed" ) var shutterSpeed : Int? = null
    @SerializedName("bulb" ) var bulb : Boolean? = null
}

class EventConfigChoicesShutterSpeed : Event(35) 
{
    @SerializedName("shutter_speed_choices" ) var shutterSpeedChoices : ArrayList<Int>? = null
}

class EventConfigGetAperture : Event(36) 
{
}

class EventConfigGetChoicesAperture : Event(37) 
{
}

class EventConfigSetAperture : Event(38) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigValueAperture : Event(39) 
{
    @SerializedName("aperture" ) var aperture : Int? = null
}

class EventConfigChoicesAperture : Event(40) 
{
    @SerializedName("aperture_choices" ) var apertureChoices : ArrayList<Int>? = null
}

class EventConfigGetISO : Event(41) 
{
}

class EventConfigGetChoicesISO : Event(42) 
{
}

class EventConfigSetISO : Event(43) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigValueISO : Event(44) 
{
    @SerializedName("iso" ) var iso : Int? = null
}

class EventConfigChoicesISO : Event(45) 
{
    @SerializedName("iso_choices" ) var isoChoices : ArrayList<Int>? = null
}

class EventConfigGetBattery : Event(46) 
{
}

class EventConfigValueBattery : Event(47) 
{
    @SerializedName("battery" ) var battery : Int? = null
}

class EventConfigGetFocalLength : Event(48) 
{
}

class EventConfigValueFocalLength : Event(49) 
{
    @SerializedName("focal_length" ) var focalLength : Int? = null
}

class EventConfigGetFocusMode : Event(50) 
{
}

class EventConfigNextFocusMode : Event(51) 
{
}

class EventConfigValueFocusMode : Event(52) 
{
    @SerializedName("focus_mode" ) var focusMode : String? = null
}

class EventConfigGetLongExpNR : Event(53) 
{
}

class EventConfigSetLongExpNR : Event(54) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigValueLongExpNR : Event(55) 
{
    @SerializedName("long_exp_nr" ) var longExpNr : Boolean? = null
}

class EventConfigGetVibRed : Event(56) 
{
}

class EventConfigSetVibRed : Event(57) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigValueVibRed : Event(58) 
{
    @SerializedName("vr" ) var vr : Boolean? = null
}

class EventConfigGetCaptureTarget : Event(59) 
{
}

class EventConfigSetCaptureTarget : Event(60) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigValueCaptureTarget : Event(61) 
{
    @SerializedName("target" ) var target : String? = null
}

class EventConfigGetExposureProgram : Event(62) 
{
}

class EventConfigValueExposureProgram : Event(63) 
{
    @SerializedName("exposure_program" ) var exposureProgram : String? = null
}

class EventConfigGetLightMeter : Event(64) 
{
}

class EventConfigValueLightMeter : Event(65) 
{
    @SerializedName("light_meter" ) var lightMeter : Float? = null
    @SerializedName("min" ) var min : Float? = null
    @SerializedName("max" ) var max : Float? = null
}

class EventConfigGetAutoISO : Event(66) 
{
}

class EventConfigSetAutoISO : Event(67) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigValueAutoISO : Event(68) 
{
    @SerializedName("auto_iso" ) var autoIso : Boolean? = null
}

class EventConfigGetAll : Event(69) 
{
}

class EventGetCurrentMode : Event(70) 
{
}

class EventValueCurrentMode : Event(71) 
{
    @SerializedName("mode" ) var mode : String? = null
}

class EventModeStopped : Event(72) 
{
}

class EventModeStop : Event(73) 
{
}

class EventModeIntervalometer : Event(74) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerStart : Event(75) 
{
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventIntervalometerDeadlineExpired : Event(76) 
{
}

class EventIntervalometerState : Event(77) 
{
    @SerializedName("state" ) var state : String? = null
    @SerializedName("intervalms" ) var intervalms : Int? = null
    @SerializedName("num_captures" ) var numCaptures : Int? = null
    @SerializedName("total_captures" ) var totalCaptures : Int? = null
}

class EventEnableEventPassThrough : Event(78) 
{
}

class EventDisableEventPassThrough : Event(79) 
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
        13 -> return gson.fromJson(json, EventCameraCmdConnect::class.java)
        14 -> return gson.fromJson(json, EventCameraCmdDisconnect::class.java)
        15 -> return gson.fromJson(json, EventCameraCmdRecoverError::class.java)
        16 -> return gson.fromJson(json, EventCameraCmdCapture::class.java)
        17 -> return gson.fromJson(json, EventCameraCmdCapture_Internal::class.java)
        18 -> return gson.fromJson(json, EventCameraCmdDownload::class.java)
        19 -> return gson.fromJson(json, EventCameraCmdDownload_Internal::class.java)
        20 -> return gson.fromJson(json, EventCameraConnected::class.java)
        21 -> return gson.fromJson(json, EventCameraReady::class.java)
        22 -> return gson.fromJson(json, EventCameraBusyOrError::class.java)
        23 -> return gson.fromJson(json, EventCameraDisconnected::class.java)
        24 -> return gson.fromJson(json, EventCameraConnectionError::class.java)
        25 -> return gson.fromJson(json, EventCameraError::class.java)
        26 -> return gson.fromJson(json, EventCameraIgnoreError::class.java)
        27 -> return gson.fromJson(json, EventCameraCmdLowLatency::class.java)
        28 -> return gson.fromJson(json, EventCameraCaptureDone::class.java)
        29 -> return gson.fromJson(json, EventGetCameraControllerState::class.java)
        30 -> return gson.fromJson(json, EventCameraControllerState::class.java)
        31 -> return gson.fromJson(json, EventConfigGetShutterSpeed::class.java)
        32 -> return gson.fromJson(json, EventConfigGetChoicesShutterSpeed::class.java)
        33 -> return gson.fromJson(json, EventConfigSetShutterSpeed::class.java)
        34 -> return gson.fromJson(json, EventConfigValueShutterSpeed::class.java)
        35 -> return gson.fromJson(json, EventConfigChoicesShutterSpeed::class.java)
        36 -> return gson.fromJson(json, EventConfigGetAperture::class.java)
        37 -> return gson.fromJson(json, EventConfigGetChoicesAperture::class.java)
        38 -> return gson.fromJson(json, EventConfigSetAperture::class.java)
        39 -> return gson.fromJson(json, EventConfigValueAperture::class.java)
        40 -> return gson.fromJson(json, EventConfigChoicesAperture::class.java)
        41 -> return gson.fromJson(json, EventConfigGetISO::class.java)
        42 -> return gson.fromJson(json, EventConfigGetChoicesISO::class.java)
        43 -> return gson.fromJson(json, EventConfigSetISO::class.java)
        44 -> return gson.fromJson(json, EventConfigValueISO::class.java)
        45 -> return gson.fromJson(json, EventConfigChoicesISO::class.java)
        46 -> return gson.fromJson(json, EventConfigGetBattery::class.java)
        47 -> return gson.fromJson(json, EventConfigValueBattery::class.java)
        48 -> return gson.fromJson(json, EventConfigGetFocalLength::class.java)
        49 -> return gson.fromJson(json, EventConfigValueFocalLength::class.java)
        50 -> return gson.fromJson(json, EventConfigGetFocusMode::class.java)
        51 -> return gson.fromJson(json, EventConfigNextFocusMode::class.java)
        52 -> return gson.fromJson(json, EventConfigValueFocusMode::class.java)
        53 -> return gson.fromJson(json, EventConfigGetLongExpNR::class.java)
        54 -> return gson.fromJson(json, EventConfigSetLongExpNR::class.java)
        55 -> return gson.fromJson(json, EventConfigValueLongExpNR::class.java)
        56 -> return gson.fromJson(json, EventConfigGetVibRed::class.java)
        57 -> return gson.fromJson(json, EventConfigSetVibRed::class.java)
        58 -> return gson.fromJson(json, EventConfigValueVibRed::class.java)
        59 -> return gson.fromJson(json, EventConfigGetCaptureTarget::class.java)
        60 -> return gson.fromJson(json, EventConfigSetCaptureTarget::class.java)
        61 -> return gson.fromJson(json, EventConfigValueCaptureTarget::class.java)
        62 -> return gson.fromJson(json, EventConfigGetExposureProgram::class.java)
        63 -> return gson.fromJson(json, EventConfigValueExposureProgram::class.java)
        64 -> return gson.fromJson(json, EventConfigGetLightMeter::class.java)
        65 -> return gson.fromJson(json, EventConfigValueLightMeter::class.java)
        66 -> return gson.fromJson(json, EventConfigGetAutoISO::class.java)
        67 -> return gson.fromJson(json, EventConfigSetAutoISO::class.java)
        68 -> return gson.fromJson(json, EventConfigValueAutoISO::class.java)
        69 -> return gson.fromJson(json, EventConfigGetAll::class.java)
        70 -> return gson.fromJson(json, EventGetCurrentMode::class.java)
        71 -> return gson.fromJson(json, EventValueCurrentMode::class.java)
        72 -> return gson.fromJson(json, EventModeStopped::class.java)
        73 -> return gson.fromJson(json, EventModeStop::class.java)
        74 -> return gson.fromJson(json, EventModeIntervalometer::class.java)
        75 -> return gson.fromJson(json, EventIntervalometerStart::class.java)
        76 -> return gson.fromJson(json, EventIntervalometerDeadlineExpired::class.java)
        77 -> return gson.fromJson(json, EventIntervalometerState::class.java)
        78 -> return gson.fromJson(json, EventEnableEventPassThrough::class.java)
        79 -> return gson.fromJson(json, EventDisableEventPassThrough::class.java)

        
        else -> return null
    }
}