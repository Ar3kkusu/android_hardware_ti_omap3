/*
 * Copyright (C) Texas Instruments - http://www.ti.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
/**
* @file CameraProperties.cpp
*
* This file maps the CameraHardwareInterface to the Camera interfaces on OMAP4 (mainly OMX).
*
*/

#include "CameraHal.h"
#include "CameraProperties.h"

#define CAMERA_ROOT         "CameraRoot"
#define CAMERA_INSTANCE     "CameraInstance"
#define TEXT_XML_ELEMENT    "#text"

namespace android {


const char CameraProperties::PROP_KEY_INVALID[]="invalid-key";
const char CameraProperties::PROP_KEY_CAMERA_NAME[]="camera-name";
const char CameraProperties::PROP_KEY_ADAPTER_DLL_NAME[]="camera-adapter-dll-name";
const char CameraProperties::PROP_KEY_CAMERA_SENSOR_INDEX[]="sensor-index";
const char CameraProperties::PROP_KEY_S3D_SUPPORTED[]="s3d-supported";
const char CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_SIZES[] = "preview-size-values";
const char CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FORMATS[] = "preview-format-values";
const char CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FRAME_RATES[] = "preview-frame-rate-values";
const char CameraProperties::PROP_KEY_SUPPORTED_PICTURE_SIZES[] = "picture-size-values";
const char CameraProperties::PROP_KEY_SUPPORTED_PICTURE_FORMATS[] = "picture-format-values";
const char CameraProperties::PROP_KEY_SUPPORTED_THUMBNAIL_SIZES[] = "jpeg-thumbnail-size-values";
const char CameraProperties::PROP_KEY_SUPPORTED_WHITE_BALANCE[] = "whitebalance-values";
const char CameraProperties::PROP_KEY_SUPPORTED_EFFECTS[] = "effect-values";
const char CameraProperties::PROP_KEY_SUPPORTED_ANTIBANDING[] = "antibanding-values";
const char CameraProperties::PROP_KEY_SUPPORTED_EXPOSURE_MODES[] = "exposure-mode-values";
const char CameraProperties::PROP_KEY_SUPPORTED_EV_MAX[] = "ev-compensation-max";
const char CameraProperties::PROP_KEY_SUPPORTED_EV_MIN[] = "ev-compensation-min";
const char CameraProperties::PROP_KEY_SUPPORTED_EV_STEP[] = "ev-compensation-step";
const char CameraProperties::PROP_KEY_SUPPORTED_ISO_VALUES[] = "iso-mode-values";
const char CameraProperties::PROP_KEY_SUPPORTED_SCENE_MODES[] = "scene-mode-values";
const char CameraProperties::PROP_KEY_SUPPORTED_FLASH_MODES[] = "flash-mode-values";
const char CameraProperties::PROP_KEY_SUPPORTED_FOCUS_MODES[] = "focus-mode-values";
const char CameraProperties::PROP_KEY_REQUIRED_PREVIEW_BUFS[] = "required-preview-bufs";
const char CameraProperties::PROP_KEY_REQUIRED_IMAGE_BUFS[] = "required-image-bufs";
const char CameraProperties::PROP_KEY_SUPPORTED_ZOOM_RATIOS[] = "zoom-ratios";
const char CameraProperties::PROP_KEY_SUPPORTED_ZOOM_STAGES[] = "zoom-stages";
const char CameraProperties::PROP_KEY_SUPPORTED_IPP_MODES[] = "ipp-values";
const char CameraProperties::PROP_KEY_SMOOTH_ZOOM_SUPPORTED[] = "smooth-zoom-supported";
const char CameraProperties::PROP_KEY_ZOOM_SUPPORTED[] = "zoom-supported";
const char CameraProperties::PROP_KEY_PREVIEW_SIZE[] = "preview-size";
const char CameraProperties::PROP_KEY_PREVIEW_FORMAT[] = "preview-format";
const char CameraProperties::PROP_KEY_PREVIEW_FRAME_RATE[] = "preview-frame-rate";
const char CameraProperties::PROP_KEY_ZOOM[] = "zoom";
const char CameraProperties::PROP_KEY_PICTURE_SIZE[] = "picture-size";
const char CameraProperties::PROP_KEY_PICTURE_FORMAT[] = "picture-format";
const char CameraProperties::PROP_KEY_JPEG_THUMBNAIL_SIZE[] = "jpeg-thumbnail-size";
const char CameraProperties::PROP_KEY_WHITEBALANCE[] = "whitebalance";
const char CameraProperties::PROP_KEY_EFFECT[] = "effect";
const char CameraProperties::PROP_KEY_ANTIBANDING[] = "antibanding";
const char CameraProperties::PROP_KEY_EXPOSURE_MODE[] = "exposure-mode";
const char CameraProperties::PROP_KEY_EV_COMPENSATION[] = "ev-compensation";
const char CameraProperties::PROP_KEY_ISO_MODE[] = "iso-mode";
const char CameraProperties::PROP_KEY_FOCUS_MODE[] = "focus-mode";
const char CameraProperties::PROP_KEY_SCENE_MODE[] = "scene-mode";
const char CameraProperties::PROP_KEY_FLASH_MODE[] = "flash-mode";
const char CameraProperties::PROP_KEY_JPEG_QUALITY[] = "jpeg-quality";
const char CameraProperties::PROP_KEY_CONTRAST[] = "contrast";
const char CameraProperties::PROP_KEY_BRIGHTNESS[] = "brightness";
const char CameraProperties::PROP_KEY_SATURATION[] = "saturation";
const char CameraProperties::PROP_KEY_SHARPNESS[] = "sharpness";
const char CameraProperties::PROP_KEY_IPP[] = "ipp";
const char CameraProperties::PROP_KEY_S3D2D_PREVIEW[] = "s3d2d-preview";
const char CameraProperties::PROP_KEY_S3D2D_PREVIEW_MODES[] = "s3d2d-preview-values";
const char CameraProperties::PROP_KEY_AUTOCONVERGENCE[] = "auto-convergence";
const char CameraProperties::PROP_KEY_AUTOCONVERGENCE_MODE[] = "auto-convergence-mode";
const char CameraProperties::PROP_KEY_MANUALCONVERGENCE_VALUES[] = "manual-convergence-values";

const char CameraProperties::PARAMS_DELIMITER []= ",";


const char CameraProperties::TICAMERA_FILE_PREFIX[] = "TICamera";
const char CameraProperties::TICAMERA_FILE_EXTN[] = ".xml";


CameraProperties::CameraProperties() : mCamerasSupported(0)
{
    LOG_FUNCTION_NAME

    LOG_FUNCTION_NAME_EXIT
}

CameraProperties::~CameraProperties()
{
    ///Deallocate memory for the properties array
    LOG_FUNCTION_NAME

    ///Delete the properties from the end to avoid copies within freeCameraProperties()
    for ( int i = ( mCamerasSupported - 1 ) ; i >= 0 ; i--)
        {
        CAMHAL_LOGVB("Freeing property array for Camera Index %d", i);
        freeCameraProps(i);
        }

    LOG_FUNCTION_NAME_EXIT
}


///Initializes the CameraProperties class
status_t CameraProperties::initialize()
{
    ///No initializations to do for now
    LOG_FUNCTION_NAME

    status_t ret;

    ret = loadProperties();

    return ret;
}


///Loads the properties XML files present inside /system/etc and loads all the Camera related properties
status_t CameraProperties::loadProperties()
{
    LOG_FUNCTION_NAME

    status_t ret = NO_ERROR;
    ///Open /system/etc directory and read all the xml file with prefix as TICamera<CameraName>
    DIR *dirp;
    struct dirent *dp;

    CAMHAL_LOGVA("Opening /system/etc directory");
    if ((dirp = opendir("/system/etc")) == NULL)
        {
        CAMHAL_LOGEA("Couldn't open directory /system/etc");
        LOG_FUNCTION_NAME_EXIT
        return UNKNOWN_ERROR;
        }

    CAMHAL_LOGVA("Opened /system/etc directory successfully");

    CAMHAL_LOGVA("Processing all directory entries to find Camera property files");
    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
            {
            CAMHAL_LOGVB("File name %s", dp->d_name);
            char* found = strstr(dp->d_name, TICAMERA_FILE_PREFIX);
            if((int32_t)found == (int32_t)dp->d_name)
                {
                ///Prefix matches
                ///Now check if it is an XML file
                if(strstr(dp->d_name, TICAMERA_FILE_EXTN))
                    {
                    ///Confirmed it is an XML file, now open it and parse it
                    CAMHAL_LOGVB("Found Camera Properties file %s", dp->d_name);
                    ////Choosing 268 because "system/etc/"+ dir_name(0...255) can be max 268 chars
                    char fullPath[268];
                    sprintf(fullPath, "/system/etc/%s",dp->d_name);
                    ret = parseAndLoadProps((const char*)fullPath);
                    if(ret!=NO_ERROR)
                        {
                        CAMHAL_LOGEB("Error when parsing the config file :%s Err[%d]",fullPath, ret);
                        LOG_FUNCTION_NAME_EXIT
                        return ret;
                        }
                    CAMHAL_LOGVA("Parsed configuration file and loaded properties");
                    }
                }
            }
        } while (dp != NULL);

