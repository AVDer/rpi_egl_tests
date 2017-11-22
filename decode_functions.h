#ifndef RPI3_DECODE_FUNC_H
#define RPI3_DECODE_FUNC_H

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>

#include "logger.h"
#include "omx_component.h"
#include "omx_support.h"

OMX_ERRORTYPE read_into_buffer_and_empty(FILE *fp, OMXComponent& component, OMX_BUFFERHEADERTYPE *buff_header, int32_t *toread)
{
  OMX_U32 buff_size = buff_header->nAllocLen;
  //OMX_U32 buff_size = 1;
  size_t nread = fread(buff_header->pBuffer, 1, buff_size, fp);
  buff_header->nFilledLen = nread;
  *toread -= nread;
  Logger::trace("File: Read %d, %d still left", nread, *toread);
  if (*toread <= 0)
  {
    Logger::trace("File: Setting EOS on input");
    buff_header->nFlags |= OMX_BUFFERFLAG_EOS;
  }
  OMX_ERRORTYPE error = OMX_EmptyThisBuffer(component.handle(), buff_header);
  if (error != OMX_ErrorNone)
  {
    Logger::error("File: Empty buffer error %s", omx_error_to_string(error).c_str());
  }
  return error;
}

#endif