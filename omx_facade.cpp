
#include "omx_facade.h"

#include "decode_functions.h"
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

void OMXFacade::decode_file(const std::string &filename)
{
  OMXComponent decode_component("OMX.broadcom.video_decode");
  decode_component.setup_ports();
  decode_component.print_state();
  decode_component.enable_ports(false);
  decode_component.set_video_format(130, OMX_VIDEO_CodingAVC);
  decode_component.enable_ports(true, {130});
  decode_component.change_state(OMX_StateIdle);
  decode_component.allocate_buffers({130});
  decode_component.wait_state(OMX_StateIdle);
  decode_component.print_state();
  sleep(1);
  decode_component.change_state(OMX_StateExecuting);
  decode_component.wait_state(OMX_StateExecuting);
  decode_component.print_state();

  sleep(1);

  FILE *input_file = fopen(filename.c_str(), "r");
  int32_t to_read = get_file_size(filename);
  OMX_BUFFERHEADERTYPE *buff_header = decode_component.buffer_header(130);


  // Read the first block so that the component can get
// the dimensions of the video and call port settings
// changed on the output port to configure it
  bool port_settings_changed {0};
  while (!port_settings_changed) {
    read_into_buffer_and_empty(input_file, decode_component, buff_header, &to_read);
    // If all the file has been read in, then
    // we have to re-read this first block.
    // Broadcom bug?
    if (to_read <= 0) {
      Logger::trace("Decode: Rewinding");
      // wind back to start and repeat
      input_file = freopen(filename.c_str(), "r", input_file);
      to_read = get_file_size(filename);
    }
    sleep(1);
  }
}