#pragma once
#include "constants.hpp"
#include "fumo_engine/core/system_base.hpp"

namespace FumoSerializer {
void serialize_entity(const EntityId& entity_id,
                      cereal::JSONOutputArchive& out_archive);
void deserialize_entity(cereal::JSONInputArchive& in_archive);
void deserialize_component_by_id(const EntityId& entity_id,
                                 const ComponentId& component_id,
                                 cereal::JSONInputArchive& in_archive);

template<typename T>
[[nodiscard]] T deserialize_component(const ComponentId& component_id,
                                      cereal::JSONInputArchive& in_archive) {
    T component;
    in_archive(cereal ::make_nvp(std::to_string(component_id), component));
    return component;
}

} // namespace FumoSerializer

// component mask: Any entity with LevelId component
// (which is all entities in the engine)
// (we might have entities that are outside the scope of the engine,
// so the component mask targets specifically "game" entities)
struct LevelSerializer: public System {
    void sys_call() override {
        // never gets called (we just need to get updated by the ECS)
    };
    void serialize_levels();
    // goes through the serialized data and assigns ScreenId and LevelId
    // based on where stuff was stored (directory and file name)
    void deserialize_levels();
};
