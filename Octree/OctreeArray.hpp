#pragma once

namespace hxa {
	template<class TYPE>
	class OctreeArray {
	private:
		TYPE* pMemory_m;
		int length_m;
	private:
		void assign(const OctreeArray<TYPE>&);
		void acquireMemory(int length, bool isCopied);
		static void copyObjects(TYPE* lValStart, const TYPE* rValStart, int length);
	public:
		OctreeArray() {
			pMemory_m = 0;
			length_m = 0;
		}
		explicit OctreeArray(const int length) {
			pMemory_m = 0;
			length_m = 0;
			OctreeArray<TYPE>::setLength(length);
		}
		~OctreeArray() {
			delete[] pMemory_m;
		}
		OctreeArray(const OctreeArray<TYPE>& other) {
			pMemory_m = 0;
			length_m = 0;
			OctreeArray<TYPE>::assign(other);
		}
		OctreeArray& operator=(const OctreeArray<TYPE>& other) {
			assign(other);
			return *this;
		}

		void setLength(const int length) {
			acquireMemory(length, false);
		}
		void swap(OctreeArray&);
		void append(const TYPE&);
		void remove(int index);

		int getLength() const;
		bool isEmpty() const;
		static  int getMaxLength();

		TYPE* getMemory() const;
		/**
		 * wraps out of bounds.
		 */
		TYPE& operator[](int index) const;
		void zeroMemory() const;
	};

	template<class TYPE>
	void OctreeArray<TYPE>::swap(OctreeArray<TYPE>& other) {
		TYPE*const tmpM = pMemory_m;
		pMemory_m = other.pMemory_m;
		other.pMemory_m = tmpM;

		const int tmpL = length_m;
		length_m = other.length_m;
		other.length_m = tmpL;
	}

	template<class TYPE>
	void OctreeArray<TYPE>::append(const TYPE& element) {
		/// expand storage, duplicating elements
		acquireMemory(length_m + 1, true);

		/// write new element into last position
		*(pMemory_m + length_m - 1) = element;
	}


	template<class TYPE>
	void OctreeArray<TYPE>::remove(const int index) {
		/// check index is within range
		if ((index >= 0) & (index < length_m)) {
			/// make smaller storage
			OctreeArray<TYPE> newOctreeArray(length_m - 1);
			/// copy elements, skipping element at index
			{
				TYPE* pDestination = newOctreeArray.pMemory_m;
				TYPE* pEnd = pDestination + newOctreeArray.length_m;
				const TYPE* pSource = pMemory_m;
				const TYPE* pIndex = pSource + index;
				while (pDestination < pEnd) {
					pSource += int(pSource == pIndex);
					*(pDestination++) = *(pSource++);
				}
			}

			/// swap new storage with this
			swap(newOctreeArray);
		}
	}




	/// queries --------------------------------------------------------------------
	template<class TYPE>
	inline int OctreeArray<TYPE>::getLength() const {
		return length_m;
	}

	template<class TYPE>
	inline bool OctreeArray<TYPE>::isEmpty() const {
		return length_m == 0;
	}

	template<class TYPE>
	inline int OctreeArray<TYPE>::getMaxLength() {
		return INT_MAX;
	}

	template<class TYPE>
	inline TYPE* OctreeArray<TYPE>::getMemory() const {
		return pMemory_m;
	}

	template<class TYPE>
	TYPE& OctreeArray<TYPE>::operator[](const int index) const {
		return pMemory_m[(index >= 0 ? index : -index) % length_m];
	}

	template<class TYPE>
	void OctreeArray<TYPE>::zeroMemory() const {
		for (int i = length_m; i-- > 0; ) {
			pMemory_m[i] = TYPE();
		}
	}




	/// implementation -------------------------------------------------------------
	template<class TYPE>
	void OctreeArray<TYPE>::assign(const OctreeArray<TYPE>& other) {
		if (&other != this) {
			acquireMemory(other.getLength(), false);
			copyObjects(getMemory(), other.getMemory(), other.getLength());
		}
	}

	template<class TYPE>
	void OctreeArray<TYPE>::acquireMemory(int newLength, const bool isCopied) {
		/// clamp to 0 min
		newLength = (newLength >= 0) ? newLength : 0;
		/// only allocate if different length
		if (newLength != length_m) {
			/// allocate new storage
			TYPE* pNewMemory = new TYPE[newLength];
			if (pNewMemory == 0) {
				throw "OctreeArray<>::acquireMemory()\n\ncouldnt get memory\n";
			}

			/// copy elements to new storage
			if (isCopied) {
				copyObjects(pNewMemory, pMemory_m, (length_m <= newLength ? length_m : newLength));
			}
			/// delete old storage and set the members
			delete[] pMemory_m;
			pMemory_m = pNewMemory;
			length_m = newLength;
		}
	}

	template<class TYPE>
	void OctreeArray<TYPE>::copyObjects(TYPE*const pDestination, const TYPE*const pSource, const int length) {
		if (length >= 0) {
			TYPE* pDestinationCursor = pDestination + length;
			const TYPE* pSourceCursor = pSource + length;
			while (pDestinationCursor > pDestination) {
				*(--pDestinationCursor) = *(--pSourceCursor);
			}
		}
	}
}
