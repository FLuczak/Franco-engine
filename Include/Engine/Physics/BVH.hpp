#pragma once
#include "geometry2d.hpp"
#include "Engine/BaseCollider.h"
#include "Engine/DebugRenderer.hpp"
#include "Engine/Entity.hpp"

namespace Physics
{

/**
 * \brief Bvh implementation based on Jacco Bikker's tutorial: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
 * can be used for collision checking and raycasting
 */
class BVH
    {
    public:
        /**
         * \brief An element of a node. Contains information about its boundary, shape and entity.
         */
        struct BVHElement
        {
            BVHElement(BaseCollider& colliderToSet, const ColliderShape shapeToSet, const geometry2d::AABB& aabbToSet): collider({ colliderToSet }), shape(shapeToSet), aabb(aabbToSet)
            {
            }

        	std::reference_wrapper<BaseCollider> collider;
            ColliderShape shape;
            geometry2d::AABB aabb;
        };

        /**
         * \brief Struct for the bvh node. It contains information about its boundary, indices of children in the BVH nodes vector, its element count and first child element index
         * in the bvh element vector.
         */
        struct BVHNode
        {
            BVHNode(unsigned leftIndex,  unsigned rightIndex,  unsigned firstElementIdx, unsigned countToSet): leftChildIndex(leftIndex),rightChildIndex(rightIndex),firstElementIndex(firstElementIdx),elementCount(countToSet)
            {
            }

			geometry2d::AABB aabb{{0, 0}, {0, 0}};
            unsigned int leftChildIndex = 0;
            unsigned int rightChildIndex = 0;
            unsigned int firstElementIndex = 0;
            unsigned int elementCount = 0;
            bool IsLeaf() const { return elementCount > 0; }
        };


        BVH(std::vector<BVHElement>& elementsToSet) { BuildBVH(elementsToSet); }
        BVH() { }

        const BVHNode& GetNodeOfIndex(unsigned int index) const { return nodes[index]; }
        const BVHElement& GetElementOfIndex(unsigned int index) const{return elements[elementIndices[index]];}
        const std::vector<BVHNode>& GetNodes() const { return nodes; }
        const std::vector<BVHElement>& GetElements() const { return elements; }
        const unsigned int GetRootIndex() const { return rootNodeIndex; }
        void DebugDraw(unsigned index, DebugRenderer& debugRenderer);
        void BuildBVH(std::vector<BVHElement>& allElements);
    private:
        std::vector<BVHNode> nodes;
        std::vector<BVHElement> elements;
        std::vector<unsigned int> elementIndices;

        unsigned int rootNodeIndex = 0;
        unsigned int nodesUsed = 1;

        void UpdateBounds( unsigned int index);
        void Subdivide(unsigned int index);
    };

}
