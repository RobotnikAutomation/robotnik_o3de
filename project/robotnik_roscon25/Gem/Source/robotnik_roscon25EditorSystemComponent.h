
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <robotnik_roscon25SystemComponent.h>

namespace robotnik_roscon25
{
    /// System component for robotnik_roscon25 editor
    class robotnik_roscon25EditorSystemComponent
        : public robotnik_roscon25SystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = robotnik_roscon25SystemComponent;
    public:
        AZ_COMPONENT(robotnik_roscon25EditorSystemComponent, "{5EA291C7-C166-4957-AD49-DC7CE15FC76C}", BaseSystemComponent);
        static void Reflect(AZ::ReflectContext* context);

        robotnik_roscon25EditorSystemComponent();
        ~robotnik_roscon25EditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace robotnik_roscon25
