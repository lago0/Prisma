#pragma once

#include <string>
#include <map>
#include <memory>

#include "node/Node.h"

enum class PropertyType
{
    INT,
    FLOAT,
    TEXT
};

/**
 * @class NodePropertyParams
 * @brief Params for Node Property (for example, with int, int max and min bounds)
*/
class NodePropertyParams
{
    public:
        virtual ~NodePropertyParams() = default;
};

class IntBoundsParam : public NodePropertyParams
{
    public:
        IntBoundsParam(int min, int max) :
            min(min),
            max(max)
        {
        }

        int min;
        int max;
};

class FloatBoundsParam : public NodePropertyParams
{
    public:
        FloatBoundsParam(float min, float max) :
            min(min),
            max(max)
        {
        }

        float min;
        float max;
};

/**
 * @class NodeProperty
 * @brief Creates an editable property for nodes. Recommended to use the abstracted functions to create and manage it.
 * @param node The node which the property is going to be added to.
 * @param initialValue The initial value of the property.
 */
class NodeProperty
{
    public:
        NodeProperty(Node* node, PropertyType type, void* initialValue) :
            node(node),
            type(type),
            value(initialValue)
        {
        }

        ~NodeProperty()
        {
            switch (this->type)
            {
            case PropertyType::INT:
                delete static_cast<int*>(value);
                break;
            case PropertyType::FLOAT:
                delete static_cast<float*>(value);
                break;
            // TODO: Add support for PropertyType::STRING
            default:
                break;
            }
        }

        void SetValue(void* value)
        {
            this->value = value;
        }

        void* GetValue()
        {
            return value;
        }

        void SetName(std::string name)
        {
            this->name = name;
        }

        void OnValueChanged()
        {
            node->SetDirty();
        }

        std::string GetName()
        {
            return name;
        }

        Node* node;

        PropertyType type;
        void* value;

        std::map<std::string, std::shared_ptr<NodePropertyParams>> params;

        std::string name;
};

/**
 * @brief Checks if a NodeProperty is a special type.
 * @param property The NodeProperty to check.
 * @param type The type that the NodeProperty must be.
 * @return Returns true if NodeProperty is that type.
 */
inline bool IsPropertyType(std::shared_ptr<NodeProperty> property, PropertyType type)
{
    return property->type == type;
}

/**
 * @brief Gets the bounds of an Int NodeProperty.
 * @param property The node property to get its bounds.
 * @return A shared pointer to the IntBoundsParam that cointains the bounds of the int, or nullptr if property is not an Int
 */
inline std::shared_ptr<IntBoundsParam> GetIntBounds(std::shared_ptr<NodeProperty> property)
{
    if (IsPropertyType(property, PropertyType::INT))
    {
        return std::dynamic_pointer_cast<IntBoundsParam>(property->params["bounds"]);
    }

    return nullptr;
}

/**
 * @brief Creates an Int NodeProperty.
 * @param node The node which the property is going to be applied.
 * @param name The visual name of the Property.
 * @param initialValue The initial value of the property.
 * @param min Minimum value of the property.
 * @param max Maximum value of the property.
 * @return A shared pointer to the IntBoundsParam that cointains the bounds of the int, or nullptr if property is not an Int
 */
inline std::shared_ptr<NodeProperty> CreateIntNodeProperty(Node* node, std::string name, int initialValue, int min, int max)
{
    int* value = new int(initialValue);
    auto property = std::make_shared<NodeProperty>(node, PropertyType::INT, (void*)value);

    property->SetName(name);
    property->params["bounds"] = std::make_shared<IntBoundsParam>(min, max);

    return property;
}

// TODO: Add support for PropertyType::FLOAT and PropertyType::STRING
