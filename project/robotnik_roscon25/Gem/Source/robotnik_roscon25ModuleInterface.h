
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <robotnik_roscon25SystemComponent.h>

namespace robotnik_roscon25
{
    class robotnik_roscon25ModuleInterface
        : public AZ::Module
    {
    public:
        AZ_RTTI(robotnik_roscon25ModuleInterface, "{11C1E8AA-A82F-4675-B122-DF9C90C25BCD}", AZ::Module);
        AZ_CLASS_ALLOCATOR(robotnik_roscon25ModuleInterface, AZ::SystemAllocator, 0);

        robotnik_roscon25ModuleInterface()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                robotnik_roscon25SystemComponent::CreateDescriptor(),
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
