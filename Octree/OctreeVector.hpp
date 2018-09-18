#pragma once
#include <math.h>

/**
* yes, its the 3d vector class! pretty much as youd expect.<br/><br/>
*
* if you write some 3d graphics software, then you MUST write your own vector
* class - its the law. so heres mine.
*/
namespace hxa {
	class OctreeVector {
	public:
		/**
		 * Позиция по X.
		 */
		float x_m;
		/**
		 * Позиция по Y.
		 */
		float y_m;
		/**
		 * Позиция по Z.
		 */
		float z_m;
	public:
		/**
		 * Конструктор по умолчанию.
		 */
		OctreeVector() {
			x_m = 0.0f;
			y_m = 0.0f;
			z_m = 0.0f;
		}

		OctreeVector(const float x, const float y, const float z) {
			x_m = x;
			y_m = y;
			z_m = z;
		}

		/**
		 * Деструктор по умолчанию.
		 */
		~OctreeVector() {}

		/**
		 * Сеттер позиции по X.
		 * @param setX - позиция по X.
		 */
		void setX(const float setX) {
			x_m = setX;
		}

		/**
		* Геттер позиции по X.
		* @return - позиция по X.
		*/
		float getX() const {
			return x_m;
		}

		/**
		 * Сеттер позиции по Y.
		 * @param setY - позиция по Y.
		 */
		void setY(const float setY) {
			y_m = setY;
		}

		/**
		* Геттер позиции по Y.
		* @return - позиция по Y.
		*/
		float getY() const {
			return y_m;
		}

		/**
		 * Сеттер позиции по Z.
		 * @param setZ - позиция по Z.
		 */
		void setZ(const float setZ) {
			z_m = setZ;
		}

		/**
		 * Геттер позиции по Z.
		 * @return - позиция по Z.
		 */
		float getZ() const {
			return z_m;
		}

		/**
		 * Получение длины вектора.
		 * @return - длина вектора.
		 */
		float length() const {
			return sqrtf(powf(x_m, 2) + powf(y_m, 2) + powf(z_m, 2));
		}

		OctreeVector  operator+ (const OctreeVector&) const;
		OctreeVector& operator+=(const OctreeVector&);

		OctreeVector  operator- (const OctreeVector&) const;
		OctreeVector& operator-=(const OctreeVector&);

		OctreeVector  operator* (float) const;
		OctreeVector& operator*=(float);

		/// logical --------------------------------------------------------------------
		bool operator==(const OctreeVector&) const;
		bool operator!=(const OctreeVector&) const;

		OctreeVector operator>(const OctreeVector&) const;
		OctreeVector operator>=(const OctreeVector&) const;

		OctreeVector operator<(const OctreeVector&) const;
		OctreeVector operator<=(const OctreeVector&) const;

		static const OctreeVector& HALF();
	};
}
