
#include <AzCore/Serialization/SerializeContext.h>
#include "robotnik_roscon25EditorSystemComponent.h"

namespace robotnik_roscon25
{
    void robotnik_roscon25EditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<robotnik_roscon25EditorSystemComponent, robotnik_roscon25SystemComponent>()
                ->Version(0);
        }
    }

    robotnik_roscon25EditorSystemComponent::robotnik_roscon25EditorSystemComponent() = default;

    robotnik_roscon25EditorSystemComponent::~robotnik_roscon25EditorSystemComponent() = default;

    void robotnik_roscon25EditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("robotnik_roscon25EditorService"));
    }

    void robotnik_roscon25EditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("robotnik_roscon25EditorService"));
    }

    void robotnik_roscon25EditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void robotnik_roscon25EditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void robotnik_roscon25EditorSystemComponent::Activate()
    {
        robotnik_roscon25SystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void robotnik_roscon25EditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        robotnik_roscon25SystemComponent::Deactivate();
    }

} // namespace robotnik_roscon25
