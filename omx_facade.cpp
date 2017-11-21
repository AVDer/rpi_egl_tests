
#include "omx_facade.h"

#include "logger.h"
#include "omx_component.h"
#include "omx_support.h"
#include "support_functions.h"

OMXFacade::OMXFacade()
{
  bcm_host_init();

  OMX_ERRORTYPE err{OMX_Init()};
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: OMX_Init() failed");
  }
}

void OMXFacade::list_components()
{
  char name[OMX_MAX_STRINGNAME_SIZE];
  OMX_ERRORTYPE err{OMX_ErrorNone};
  for (uint32_t i = 0; OMX_ErrorNoMore != err; ++i)
  {
    err = OMX_ComponentNameEnum(name, OMX_MAX_STRINGNAME_SIZE, i);
    if (OMX_ErrorNone == err)
    {
      Logger::debug("OMX: Component is: %s", name);
      list_roles(name);
    }
  }
  Logger::debug("OMX: No more components", name);
}

void OMXFacade::list_roles(char *name)
{
  OMX_U32 roles_number;
  OMX_U8 *roles[MAX_ROLES_NUMBER];
  /* get the number of roles by passing in a NULL roles param */
  OMX_ERRORTYPE err = OMX_GetRolesOfComponent(name, &roles_number, nullptr);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: Getting roles failed");
    return;
  }

  Logger::debug("OMX: Number of roles is: %d", roles_number);

  if (roles_number > MAX_ROLES_NUMBER)
  {
    Logger::error("OMX: Too many roles to list");
    return;
  }

  /* now get the roles */
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    roles[n] = (OMX_U8 *)malloc(OMX_MAX_STRINGNAME_SIZE);
  }
  err = OMX_GetRolesOfComponent(name, &roles_number, roles);
  if (err != OMX_ErrorNone)
  {
    Logger::error("OMX: Getting roles failed");
    return;
  }
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    Logger::debug("OMX: Role: %s", roles[n]);
    free(roles[n]);
  }
}

OMX_ERRORTYPE read_into_buffer_and_empty(FILE *fp, OMXComponent component, OMX_BUFFERHEADERTYPE *buff_header, int *toread)
{
  int buff_size = buff_header->nAllocLen;
  int nread = fread(buff_header->pBuffer, 1, buff_size, fp);
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

void OMXFacade::decode_file(const std::string &filename)
{
  get_file_size(filename);
  OMXComponent component("OMX.broadcom.video_decode");
  component.setup_ports();
  component.print_state();
  component.enable_ports(false);
  component.change_state(OMX_StateIdle);
  component.set_video_format(130, OMX_VIDEO_CodingAVC);
  component.enable_ports(true, {130});
  component.allocate_buffers({130});  
  component.wait_state(OMX_StateIdle);
  component.print_state();
  component.change_state(OMX_StateExecuting);
  component.wait_state(OMX_StateExecuting);
  component.print_state();
  sleep(1);
}