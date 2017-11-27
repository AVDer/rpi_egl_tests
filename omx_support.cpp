#include "omx_support.h"

const std::string omx_error_to_string(OMX_ERRORTYPE err)
{
  switch (err)
  {
  case OMX_ErrorInsufficientResources:
    return "OMX_ErrorInsufficientResources";
  case OMX_ErrorUndefined:
    return "OMX_ErrorUndefined";
  case OMX_ErrorInvalidComponentName:
    return "OMX_ErrorInvalidComponentName";
  case OMX_ErrorComponentNotFound:
    return "OMX_ErrorComponentNotFound";
  case OMX_ErrorInvalidComponent:
    return "OMX_ErrorInvalidComponent";
  case OMX_ErrorBadParameter:
    return "OMX_ErrorBadParameter";
  case OMX_ErrorNotImplemented:
    return "OMX_ErrorNotImplemented";
  case OMX_ErrorUnderflow:
    return "OMX_ErrorUnderflow";
  case OMX_ErrorOverflow:
    return "OMX_ErrorOverflow";
  case OMX_ErrorHardware:
    return "OMX_ErrorHardware";
  case OMX_ErrorInvalidState:
    return "OMX_ErrorInvalidState";
  case OMX_ErrorStreamCorrupt:
    return "OMX_ErrorStreamCorrupt";
  case OMX_ErrorPortsNotCompatible:
    return "OMX_ErrorPortsNotCompatible";
  case OMX_ErrorResourcesLost:
    return "OMX_ErrorResourcesLost";
  case OMX_ErrorNoMore:
    return "OMX_ErrorNoMore";
  case OMX_ErrorVersionMismatch:
    return "OMX_ErrorVersionMismatch";
  case OMX_ErrorNotReady:
    return "OMX_ErrorNotReady";
  case OMX_ErrorTimeout:
    return "OMX_ErrorTimeout";
  case OMX_ErrorSameState:
    return "OMX_ErrorSameState";
  case OMX_ErrorResourcesPreempted:
    return "OMX_ErrorResourcesPreempted";
  case OMX_ErrorPortUnresponsiveDuringAllocation:
    return "OMX_ErrorPortUnresponsiveDuringAllocation ";
  case OMX_ErrorPortUnresponsiveDuringDeallocation:
    return "OMX_ErrorPortUnresponsiveDuringDeallocation ";
  case OMX_ErrorPortUnresponsiveDuringStop:
    return "OMX_ErrorPortUnresponsiveDuringStop";
  case OMX_ErrorIncorrectStateTransition:
    return "OMX_ErrorIncorrectStateTransition";
  case OMX_ErrorIncorrectStateOperation:
    return "OMX_ErrorIncorrectStateOperation";
  case OMX_ErrorUnsupportedSetting:
    return "OMX_ErrorUnsupportedSetting";
  case OMX_ErrorUnsupportedIndex:
    return "OMX_ErrorUnsupportedIndex";
  case OMX_ErrorBadPortIndex:
    return "OMX_ErrorBadPortIndex";
  case OMX_ErrorPortUnpopulated:
    return "OMX_ErrorPortUnpopulated";
  case OMX_ErrorComponentSuspended:
    return "OMX_ErrorComponentSuspended";
  case OMX_ErrorDynamicResourcesUnavailable:
    return "OMX_ErrorDynamicResourcesUnavailable ";
  case OMX_ErrorMbErrorsInFrame:
    return "OMX_ErrorMbErrorsInFrame";
  case OMX_ErrorFormatNotDetected:
    return "OMX_ErrorFormatNotDetected";
  case OMX_ErrorContentPipeOpenFailed:
    return "OMX_ErrorContentPipeOpenFailed";
  case OMX_ErrorContentPipeCreationFailed:
    return "OMX_ErrorContentPipeCreationFailed";
  case OMX_ErrorSeperateTablesUsed:
    return "OMX_ErrorSeperateTablesUsed";
  case OMX_ErrorTunnelingUnsupported:
    return "OMX_ErrorTunnelingUnsupported";
  default:
    return "OMX unknown error";
  }
}

const std::string omx_state_to_string(OMX_STATETYPE state)
{
  switch (state)
  {
  case OMX_StateLoaded:
    return "StateLoaded";
  case OMX_StateIdle:
    return "StateIdle";
  case OMX_StateExecuting:
    return "StateExecuting";
  case OMX_StatePause:
    return "StatePause";
  case OMX_StateWaitForResources:
    return "StateWait";
  case OMX_StateInvalid:
    return "StateInvalid";
  default:
    return "State unknown";
  }
}

const std::string omx_vcodec_to_string(OMX_VIDEO_CODINGTYPE codec)
{
  switch (codec)
  {
  case OMX_VIDEO_CodingUnused:
    return "Unknown";
  case OMX_VIDEO_CodingAutoDetect:
    return "Autodetect";
  case OMX_VIDEO_CodingMPEG2:
    return "MPEG-2/H.262";
  case OMX_VIDEO_CodingH263:
    return "H.263";
  case OMX_VIDEO_CodingMPEG4:
    return "MPEG-4";
  case OMX_VIDEO_CodingWMV:
    return "WMV";
  case OMX_VIDEO_CodingRV:
    return "Real video";
  case OMX_VIDEO_CodingAVC:
    return "H.264/AVC";
  case OMX_VIDEO_CodingMJPEG:
    return "Motion JPEG";
  default:
    return "Non standard";
  }
}

