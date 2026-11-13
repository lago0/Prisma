#pragma once

#include <string>
#include <map>
#include <memory>

#include "node/Node.h"
#include "imagegallery/ImageGallery.h"

enum class PropertyType
{
    INT,
    FLOAT,
    TEXT,
    IMAGE
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

/**
 * @class IntBoundsParam
 * @brief Used for designing bounds on properties. It is deprecated and should not be used as it is going to be deleted soon.
 * @deprecated
 */
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

/**
 * @class FloatBoundsParam
 * @brief Used for designing bounds on properties. It is deprecated and should not be used as it is going to be deleted soon.
 * @deprecated
 */
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
 * @brief Base class for node properties.
 * @param name Name of the property.
 */
class NodeProperty
{
    public:
        NodeProperty(std::string name) :
            name(name)
        {
        };
        
        virtual ~NodeProperty()
        {
        };

        virtual void SetName(std::string newName)
        {
            name = newName;
        }

        virtual std::string GetName()
        {
            return name;
        }

        std::string name;
        PropertyType type;
};

/**
 * @class IntNodeProperty
 * @brief Creates an editable int property for nodes. Remplaces the deprecated NodeProperty class.
 * @param node The node which the property is going to be added to.
 * @param initialValue The initial value of the property.
 * @param min Minimum value of the property.
 * @param max Maximum value of the property.
 */
class IntNodeProperty : public NodeProperty
{
    public:
        IntNodeProperty(Node* node, std::string name, int initialValue, int min, int max) :
            NodeProperty(name),
            node(node),
            min(min),
            max(max),
            value(initialValue)
        {
            this->type = PropertyType::INT;
        }

        virtual ~IntNodeProperty()
        {
        };

        int GetValue()
        {
            return value;
        }

        void SetValue(int newValue)
        {
            if (newValue < min)
            {
                value = min;
            }
            else if (newValue > max)
            {
                value = max;
            }
            else
            {
                value = newValue;
            }

            OnValueChanged();
        }

        void OnValueChanged()
        {
            node->SetDirty();
        }

        int min, max;
        int value;

        Node* node;
};

/**
 * @class IntNodeProperty
 * @brief Creates an editable int property for nodes. Remplaces the deprecated NodeProperty class.
 * @param node The node which the property is going to be added to.
 * @param initialValue The initial value of the property.
 * @param min Minimum value of the property.
 * @param max Maximum value of the property.
 */
class ImageNodeProperty : public NodeProperty
{
    public:
        ImageNodeProperty(Node* node, std::string name) :
            NodeProperty(name),
            node(node)
        {
            this->type = PropertyType::IMAGE;
        }

        virtual ~ImageNodeProperty()
        {
        };

        std::shared_ptr<Image> GetValue()
        {
            return image;
        }

        void SetValue(std::shared_ptr<Image> newImage)
        {
            image = newImage;
            OnValueChanged();
        }

        void OnValueChanged()
        {
            node->SetDirty();
        }

        std::shared_ptr<Image> image;

        Node* node;
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
        return std::make_shared<IntBoundsParam>(static_cast<IntNodeProperty*>(property.get())->min, static_cast<IntNodeProperty*>(property.get())->max);
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
 * @return A shared pointer to the IntNodeProperty
 */
inline std::shared_ptr<IntNodeProperty> CreateIntNodeProperty(Node* node, std::string name, int initialValue, int min, int max)
{
    int* value = new int(initialValue);
    auto property = std::make_shared<IntNodeProperty>(node, name, initialValue, min, max);

    return property;
}

/**
 * @brief Creates an Image node property.
 * @param node The node which the property is going to be applied.
 * @param name The visual name of the Property.
 * @return A shared pointer to the ImageNodeProperty
 */
inline std::shared_ptr<NodeProperty> CreateImageNodeProperty(Node* node, std::string name)
{
    auto property = std::make_shared<ImageNodeProperty>(node, name);

    return property;
}

// TODO: Add support for PropertyType::FLOAT and PropertyType::STRING
