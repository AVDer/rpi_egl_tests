#ifndef RPI3_DECODE_FUNC_H
#define RPI3_DECODE_FUNC_H

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

#include "logger.h"
#include "omx_component.h"
#include "omx_support.h"

class EGLWriter {
public:
  explicit EGLWriter(OMXComponent& component, OMX_BUFFERHEADERTYPE* buffer_header) :
  component_(component),
  buffer_header_(buffer_header)
    {}
  void operator()(OMX_BUFFERHEADERTYPE* buffer_header) {
    OMX_ERRORTYPE error = OMX_FillThisBuffer(component_.handle(), buffer_header);
    if (error != OMX_ErrorNone) {
      Logger::error("OMX-EGL: OMX_FillThisBuffer failed in callback: %s", omx_error_to_string(error).c_str());
    }
  }
private:
  OMXComponent& component_;
  OMX_BUFFERHEADERTYPE* buffer_header_;
};

OMX_ERRORTYPE read_into_buffer_and_empty(FILE *fp, OMXComponent& component, OMX_BUFFERHEADERTYPE *buff_header, int32_t *toread)
{
  OMX_U32 buff_size = buff_header->nAllocLen;
  size_t nread = fread(buff_header->pBuffer, 1, buff_size, fp);
  buff_header->nFilledLen = nread;
  *toread -= nread;
  Logger::verbose("File: Read %d, %d still left", nread, *toread);
  if (*toread <= 0)
  {
    Logger::verbose("File: Setting EOS on input");
    buff_header->nFlags |= OMX_BUFFERFLAG_EOS;
  }
  OMX_ERRORTYPE error = OMX_EmptyThisBuffer(component.handle(), buff_header);
  if (error != OMX_ErrorNone)
  {
    Logger::error("File: Empty buffer error %s", omx_error_to_string(error).c_str());
  }
  return error;
}

OMX_ERRORTYPE save_info_from_filled_buffer(OMXComponent& component, OMX_BUFFERHEADERTYPE * buff_header) {
  Logger::verbose("File: Got a filled buffer with %d, allocated %d", buff_header->nFilledLen, buff_header->nAllocLen);
  if (buff_header->nFlags & OMX_BUFFERFLAG_EOS) {
    Logger::verbose("File: Got EOS on output");
    exit(0);
  }
  // and then refill it
  OMX_ERRORTYPE error = OMX_FillThisBuffer(component.handle(), buff_header);
  if (error != OMX_ErrorNone) {
    Logger::error("File: Fill buffer error %s", omx_error_to_string(error).c_str());
  }
  return error;
}

#endif