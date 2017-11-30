#include "omx_facade.h"

#include "decode_functions.h"
#include "logger.h"
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

void OMXFacade::setup_tunnel(OMXComponent& out_component, OMX_U32 out_port, OMXComponent& in_component, OMX_U32 in_port) {
  OMX_ERRORTYPE error = OMX_SetupTunnel(out_component.handle(), out_port, in_component.handle(), in_port);
  if (error != OMX_ErrorNone)
  {
    Logger::error("OMX: Tunnel seup failed");
    return;
  } 
}

void OMXFacade::decode_file(const std::string &filename)
{
  OMXComponent decode_component("OMX.broadcom.video_decode");
  decode_component.setup_ports();
  decode_component.print_state();
  decode_component.enable_ports(false);
  decode_component.change_state(OMX_StateIdle);
  decode_component.port(130)->set_video_format(OMX_VIDEO_CodingAVC);
  decode_component.enable_ports(true, {130});
  decode_component.allocate_buffers({130});
  decode_component.wait_state(OMX_StateIdle);
  decode_component.print_state();
  decode_component.change_state(OMX_StateExecuting);
  decode_component.wait_state(OMX_StateExecuting);
  decode_component.print_state();

  FILE *input_file = fopen(filename.c_str(), "r");
  int32_t to_read = get_file_size(filename);
  OMX_BUFFERHEADERTYPE *buff_header;

  decode_component.port(131)->set_flag(PortFlag::changed, false);

  while (!decode_component.port(131)->get_flag(PortFlag::changed)) { 
    buff_header = decode_component.port(130)->get_buffer();
    read_into_buffer_and_empty(input_file, decode_component, buff_header, &to_read);
    // If all the file has been read in, then we have to re-read this first block.
    if (to_read <= 0) {
      input_file = freopen(filename.c_str(), "r", input_file);
      to_read = get_file_size(filename);
    }
  }

  decode_component.port(131)->print_video_settings();
  decode_component.enable_ports(true, {131});
  decode_component.allocate_buffers({131});
  // now work through the file
  while (to_read > 0) {
    // do we have an input buffer we can fill and empty?
    buff_header = decode_component.port(130)->get_buffer(false);
    if (buff_header != nullptr) {
      read_into_buffer_and_empty(input_file, decode_component, buff_header, &to_read);
    }
    // do we have an output buffer that has been filled?
    buff_header = decode_component.port(131)->get_buffer(false);
    if (buff_header != nullptr) {
      save_info_from_filled_buffer(decode_component, buff_header);
    } else {
      Logger::verbose("Decode: No filled buffer");
    }
  }
}

void OMXFacade::render_file(const std::string& filename) {
  OMXComponent decode_component("OMX.broadcom.video_decode");
  decode_component.setup_ports();
  OMXComponent render_component("OMX.broadcom.video_render");
  render_component.setup_ports();

  decode_component.enable_ports(false);
  render_component.enable_ports(false);
  decode_component.change_state(OMX_StateIdle);
  decode_component.port(130)->set_video_format(OMX_VIDEO_CodingAVC);
  render_component.change_state(OMX_StateIdle);
  decode_component.enable_ports(true, {130});
  decode_component.allocate_buffers({130});
  decode_component.change_state(OMX_StateExecuting);
  decode_component.wait_state(OMX_StateExecuting);

  FILE *input_file = fopen(filename.c_str(), "r");
  int32_t to_read = get_file_size(filename);
  OMX_BUFFERHEADERTYPE *buff_header;

  decode_component.port(131)->set_flag(PortFlag::changed, false);

  while (!decode_component.port(131)->get_flag(PortFlag::changed)) { 
    buff_header = decode_component.port(130)->get_buffer();
    read_into_buffer_and_empty(input_file, decode_component, buff_header, &to_read);
    if (to_read <= 0) {
      input_file = freopen(filename.c_str(), "r", input_file);
      to_read = get_file_size(filename);
    }
  }

  decode_component.port(131)->print_video_settings();

  decode_component.change_state(OMX_StateIdle);
  decode_component.wait_state(OMX_StateIdle);

  setup_tunnel(decode_component, 131, render_component, 90);
  decode_component.enable_ports(true, {131});
  render_component.enable_ports(true, {90});
  decode_component.change_state(OMX_StateExecuting);
  decode_component.wait_state(OMX_StateExecuting);
  render_component.change_state(OMX_StateExecuting);
  render_component.wait_state(OMX_StateExecuting);

  while (to_read > 0) {
    buff_header = decode_component.port(130)->get_buffer(false);
    if (buff_header != nullptr) {
      read_into_buffer_and_empty(input_file, decode_component, buff_header, &to_read);
    }
  }
}