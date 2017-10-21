
#include "omx_facade.h"

#include <iostream>

OMXFacade::OMXFacade()
{
  bcm_host_init();

  OMX_ERRORTYPE err{OMX_Init()};
  if (err != OMX_ErrorNone)
  {
    std::cout << "OMX_Init() failed" << std::endl;
  }
}

void OMXFacade::check_roles()
{
  char name[OMX_MAX_STRINGNAME_SIZE];
  OMX_ERRORTYPE err{OMX_ErrorNone};
  for (uint32_t i = 0; OMX_ErrorNoMore != err; ++i)
  {
    err = OMX_ComponentNameEnum(name, OMX_MAX_STRINGNAME_SIZE, i);
    if (OMX_ErrorNone == err)
    {
      std::cout << "Component is: " << name << std::endl;
      //list_roles(name);
    }
  }
  std::cout << "No more components" << std::endl;
}

void OMXFacade::list_roles(char *name)
{
  OMX_U32 roles_number;
  OMX_U8 *roles[MAX_ROLES_NUMBER];
  /* get the number of roles by passing in a NULL roles param */
  OMX_ERRORTYPE err = OMX_GetRolesOfComponent(name, &roles_number, nullptr);
  if (err != OMX_ErrorNone)
  {
    std::cout << "Getting roles failed" << std::endl;
    return;
  }

  std::cout << "Number of roles is: " << roles_number << std::endl;

  if (roles_number > MAX_ROLES_NUMBER)
  {
    std::cout << "Too many roles to list" << std::endl;
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
    std::cout << "Getting roles failed" << std::endl;
    return;
  }
  for (OMX_U32 n = 0; n < roles_number; ++n)
  {
    std::cout << "Role: " << roles[n] << std::endl;
    free(roles[n]);
  }
}

void OMXFacade::get_port_info(const std::string &component_sname, uint32_t port_index)
{
  OMX_CALLBACKTYPE callbacks = {.EventHandler = nullptr,
                                .EmptyBufferDone = nullptr,
                                .FillBufferDone = nullptr};

  char component_name[128]{0};
  strcpy(component_name, component_sname.c_str());

  // Ask the core for a handle to the component
  OMX_HANDLETYPE handle;
  OMX_ERRORTYPE err = OMX_GetHandle(&handle, component_name, NULL, &callbacks);
  if (err != OMX_ErrorNone)
  {
    std::cerr << "OMX_GetHandle faile" << std::endl;
    return;
  }
  // Get some version info
  OMX_VERSIONTYPE spec_version;
  OMX_VERSIONTYPE comp_version;
  OMX_UUIDTYPE uid;
  err = OMX_GetComponentVersion(handle, component_name,
                                &comp_version, &spec_version,
                                &uid);
  if (err != OMX_ErrorNone)
  {
    std::cerr << "OMX_GetComponentVersion failed" << std::endl;
    return;
  }
  std::cout << "Component name: " << component_name
            << " version " << comp_version.s.nVersionMajor << '.' << comp_version.s.nVersionMinor
            << ", Spec version: " << spec_version.s.nVersionMajor << '.' << spec_version.s.nVersionMinor << std::endl;

  OMX_PARAM_PORTDEFINITIONTYPE port_definition;
  memset(&port_definition, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  port_definition.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
  port_definition.nVersion.nVersion = OMX_VERSION;
  port_definition.nPortIndex = port_index;
  if (get_port_info(handle, &port_definition) == OMX_ErrorNone)
  {
    print_port_info(&port_definition);
  }
}

OMX_ERRORTYPE OMXFacade::get_port_info(OMX_HANDLETYPE handle, OMX_PARAM_PORTDEFINITIONTYPE *portdef)
{
  return OMX_GetParameter(handle, OMX_IndexParamPortDefinition, portdef);
}

void OMXFacade::print_port_info(OMX_PARAM_PORTDEFINITIONTYPE *port_definition)
{
  std::string domain;
  std::cout << "Port " << port_definition->nPortIndex;
  std::cout << ((port_definition->eDir == OMX_DirInput) ? " is input port" : " is output port") << std::endl;

  switch (port_definition->eDomain)
  {
  case OMX_PortDomainAudio:
    domain = "Audio";
    break;
  case OMX_PortDomainVideo:
    domain = "Video";
    break;
  case OMX_PortDomainImage:
    domain = "Image";
    break;
  case OMX_PortDomainOther:
    domain = "Other";
    break;
  default:
    break;
  }

  std::cout << " Domain is " << domain << '\n'
            << "Buffer count " << port_definition->nBufferCountActual << '\n'
            << "Buffer minimum count " << port_definition->nBufferCountMin << '\n'
            << "Buffer size " << port_definition->nBufferSize << " bytes" << std::endl;
}
