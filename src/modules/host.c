#include "fastfetch.h"

void ffPrintHost(FFinstance* instance)
{
    if(ffPrintCachedValue(instance, "Host"))
        return;

    FF_STRBUF_CREATE(family);
    ffGetFileContent("/sys/devices/virtual/dmi/id/product_family", &family);

    FF_STRBUF_CREATE(name);
    ffGetFileContent("/sys/devices/virtual/dmi/id/product_name", &name);
    
    FF_STRBUF_CREATE(version);
    ffGetFileContent("/sys/devices/virtual/dmi/id/product_version", &version);

    FF_STRBUF_CREATE(host);

    if(ffStrbufIsEmpty(&instance->config.hostFormat))
    {
        ffParseFormatString(&host, &instance->config.hostFormat, 3,
            (FFformatarg){FF_FORMAT_ARG_TYPE_STRBUF, &family},
            (FFformatarg){FF_FORMAT_ARG_TYPE_STRBUF, &name},
            (FFformatarg){FF_FORMAT_ARG_TYPE_STRBUF, &version}
        );
    }
    else
    {
        if(ffStrbufIsEmpty(&family) && ffStrbufIsEmpty(&name))
        {
            ffPrintError(instance, "Host", "neither family nor name could be determined");
            return;
        }
        else if(ffStrbufIsEmpty(&name))
        {
            ffStrbufAppend(&host, &family);
        }
        else if(ffStrbufIsEmpty(&family))
        {
            ffStrbufAppend(&host, &name);
        }
        else
        {
            ffStrbufAppend(&host, &family);
            ffStrbufAppendC(&host, ' ');
            ffStrbufAppend(&host, &name);
        }

        if(!ffStrbufIsEmpty(&version) && ffStrbufIgnCaseCompS(&version, "None") != 0)
        {
            ffStrbufAppendC(&host, ' ');
            ffStrbufAppend(&host, &version);
        }
    }

    ffPrintAndSaveCachedValue(instance, "Host", &host);
    ffStrbufDestroy(&host);

    ffStrbufDestroy(&family);
    ffStrbufDestroy(&name);
    ffStrbufDestroy(&version);
}