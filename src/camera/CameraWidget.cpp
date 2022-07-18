#include "CameraWidget.h"

#include "CameraExceptions.h"
#include "CameraWrapper.h"

#include "PrintLogger.h"
namespace gphotow
{

CameraWidgetBase::CameraWidgetBase(CameraWrapper& camera, string config_name)
    : camera(camera), widget_name(config_name)
{
    int result = gp_camera_get_single_config(camera.camera, config_name.c_str(),
                                             &widget, camera.context);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

CameraWidgetBase::CameraWidgetBase(CameraWrapper& camera, CameraWidget* widget)
    : camera(camera), widget(widget)
{
    widget_name = getName();
}

CameraWidgetBase::~CameraWidgetBase()
{
    auto log = Logging::getLogger("~CameraWidgetBase");

    if (widget != nullptr){
        // const char* name;
        // gp_widget_get_name(widget, &name);
        // LOG_DEBUG(log, "Destroyed widget {}", string(name));
        gp_widget_free(widget);
    }
}

CameraWidgetType CameraWidgetBase::getType() { return getType(widget); }

CameraWidgetType CameraWidgetBase::getType(CameraWidget* widget)
{
    CameraWidgetType type;
    int result = gp_widget_get_type(widget, &type);
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    return type;
}

string CameraWidgetBase::getName()
{
    const char* value;
    int result = gp_widget_get_name(widget, &value);

    if (result != GP_OK)
    {
        return "";
    }
    else
    {
        return string(value);
    }
}

bool CameraWidgetBase::isReadOnly()
{
    int value;
    int result = gp_widget_get_readonly(widget, &value);

    if (result != GP_OK)
    {
        return true;
    }
    else
    {
        return value != 0;
    }
}

void CameraWidgetBase::apply()
{
    if (!isReadOnly())
    {
        int result = gp_camera_set_single_config(
            camera.camera, widget_name.c_str(), widget, camera.context);

        if (result != GP_OK)
        {
            throw GPhotoError(result);
        }
    }
    else
    {
        throw WidgetReadOnlyError(getName());
    }
}

CameraWidgetRadio::CameraWidgetRadio(CameraWrapper& camera,
                                       string config_name)
    : CameraWidgetBase(camera, config_name)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_MENU && type != GP_WIDGET_RADIO)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

CameraWidgetRadio::CameraWidgetRadio(CameraWrapper& camera,
                                       CameraWidget* widget)
    : CameraWidgetBase(camera, widget)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_MENU && type != GP_WIDGET_RADIO)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

string CameraWidgetRadio::getValue()
{
    char* value;
    int result = gp_widget_get_value(widget, &value);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    else
    {
        return string(value);
    }
}

void CameraWidgetRadio::setValue(string value)
{
    int result = gp_widget_set_value(widget, value.c_str());
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

void CameraWidgetRadio::setValue(unsigned int id)
{
    const char* choice;
    int result = gp_widget_get_choice(widget, id, &choice);
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    setValue(string(choice));
}

vector<string> CameraWidgetRadio::getChoices()
{
    int choices_num = gp_widget_count_choices(widget);

    if (choices_num < 0)
    {
        throw GPhotoError(choices_num);
    }

    const char* choice;
    vector<string> choices;

    for (int i = 0; i < choices_num; i++)
    {
        if (gp_widget_get_choice(widget, i, &choice) == GP_OK)
        {
            choices.push_back(string{choice});
        }
        else
        {
            throw GPhotoError(choices_num);
        }
    }
    return choices;
}

unsigned int CameraWidgetRadio::getID()
{
    vector<string> choices = getChoices();
    string val = getValue();
    for(unsigned int i = 0; i < choices.size(); ++i)
    {
        if(val == choices.at(i))
            return i;
    }
    throw CameraException(fmt::format("Widget ID not found ({})", getName()));
}

CameraWidgetRange::CameraWidgetRange(CameraWrapper& camera, string config_name)
    : CameraWidgetBase(camera, config_name)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_RANGE)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

CameraWidgetRange::CameraWidgetRange(CameraWrapper& camera,
                                     CameraWidget* widget)
    : CameraWidgetBase(camera, widget)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_RANGE)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

float CameraWidgetRange::getValue()
{
    float value;
    int result = gp_widget_get_value(widget, &value);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    else
    {
        return value;
    }
}

void CameraWidgetRange::setValue(float value)
{
    // Log.d("[CameraWidget] %s = '%f'", widget_name.c_str(), value);
    int result = gp_widget_set_value(widget, &value);
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

CameraWidgetRange::Range CameraWidgetRange::getRange()
{
    Range range;
    int result =
        gp_widget_get_range(widget, &range.min, &range.max, &range.step);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    else
    {
        return range;
    }
}

CameraWidgetToggle::CameraWidgetToggle(CameraWrapper& camera, string config_name)
    : CameraWidgetBase(camera, config_name)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_TOGGLE)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

CameraWidgetToggle::CameraWidgetToggle(CameraWrapper& camera,
                                     CameraWidget* widget)
    : CameraWidgetBase(camera, widget)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_TOGGLE)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}


int CameraWidgetToggle::getValue()
{
    int value;
    int result = gp_widget_get_value(widget, &value);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    else
    {
        return value;
    }
}

void CameraWidgetToggle::setValue(int value)
{
    int result = gp_widget_set_value(widget, &value);
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

CameraWidgetText::CameraWidgetText(CameraWrapper& camera, string config_name)
    : CameraWidgetBase(camera, config_name)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_TEXT)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}

CameraWidgetText::CameraWidgetText(CameraWrapper& camera,
                                     CameraWidget* widget)
    : CameraWidgetBase(camera, widget)
{
    CameraWidgetType type = CameraWidgetBase::getType();
    if (type != GP_WIDGET_TEXT)
    {
        throw GPhotoError(GP_ERROR_BAD_PARAMETERS);
    }
}


string CameraWidgetText::getValue()
{
    const char* value;
    int result = gp_widget_get_value(widget, &value);

    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
    else
    {
        return string{value};
    }
}

void CameraWidgetText::setValue(string value)
{
    int result = gp_widget_set_value(widget, &value);
    if (result != GP_OK)
    {
        throw GPhotoError(result);
    }
}

}  // namespace gphotow