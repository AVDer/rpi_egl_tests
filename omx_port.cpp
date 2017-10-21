#include "omx_port.h"

#include <cstring>
#include <iostream>

OMXPort::OMXPort(uint32_t port_index, const OMX_HANDLETYPE &handle) : handle_(handle)
{
  memset(&port_definition_, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  port_definition_.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
  port_definition_.nVersion.nVersion = OMX_VERSION;
  port_definition_.nPortIndex = port_index;
  if (OMX_GetParameter(handle_, OMX_IndexParamPortDefinition, &port_definition_) != OMX_ErrorNone)
  {
    std::cerr << port_index << ": Failed to get definition" << std::endl;
  }
}

void OMXPort::print_info()
{
  std::string domain;
  std::cout << "-----------------------" << std::endl;
  std::cout << "Port " << port_definition_.nPortIndex;
  std::cout << ((port_definition_.eDir == OMX_DirInput) ? " is input port" : " is output port") << std::endl;

  switch (port_definition_.eDomain)
  {
  case OMX_PortDomainAudio:
    domain = "Audio";
    break;
  case OMX_PortDomainVideo:
    domain = "Video";
    //std::cout << "Video compression format: " << port_definition->format.video.eCompressionFormat << std::endl;
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

  std::cout << "Domain is " << domain << '\n'
            << "Buffer count " << port_definition_.nBufferCountActual << '\n'
            << "Buffer minimum count " << port_definition_.nBufferCountMin << '\n'
            << "Buffer size " << port_definition_.nBufferSize << " bytes" << std::endl;
  std::cout << "-----------------------" << std::endl;
}

/*
void get_supported_video_formats(uint32_t port_number)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE video_port_format;
  video_port_format.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
  video_port_format.nVersion = 
  setHeader(&sAudioPortFormat, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
  sAudioPortFormat.nIndex = 0;
  sAudioPortFormat.nPortIndex = portNumber;
  printf("Supported audio formats are:\n");
  for (;;)
  {
    err = OMX_GetParameter(handle,
                           OMX_IndexParamAudioPortFormat,
                           &sAudioPortFormat);
    if (err == OMX_ErrorNoMore)
    {
      printf("No more formats supported\n");
    }
    return;
    // This shouldn't occur, but does with Broadcom library
    if (sAudioPortFormat.eEncoding == OMX_AUDIO_CodingUnused)
    {
      printf("No coding format returned\n");
      return;
    }
    switch (sAudioPortFormat.eEncoding)
    {
    case OMX_AUDIO_CodingPCM:
      printf("Supported encoding is PCM\n");
      getPCMInformation(portNumber);
      break;
    case OMX_AUDIO_CodingVORBIS:
      printf("Supported encoding is Ogg Vorbis\n");
      break;
    case OMX_AUDIO_CodingMP3:
      printf("Supported encoding is MP3\n");
      getMP3Information(portNumber);
      break;
#ifdef RASPBERRY_PI
    case OMX_AUDIO_CodingFLAC:
      printf("Supported encoding is FLAC\n");
      break;
    case OMX_AUDIO_CodingDDP:
      printf("Supported encoding is DDP\n");
      break;
    case OMX_AUDIO_CodingDTS:
      printf("Supported encoding is DTS\n");
      break;
    case OMX_AUDIO_CodingWMAPRO:
      printf("Supported encoding is WMAPRO\n");
      break;
    case OMX_AUDIO_CodingATRAC3:
      printf("Supported encoding is ATRAC3\n");
      break;
    case OMX_AUDIO_CodingATRACX:
      printf("Supported encoding is ATRACX\n");
      break;
    case OMX_AUDIO_CodingATRACAAL:
      printf("Supported encoding is ATRACAAL\n");
      break;
#endif
    case OMX_AUDIO_CodingAAC:
      printf("Supported encoding is AAC\n");
      break;
    case OMX_AUDIO_CodingWMA:
      printf("Supported encoding is WMA\n");
      break;
    case OMX_AUDIO_CodingRA:
      printf("Supported encoding is RA\n");
      break;
    case OMX_AUDIO_CodingAMR:
      printf("Supported encoding is AMR\n");
      break;
    case OMX_AUDIO_CodingEVRC:
      printf("Supported encoding is EVRC\n");
      break;
    }
  }
case OMX_AUDIO_CodingG726:
  printf("Supported encoding is G726\n");
  break;
case OMX_AUDIO_CodingMIDI:
  printf("Supported encoding is MIDI\n");
  break;
default:
  printf("Supported encoding is not PCM or MP3 or Vorbis, is 0x%X\n",
         sAudioPortFormat.eEncoding);
}
sAudioPortFormat.nIndex++;
}
*/