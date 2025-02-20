#ifndef SCHEDULER_ECS_HPP
#define SCHEDULER_ECS_HPP
#include "ECS.hpp"
#include "constants.hpp"
#include "entity_query.hpp"
#include "fumo_engine/component_array.hpp"
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include <memory>
#include <ostream>
// template <class T, class S, class C>
// S& Container(std::priority_queue<T, S, C>& q) {
//     struct HackedQueue : private std::priority_queue<T, S, C> {
//         static S& Container(std::priority_queue<T, S, C>& q) {
//             return q.*&HackedQueue::c;
//         }
//     };
//     return HackedQueue::Container(q);
// }
// template<class T>
// class PQV : public std::priority_queue<T> {
//   public:
//     typedef std::vector<T> TVec;
//     TVec getVector() {
//         TVec r(this->c.begin(), this->c.end());
//         // c is already a heap
//         sort_heap(r.begin(), r.end(), this->comp);
//         // Put it into priority-queue order:
//         reverse(r.begin(), r.end());
//         return r;
//     }
// };
// class Scheduler {
//   public:

// Array of component_masks where the index corresponds to the entity ID
// used to delay updates to the end of each frame
// std::set<EntityId> entities_to_create{};
// std::set<EntityId> entities_to_destroy{};
// each entityId corresponds to an index on this array
// std::set<EntityId> entities_to_update_components{};
// std::array<ComponentMask, MAX_ENTITY_IDS> scheduled_entity_component_masks{};
// NOTE: i have concluded that there isnt a real need to schedule these changes
// to the end of the frame. i can still do this, but i feel it adds little benefit
// as we might want to create systems that depend on some of these changes
// plus its not even necessarily more efficient as shared state is still required
// };
struct SystemCompare {
    inline bool operator()(const std::shared_ptr<System>& sysA,
                           const std::shared_ptr<System>& sysB) const {
        return sysA->priority < sysB->priority;
    }
};

class SchedulerECS {
  private:
    //------------------------------------------------------------------------------
    // std::array<std::shared_ptr<System>, MAX_SYSTEMS> system_scheduler{};
    // index is the priority for this system
    std::set<std::shared_ptr<System>, SystemCompare> system_scheduler{};

    // NOTE: if the system is not awake, we put it in unscheduled_systems;
    // (it is stored with the other systems that never want to be scheduled)
    std::unordered_map<std::string_view, std::shared_ptr<System>> unscheduled_systems{};
    //------------------------------------------------------------------------------

    std::unique_ptr<ECS> ecs;
    // Priority current_max_priority{};

    std::unordered_map<std::string_view, std::shared_ptr<System>> debug_scheduler{};
    std::array<EntityId, MAX_ENTITY_IDS> all_entity_ids_debug{};

  public:
    void initialize() {
        ecs = std::make_unique<ECS>();
        ecs->initialize();
    }
    // --------------------------------------------------------------------------------------
    // entity stuff
    [[nodiscard]] EntityId create_entity() {
        // added "scheduled" to allude to the fact this action only happens by
        // the end of the frame, can remove later
        EntityId entity_id = ecs->create_entity();
        // scheduler.entities_to_create.insert(entity_id);
        all_entity_ids_debug[entity_id] = entity_id;
        return entity_id;
    };