    CAMHAL_LOGVA("Closing the directory handle");
    (void) closedir(dirp);

    LOG_FUNCTION_NAME_EXIT
    return ret;
}

status_t CameraProperties::parseAndLoadProps(const char* file)
{
    LOG_FUNCTION_NAME

    xmlTextReaderPtr reader;
    const xmlChar *name=NULL, *value = NULL;
    status_t ret = NO_ERROR;

#if _DEBUG_XML_FILE
        reader = xmlNewTextReaderFilename(file);
        if (reader != NULL)
            {
            ret = xmlTextReaderRead(reader);
            while (ret == 1)
                {
                name = xmlTextReaderConstName(reader);
                value = xmlTextReaderConstValue(reader);
                CAMHAL_LOGEB("Tag %s value %s",name, value);
                ret = xmlTextReaderRead(reader);
                }
            xmlFreeTextReader(reader);
            }
        return NO_ERROR;
#endif

    reader = xmlNewTextReaderFilename(file);
    if (reader != NULL)
        {
        ret = xmlTextReaderRead(reader);
        if (ret != 1)
            {
            CAMHAL_LOGEB("%s : failed to parse\n", file);
            }

        ret = parseCameraElements(reader);
        }
    else
        {
        CAMHAL_LOGEB("Unable to open %s\n", file);
        }

    CAMHAL_LOGVA("Freeing the XML Reader");
    xmlFreeTextReader(reader);

    LOG_FUNCTION_NAME_EXIT

    return ret;
}

