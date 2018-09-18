#pragma once
#include "OctreeVector.hpp"

/**
 * global octree data -- one instance for whole octree constant.
 */
namespace hxa {
	class OctreeDimensions {
	private:
		/**
		 * Позиция нижнего угла.
		 */
		OctreeVector position;
		/**
		 * Размер.
		 */
		float size;
		/**
		 * Максимальное количество элементов в ячейке.
		 */
		int maxItemsPerCell;
		/**
		 * Максимальное количество уровней.
		 */
		int maxLevel;
	public:
		void setPosition(const OctreeVector setPosition) {
			position = setPosition;
		}

		const OctreeVector getPosition() const {
			return position;
		}

		void setSize(const float setSize) {
			if (setSize >= 0.0f) {
				size = setSize;
			}
			else {
				size = -setSize;
			}
		}

		float getSize() const {
			return size;
		}

		void setMaxItemCountPerCell(const int setMaxItemCountPerCell) {
			if (setMaxItemCountPerCell > 0) {
				maxItemsPerCell = setMaxItemCountPerCell;
			}
			else {
				maxItemsPerCell = 1;
			}
		}

		int getMaxItemCountPerCell() const {
			return maxItemsPerCell;
		}

		void setMaxLevel(const int setMaxLevel) {
			if (setMaxLevel > 0) {
				maxLevel = setMaxLevel - 1;
			}
			else {
				maxLevel = 0;
			}
		}

		const int getMaxLevel() const {
			return maxLevel;
		}

		bool isSubdivide(const int itemCount, const int level) const {
			return itemCount > maxItemsPerCell && level < maxLevel;
		}
	};
}