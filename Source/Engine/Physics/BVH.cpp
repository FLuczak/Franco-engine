#include "Engine/Physics/BVH.hpp"

#include "SFML/SFMLMath.hpp"

void Physics::BVH::DebugDraw(unsigned index, DebugRenderer& debugRenderer)
{
    if (nodes.empty()) return;
    BVHNode& node = nodes[index];

    const auto& tempPolygon = node.aabb.ComputeBoundary();

    for (size_t i = 0; i < tempPolygon.size(); i++)
    {
        debugRenderer.AddLine(DebugCategory::Physics, tempPolygon[i], tempPolygon[(i + 1) % tempPolygon.size()], sf::Color::Green);
    }

    if (node.IsLeaf()) return;

    DebugDraw(node.leftChildIndex, debugRenderer);
    DebugDraw(node.rightChildIndex, debugRenderer);
}

/**
 * \brief Given an array of BVHElement vector, build and subdivide the bvh
 * \param allElements - a vector of bvh element structs
 */
void Physics::BVH::BuildBVH(std::vector<BVHElement>& allElements)
{
    if (allElements.empty()) return;
    elementIndices.clear();
    //Create root node
    elements = allElements;

    for (size_t i =0 ; i < elements.size();i++)
    {
        elementIndices.emplace_back(i);
    }

    nodes.clear();
    nodes.emplace_back( 0u, 0u,0u,static_cast<unsigned>(allElements.size()));
    nodesUsed = 1;

    UpdateBounds(rootNodeIndex);
    Subdivide(rootNodeIndex);
}

/**
 * \brief Given a node's index. Calculate its bounds based on all the elements that are in the node
 * \param index - index of a node that bounds should be updated
 */
void Physics::BVH::UpdateBounds(const unsigned index)
{
    BVHNode& node = nodes[index];

    sf::Vector2f min = sf::Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    sf::Vector2f max = sf::Vector2f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    int first = node.firstElementIndex;
    for (size_t i = 0; i < node.elementCount;i++)
    {
        min = sf::min(elements[elementIndices[first + i]].aabb.GetMin(),min);
        max = sf::max(elements[elementIndices[first + i]].aabb.GetMax(),max);
    }

    node.aabb = geometry2d::AABB(min, max);
}

/**
 * \brief Main bvh function. Subdivide the bvh nodes into smaller ones until it is possible
 * \param index - index of a node that should be subdivided
 */
void Physics::BVH::Subdivide(unsigned index)
{
    if (nodes[index].elementCount <= 2)return;
    const sf::Vector2f extent = nodes[index].aabb.GetMax() - nodes[index].aabb.GetMin();

    int axis = 0;
    if (extent.y > extent.x)
    {
        axis = 1;
    }


    const float splitPos = nodes[index].aabb.GetMin()[axis] + extent[axis] * 0.5f;

    int i = static_cast<int>(nodes[index].firstElementIndex);
    int j = static_cast<int>(i + nodes[index].elementCount - 1);

    while (i <= j)
    {
        if (elements[elementIndices[i]].aabb.ComputeCenter()[axis] < splitPos)
        {
            i++;
        }
        else
        {
            std::swap(elementIndices[i], elementIndices[j]);
            j--;
        }
    }

    int leftCount = i - nodes[index].firstElementIndex;

    if (leftCount == 0 || leftCount == nodes[index].elementCount)return;

    const int leftChildIdx = nodesUsed++;
    const int rightChildIdx = nodesUsed++;

    nodes[index].leftChildIndex = leftChildIdx;
    nodes[index].rightChildIndex = rightChildIdx;

    const int rightCount = nodes[index].elementCount - leftCount;

    nodes.emplace_back(0, 0, nodes[index].firstElementIndex, leftCount);
    nodes.emplace_back(0, 0, i, rightCount);

    nodes[index].elementCount = 0;

    UpdateBounds(leftChildIdx);
    UpdateBounds(rightChildIdx);

    Subdivide(leftChildIdx);
    Subdivide(rightChildIdx);
}