    void destroy_entity(EntityId entity_id) {
        // schedule deletion for the end of the frame
        // scheduler.entities_to_destroy.insert(entity_id);
        ecs->destroy_entity(entity_id);
        all_entity_ids_debug[entity_id] = NO_ENTITY_FOUND;
    }
    // get all matching entities for these types
    // template<typename ...Types>
    // [[nodiscard]] EntityId get_entities() {
    //     ComponentMask component_mask = make_component_mask<Types...>();
    // }
    // template<typename ...Types>
    // [[nodiscard]] EntityId get_entity() {
    //     ComponentMask component_mask = make_component_mask<Types...>();
    // }
    // // useful when we know we only have one entity or are looking for a flag struct
    // template<typename T>
    // [[nodiscard]] EntityId get_flagged_entity() {
    //
    //     ComponentMask component_mask = 0;
    //     component_mask |= get_component_id<T>();
    // }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // component stuff
    template<typename T>
    void register_component() {
        ecs->register_component<T>();
    }
    // TODO: consider adding components in bulk when creating entities
    // to minimize the number of update calls we make to systems (this really would help)
    template<typename T>
    void entity_add_component(EntityId entity_id, T component) {
        // scheduler.scheduled_entity_component_masks[entity_id] =
        ecs->entity_add_component(entity_id, component);
    }
    template<typename T> // remove from entity
    void entity_remove_component(EntityId entity_id) {
        // scheduler.scheduled_entity_component_masks[entity_id] =
        ecs->entity_remove_component<T>(entity_id);
    }
    template<typename T>
    [[nodiscard]] T& get_component(EntityId entity_id) {
        return ecs->get_component<T>(entity_id);
    }
    template<typename T>
    [[nodiscard]] ComponentId get_component_id() {
        return ecs->get_component_id<T>();
    }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // system stuff
    template<typename T, Priority priority, typename... Types>
    void register_system(EntityQuery entity_query, Types&... args) {

        // DEBUG_ASSERT(system_scheduler[priority] == nullptr, "already used this
        // priority.",
        //              system_scheduler);

        std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);
        ecs->register_system<T>(entity_query, system_ptr);
        system_ptr->priority = priority;

        system_scheduler.insert(system_ptr);
        // system_scheduler[priority] = system_ptr;
        // current_max_priority++;
        // return system_ptr;
        // NOTE: remove later if not using debugger
        std::string_view t_name = libassert::type_name<T>();
        debug_scheduler.insert({t_name, system_ptr});
    }

    // WARNING: this system wont be called in run_systems() call
    template<typename T, typename... Types>
    void register_system_unscheduled(EntityQuery entity_query, Types&... args) {
        const std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);
        ecs->register_system<T>(entity_query, system_ptr);
        // return system_ptr;
    }

    //------------------------------------------------------------------
    // WARNING: this system will get no updates to it's sys_entities

    template<typename T, Priority priority, typename... Types>
    void add_unregistered_system(Types&... args) {

        const std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);
        ecs->add_unregistered_system(system_ptr);
        system_ptr->priority = priority;
        // can be changed to be in the constructor or test this with initializer list

        system_scheduler.insert(system_ptr);

        // NOTE: remove later if not using debugger
        std::string_view t_name = libassert::type_name<T>();
        debug_scheduler.insert({t_name, system_ptr});
    }
    // this system wont be called in run_systems() call
    template<typename T, typename... Types>
    void add_unregistered_system_unscheduled(Types&... args) {
        const std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);

        ecs->add_unregistered_system(system_ptr);

        system_scheduler.insert(system_ptr);
    }

    //------------------------------------------------------------------
    void run_systems() {
        for (auto system_ptr : system_scheduler) {
            system_ptr->sys_call();
        }
    }
    // returns the system **cast** from the System interface
    template<typename T>
    [[nodiscard]] std::shared_ptr<T> get_system() {
        std::string_view type_name = libassert::type_name<T>();

        // added so we only need one method to find all systems even if not registered
        return std::static_pointer_cast<T>(ecs->get_system(type_name));
    }

    template<typename... Types>
    [[nodiscard]] ComponentMask make_component_mask() {
        ComponentMask component_mask = 0;
        (add_id_to_component_mask<Types>(component_mask), ...);
        return component_mask;
    }

  private:
    template<typename T>
    void add_id_to_component_mask(ComponentMask& component_mask) {

        auto var = std::is_base_of_v<System, T>;
        DEBUG_ASSERT(var != true,
                     "cant add a system to another system's component mask.");

        component_mask |= 1 << get_component_id<T>();
    }
    // --------------------------------------------------------------------------------------
  public:
    // filtering methods for entity ids
    [[nodiscard]] bool filter(EntityId entity_id, EntityQuery entity_query) {
        return entity_query.filter(ecs->get_component_mask(entity_id));
    }

    void debug_print() {
        PRINT(all_entity_ids_debug);
        ecs->debug_print();
        debug_print_scheduler();
        std::cerr << std::endl << std::endl;
    }
    void debug_print_scheduler() {
        PRINT(system_scheduler);
        PRINT(debug_scheduler);
        for (auto const& pair : debug_scheduler) {
            auto const& t_name = pair.first;
            auto const& system = pair.second;
            std::cout << libassert::highlight_stringify(t_name) << " -----> "
                      << libassert::highlight_stringify(system->priority) << std::endl;
        }
    }
};

#endif