status_t CameraProperties::parseCameraElements(xmlTextReaderPtr &reader)
{
    status_t ret = NO_ERROR;
    const xmlChar *name = NULL, *value = NULL;
    char val[256];
    const xmlChar *nextName = NULL;
    int curCameraIndex;
    int propertyIndex;

    LOG_FUNCTION_NAME

    ///xmlNode passed to this function is a cameraElement
    ///It's child nodes are the properties
    ///XML structure looks like this
    /**<CameraRoot>
      * <CameraInstance>
      * <CameraProperty1>Value</CameraProperty1>
      * ....
      * <CameraPropertyN>Value</CameraPropertyN>
      * </CameraInstance>
      *</CameraRoot>
      * CameraPropertyX Tags are the constant property keys defined in CameraProperties.h
      */

    name = xmlTextReaderConstName(reader);
    ret = xmlTextReaderRead(reader);
    if( ( strcmp( (const char *) name, CAMERA_ROOT) == 0 ) &&
         ( 1 == ret))
        {

        //Start parsing the Camera Instances
        ret = xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);

        //skip #text tag
        xmlTextReaderRead(reader);
        if ( 1 != ret)
            {
            CAMHAL_LOGEA("XML File Reached end prematurely or parsing error");
            return ret;
            }

        while( strcmp( (const char *) name, CAMERA_INSTANCE) == 0)
            {
            CAMHAL_LOGVB("Camera Element Name:%s", name);

            curCameraIndex  = mCamerasSupported;

            ///Increment the number of cameras supported
            mCamerasSupported++;
            CAMHAL_LOGVB("Incrementing the number of cameras supported %d",mCamerasSupported);

            ///Create the properties array and populate all keys
            CameraProperties::CameraProperty** t = (CameraProperties::CameraProperty**)mCameraProps[curCameraIndex];
            ret = createPropertiesArray(t);
            if ( NO_ERROR != ret)
                {
                CAMHAL_LOGEA("Allocation failed for properties array");
                LOG_FUNCTION_NAME_EXIT
                return ret;
                }
            while(1)
                {
                propertyIndex = 0;

                if ( NULL == nextName )
                    {
                    ret = xmlTextReaderRead(reader);
                    if ( 1 != ret)
                        {
                        CAMHAL_LOGEA("XML File Reached end prematurely or parsing error");
                        break;
                        }
                    ///Get the tag name
                    name = xmlTextReaderConstName(reader);
                    CAMHAL_LOGVB("Found tag:%s", name);
                    }
                else
                    {
                    name = nextName;
                    }

                ///Check if the tag is CameraInstance, if so we are done with properties for current camera
                ///Move on to next one if present
                if ( strcmp((const char *) name, "CameraInstance") == 0)
                    {
                    ///End of camera element
                    if ( xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT )
                        {
                        CAMHAL_LOGVA("CameraInstance close tag found");
                        }
                    else
                        {
                        CAMHAL_LOGVA("CameraInstance close tag not found. Please check properties XML file");
                        }

                    break;

                    }

                ///The next tag should be #text and should contain the value, else process it next time around as regular tag
                ret = xmlTextReaderRead(reader);
                if(1 != ret)
                    {
                    CAMHAL_LOGVA("XML File Reached end prematurely or parsing error");
                    break;
                    }

                ///Get the next tag name
                nextName = xmlTextReaderConstName(reader);
                CAMHAL_LOGVB("Found next tag:%s", name);
                if ( strcmp((const char *) nextName, TEXT_XML_ELEMENT) == 0)
                    {
                    nextName = NULL;
                    value = xmlTextReaderConstValue(reader);
                    strcpy(val, (const char *) value);
                    value = (const xmlChar *) val;
                    CAMHAL_LOGVB("Found next tag value: %s", value);
                    }

                ///Read the closing tag
                ret = xmlTextReaderRead(reader);
                if ( xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT )
                    {
                    CAMHAL_LOGVA("Found matching ending tag");
                    }
                else
                    {
                    CAMHAL_LOGVA("Couldn't find matching ending tag");
                    }
                ///Read the #text tag for closing tag
                ret = xmlTextReaderRead(reader);

                CAMHAL_LOGVB("Tag Name %s Tag Value %s", name, value);
                if ( (propertyIndex = getCameraPropertyIndex( (const char *) name)))
                    {
                    ///If the property already exists, update it with the new value
                    CAMHAL_LOGVB("Found matching property, updating property entry for %s=%s", name, value);
                    CAMHAL_LOGVB("mCameraProps[curCameraIndex][propertyIndex] = 0x%x", (unsigned int)mCameraProps[curCameraIndex][propertyIndex]);
                    ret = mCameraProps[curCameraIndex][propertyIndex]->setValue((const char *) value);
                    if(NO_ERROR != ret)
                        {
                        CAMHAL_LOGEB("Cannot set value for key %s value %s", name, value );
                        LOG_FUNCTION_NAME_EXIT
                        return ret;
                        }
                    CAMHAL_LOGVA("Updated property..");
                    }
                }
            //skip #text tag
            ret = xmlTextReaderRead(reader);
            if(1 != ret)
                {
                CAMHAL_LOGVA("Completed parsing the XML file");
                ret = NO_ERROR;
                goto exit;
                }

            ret = xmlTextReaderRead(reader);
            if(1 != ret)
                {
                CAMHAL_LOGVA("Completed parsing the XML file");
                ret = NO_ERROR;
                goto exit;
                }

            ///Get the tag name
            name = xmlTextReaderConstName(reader);
            nextName = NULL;
            CAMHAL_LOGVB("Found tag:%s", name);
            if ( ( xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT ) &&
                    ( strcmp( (const char *) name, CAMERA_ROOT) == 0 ) )
                {
                CAMHAL_LOGVA("Completed parsing the XML file");
                ret = NO_ERROR;
                goto exit;
                }

            //skip #text tag
            xmlTextReaderRead(reader);
            if(1 != ret)
                {
                CAMHAL_LOGVA("Completed parsing the XML file");
                ret = NO_ERROR;
                goto exit;
                }

            }
        }

