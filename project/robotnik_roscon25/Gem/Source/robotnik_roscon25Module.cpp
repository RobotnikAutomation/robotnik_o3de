
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "robotnik_roscon25SystemComponent.h"
#include "robotnik_roscon25SampleComponent.h"

namespace robotnik_roscon25
{
    class robotnik_roscon25Module
        : public AZ::Module
    {
    public:
        AZ_RTTI(robotnik_roscon25Module, "{BA9CA48F-2B79-4BEA-9DA9-807C4FBD7A59}", AZ::Module);
        AZ_CLASS_ALLOCATOR(robotnik_roscon25Module, AZ::SystemAllocator);

        robotnik_roscon25Module()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                robotnik_roscon25SystemComponent::CreateDescriptor(),
                robotnik_roscon25SampleComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<robotnik_roscon25SystemComponent>(),
            };
        }
    };
}// namespace robotnik_roscon25

AZ_DECLARE_MODULE_CLASS(Gem_robotnik_roscon25, robotnik_roscon25::robotnik_roscon25Module)
