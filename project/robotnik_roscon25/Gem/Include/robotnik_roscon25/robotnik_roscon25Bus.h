/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace robotnik_roscon25
{
    class robotnik_roscon25Requests
    {
    public:
        AZ_RTTI(robotnik_roscon25Requests, "{EA61903B-4CE1-4559-893C-650C3776C8CB}");
        virtual ~robotnik_roscon25Requests() = default;
        // Put your public methods here
    };

    class robotnik_roscon25BusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using robotnik_roscon25RequestBus = AZ::EBus<robotnik_roscon25Requests, robotnik_roscon25BusTraits>;
    using robotnik_roscon25Interface = AZ::Interface<robotnik_roscon25Requests>;

} // namespace robotnik_roscon25