exit:

    LOG_FUNCTION_NAME_EXIT

    return ret;
}

status_t CameraProperties::createPropertiesArray(CameraProperties::CameraProperty** &cameraProps)
{
    int i;

    LOG_FUNCTION_NAME

    for( i = 0 ; i < CameraProperties::PROP_INDEX_MAX; i++)
        {

        ///Creating key with NULL value
        cameraProps[i] = new CameraProperties::CameraProperty( (const char *) getCameraPropertyKey((CameraProperties::CameraPropertyIndex)i), "");
        if ( NULL == cameraProps[i] )
            {
            CAMHAL_LOGEB("Allocation failed for camera property class for key %s"
                                , getCameraPropertyKey( (CameraProperties::CameraPropertyIndex) i));

            goto no_memory;
            }
        }

    LOG_FUNCTION_NAME_EXIT

    return NO_ERROR;

no_memory:

    for( int j = 0 ; j < i; j++)
        {
        delete cameraProps[j];
        }

    LOG_FUNCTION_NAME_EXIT

    return NO_MEMORY;
}

status_t CameraProperties::freeCameraProps(int cameraIndex)
{
    LOG_FUNCTION_NAME

    if(cameraIndex>=mCamerasSupported)
        {
        return BAD_VALUE;
        }

    CAMHAL_LOGVB("Freeing properties for camera index %d", cameraIndex);
    ///Free the property array for the given camera
    for(int i=0;i<CameraProperties::PROP_INDEX_MAX;i++)
        {
        if(mCameraProps[cameraIndex][i])
            {
            delete mCameraProps[cameraIndex][i];
            mCameraProps[cameraIndex][i] = NULL;
            }
        }

    CAMHAL_LOGVA("Freed properties");

    ///Move the camera properties for the other cameras ahead
    int j=cameraIndex;
    for(int i=cameraIndex+1;i<mCamerasSupported;i--,j++)
        {
        for(int k=0;k<CameraProperties::PROP_INDEX_MAX;k++)
            {
            mCameraProps[j][k] = mCameraProps[i][k];
            }
        }
    CAMHAL_LOGVA("Rearranged Property array");

    ///Decrement the number of Cameras supported by 1
    mCamerasSupported--;

    CAMHAL_LOGVB("Number of cameras supported is now %d", mCamerasSupported);

    LOG_FUNCTION_NAME_EXIT

    return NO_ERROR;
}

