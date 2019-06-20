#include <mzx/convert.h>
#include <mzx/string/string_util.h>

#include <ecs/component/serialize/component_serialize.h>
#include <ecs/component/single/scene_grid_view.h>

namespace cherry
{

static bool SerializeSceneGridView(const mzx::ComponentBase *base,
                                   std::string *data)
{
    if (!base || !data)
    {
        return false;
    }
    auto *component =
        static_cast<const mzx::Component<SceneGridView> *>(base)->Get();
    *data = mzx::ConvertTo<std::string>(component->grid_size);
    *data += " grid:";
    for (auto &iter_grid : component->grid_list)
    {
        *data += mzx::Concat("[", iter_grid.first >> 16, ",",
                             iter_grid.first & 0xFFFF, "](");
        for (auto &entity : iter_grid.second)
        {
            *data += mzx::Concat(entity->ID(), ",");
        }
        *data += ")";
    }
    *data += " entity:";
    for (auto &iter_grid : component->entity_grid_index)
    {
        *data +=
            mzx::Concat("(", iter_grid.first->ID(), ",", iter_grid.second >> 16,
                        ",", iter_grid.second & 0xFFFF, ")");
    }
    *data += " publish add:";
    for (auto &entity : component->publish_add_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    *data += " subscribe add:";
    for (auto &entity : component->subscribe_add_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    *data += " position change:";
    for (auto &entity : component->position_change_list)
    {
        *data += mzx::Concat(entity->ID(), ",");
    }
    return true;
}

CHERRY_COMPONENT_SERIALIZE_REGIST(SceneGridView, SerializeSceneGridView,
                                  nullptr);

} // namespace cherry

