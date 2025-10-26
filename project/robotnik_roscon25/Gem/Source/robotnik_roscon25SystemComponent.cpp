

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

#include "robotnik_roscon25SystemComponent.h"

namespace robotnik_roscon25
{
    void robotnik_roscon25SystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<robotnik_roscon25SystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<robotnik_roscon25SystemComponent>("robotnik_roscon25", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void robotnik_roscon25SystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("robotnik_roscon25Service"));
    }

    void robotnik_roscon25SystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("robotnik_roscon25Service"));
    }

    void robotnik_roscon25SystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void robotnik_roscon25SystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    robotnik_roscon25SystemComponent::robotnik_roscon25SystemComponent()
    {
        if (robotnik_roscon25Interface::Get() == nullptr)
        {
            robotnik_roscon25Interface::Register(this);
        }
    }

    robotnik_roscon25SystemComponent::~robotnik_roscon25SystemComponent()
    {
        if (robotnik_roscon25Interface::Get() == this)
        {
            robotnik_roscon25Interface::Unregister(this);
        }
    }

    void robotnik_roscon25SystemComponent::Init()
    {
    }

    void robotnik_roscon25SystemComponent::Activate()
    {
        robotnik_roscon25RequestBus::Handler::BusConnect();
    }

    void robotnik_roscon25SystemComponent::Deactivate()
    {
        robotnik_roscon25RequestBus::Handler::BusDisconnect();
    }
}