CameraProperties::CameraPropertyIndex CameraProperties::getCameraPropertyIndex(const char* propName)
{
    LOG_FUNCTION_NAME

    CAMHAL_LOGVB("Property name = %s", propName);

    ///Do a string comparison on the property name passed with the supported property keys
    ///and return the corresponding property index
    if(!strcmp(propName,CameraProperties::PROP_KEY_CAMERA_NAME))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_CAMERA_NAME");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_CAMERA_NAME;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_ADAPTER_DLL_NAME))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_CAMERA_ADAPTER_DLL_NAME");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_CAMERA_ADAPTER_DLL_NAME;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_SIZES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_PREVIEW_SIZES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_SIZES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FORMATS))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_PREVIEW_FORMATS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_FORMATS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FRAME_RATES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_PREVIEW_FRAME_RATES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_FRAME_RATES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_PICTURE_SIZES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_PICTURE_SIZES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_PICTURE_SIZES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_PICTURE_FORMATS))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_PICTURE_FORMATS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_PICTURE_FORMATS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_THUMBNAIL_SIZES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_THUMBNAIL_SIZES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_THUMBNAIL_SIZES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_WHITE_BALANCE))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_WHITE_BALANCE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_WHITE_BALANCE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_EFFECTS))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_EFFECTS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_EFFECTS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_ANTIBANDING))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_ANTIBANDING");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_ANTIBANDING;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_SCENE_MODES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_SCENE_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_SCENE_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_FLASH_MODES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_FLASH_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_FLASH_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_FOCUS_MODES))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_SUPPORTED_FOCUS_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_FOCUS_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_REQUIRED_PREVIEW_BUFS))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_REQUIRED_PREVIEW_BUFS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_REQUIRED_PREVIEW_BUFS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_REQUIRED_IMAGE_BUFS))
        {
        CAMHAL_LOGVA("Returning PROP_INDEX_REQUIRED_IMAGE_BUFS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_REQUIRED_IMAGE_BUFS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_EXPOSURE_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_EXPOSURE_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_EXPOSURE_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_EV_MIN))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_EV_MIN");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_EV_MIN;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_EV_MAX))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_EV_MAX");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_EV_MAX;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_EV_STEP))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_EV_STEP");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_EV_STEP;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_ISO_VALUES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_ISO_VALUES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_ISO_VALUES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_SCENE_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_SCENE_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_SCENE_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_FLASH_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_FLASH_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_FLASH_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_FOCUS_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_FOCUS_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_FOCUS_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_ZOOM_RATIOS))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_ZOOM_RATIOS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_ZOOM_RATIOS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_ZOOM_STAGES))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SUPPORTED_ZOOM_STAGES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_ZOOM_STAGES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_ZOOM_SUPPORTED))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_ZOOM_SUPPORTED");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_ZOOM_SUPPORTED;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SMOOTH_ZOOM_SUPPORTED))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_SMOOTH_ZOOM_SUPPORTED");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SMOOTH_ZOOM_SUPPORTED;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_PREVIEW_SIZE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_PREVIEW_SIZE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_PREVIEW_SIZE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_PREVIEW_FORMAT))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_PREVIEW_FORMAT");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_PREVIEW_FORMAT;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_PREVIEW_FRAME_RATE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_PREVIEW_FRAME_RATE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_PREVIEW_FRAME_RATE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_ZOOM))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_ZOOM");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_ZOOM;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_PICTURE_SIZE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_PICTURE_SIZE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_PICTURE_SIZE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_PICTURE_FORMAT))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_PICTURE_FORMAT");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_PICTURE_FORMAT;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_JPEG_THUMBNAIL_SIZE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_JPEG_THUMBNAIL_SIZE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_JPEG_THUMBNAIL_SIZE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_WHITEBALANCE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_WHITEBALANCE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_WHITEBALANCE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_EFFECT))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_EFFECT");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_EFFECT;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_ANTIBANDING))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_ANTIBANDING");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_ANTIBANDING;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_EXPOSURE_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_EXPOSURE_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_EXPOSURE_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_EV_COMPENSATION))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_EV_COMPENSATION");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_EV_COMPENSATION;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_ISO_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_ISO_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_ISO_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_FOCUS_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_FOCUS_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_FOCUS_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SCENE_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_SCENE_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SCENE_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_FLASH_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_FLASH_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_FLASH_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_CONTRAST))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_CONTRAST");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_CONTRAST;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_BRIGHTNESS))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_BRIGHTNESS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_BRIGHTNESS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SATURATION))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_SATURATION");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SATURATION;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SHARPNESS))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_SHARPNESS");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SHARPNESS;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_CAMERA_SENSOR_INDEX))
        {
        CAMHAL_LOGDA("Returning PROP_INDEX_CAMERA_SENSOR_INDEX");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_CAMERA_SENSOR_INDEX;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_SUPPORTED_IPP_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_SUPPORTED_IPP_MODES)");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_SUPPORTED_IPP_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_IPP))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_IPP");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_IPP;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_S3D_SUPPORTED))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_S3D_SUPPORTED");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_S3D_SUPPORTED;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_S3D2D_PREVIEW))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_S3D2D_PREVIEW");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_S3D2D_PREVIEW;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_S3D2D_PREVIEW_MODES))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_S3D2D_PREVIEW_MODES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_S3D2D_PREVIEW_MODES;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_AUTOCONVERGENCE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_AUTOCONVERGENCE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_AUTOCONVERGENCE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_AUTOCONVERGENCE_MODE))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_AUTOCONVERGENCE_MODE");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_AUTOCONVERGENCE_MODE;
        }
    else if(!strcmp(propName,CameraProperties::PROP_KEY_MANUALCONVERGENCE_VALUES))
        {
        CAMHAL_LOGDA("Returning PROP_KEY_MANUALCONVERGENCE_VALUES");
        LOG_FUNCTION_NAME_EXIT
        return CameraProperties::PROP_INDEX_MANUALCONVERGENCE_VALUES;
        }


    CAMHAL_LOGVA("Returning PROP_INDEX_INVALID");
    LOG_FUNCTION_NAME_EXIT
    return CameraProperties::PROP_INDEX_INVALID;

}

