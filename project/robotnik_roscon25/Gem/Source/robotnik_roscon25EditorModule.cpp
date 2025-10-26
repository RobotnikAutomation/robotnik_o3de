
#include <robotnik_roscon25ModuleInterface.h>
#include "robotnik_roscon25EditorSystemComponent.h"

#include "robotnik_roscon25SampleComponent.h"
namespace robotnik_roscon25
{
    class robotnik_roscon25EditorModule
        : public robotnik_roscon25ModuleInterface
    {
    public:
        AZ_RTTI(robotnik_roscon25EditorModule, "{BA9CA48F-2B79-4BEA-9DA9-807C4FBD7A59}", robotnik_roscon25ModuleInterface);
        AZ_CLASS_ALLOCATOR(robotnik_roscon25EditorModule, AZ::SystemAllocator, 0);

        robotnik_roscon25EditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                robotnik_roscon25EditorSystemComponent::CreateDescriptor(),
                robotnik_roscon25SampleComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<robotnik_roscon25EditorSystemComponent>(),
            };
        }
    };
}// namespace robotnik_roscon25

AZ_DECLARE_MODULE_CLASS(Gem_robotnik_roscon25, robotnik_roscon25::robotnik_roscon25EditorModule)
