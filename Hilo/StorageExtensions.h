//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    inline Windows::Foundation::DateTime DateForFileInformation(Windows::Storage::BulkAccess::FileInformation^ fileInfo)
    {
        auto dateTaken = fileInfo->ImageProperties->DateTaken;
        if (dateTaken.UniversalTime == 0)
        {
            dateTaken = fileInfo->BasicProperties->DateModified;
        }
        return dateTaken;
    }
}