const char* CameraProperties::getCameraPropertyKey(CameraProperties::CameraPropertyIndex index)
{
    LOG_FUNCTION_NAME

    CAMHAL_LOGVB("Property index = %d", index);

    switch(index)
        {
        case CameraProperties::PROP_INDEX_INVALID:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_INVALID );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_INVALID;

        case CameraProperties::PROP_INDEX_CAMERA_NAME:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_CAMERA_NAME );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_CAMERA_NAME;

        case CameraProperties::PROP_INDEX_CAMERA_ADAPTER_DLL_NAME:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ADAPTER_DLL_NAME );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_ADAPTER_DLL_NAME;

        case CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_SIZES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ADAPTER_DLL_NAME );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_SIZES;

        case CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_FORMATS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_SIZES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FORMATS;

        case CameraProperties::PROP_INDEX_SUPPORTED_PREVIEW_FRAME_RATES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FORMATS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_PREVIEW_FRAME_RATES;

        case CameraProperties::PROP_INDEX_SUPPORTED_PICTURE_SIZES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_PICTURE_SIZES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_PICTURE_SIZES;

        case CameraProperties::PROP_INDEX_SUPPORTED_PICTURE_FORMATS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_PICTURE_FORMATS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_PICTURE_FORMATS;

        case CameraProperties::PROP_INDEX_SUPPORTED_THUMBNAIL_SIZES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_THUMBNAIL_SIZES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_THUMBNAIL_SIZES;

        case CameraProperties::PROP_INDEX_SUPPORTED_WHITE_BALANCE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_WHITE_BALANCE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_WHITE_BALANCE;

        case CameraProperties::PROP_INDEX_SUPPORTED_EFFECTS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_EFFECTS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_EFFECTS;

        case CameraProperties::PROP_INDEX_SUPPORTED_ANTIBANDING:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_ANTIBANDING );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_ANTIBANDING;

        case CameraProperties::PROP_INDEX_SUPPORTED_EXPOSURE_MODES:
            CAMHAL_LOGDB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_EXPOSURE_MODES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_EXPOSURE_MODES;

        case CameraProperties::PROP_INDEX_SUPPORTED_EV_MIN:
            CAMHAL_LOGDB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_EV_MIN );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_EV_MIN;

        case CameraProperties::PROP_INDEX_SUPPORTED_EV_MAX:
            CAMHAL_LOGDB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_EV_MAX );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_EV_MAX;

        case CameraProperties::PROP_INDEX_SUPPORTED_EV_STEP:
            CAMHAL_LOGDB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_EV_STEP );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_EV_STEP;

        case CameraProperties::PROP_INDEX_SUPPORTED_ISO_VALUES:
            CAMHAL_LOGDB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_ISO_VALUES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_ISO_VALUES;

        case CameraProperties::PROP_INDEX_SUPPORTED_SCENE_MODES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_SCENE_MODES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_SCENE_MODES;

        case CameraProperties::PROP_INDEX_SUPPORTED_FLASH_MODES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_FLASH_MODES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_FLASH_MODES;

        case CameraProperties::PROP_INDEX_SUPPORTED_FOCUS_MODES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_FOCUS_MODES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_FOCUS_MODES;

        case CameraProperties::PROP_INDEX_REQUIRED_PREVIEW_BUFS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_REQUIRED_PREVIEW_BUFS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_REQUIRED_PREVIEW_BUFS;

        case CameraProperties::PROP_INDEX_REQUIRED_IMAGE_BUFS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_REQUIRED_IMAGE_BUFS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_REQUIRED_IMAGE_BUFS;

        case CameraProperties::PROP_INDEX_SUPPORTED_ZOOM_RATIOS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_ZOOM_RATIOS );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_ZOOM_RATIOS;

        case CameraProperties::PROP_INDEX_SUPPORTED_ZOOM_STAGES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_ZOOM_STAGES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_ZOOM_STAGES;

        case CameraProperties::PROP_INDEX_ZOOM_SUPPORTED:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ZOOM_SUPPORTED );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_ZOOM_SUPPORTED;

        case CameraProperties::PROP_INDEX_SMOOTH_ZOOM_SUPPORTED:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SMOOTH_ZOOM_SUPPORTED );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SMOOTH_ZOOM_SUPPORTED;

        case CameraProperties::PROP_INDEX_PREVIEW_FORMAT:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_PREVIEW_FORMAT );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_PREVIEW_FORMAT;

        case CameraProperties::PROP_INDEX_PREVIEW_FRAME_RATE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_PREVIEW_FRAME_RATE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_PREVIEW_FRAME_RATE;

        case CameraProperties::PROP_INDEX_ZOOM:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ZOOM );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_ZOOM;

        case CameraProperties::PROP_INDEX_PICTURE_SIZE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_PICTURE_SIZE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_PICTURE_SIZE;

        case CameraProperties::PROP_INDEX_PICTURE_FORMAT:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_PICTURE_FORMAT );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_PICTURE_FORMAT;

        case CameraProperties::PROP_INDEX_JPEG_THUMBNAIL_SIZE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_JPEG_THUMBNAIL_SIZE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_JPEG_THUMBNAIL_SIZE;

        case CameraProperties::PROP_INDEX_WHITEBALANCE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_WHITEBALANCE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_WHITEBALANCE;

        case CameraProperties::PROP_INDEX_EFFECT:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_EFFECT );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_EFFECT;

        case CameraProperties::PROP_INDEX_ANTIBANDING:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ANTIBANDING );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_ANTIBANDING;

        case CameraProperties::PROP_INDEX_EXPOSURE_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_EXPOSURE_MODE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_EXPOSURE_MODE;

        case CameraProperties::PROP_INDEX_EV_COMPENSATION:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_EV_COMPENSATION );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_EV_COMPENSATION;

        case CameraProperties::PROP_INDEX_ISO_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_ISO_MODE);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_ISO_MODE;

        case CameraProperties::PROP_INDEX_FOCUS_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_FOCUS_MODE);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_FOCUS_MODE;

        case CameraProperties::PROP_INDEX_SCENE_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SCENE_MODE);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SCENE_MODE;

        case CameraProperties::PROP_INDEX_FLASH_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_FLASH_MODE);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_FLASH_MODE;

        case CameraProperties::PROP_INDEX_JPEG_QUALITY:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_JPEG_QUALITY);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_JPEG_QUALITY;

        case CameraProperties::PROP_INDEX_CONTRAST:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_CONTRAST);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_CONTRAST;

        case CameraProperties::PROP_INDEX_BRIGHTNESS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_BRIGHTNESS);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_BRIGHTNESS;

        case CameraProperties::PROP_INDEX_SHARPNESS:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SHARPNESS);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SHARPNESS;

        case CameraProperties::PROP_INDEX_SATURATION:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SATURATION);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SATURATION;

        case CameraProperties::PROP_INDEX_CAMERA_SENSOR_INDEX:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_CAMERA_SENSOR_INDEX );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_CAMERA_SENSOR_INDEX;

        case CameraProperties::PROP_INDEX_SUPPORTED_IPP_MODES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_SUPPORTED_IPP_MODES);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_SUPPORTED_IPP_MODES;

        case CameraProperties::PROP_INDEX_IPP:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_IPP );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_IPP;

        case CameraProperties::PROP_INDEX_S3D_SUPPORTED:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_S3D_SUPPORTED);
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_S3D_SUPPORTED;

        case CameraProperties::PROP_INDEX_AUTOCONVERGENCE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_AUTOCONVERGENCE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_AUTOCONVERGENCE;

        case CameraProperties::PROP_INDEX_AUTOCONVERGENCE_MODE:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_AUTOCONVERGENCE_MODE );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_AUTOCONVERGENCE_MODE;

        case CameraProperties::PROP_INDEX_MANUALCONVERGENCE_VALUES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_MANUALCONVERGENCE_VALUES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_MANUALCONVERGENCE_VALUES;

        case CameraProperties::PROP_INDEX_S3D2D_PREVIEW:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_S3D2D_PREVIEW );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_S3D2D_PREVIEW;

        case CameraProperties::PROP_INDEX_S3D2D_PREVIEW_MODES:
            CAMHAL_LOGVB("Returning key: %s ",CameraProperties::PROP_KEY_S3D2D_PREVIEW_MODES );
            LOG_FUNCTION_NAME_EXIT
            return CameraProperties::PROP_KEY_S3D2D_PREVIEW_MODES;


        default:
            CAMHAL_LOGVB("Returning key: %s ","none" );
            LOG_FUNCTION_NAME_EXIT
            return "none";
        }

        return "none";
}


