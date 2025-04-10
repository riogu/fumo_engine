#pragma once
#include <libassert/assert.hpp>
#include <unordered_map>

#include "cereal/archives/json.hpp"
#include "engine_constants.hpp"

// const uint64_t MAX_ELEMENTS = 100;
//
// template<typename T>
// class array_wrapper {
//   private:
//     std::array<T, MAX_ELEMENTS> wrapped_array;
//     size_t current_size;
//
//   public:
//     [[nodiscard]] auto add(T element) {
//         size_t index = current_size;
//         wrapped_array[index] = element;
//         return &wrapped_array[index];
//     }
//     // only need to remove when we own the index
//     void remove(T &element) {
//
//         current_size--;
//     }
// };

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void entity_destroyed(EntityId entity_id) = 0;
    virtual void
    serialize_component(const EntityId& entity_id,
                        const ComponentId& component_id,
                         cereal::JSONOutputArchive& out_archive) = 0;
};

using Index = uint64_t;

template<typename T>
class ComponentArray: public IComponentArray {
  private:
    std::array<T, MAX_ENTITY_IDS> all_components {};
    std::unordered_map<EntityId, Index> entity_to_index;
    std::unordered_map<Index, EntityId> index_to_entity;
    size_t component_count {};

  public:
    void serialize_component(const EntityId& entity_id,
                             const ComponentId& component_id,
                             cereal::JSONOutputArchive& out_archive) override {
        // out_archive(cereal ::make_nvp(std::to_string(component_id),
        //                               all_components[entity_to_index[entity_id]]));
    }

    void add_component_data(EntityId entity_id, T component) {
        // add new entities to the end of the array
        DEBUG_ASSERT(!entity_to_index.contains(entity_id),
                     "Component added twice.",
                     entity_to_index);
        size_t new_index = component_count;

        entity_to_index[entity_id] = new_index;
        index_to_entity[new_index] = entity_id;

        all_components[new_index] = component;

        // move forward current pointer
        component_count++;
    };

    void remove_component_data(EntityId entity_id) {
        DEBUG_ASSERT(entity_to_index.contains(entity_id),
                     "Removing non-existent component.",
                     entity_to_index);
        // removing components means we want to:
        //  -> place the last component in the new empty spot to keep our array packed
        //  -> update our unordered_maps to account for this removal

        size_t removed_entity_index = entity_to_index[entity_id];
        size_t last_component_index = component_count - 1;
        all_components[removed_entity_index] = all_components[last_component_index];

        // get the entity we need to move to empty spot
        EntityId last_component_entity = index_to_entity[last_component_index];
        index_to_entity[removed_entity_index] = last_component_entity;
        entity_to_index[last_component_entity] = removed_entity_index;

        // clear our maps
        entity_to_index.erase(entity_id);
        index_to_entity.erase(last_component_index);

        component_count--;
    }

    T& get_component_data(EntityId entity_id) {
        DEBUG_ASSERT(entity_to_index.contains(entity_id),
                     "this entity does not have this component.",
                     entity_id);
        // notice that there is an overhead from the non-contiguous unordered_map access
        // on this method possibly replace this with something else later
        return all_components[entity_to_index[entity_id]];
    }

    void entity_destroyed(EntityId entity_id) override {
        // used in EntityManager
        if (entity_to_index.contains(entity_id)) {
            remove_component_data(entity_id);
        }
    }
};
