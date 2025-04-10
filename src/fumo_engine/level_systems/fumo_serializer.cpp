// #include "fumo_engine/level_systems/fumo_serializer.hpp"
//
// #include <fstream>
//
// #include "filesystem"
// #include "fumo_engine/core/global_state.hpp"
// #include "fumo_engine/level_systems/fumo_serializer.hpp"
//
// extern std::unique_ptr<GlobalState> global;
// // NOTE: maybe switch this to a namespace later if this class is useless
//
// namespace fs = std::filesystem;
//
// void LevelSerializer::serialize_levels() {
//     for (const auto& entity_id : sys_entities) {
//         const auto& screen_id = global->ECS->get_component<ScreenId>(entity_id);
//         const auto& level_id = global->ECS->get_component<LevelId>(entity_id);
//
//         fs::path level_screen_path = std::format("level{}", level_id.level_id);
//         fs::create_directory(level_screen_path);
//
//         level_screen_path /= std::format("screen{}.json", screen_id.screen_id);
//
//         std::ofstream out_stream(level_screen_path);
//
//         if (out_stream.fail()) [[unlikely]]
//             PANIC("failed to open or create file\n");
//
//         // NOTE:
//         // dont forget to let the archive go out of scope so it flushes out the
//         // contents
//         cereal::JSONOutputArchive out_archive(out_stream);
//         FumoSerializer::serialize_entity(entity_id, out_archive);
//     }
// };
//
// // structure for the json is:
// // entity id
// // component_mask
// // all components
// void FumoSerializer::serialize_entity(const EntityId& entity_id,
//                                       cereal::JSONOutputArchive& out_archive) {
//     const auto& component_mask = global->ECS->get_component_mask(entity_id);
//
//     out_archive(CEREAL_NVP(entity_id));
//     out_archive(CEREAL_NVP(component_mask));
//
//     for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
//         if (component_mask & (1 << id)) {
//             global->ECS->serialize_component(entity_id, id, out_archive);
//         }
//     }
// }
//
// #define DESERIALIZE_COMPONENT(Type) \
//     global->ECS->entity_add_component( \
//         entity_id, \
//         FumoSerializer::deserialize_component<Type>(component_id, in_archive))
//
// enum struct AllComponentTypes {
//     Body,
//     Circle,
//     AnimationInfo,
//     Timer,
//     Render,
//     Rectangle,
//     PlayerShape,
//     ParallelGravityField,
//     CircularGravityField,
//     ColliderObjectFlag,
//     GravFieldFlag,
//     OutlineRectFlag,
//     EntityState,
//     MovedEventData,
// };
//
// void FumoSerializer::deserialize_component_by_id(
//     const EntityId& entity_id,
//     const ComponentId& component_id,
//     cereal::JSONInputArchive& in_archive) {
//     switch (AllComponentTypes {component_id}) {
//         case AllComponentTypes::Body:
//             DESERIALIZE_COMPONENT(Body);
//             break;
//         case AllComponentTypes::Circle:
//             DESERIALIZE_COMPONENT(Circle);
//             break;
//         case AllComponentTypes::AnimationInfo:
//             DESERIALIZE_COMPONENT(AnimationInfo);
//             break;
//         case AllComponentTypes::Timer:
//             DESERIALIZE_COMPONENT(Timer);
//             break;
//         case AllComponentTypes::Render:
//             DESERIALIZE_COMPONENT(Render);
//             break;
//         case AllComponentTypes::Rectangle:
//             DESERIALIZE_COMPONENT(Rectangle);
//             break;
//         case AllComponentTypes::PlayerShape:
//             DESERIALIZE_COMPONENT(PlayerShape);
//             break;
//         case AllComponentTypes::ParallelGravityField:
//             DESERIALIZE_COMPONENT(PlayerShape);
//             break;
//         case AllComponentTypes::CircularGravityField:
//             DESERIALIZE_COMPONENT(CircularGravityField);
//             break;
//         case AllComponentTypes::ColliderObjectFlag:
//             DESERIALIZE_COMPONENT(ColliderObjectFlag);
//             break;
//         case AllComponentTypes::GravFieldFlag:
//             DESERIALIZE_COMPONENT(GravFieldFlag);
//             break;
//         case AllComponentTypes::OutlineRectFlag:
//             DESERIALIZE_COMPONENT(OutlineRectFlag);
//             break;
//         case AllComponentTypes::EntityState:
//             DESERIALIZE_COMPONENT(EntityState);
//             break;
//         case AllComponentTypes::MovedEventData:
//             DESERIALIZE_COMPONENT(MovedEventData);
//             break;
//     }
// }
//
// void FumoSerializer::deserialize_entity(cereal::JSONInputArchive& in_archive) {
//     // follows the save order for serialized data
//     EntityId entity_id;
//     ComponentMask component_mask;
//     in_archive(entity_id);
//     in_archive(component_mask);
//
//     for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
//         if (component_mask & (1 << id)) {
//             FumoSerializer::deserialize_component_by_id(entity_id, id, in_archive);
//         }
//     }
// }
//
// void LevelSerializer::deserialize_levels() {}