///Returns the number of Cameras found
int CameraProperties::camerasSupported()
{
    LOG_FUNCTION_NAME
    return mCamerasSupported;
}


///Returns the properties array for a specific Camera
///Each value is indexed by the CameraProperties::CameraPropertyIndex enum
CameraProperties::CameraProperty** CameraProperties::getProperties(int cameraIndex)
{
    LOG_FUNCTION_NAME

    CAMHAL_LOGVA("Refreshing properties files");
    ///Refresh the properties file - reload property files if changed from last time
    refreshProperties();

    CAMHAL_LOGVA("Refreshed properties file");

    if(cameraIndex>=mCamerasSupported)
        {
        LOG_FUNCTION_NAME_EXIT
        return NULL;
        }

    ///Return the Camera properties for the requested camera index
    LOG_FUNCTION_NAME_EXIT

    return ( CameraProperties::CameraProperty **) mCameraProps[cameraIndex];
}

void CameraProperties::refreshProperties()
{
    LOG_FUNCTION_NAME
    ///@todo Implement this function
    return;
}

status_t CameraProperties::CameraProperty::setValue(const char * value)
{
    CAMHAL_LOGVB("setValue = %s", value);
    if(!value)
        {
        return BAD_VALUE;
        }
    strcpy(mPropValue, value);
    CAMHAL_LOGVB("mPropValue = %s", mPropValue);
    return NO_ERROR;
}


};