const std::string omx_event_to_string(OMX_EVENTTYPE event)
{
  switch (event)
  {
  case OMX_EventCmdComplete:
    return "OMX_EventCmdComplete";
  case OMX_EventError:
    return "OMX_EventError";
  case OMX_EventMark:
    return "OMX_EventMark";
  case OMX_EventPortSettingsChanged:
    return "OMX_EventPortSettingsChanged";
  case OMX_EventBufferFlag:
    return "OMX_EventBufferFlag";
  case OMX_EventResourcesAcquired:
    return "OMX_EventResourcesAcquired";
  default:
    return "Unknown event";
  }
}

const std::string omx_command_to_string(OMX_COMMANDTYPE command)
{
  switch (command)
  {
  case OMX_CommandStateSet:
    return "OMX_CommandStateSet";
  case OMX_CommandFlush:
    return "OMX_CommandFlush";
  case OMX_CommandPortDisable:
    return "OMX_CommandPortDisable";
  case OMX_CommandPortEnable:
    return "OMX_CommandPortEnable";
  case OMX_CommandMarkBuffer:
    return "OMX_CommandMarkBuffer";
  default:
    return "Unknown command";
  }
}

const std::string omx_color_format_to_string(OMX_COLOR_FORMATTYPE format)
{
  switch (format)
  {
  case OMX_COLOR_FormatUnused:
    return "OMX_COLOR_FormatUnused";
  case OMX_COLOR_FormatMonochrome:
    return "OMX_COLOR_FormatMonochrome";
  case OMX_COLOR_Format8bitRGB332:
    return "OMX_COLOR_Format8bitRGB332";
  case OMX_COLOR_Format12bitRGB444:
    return "OMX_COLOR_Format12bitRGB444";
  case OMX_COLOR_Format16bitARGB4444:
    return "OMX_COLOR_Format16bitARGB4444";
  case OMX_COLOR_Format16bitARGB1555:
    return "OMX_COLOR_Format16bitARGB1555";
  case OMX_COLOR_Format16bitRGB565:
    return "OMX_COLOR_Format16bitRGB565";
  case OMX_COLOR_Format16bitBGR565:
    return "OMX_COLOR_Format16bitBGR565";
  case OMX_COLOR_Format18bitRGB666:
    return "OMX_COLOR_Format18bitRGB666";
  case OMX_COLOR_Format18bitARGB1665:
    return "OMX_COLOR_Format18bitARGB1665";
  case OMX_COLOR_Format19bitARGB1666:
    return "OMX_COLOR_Format19bitARGB1666";
  case OMX_COLOR_Format24bitRGB888:
    return "OMX_COLOR_Format24bitRGB888";
  case OMX_COLOR_Format24bitBGR888:
    return "OMX_COLOR_Format24bitBGR888";
  case OMX_COLOR_Format24bitARGB1887:
    return "OMX_COLOR_Format24bitARGB1887";
  case OMX_COLOR_Format25bitARGB1888:
    return "OMX_COLOR_Format25bitARGB1888";
  case OMX_COLOR_Format32bitBGRA8888:
    return "OMX_COLOR_Format32bitBGRA8888";
  case OMX_COLOR_Format32bitARGB8888:
    return "OMX_COLOR_Format32bitARGB8888";
  case OMX_COLOR_FormatYUV411Planar:
    return "OMX_COLOR_FormatYUV411Planar";
  case OMX_COLOR_FormatYUV411PackedPlanar:
    return "OMX_COLOR_FormatYUV411PackedPlanar";
  case OMX_COLOR_FormatYUV420Planar:
    return "OMX_COLOR_FormatYUV420Planar";
  case OMX_COLOR_FormatYUV420PackedPlanar:
    return "OMX_COLOR_FormatYUV420PackedPlanar";
  case OMX_COLOR_FormatYUV420SemiPlanar:
    return "OMX_COLOR_FormatYUV420SemiPlanar";
  case OMX_COLOR_FormatYUV422Planar:
    return "OMX_COLOR_FormatYUV422Planar";
  case OMX_COLOR_FormatYUV422PackedPlanar:
    return "OMX_COLOR_FormatYUV422PackedPlanar";
  case OMX_COLOR_FormatYUV422SemiPlanar:
    return "OMX_COLOR_FormatYUV422SemiPlanar";
  case OMX_COLOR_FormatYCbYCr:
    return "OMX_COLOR_FormatYCbYCr";
  case OMX_COLOR_FormatYCrYCb:
    return "OMX_COLOR_FormatYCrYCb";
  case OMX_COLOR_FormatCbYCrY:
    return "OMX_COLOR_FormatCbYCrY";
  case OMX_COLOR_FormatCrYCbY:
    return "OMX_COLOR_FormatCrYCbY";
  case OMX_COLOR_FormatYUV444Interleaved:
    return "OMX_COLOR_FormatYUV444Interleaved";
  case OMX_COLOR_FormatRawBayer8bit:
    return "OMX_COLOR_FormatRawBayer8bit";
  case OMX_COLOR_FormatRawBayer10bit:
    return "OMX_COLOR_FormatRawBayer10bit";
  case OMX_COLOR_FormatRawBayer8bitcompressed:
    return "OMX_COLOR_FormatRawBayer8bitcompressed";
  case OMX_COLOR_FormatL2:
    return "OMX_COLOR_FormatL2";
  case OMX_COLOR_FormatL4:
    return "OMX_COLOR_FormatL4";
  case OMX_COLOR_FormatL8:
    return "OMX_COLOR_FormatL8";
  case OMX_COLOR_FormatL16:
    return "OMX_COLOR_FormatL16";
  case OMX_COLOR_FormatL24:
    return "OMX_COLOR_FormatL24";
  case OMX_COLOR_FormatL32:
    return "OMX_COLOR_FormatL32";
  case OMX_COLOR_FormatMax:
    return "OMX_COLOR_FormatMax";
  default:
    return "Unknown color format";
  }
}